/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  AsioNetManager.h
    ˵    ����  asio ͨ�Ų����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  ��Ҫ����:
                1. Terminal���װio_service���������ӵĽ�����Ͽ�
                2. Connection���װSocket��  ������Ϣ���շ�
******************************************************************/
#pragma once

#include <map>
#include <list>
#include <vector>
#include "INetManager.h"
#include "NetEvent.h"
#include "libBase/SyncObjs.h"
#include "libBase/TypeDefine.h"
#include "libBase/KeyCreater.h"

namespace server
{
    // ǰ������
    class CTerminal;
    class CConnection;

    class CAsioNetManager : public INetManager
        , public net_interface::IServerConnectEvent
        , public net_interface::IClientConnectEvent
    {
        typedef std::map<uint32_t, CConnection*>			    ConnectMap;
        typedef std::list<CConnection*>							CacheConnectList;
        typedef std::list<CConnection*>::iterator				CacheConnectListItor;
        typedef std::map<CConnection*, int64_t>					CacheConnectMap;
        typedef std::map<CConnection*, int64_t>::iterator		CacheConnectMapItor;
        typedef std::vector<CTerminal*>							TerminalVec;
        typedef std::vector<CTerminal*>::iterator				TerminalVecItor;
        typedef std::list<PCLIENT_CONNECT_RESULT>				CacheResultList;
        typedef std::list<PCLIENT_CONNECT_RESULT>::iterator		CacheResultListItor;

    public:
        CAsioNetManager();
        ~CAsioNetManager();

    public:
        virtual bool	init(
            net_interface::INetEvent* pNetEvent,
            uint32_t unServerType,
            uint32_t unID);

        virtual bool	run();
        virtual void	release();

    public:
        // �������������IO
        virtual bool createServerNet(
            int32_t netType,
            const char* pIp,
            uint32_t unPort,
            uint32_t unRecvBufSize,
            uint32_t unSendBufSize,
            ePackParserType eType,
            int32_t iAF = 0,
            int32_t iType = 1,
            int32_t iProto = 6,
            uint32_t unMaxConnection = 0);

        // �������ӷ�����
        virtual bool connectTo(ServerLinkInfo& data);

        // ������Ϣ
        virtual bool sendMsg(
            uint32_t unSocketID,
            void* pMsg,
            uint32_t unSize);

        // �����Ͽ�Socket����
        virtual void disConnect(uint32_t unSocketID);

    public:
        // ��������
        virtual int32_t getTerminalType(uint32_t unSocketID);

        // ��������
        virtual bool setConnectType(uint32_t unSocketID, int32_t nType);
        virtual int32_t	 getConnectType(uint32_t unSocketID);

        // ��������
        virtual int32_t	getNetType(uint32_t unSocketID);

        // ������ID
        virtual bool setID(uint32_t unSocketID, int64_t n64Id);
        virtual int64_t	 getID(uint32_t unSocketID);

        // ����ȡ��Ϣ������
        virtual bool setMaxRead(uint32_t unSocketID, int32_t nCount);
        virtual int32_t	 getMaxRead(uint32_t unSocketID);

        // ��Ϣ���Ͱ�ȫ��������
        virtual bool setMinMsgType(uint32_t unSocketID, int32_t nMessageType);
        virtual int32_t	 getMinMsgType(uint32_t unSocketID);

        // ��Ϣ���Ͱ�ȫ��������
        virtual bool setMaxMsgType(uint32_t unSocketID, int32_t nMessageType);
        virtual int32_t	 getMaxMsgType(uint32_t unSocketID);

        // ��ȡԶ��IP
        virtual const char* getRemoteAddress(uint32_t unSocketID);

        // ��ȡSocket�б�
        virtual std::vector<uint32_t> getSocketList();

    public:	// ASIO ͨ���¼�
        // ��������
        virtual void onBeConnected(CTerminal* pTerminal, CConnection* pCon);

        // ���ӷ�����ʧ��
        virtual void onConnectFail(CTerminal* pTerminal, ServerLinkInfo& data);

        // ���ӷ������ɹ�
        virtual void onConnectSuccess(CTerminal* pTerminal, CConnection* pCon, ServerLinkInfo& data);

    private:
        // ���IOCP����
        bool addTerminal(CTerminal* pTerminal);

        // �������
        bool addConnection(CConnection* pConnection);

        // ɾ������
        void delConnection(CConnection* pConnection);

        // ����״���������������
        bool addFirstConnection(CTerminal* pTerminal, CConnection* pConnection);

        // ���ʧЧ������
        void pushDelConnection(CConnection* pConnection);

    private:
        // �����µ�����
        void listenNewConnectionsRun();

        // �����״�����
        void firstConnectionsRun();

        // �������ӵĶ�д
        void connectionsRun();

        // ����ʧЧ����
        void delConnectionsRun();

        // ����ͻ����첽���ӷ���
        void clientConnectResultRun();

    private:
        net_interface::INetEvent* m_pNetEvent;				// ͨ���¼�֪ͨ
        TerminalVec					m_vector_Terminal;			// �����IOCP��Listen Socket
        ConnectMap					m_map_Connections;			// �������е�����
        CacheConnectMap				m_map_FirstConnections;		// �����״���������������(����һ����Ϣ��ճ������)
        CacheConnectList			m_list_DelConnections;		// ����ʧЧ������

        CacheResultList				m_list_ClientConnectResult;	// ����ͻ����첽���ӵķ��ؽ��
        base::CCriticalSection      m_lockClientConnect;		// client�����첽�ٽ���
        base::CUniqueKeyCreater		m_KeyCreate;				// key������
        uint32_t				    m_ServerType;				// ���������ͱ�ʾ
        uint32_t				    m_ServerID;					// ������ID
    };

}

