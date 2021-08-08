/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  MessageSend.h
    说    明：  消息发送类

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma once
#include "IMessageSend.h"

namespace server
{

    class CMessageSend : public IMessageSend
    {
    public:
        CMessageSend();
        virtual ~CMessageSend();
        virtual base::CIStream& getIStream();

        virtual bool                sendMsg(void* pMsg, uint32_t len, uint32_t unSocketID);
        virtual bool				sendMsgByServerType(void* pMsg, uint32_t len, uint32_t unServerType);
        virtual bool				sendMsgByServerType(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t nServerId);
        virtual bool                sendMsgToAllServer(void* pMsg, uint32_t len, uint32_t unServerType);
        virtual bool				routingMsg(void* pMsg, uint32_t len, uint32_t unServerType, uint32_t unServerId);

    private:
        base::CIStream			    m_IStream;		// 动态结构体
        char* m_pBuffer;
    };

}
