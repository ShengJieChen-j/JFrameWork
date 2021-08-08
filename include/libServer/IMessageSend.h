/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  IMessageSend.h
    ˵    ����  ��Ϣ���ͽӿ�

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  ��֧�ֶ��߳�

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

