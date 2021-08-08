/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  INetManager.h
    说    明：  网络层接口, 用于抽象不同的网络底层接口

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "NetPublicData.h"

namespace server
{
    namespace net_interface
    {
        class INetEvent;
    }

    class INetManager
    {
    public:
        virtual ~INetManager() {};
        virtual bool init(
            net_interface::INetEvent* pNetEvent,
            uint32_t unServerType,
            uint32_t unID) = 0;

        virtual bool run() = 0;
        virtual void release() = 0;

        // 创建服务端网络IO
        virtual bool createServerNet(
            int32_t netType,
            const char* pIp,
            uint32_t unPort,
            uint32_t unRecvBufSize,
            uint32_t unSendBufSize,
            ePackParserType eType,
            int32_t nAF = 0,
            int32_t nType = 1,
            int32_t nProto = 6,
            uint32_t unMaxConnection = 0) = 0;


        // 主动连接服务器
        virtual bool connectTo(ServerLinkInfo& data) = 0;

        // 发送消息
        virtual bool sendMsg(uint32_t unSocketID, void* pMsg, uint32_t unSize) = 0;

        // 主动断开Socket链接
        virtual void disConnect(uint32_t unSocketID) = 0;

        // 端子类型
        virtual int32_t getTerminalType(uint32_t unSocketID) = 0;

        // 链接类型
        virtual bool setConnectType(uint32_t unSocketID, int32_t nType) = 0;
        virtual int32_t	getConnectType(uint32_t unSocketID) = 0;

        // 网络类型
        virtual int32_t	getNetType(uint32_t unSocketID) = 0;

        // 服务器ID
        virtual bool setID(uint32_t unSocketID, int64_t n64Id) = 0;
        virtual int64_t	getID(uint32_t unSocketID) = 0;

        // 最大读取消息包数量
        virtual bool setMaxRead(uint32_t unSocketID, int32_t nCount) = 0;
        virtual int32_t	getMaxRead(uint32_t unSocketID) = 0;

        // 消息类型安全区间下限
        virtual bool setMinMsgType(uint32_t unSocketID, int32_t nMessageType) = 0;
        virtual int32_t	getMinMsgType(uint32_t unSocketID) = 0;

        // 消息类型安全区间上限
        virtual bool setMaxMsgType(uint32_t unSocketID, int32_t nMessageType) = 0;
        virtual int32_t	getMaxMsgType(uint32_t unSocketID) = 0;

        // 获取远程IP
        virtual const char* getRemoteAddress(uint32_t unSocketID) = 0;

        // 获取Socket列表
        virtual std::vector<uint32_t> getSocketList() = 0;
    };

}
