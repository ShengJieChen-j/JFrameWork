/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  INetManager.h
    ˵    ����  �����ӿ�, ���ڳ���ͬ������ײ�ӿ�

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "NetPublicData.h"

namespace server
{
    namespace net_interface
    {
        class INetEvent;
    }

    class INetManager
    {
    public:
        virtual ~INetManager() {};
        virtual bool init(
            net_interface::INetEvent* pNetEvent,
            uint32_t unServerType,
            uint32_t unID) = 0;

        virtual bool run() = 0;
        virtual void release() = 0;

        // �������������IO
        virtual bool createServerNet(
            int32_t netType,
            const char* pIp,
            uint32_t unPort,
            uint32_t unRecvBufSize,
            uint32_t unSendBufSize,
            ePackParserType eType,
            int32_t nAF = 0,
            int32_t nType = 1,
            int32_t nProto = 6,
            uint32_t unMaxConnection = 0) = 0;


        // �������ӷ�����
        virtual bool connectTo(ServerLinkInfo& data) = 0;

        // ������Ϣ
        virtual bool sendMsg(uint32_t unSocketID, void* pMsg, uint32_t unSize) = 0;

        // �����Ͽ�Socket����
        virtual void disConnect(uint32_t unSocketID) = 0;

        // ��������
        virtual int32_t getTerminalType(uint32_t unSocketID) = 0;

        // ��������
        virtual bool setConnectType(uint32_t unSocketID, int32_t nType) = 0;
        virtual int32_t	getConnectType(uint32_t unSocketID) = 0;

        // ��������
        virtual int32_t	getNetType(uint32_t unSocketID) = 0;

        // ������ID
        virtual bool setID(uint32_t unSocketID, int64_t n64Id) = 0;
        virtual int64_t	getID(uint32_t unSocketID) = 0;

        // ����ȡ��Ϣ������
        virtual bool setMaxRead(uint32_t unSocketID, int32_t nCount) = 0;
        virtual int32_t	getMaxRead(uint32_t unSocketID) = 0;

        // ��Ϣ���Ͱ�ȫ��������
        virtual bool setMinMsgType(uint32_t unSocketID, int32_t nMessageType) = 0;
        virtual int32_t	getMinMsgType(uint32_t unSocketID) = 0;

        // ��Ϣ���Ͱ�ȫ��������
        virtual bool setMaxMsgType(uint32_t unSocketID, int32_t nMessageType) = 0;
        virtual int32_t	getMaxMsgType(uint32_t unSocketID) = 0;

        // ��ȡԶ��IP
        virtual const char* getRemoteAddress(uint32_t unSocketID) = 0;

        // ��ȡSocket�б�
        virtual std::vector<uint32_t> getSocketList() = 0;
    };

}
