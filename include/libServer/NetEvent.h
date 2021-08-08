/****************************************************************
    创建日期：  2021年7月24日
    文件名称：  NetEvent.h
    说    明：  通信层消息事件接口

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "ServerType.h"
#include "ServerData.h"

namespace server
{
    // 前置声明
    class IMsg;
    class CTerminal;
    class CConnection;

    namespace net_interface
    {
        class INetEvent
        {
        public:
            virtual ~INetEvent() {};

            // 解析消息包(通信层消息的统一入口)
            virtual void doResolveMsg(IMsg* pMsg) = 0;

            // Socket第一次连接上来触发
            virtual bool doOnConnect(IMsg* pMsg) = 0;

            // Socket断开连接触发
            virtual void doOnClose(uint32_t unSocketID) = 0;

            // 主动连接成功后触发
            virtual void doDoConnectSuccess(ServerLinkInfo& data) = 0;

            // 主动连接失败后触发
            virtual void doDoConnectFail(ServerLinkInfo& data) = 0;

            // 提供网站连接 不需要解析消息头
            virtual bool doWebConnect(IMsg* pMsg) = 0;
        };

        class IServerConnectEvent
        {
        public:
            virtual ~IServerConnectEvent() {}
            // 有新链接
            virtual void onBeConnected(CTerminal* pTerminal, CConnection* pCon) = 0;
        };

        class IClientConnectEvent
        {
        public:
            virtual ~IClientConnectEvent() {}

            // 连接服务器失败
            virtual void onConnectFail(CTerminal* pTerminal, ServerLinkInfo& data) = 0;

            // 连接服务器成功
            virtual void onConnectSuccess(CTerminal* pTerminal, CConnection* pCon, ServerLinkInfo& data) = 0;
        };

    };

}
