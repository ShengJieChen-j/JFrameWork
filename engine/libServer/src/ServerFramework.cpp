#include "stdafx.h"
#include "libBase/ExceptionHandler.h"
#include "libBase/basecrt.h"
#include "libBase/StringTools.h"
#include "ServerFramework.h"
#include "SystemTool.h"
#include "ServerNetEvent.h"
#include "ServerConfig.h"
#include "MessageSubject.h"
#include "MessageSend.h"
#include "ServerLink.h"
#include "BandwidthMgr.h"
#include "CommandService.h"
#include "AsioNetManager.h"
#include "DataBaseMgr.h"
#include "DataBaseMgrNew.h"
#include "libBase/TimeTools.h"
#include "TickerMgr.h"
#include "Profiling.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <boost/asio/error.hpp>

namespace server
{

	CServerFramework::CServerFramework()
		: m_bRunning(false)
		, m_unServerType(0)
		, m_unServerID(0)
		, m_nCycle(0)
		, m_nTotalTime(0)
		, m_nCycleCount(0)
	{
		ASSERT(CServerFramework::Inst() == NULL);

		this->m_pTickerMgr = NULL;
		this->m_pServerConfig = NULL;
		this->m_pCommandService = NULL;
		this->m_pNetEvent = NULL;
		this->m_pMessageSubject = NULL;
		this->m_pServerLink = NULL;
		this->m_pNetManager = NULL;
		this->m_pDataBaseMgr = NULL;
		this->m_pBandwidthMgr = NULL;

		this->m_nPreFrameTime = getHighPrecisionTime();

		CServerFramework::Inst() = this;
	}

	CServerFramework*& CServerFramework::Inst()
	{
		static CServerFramework* s_Inst;

		return s_Inst;
	}

	CServerFramework::~CServerFramework()
	{

	}

	void CServerFramework::startup(int32_t nCycle, uint32_t unServerType, const std::string& strConfig)
	{
		// 设置当前路径
		char appFilename[MAX_PATH] = { 0 };
		::GetModuleFileNameA(0, appFilename, MAX_PATH);
		*strrchr(appFilename, '\\') = 0;
		SetCurrentDirectoryA(appFilename);

		// 初始化异常处理
		base::InitProcessExceptionHandler();
		base::InitThreadExceptionHandler();

		this->m_bRunning = true;

		if (!this->frameworkInit(nCycle, unServerType, strConfig))
			return;

		while (this->m_bRunning)
		{
			this->frameworkRun();
		}

		this->frameworkRelease();
		// run
	}

	void CServerFramework::stop()
	{
		m_bRunning = false;
	}

	bool CServerFramework::frameworkInit(int32_t nCycle, uint32_t unServerType, const std::string& strConfig)
	{
		// 系统初始化
		SystemTool::init();
		SystemTool::forbidCtrlC();
		SystemTool::forbidWindowClose();

		// 初始化随机种子
		srand(static_cast<uint32_t>(time(NULL)));

		// 框架初始化
		this->m_pServerConfig = new CServerConfig();
		if (!this->m_pServerConfig->init(strConfig.c_str()))
			return false;

		// 设置窗口标题
		char cTitleName[BUF_SIZE_256] = { 0 };
		int res = base::crt::sprintf(cTitleName, sizeof(cTitleName), "%s-%d",
			m_pServerConfig->getString("System/Title").c_str(),
			m_pServerConfig->getInt32("System/ID"));
		if (-1 == res)
			return false;

		::SetConsoleTitleA(cTitleName);
		m_unServerType = unServerType;
		m_unServerID = m_pServerConfig->getInt32("System/ID");
		m_nCycle = nCycle;

		if (!base::initLog(m_pServerConfig->getString("System/LogPath").c_str(), m_pServerConfig->getString("System/ServerName").c_str()))
			return false;

		initProfiling();

		enableProfiling(m_pServerConfig->getInt32("System/Profiling") > 0);

		this->m_pTickerMgr = new CTickerMgr();
		if (m_pTickerMgr == NULL)
		{
			return false;
		}

		this->m_pNetEvent = new CServerNetEvent();
		if (m_pNetEvent == NULL)
		{
			return false;
		}

		this->m_pMessageSubject = new CMessageSubject();
		if (m_pMessageSubject == NULL)
		{
			return false;
		}

		this->m_pMessageSend = new CMessageSend();
		if (m_pMessageSend == NULL)
		{
			return false;
		}

		this->m_pBandwidthMgr = new CBandwidthMgr();
		if (m_pBandwidthMgr == NULL)
		{
			return false;
		}

		this->m_pServerLink = new CServerLink();
		if (m_pServerLink == NULL || !this->m_pServerLink->init(unServerType, m_unServerID))
			return false;

		this->m_pNetManager = new CAsioNetManager();
		if (m_pNetManager == NULL || !this->m_pNetManager->init(m_pNetEvent, m_unServerType, m_unServerID))
			return false;

		this->m_pDataBaseMgr = new CDataBaseMgr();
		if (m_pDataBaseMgr == NULL)
		{
			return false;
		}

		this->m_pDataBaseMgrNew = new CDataBaseMgrNew();
		if (m_pDataBaseMgrNew == NULL)
		{
			return false;
		}

		this->m_pCommandService = new CCommandService();
		if (m_pCommandService == NULL || !this->m_pCommandService->init())
			return false;

		if (!this->onInit())
			return false;

		// 为了解决同步加载时候导致时间模块停止不走的情况
		this->m_pTickerMgr->setCurTickTime();

		// 解决命令行卡顿感问题
		m_pCommandService->startTicker(m_nCycle);

		PrintInfo("server start");
		PrintInfo("server info ServerName: %s Validate: %d Profiling: %d PM: %d",
			m_pServerConfig->getString("System/ServerName").c_str(),
			m_pServerConfig->getInt32("System/Validate"),
			m_pServerConfig->getInt32("System/Profiling"),
			m_pServerConfig->getInt32("System/PM"));

		printf_s("server start success\n");
		printf_s("\nCommand>\n");

		// 程序启动没问题就写OK.txt
		std::string strServerName = m_pServerConfig->getString("System/ServerName") + "_OK.txt";
		FILE* pLogFile = fopen(strServerName.c_str(), "at+");
		if (pLogFile)
		{
			std::string strErrorTime = base::time_tool::getLocalTimeStr() + "\n";
			fwrite(strErrorTime.c_str(), 1, strlen(strErrorTime.c_str()), pLogFile);
			fclose(pLogFile);
			pLogFile = NULL;
		}

		return true;
	}

