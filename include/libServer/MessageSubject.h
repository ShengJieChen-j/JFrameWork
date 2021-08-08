/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MessageSubject.h
    ˵    ����  ϵͳ����Ϣ�ַ���ܵ�����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "Subject.h"
#include "Observer.h"
#include "MessageService.h"
#include "MessageMapManager.h"
#include "MsgAdapter.h"

namespace server
{
    class CMessageSubject : public Subject<IMessageService>
    {
    private:
        typedef void (CMessageSubject::* pMsgChangeFunc)(const IMsg* pMsg);

    public:
        CMessageSubject();
        ~CMessageSubject();

    public:
        // <Subject�ӿ�ʵ��>
        virtual void regInsideObserver(uint16_t usMessageType, IInsideObserver* observer);
        virtual void regObserver(uint16_t usMessageType, IObserver* observer);

        // <##ʵ������Subject�����ķ���>
        // <��״̬�ı�ʱ����(�����ݷ�ʽ)>
        virtual void onChange();

    public:
        // <������Ϣ��>
        void resolveMsg(IMsg* pMsg);
        // ��������Ϣ��
        void resolveMsg(IMsg* pMsg, const void* pExtraData, uint32_t nExtraSize);

    private:
        // <��״̬�ı�ʱ����(�����ݷ�ʽ)>
        void onInsideChange(const IMsg* pMsg);
        void onChange(const IMsg* pMsg, const void* pExtraData, uint32_t nExtraSize);


    private:
        // <��Ϣ�۲�������>
        MessageMapManager<IInsideObserver>				m_map_InsideMsg;
        MessageMapManager<IObserver>					m_map_OutsideMsg;
    };

}


