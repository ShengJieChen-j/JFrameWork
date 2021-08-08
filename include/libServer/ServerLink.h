/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  ServerLink.h
    ˵    ����  ���������ӹ���
                1. �������������
                2. ����Ͽ��Զ�����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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

    // ǰ������
    class IMsg;

    class CServerLink : public IObserver
    {
    public:
        // ServerID��ServerLinkInfo��Ӧ��
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
        // �۲���ʵ��
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
        ServerLinkMap				            m_map_ServerLink;		// �����������������
        base::CTimer				            m_Timer;				// ����������ʱ��
        base::CTimer				            m_KeepLiveTimer;		// ��������ʱ��
        std::vector<ServerLinkInfo>		        m_vector_AutoConnect;	// �Զ���������
        uint32_t					            m_ServerType;			// ����������
        uint32_t					            m_ServerID;				// ������ID
        std::map<int32_t, std::string>          m_ServerName;		    // ����������
        std::map<std::string, ServerLinkInfo>   m_ConnectingServers;	// ��Ҫ���ӵķ�������Ϣ
    };

}