	void CServerFramework::frameworkRun()
	{
		int64_t nBeginTime = getHighPrecisionTime();
		this->m_nPreFrameTime = nBeginTime;

		//profilingBeginByLabel("Idle");
		int64_t nTimeError = this->m_pTickerMgr->getCurLogicTime() + this->m_nCycle - getNaturePassTime();
		if (nTimeError > 0)
		{
			if (nTimeError > this->m_nCycle)
				nTimeError = this->m_nCycle;
			Sleep((DWORD)nTimeError);
		}
		else
		{
			PrintInfo("server is busy time_error: %d", (uint32_t)(-nTimeError));
		}
		//profilingEndByLabel("Idle");

		PROFILING_BEGIN(this->m_pTickerMgr->update);
		this->m_pTickerMgr->update(this->m_nCycle);
		PROFILING_END(this->m_pTickerMgr->update);

		PROFILING_BEGIN(this->m_pNetManager->run);
		this->m_pNetManager->run();
		PROFILING_END(this->m_pNetManager->run);

		PROFILING_BEGIN(this->m_pDataBaseMgr->run);
		this->m_pDataBaseMgr->run();
		PROFILING_END(this->m_pDataBaseMgr->run);

		PROFILING_BEGIN(this->onRun);
		this->onRun();
		PROFILING_END(this->onRun);

		int64_t nEndTime = getHighPrecisionTime();
		this->m_nTotalTime = this->m_nTotalTime + (uint32_t)(nEndTime - nBeginTime);

		++this->m_nCycleCount;
		if (this->m_nTotalTime >= 60000000)
		{
#ifdef __PROFILING_OPEN
			server::profiling(this->m_nTotalTime);
#endif
			this->m_nTotalTime = 0;
		}

		// printfHardwareInfo(nTimeError); // TODO
	}

	void CServerFramework::frameworkRelease()
	{
		PrintInfo("server stop!");
		printf_s("\nserver stop! wait for backup data...");

		this->m_pDataBaseMgr->release();

		this->onRelease();

		// 框架释放
		this->m_pCommandService->release();
		SAFE_DELETE(this->m_pCommandService);
		SAFE_DELETE(this->m_pServerConfig);
		SAFE_DELETE(this->m_pMessageSubject);
		SAFE_DELETE(this->m_pMessageSend);
		SAFE_DELETE(this->m_pDataBaseMgr);
		this->m_pServerLink->release();
		SAFE_DELETE(m_pServerLink);
		this->m_pNetManager->release();
		SAFE_DELETE(this->m_pNetManager);
		SAFE_DELETE(this->m_pNetEvent);
		SAFE_DELETE(this->m_pTickerMgr);
		SAFE_DELETE(this->m_pBandwidthMgr);

		uninitProfiling();
		base::uninitLog();
	}

