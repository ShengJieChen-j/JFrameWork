/****************************************************************
    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MessageService.h
    ˵    ����  ��Ϣ���񿪷Žӿ�API

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once
#include <stdint.h>

namespace server
{
    // ǰ�ö��壺����Ϣ����
    class IInsideObserver;
    class IObserver;
    class IMsg;

    class IMessageService
    {
    public:
        virtual ~IMessageService() {}

        virtual void regInsideObserver(uint16_t usMessageType, IInsideObserver* observer) = 0;
        virtual void regObserver(uint16_t usMessageType, IObserver* observer) = 0;
    };

}
