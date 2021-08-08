/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  ServerNetEvent.h
    说    明：  处理通信层事件

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

*********************************************************************/
#pragma once
#include "libServer/NetEvent.h"

namespace server
{

    // 前置声明
    class IMsg;

    class CServerNetEvent : public net_interface::INetEvent
    {
    public:
        CServerNetEvent();
        ~CServerNetEvent();

    public:
        //INetEvent接口实现

        // 解析消息包(通信层消息的统一入口)
        virtual void doResolveMsg(IMsg* pMsg);

        // Socket第一次连接上来触发
        virtual bool doOnConnect(IMsg* pMsg);

        // Socket断开连接触发
        virtual void doOnClose(uint32_t unSocketID);

        // 主动连接成功后触发
        virtual void doDoConnectSuccess(ServerLinkInfo& data);

        // 主动连接失败后触发
        virtual void doDoConnectFail(ServerLinkInfo& data);

        // 提供网站连接 不需要解析消息头
        virtual bool doWebConnect(IMsg* pMsg);
    };

}