	void CServerFramework::printfHardwareInfo(int64_t nSleepTime)
	{
		static int32_t nTime = 0;
		nTime += m_nCycle;
		if (nTime > 1000)
		{
			nTime = 0;
			/// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
			uint64_t un64Mem = 0;
			uint64_t un64VMem = 0;
			uint64_t un64Read = 0;
			uint64_t un64Write = 0;
			int res_mem = SystemTool::getMemoryUsage(&un64Mem, &un64Mem);
			int res_io = SystemTool::getIOBytes(&un64Read, &un64Write);

			if (0 == res_mem && 0 == res_io)
			{
				char szLog[512] = { 0 };
				char szLogFile[128] = { 0 };
				base::crt::sprintf(szLogFile, sizeof(szLogFile), "%s\\hardware.log", m_pServerConfig->getString("System/LogPath").c_str());
				base::crt::sprintf(szLog, sizeof(szLog), "%s cup:%d	mem:%I64u-%I64u	io:%I64u-%I64u	sleep:%I64d",
					base::time_tool::getLocalTimeStr().c_str(), SystemTool::getCpuUsage(), un64Mem, un64VMem, un64Read, un64Write, nSleepTime);

				FILE* fp = ::fopen(szLogFile, "a+");
				if (!fp)
					return;

				::fprintf(fp, "%s\n", szLog);
				::fclose(fp);

			}
		}
	}

	bool CServerFramework::listen(int32_t netType)
	{
		if (netType != NetType_Inet &&
			netType != NetType_Snet)
		{
			return false;
		}

		std::string strIp = m_pServerConfig->getString(netType == NetType_Snet ? "System/Snet/IP" : "System/Inet/IP");
		// uint32_t unPort = m_pServerConfig->getInt32(netType == NetType_Snet ? "System/Snet/Port" : "System/Inet/Port");
		std::string strPort = m_pServerConfig->getString(netType == NetType_Snet ? "System/Snet/Port" : "System/Inet/Port");
		uint32_t unRecvBufSize = m_pServerConfig->getInt32(netType == NetType_Snet ? "System/Snet/ReceiveBuffer" : "System/Inet/ReceiveBuffer");
		uint32_t unSendBufSize = m_pServerConfig->getInt32(netType == NetType_Snet ? "System/Snet/SendBuffer" : "System/Inet/SendBuffer");
		uint32_t unMaxConnection = m_pServerConfig->getInt32(netType == NetType_Snet ? "System/Snet/MaxConnect" : "System/Inet/MaxConnect");

		std::vector<int32_t> vecPort;
		bool res = true;
		base::string_tool::getIntValueSplitByCommaEx(strPort.c_str(), vecPort);
		for (size_t i = 0; i < vecPort.size(); ++i)
		{
			try
			{
				if (!m_pNetManager->createServerNet(netType, strIp.c_str(), vecPort[i], unRecvBufSize, unSendBufSize, PACK_PARSER_NOENCRYPT, 0, 1, 6, unMaxConnection))
				{
					res = false;
					break;
				}
			}
			catch (const std::exception&)
			{
				LogError("server listen exception.");
				res = false;
				break;
			}
		}

		return res;
	}

	bool CServerFramework::connect(const std::string& serverName, uint32_t serverType)
	{
		bool res = true;
		char cTemp[BUF_SIZE_128] = { 0 };
		int32_t nCount = this->getServerConfig()->getElementCount(serverName.c_str());
		if (nCount <= 0)
		{
			return false;
		}

		for (int32_t i = 1; i <= nCount; ++i)
		{
			base::crt::sprintf(cTemp, sizeof(cTemp), "%s/Info%d", serverName.c_str(), i);
			std::string key = cTemp;

			ServerLinkInfo data;
			data.client_ReceiveBuffer = m_pServerConfig->getInt32(key + "/ReceiveBuffer");
			data.client_SendBuffer = m_pServerConfig->getInt32(key + "/SendBuffer");
			data.server_ServerType = serverType;
			data.server_ServerId = 1;	// TODO(jianxihua): 未来将加入支持ID扩展，现默认ID为1
			base::crt::strcpy(data.server_ip, sizeof(data.server_ip), m_pServerConfig->getString(key + "/IP").c_str());
			data.server_port = static_cast<uint16_t>(m_pServerConfig->getInt32(key + "/Port"));
			data.nPackParserType = PACK_PARSER_NOENCRYPT;
			data.bAutoConnect = true;
			base::crt::strncpy(data.cValidate, sizeof(data.cValidate), m_pServerConfig->getString("System/Validate").c_str(), sizeof(data.cValidate) - 1);
			base::crt::strncpy(data.client_ServerName, sizeof(data.client_ServerName), m_pServerConfig->getString("System/ServerName").c_str(), sizeof(data.client_ServerName) - 1);
			base::crt::strncpy(data.server_ServerName, sizeof(data.server_ServerName), serverName.c_str(), sizeof(data.server_ServerName) - 1);

			res &= m_pNetManager->connectTo(data);
		}

		return res;
	}

