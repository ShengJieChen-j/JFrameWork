/******************************************************************

    创建日期：  2021年7月24日
    文件名称：  ServerFramework.h
    说    明：

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

*******************************************************************/
#pragma once

#include <string>
#include <vector>
#include "libBase/noncopyable.h"
#include "libBase/Console.h"
#include "libBase/IStream.h"
#include "ServerData.h"
#include "Ticker.h"


namespace server
{
    class ILogService;
    class CLogService;
    class CServerNetEvent;
    class IServerConfig;
    class CServerConfig;
    class CMessageSubject;
    class IMsg;
    class IObserver;
    class IMessageSend;
    class CServerLink;
    class CCommandService;
    class INetManager;
    class CDataBaseMgr;
    class CDataBaseMgrNew;
    class IDBService;
    class IDBServiceNew;
    class CBandwidthMgr;


    class CServerFramework : public base::noncopyable_::noncopyable
    {
    public:
        CServerFramework();
        virtual ~CServerFramework();

        static CServerFramework*& Inst();

    public:
        void			    startup(int32_t nCycle, uint32_t unServerType, const std::string& strConfig);		// 启动服务
        void			    stop();																				// 关闭服务

    public:
        // 提供子类实现接口
        virtual	bool        onInit() = 0; // 初始化接口(晚于框架的初始化)
        virtual void        onRun() = 0; // 自定义帧数
        virtual void        onRelease() = 0; // 释放接口(先于框架的释放)

        virtual bool        doOnConnect(server::IMsg* pMsg) = 0; // Socket第一次连接上来触发
        virtual void        doOnClose(uint32_t unSocketID) = 0; // Socket断开连接触发
        virtual void		onConnectivityReady() {}								 // 全连通事件
        virtual int32_t		getRoutingServerId() { return 1; }						 // 路由服务器ID

    public:
        // 网络
        bool                listen(int32_t netType);
        bool                connect(const std::string& serverName, uint32_t serverType);
        bool				connect(ServerLinkInfo& data);
        void				disconnect(uint32_t unSocketId);

        // 定时器
        void				registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
        void			    unRegistTicker(CTicker* pTicker);

        // 消息
        void                registMsg(uint16_t usMessageType, IObserver* observer);

        // 命令
        void				registCommand(const base::SCommandInfo& sCommandInfo);

        IServerConfig* getServerConfig() const;
        IDBService* getDBService() const;
        IDBServiceNew* getDBServiceNew() const;
        INetManager* getNetManager() const;
        IMessageSend* getMessageSend() const;
        uint32_t            getServerType() const;
        uint32_t            getServerId() const;
        std::vector<uint32_t>& getConnectServerId(uint32_t unServerType);
        CTickerMgr* getTickerMgr() const;

    public:
        //                  消息发送接口
        template            <typename T>
        inline bool         sendMsg(T* pMsg, uint32_t unSocketID)
        {
            if (unSocketID <= 0)
                return false;

            CIStream& input_stream = m_pMessageSend->getIStream();
            if (pMsg->packData(input_stream))
                return m_pMessageSend->sendMsg(input_stream.getBuf(), input_stream.getPos(), unSocketID);

            return false;
        }

        // 这个函数主要用于本服内的
        template            <typename T>
        inline bool         sendMsgByServerType(T* pMsg, uint32_t unServerType)
        {
            CIStream& input_stream = m_pMessageSend->getIStream();
            if (pMsg->packData(input_stream))
                return m_pMessageSend->sendMsgByServerType(input_stream.getBuf(), input_stream.getPos(), unServerType);

            return false;
        }
        // 这个函数主要用于全局服务器，比如disp服务器给各个服务器发送消息
        template            <typename T>
        inline bool         sendMsgByServerType(T* pMsg, uint32_t unServerType, uint32_t nServerId)
        {
            CIStream& input_stream = m_pMessageSend->getIStream();
            if (pMsg->packData(input_stream))
                return m_pMessageSend->sendMsgByServerType(input_stream.getBuf(), input_stream.getPos(), unServerType, nServerId);

            return false;
        }

        template            <typename T>
        inline bool         sendMsgToAllServer(T* pMsg, uint32_t unServerType)
        {
            CIStream& input_stream = m_pMessageSend->getIStream();
            if (pMsg->packData(input_stream))
                return m_pMessageSend->sendMsgToAllServer(input_stream.getBuf(), input_stream.getPos(), unServerType);

            return false;
        }

        template            <typename T>
        inline bool         routingMsg(T* pMsg, uint32_t unServerType, uint32_t unServerId)
        {
            CIStream& input_stream = m_pMessageSend->getIStream();
            input_stream << static_cast<uint16_t>(4);
            input_stream << static_cast<uint16_t>(_MSG_ROUTING_MESSAGE);
            input_stream << unServerType;
            input_stream << unServerId;	// -1 代表广播给所有unServerType类型的服务器
            uint16_t usHeadSize = input_stream.getPos();
            if (pMsg->packData(input_stream))
            {
                input_stream.setMsgSize(usHeadSize);
                return m_pMessageSend->routingMsg(
                    input_stream.getBuf(),
                    input_stream.getPos(),
                    CONNECT_TYPE_ROUTINGSERVER,
                    this->getRoutingServerId());
            }

            return false;
        }

        bool				routingStreamBuf(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t unServerId);

        bool                sendStreamBuf(void* pMsg, uint32_t len, uint32_t unSocketID);
        bool				sendStreamBufByServerType(void* pMsg, uint32_t len, uint32_t unServerType);
        bool				sendStreamBufByServerType(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t nServerID);
        bool				sendStreamBufToAllServer(void* pMsg, uint32_t len, uint32_t unServerType);

        ////////////////////////////////////////////////////////////
        // 以下接口仅供框架层使用
        ////////////////////////////////////////////////////////////
    public:
        CCommandService* getCommandService() const;
        CMessageSubject* getMessageSubject() const;
        CServerLink* getServerLink() const;
        CBandwidthMgr* getBandwidthMgr() const;

    private:
        bool                frameworkInit(int32_t nCycle, uint32_t unServerType, const std::string& strConfig);
        void                frameworkRun();
        void                frameworkRelease();
        void				printfHardwareInfo(int64_t nSleepTime);

    protected:
        IMessageSend* m_pMessageSend;
    public:
        // 运行状态
        volatile bool	    m_bRunning;
        uint32_t			m_unServerType;
        uint32_t			m_unServerID;
        int32_t				m_nCycle;
        int64_t				m_nPreFrameTime;
        uint32_t			m_nCycleCount;
        uint32_t			m_nTotalTime;

    private:
        CTickerMgr* m_pTickerMgr;
        CServerConfig* m_pServerConfig;
        CCommandService* m_pCommandService;
        CServerNetEvent* m_pNetEvent;
        CMessageSubject* m_pMessageSubject;
        CServerLink* m_pServerLink;
        INetManager* m_pNetManager;
        CDataBaseMgr* m_pDataBaseMgr;
        CDataBaseMgrNew* m_pDataBaseMgrNew;
        CBandwidthMgr* m_pBandwidthMgr;
    };
}
