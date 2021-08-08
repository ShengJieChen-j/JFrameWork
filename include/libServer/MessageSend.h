/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MessageSend.h
    ˵    ����  ��Ϣ������

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
        base::CIStream			    m_IStream;		// ��̬�ṹ��
        char* m_pBuffer;
    };

}
