/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  ServerNetEvent.h
    ˵    ����  ����ͨ�Ų��¼�

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

*********************************************************************/
#pragma once
#include "libServer/NetEvent.h"

namespace server
{

    // ǰ������
    class IMsg;

    class CServerNetEvent : public net_interface::INetEvent
    {
    public:
        CServerNetEvent();
        ~CServerNetEvent();

    public:
        //INetEvent�ӿ�ʵ��

        // ������Ϣ��(ͨ�Ų���Ϣ��ͳһ���)
        virtual void doResolveMsg(IMsg* pMsg);

        // Socket��һ��������������
        virtual bool doOnConnect(IMsg* pMsg);

        // Socket�Ͽ����Ӵ���
        virtual void doOnClose(uint32_t unSocketID);

        // �������ӳɹ��󴥷�
        virtual void doDoConnectSuccess(ServerLinkInfo& data);

        // ��������ʧ�ܺ󴥷�
        virtual void doDoConnectFail(ServerLinkInfo& data);

        // �ṩ��վ���� ����Ҫ������Ϣͷ
        virtual bool doWebConnect(IMsg* pMsg);
    };

}
