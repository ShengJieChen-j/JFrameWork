/******************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  ServerFramework.h
    ˵    ����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
        void			    startup(int32_t nCycle, uint32_t unServerType, const std::string& strConfig);		// ��������
        void			    stop();																				// �رշ���

    public:
        // �ṩ����ʵ�ֽӿ�
        virtual	bool        onInit() = 0; // ��ʼ���ӿ�(���ڿ�ܵĳ�ʼ��)
        virtual void        onRun() = 0; // �Զ���֡��
        virtual void        onRelease() = 0; // �ͷŽӿ�(���ڿ�ܵ��ͷ�)

        virtual bool        doOnConnect(server::IMsg* pMsg) = 0; // Socket��һ��������������
        virtual void        doOnClose(uint32_t unSocketID) = 0; // Socket�Ͽ����Ӵ���
        virtual void		onConnectivityReady() {}								 // ȫ��ͨ�¼�
        virtual int32_t		getRoutingServerId() { return 1; }						 // ·�ɷ�����ID

    public:
        // ����
        bool                listen(int32_t netType);
        bool                connect(const std::string& serverName, uint32_t serverType);
        bool				connect(ServerLinkInfo& data);
        void				disconnect(uint32_t unSocketId);

        // ��ʱ��
        void				registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
        void			    unRegistTicker(CTicker* pTicker);

        // ��Ϣ
        void                registMsg(uint16_t usMessageType, IObserver* observer);

        // ����
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
        //                  ��Ϣ���ͽӿ�
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

        // ���������Ҫ���ڱ����ڵ�
        template            <typename T>
        inline bool         sendMsgByServerType(T* pMsg, uint32_t unServerType)
        {
            CIStream& input_stream = m_pMessageSend->getIStream();
            if (pMsg->packData(input_stream))
                return m_pMessageSend->sendMsgByServerType(input_stream.getBuf(), input_stream.getPos(), unServerType);

            return false;
        }
        // ���������Ҫ����ȫ�ַ�����������disp������������������������Ϣ
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
            input_stream << unServerId;	// -1 ����㲥������unServerType���͵ķ�����
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
        // ���½ӿڽ�����ܲ�ʹ��
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
        // ����״̬
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