	bool CServerFramework::connect(ServerLinkInfo& data)
	{
		return m_pNetManager->connectTo(data);
	}

	void CServerFramework::disconnect(uint32_t unSocketId)
	{
		m_pNetManager->disConnect(unSocketId);
	}

	void CServerFramework::registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext)
	{
		if (NULL == pTicker)
			return;

		this->m_pTickerMgr->registTicker(pTicker, nStartTime, nIntervalTime, nContext);
	}

	void CServerFramework::unRegistTicker(CTicker* pTicker)
	{
		if (NULL == pTicker)
			return;

		if (NULL != this->m_pTickerMgr)
		{
			this->m_pTickerMgr->unRegistTicker(pTicker);
		}
		else
		{
			LogError("NULL != this->m_pTickerMgr");
		}
	}

	void CServerFramework::registCommand(const base::SCommandInfo& sCommandInfo)
	{
		this->m_pCommandService->registCommand(sCommandInfo);
	}

	void CServerFramework::registMsg(uint16_t usMessageType, IObserver* observer)
	{
		this->m_pMessageSubject->regObserver(usMessageType, observer);
	}

	IServerConfig* CServerFramework::getServerConfig() const
	{
		return this->m_pServerConfig;
	}

	CCommandService* CServerFramework::getCommandService() const
	{
		return this->m_pCommandService;
	}

	CMessageSubject* CServerFramework::getMessageSubject() const
	{
		return this->m_pMessageSubject;
	}

	IDBService* CServerFramework::getDBService() const
	{
		return this->m_pDataBaseMgr;
	}

	IDBServiceNew* CServerFramework::getDBServiceNew() const
	{
		return this->m_pDataBaseMgrNew;
	}

	CServerLink* CServerFramework::getServerLink() const
	{
		return this->m_pServerLink;
	}

	CBandwidthMgr* CServerFramework::getBandwidthMgr() const
	{
		return this->m_pBandwidthMgr;
	}

	INetManager* CServerFramework::getNetManager() const
	{
		return this->m_pNetManager;
	}

	IMessageSend* CServerFramework::getMessageSend() const
	{
		return this->m_pMessageSend;
	}

	uint32_t CServerFramework::getServerType() const
	{
		return m_unServerType;
	}

	uint32_t CServerFramework::getServerId() const
	{
		return m_unServerID;
	}

	std::vector<uint32_t>& CServerFramework::getConnectServerId(uint32_t unServerType)
	{
		return this->getServerLink()->getServerID(unServerType);
	}

#define _CONNECT_TYPE_ROUTINGSERVER	4

	bool CServerFramework::routingStreamBuf(void* pMsg, uint32_t nLen, uint32_t unServerType, uint32_t unServerId)
	{
		if (NULL == pMsg)
			return false;

		base::CIStream& is = m_pMessageSend->getIStream();
		is.resetPosition();
		is << static_cast<uint16_t>(4);
		is << static_cast<uint16_t>(_MSG_ROUTING_MESSAGE);
		is << unServerType;
		is << unServerId;

		if (!is.append(pMsg, (uint16_t)nLen))
			return false;

		is.setMsgSize();

		return m_pMessageSend->routingMsg(
			is.getBuf(),
			is.getPos(),
			_CONNECT_TYPE_ROUTINGSERVER,
			this->getRoutingServerId());
	}

	bool CServerFramework::sendStreamBuf(void* pMsg, uint32_t len, uint32_t unSocketID)
	{
		return m_pMessageSend->sendMsg(pMsg, len, unSocketID);
	}

	bool CServerFramework::sendStreamBufByServerType(void* pMsg, uint32_t len, uint32_t unServerType)
	{
		return m_pMessageSend->sendMsgByServerType(pMsg, len, unServerType);
	}

	bool CServerFramework::sendStreamBufByServerType(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t nServerID)
	{
		return m_pMessageSend->sendMsgByServerType(pMsg, len, unServerType, nServerID);
	}

	bool CServerFramework::sendStreamBufToAllServer(void* pMsg, uint32_t len, uint32_t unServerType)
	{
		return m_pMessageSend->sendMsgToAllServer(pMsg, len, unServerType);
	}

	CTickerMgr* CServerFramework::getTickerMgr() const
	{
		return this->m_pTickerMgr;
	}

}
