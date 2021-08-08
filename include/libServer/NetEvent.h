/****************************************************************
    �������ڣ�  2021��7��24��
    �ļ����ƣ�  NetEvent.h
    ˵    ����  ͨ�Ų���Ϣ�¼��ӿ�

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "ServerType.h"
#include "ServerData.h"

namespace server
{
    // ǰ������
    class IMsg;
    class CTerminal;
    class CConnection;

    namespace net_interface
    {
        class INetEvent
        {
        public:
            virtual ~INetEvent() {};

            // ������Ϣ��(ͨ�Ų���Ϣ��ͳһ���)
            virtual void doResolveMsg(IMsg* pMsg) = 0;

            // Socket��һ��������������
            virtual bool doOnConnect(IMsg* pMsg) = 0;

            // Socket�Ͽ����Ӵ���
            virtual void doOnClose(uint32_t unSocketID) = 0;

            // �������ӳɹ��󴥷�
            virtual void doDoConnectSuccess(ServerLinkInfo& data) = 0;

            // ��������ʧ�ܺ󴥷�
            virtual void doDoConnectFail(ServerLinkInfo& data) = 0;

            // �ṩ��վ���� ����Ҫ������Ϣͷ
            virtual bool doWebConnect(IMsg* pMsg) = 0;
        };

        class IServerConnectEvent
        {
        public:
            virtual ~IServerConnectEvent() {}
            // ��������
            virtual void onBeConnected(CTerminal* pTerminal, CConnection* pCon) = 0;
        };

        class IClientConnectEvent
        {
        public:
            virtual ~IClientConnectEvent() {}

            // ���ӷ�����ʧ��
            virtual void onConnectFail(CTerminal* pTerminal, ServerLinkInfo& data) = 0;

            // ���ӷ������ɹ�
            virtual void onConnectSuccess(CTerminal* pTerminal, CConnection* pCon, ServerLinkInfo& data) = 0;
        };

    };

}
