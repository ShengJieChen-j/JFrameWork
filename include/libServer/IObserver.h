/********************************************************************
    �������ڣ�  2021��7��17��
    �ļ����ƣ�  IObserver.h
    ˵    ����  ͨ�Źܵ��۲���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

********************************************************************/
#pragma once
#include "libBase/Base.h"

namespace server
{
    // ǰ������
    class IMsg;

    // <�ⲿͨ�Źܵ��۲���>
    class IObserver
    {
    public:
        virtual ~IObserver() {}

        // ��ͨ����Ϣ��Ӧ�ص�
        virtual void doOnMessage(const IMsg* pMsg) = 0;
        // ����64K����Ϣ��Ӧ�ص���һ�����ִ���Ϣ���ݶ��Զ������ݵ���ʽ����
        virtual void doOnMessage(const IMsg* pMsg, const void* pExtraData, uint32_t unExtraSize) = 0;
    };
}
