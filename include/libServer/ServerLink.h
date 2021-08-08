/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  ServerLink.h
    说    明：  服务器链接管理
                1. 管理服务器链接
                2. 处理断开自动重连

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma  once

#include <map>
#include <vector>
#include <string>
#include "Observer.h"
#include "ServerData.h"
#include "libBase/Timer.h"

namespace server
{

    // 前置声明
    class IMsg;

    class CServerLink : public IObserver
    {
    public:
        // ServerID和ServerLinkInfo对应表
        typedef std::map<int32_t, std::map<int32_t, ServerLinkInfo>>					ServerLinkMap;
        typedef std::map<int32_t, std::map<int32_t, ServerLinkInfo>>::iterator		    ServerLinkMapItor;
        typedef std::map<int32_t, std::map<int32_t, ServerLinkInfo>>::const_iterator	ServerLinkMapConstItor;

    public:
        CServerLink();
        ~CServerLink();

    public:
		bool						        init(uint32_t nServerType, uint32_t nID);
		void						        release();
		void						        run();

    public:
        // 观察者实现
        virtual void				        doOnMessage(const IMsg* pMsg);

    public:
        void						        addServer(const IMsg* pMsg);
        void						        removeServer(uint32_t unSocketID);
        uint32_t						    getSocketID(uint32_t uiType, uint32_t uiID);
        std::vector<uint32_t>&              getSocketID(uint32_t uiType);
        std::vector<uint32_t>&              getServerID(uint32_t uiType);
        void						        getSocketID(uint32_t uiType, std::vector<uint32_t>& vSocket);
        std::map<int32_t, ServerLinkInfo>&  getServerLinkInfo(uint32_t uiType);
        void							    printConnectInfo();

        void							    addConnectingServer(const ServerLinkInfo& data);
        void							    removeConnectingServer(const ServerLinkInfo& data);
        void							    checkConnectingServers() const;

    public:
        void						        addAutoConnect(ServerLinkInfo& data);
        void						        registerServer(ServerLinkInfo& data);

    private:
		void						        onServerRegister(const IMsg* pMsg);
		void						        autoConnect();
		void						        keepLive();
		std::string					        getServerName(uint32_t uiType);

    private:
        ServerLinkMap				            m_map_ServerLink;		// 保存服务器链接容器
        base::CTimer				            m_Timer;				// 断线重连计时器
        base::CTimer				            m_KeepLiveTimer;		// 心跳包计时器
        std::vector<ServerLinkInfo>		        m_vector_AutoConnect;	// 自动重连容器
        uint32_t					            m_ServerType;			// 服务器类型
        uint32_t					            m_ServerID;				// 服务器ID
        std::map<int32_t, std::string>          m_ServerName;		    // 服务器名称
        std::map<std::string, ServerLinkInfo>   m_ConnectingServers;	// 需要连接的服务器信息
    };

}
