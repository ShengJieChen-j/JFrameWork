/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  AsioNetManager.h
    说    明：  asio 通信层管理

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  主要功能:
                1. Terminal类封装io_service，管理链接的建立与断开
                2. Connection类封装Socket，  管理消息的收发
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
    // 前置声明
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
        // 创建服务端网络IO
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

        // 主动连接服务器
        virtual bool connectTo(ServerLinkInfo& data);

        // 发送消息
        virtual bool sendMsg(
            uint32_t unSocketID,
            void* pMsg,
            uint32_t unSize);

        // 主动断开Socket链接
        virtual void disConnect(uint32_t unSocketID);

    public:
        // 端子类型
        virtual int32_t getTerminalType(uint32_t unSocketID);

        // 链接类型
        virtual bool setConnectType(uint32_t unSocketID, int32_t nType);
        virtual int32_t	 getConnectType(uint32_t unSocketID);

        // 网络类型
        virtual int32_t	getNetType(uint32_t unSocketID);

        // 服务器ID
        virtual bool setID(uint32_t unSocketID, int64_t n64Id);
        virtual int64_t	 getID(uint32_t unSocketID);

        // 最大读取消息包数量
        virtual bool setMaxRead(uint32_t unSocketID, int32_t nCount);
        virtual int32_t	 getMaxRead(uint32_t unSocketID);

        // 消息类型安全区间下限
        virtual bool setMinMsgType(uint32_t unSocketID, int32_t nMessageType);
        virtual int32_t	 getMinMsgType(uint32_t unSocketID);

        // 消息类型安全区间上限
        virtual bool setMaxMsgType(uint32_t unSocketID, int32_t nMessageType);
        virtual int32_t	 getMaxMsgType(uint32_t unSocketID);

        // 获取远程IP
        virtual const char* getRemoteAddress(uint32_t unSocketID);

        // 获取Socket列表
        virtual std::vector<uint32_t> getSocketList();

    public:	// ASIO 通信事件
        // 有新链接
        virtual void onBeConnected(CTerminal* pTerminal, CConnection* pCon);

        // 连接服务器失败
        virtual void onConnectFail(CTerminal* pTerminal, ServerLinkInfo& data);

        // 连接服务器成功
        virtual void onConnectSuccess(CTerminal* pTerminal, CConnection* pCon, ServerLinkInfo& data);

    private:
        // 添加IOCP端子
        bool addTerminal(CTerminal* pTerminal);

        // 添加链接
        bool addConnection(CConnection* pConnection);

        // 删除链接
        void delConnection(CConnection* pConnection);

        // 添加首次连接上来的链接
        bool addFirstConnection(CTerminal* pTerminal, CConnection* pConnection);

        // 添加失效的链接
        void pushDelConnection(CConnection* pConnection);

    private:
        // 监听新的链接
        void listenNewConnectionsRun();

        // 处理首次链接
        void firstConnectionsRun();

        // 处理链接的读写
        void connectionsRun();

        // 处理失效链接
        void delConnectionsRun();

        // 处理客户端异步链接返回
        void clientConnectResultRun();

    private:
        net_interface::INetEvent* m_pNetEvent;				// 通信事件通知
        TerminalVec					m_vector_Terminal;			// 服务端IOCP，Listen Socket
        ConnectMap					m_map_Connections;			// 保存所有的链接
        CacheConnectMap				m_map_FirstConnections;		// 缓存首次连接上来的链接(处理一个消息的粘包问题)
        CacheConnectList			m_list_DelConnections;		// 缓存失效的链接

        CacheResultList				m_list_ClientConnectResult;	// 缓存客户端异步链接的返回结果
        base::CCriticalSection      m_lockClientConnect;		// client链接异步临界区
        base::CUniqueKeyCreater		m_KeyCreate;				// key生成器
        uint32_t				    m_ServerType;				// 服务器类型标示
        uint32_t				    m_ServerID;					// 服务器ID
    };

}

