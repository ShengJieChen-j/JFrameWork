/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  IMessageSend.h
    说    明：  消息发送接口

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  不支持多线程

********************************************************************/
#pragma once
#include "libBase/IStream.h"
#include <stdint.h>

namespace server
{

    class IMessageSend
    {
    public:
        virtual ~IMessageSend() {};
        virtual base::CIStream& getIStream() = 0;

        virtual bool                sendMsg(void* pMsg, uint32_t len, uint32_t unSocketID) = 0;
        virtual bool				sendMsgByServerType(void* pMsg, uint32_t len, uint32_t unServerType) = 0;
        virtual bool				sendMsgByServerType(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t nServerId) = 0;
        virtual bool                sendMsgToAllServer(void* pMsg, uint32_t len, uint32_t unServerType) = 0;
        virtual bool				routingMsg(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t unServerId) = 0;
    };

}

