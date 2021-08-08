/****************************************************************
    �������ڣ�  2021��7��23��
    �ļ����ƣ�  ServerData.h
    ˵    ����  ��ܲ����ݶ���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include <stdint.h>
#include <string.h>
#include "ServerType.h"

//��Ϣͷ
struct  MsgHead
{
    uint16_t			usSize;                          // ��Ϣ���ݵĴ�С
    uint16_t			usType;                          // ��Ϣ������

    MsgHead() :usType(0), usSize(0) { ; }
    MsgHead(uint16_t _usType, uint16_t _usSize) :usType(_usType), usSize(_usSize) { ; }
};

struct MsgEncodeHead : public MsgHead
{
    unsigned char ucClientKey[4];	// �ͻ��˼��ܵ�key

    MsgEncodeHead()
    {
    }

    MsgEncodeHead(uint16_t _usType, uint16_t _usSize)
        : MsgHead(_usType, _usSize)
    {
    }
};

// ע����Ϣͷ
struct REGISTER_HEAD : public MsgHead
{
    uint32_t          unType;							// ����������
    uint32_t          unId;							    // ������id
    REGISTER_HEAD()
        : MsgHead(_MSG_SERVER_REGISTER, sizeof(*this))
        , unType(0)
        , unId(0)
    {

    }
};

struct MSG_ROUTING_MESSAGE : public MsgHead
{
    int32_t	nToServerType;		// Ŀ�����������
    int32_t	nToServerID;		// Ŀ�������ID(-1:����㲥ͬһ���͵����з�����)
    MSG_ROUTING_MESSAGE()
        : MsgHead(_MSG_ROUTING_MESSAGE, sizeof(*this))
        , nToServerType(-1)
        , nToServerID(-1)
    {
    }
};

namespace server
{
    class IMsg;

    // ��������·��Ϣ
    struct ServerLinkInfo
    {
        uint32_t	client_ServerType;					// [����Ҫ��ֵ]��Ϊ�ͻ��˵ķ���������(��ܲ��æ��ֵ)
        uint32_t	client_ServerId;					// [����Ҫ��ֵ]��Ϊ�ͻ��˵ķ�����ID(��ܲ��æ��ֵ)
        char        client_ServerName[MAX_SERVER_NAME]; // [����Ҫ��ֵ]��Ϊ�ͻ��˵ķ���������(��ܲ��æ��ֵ)
        char		client_ip[MAX_IP];					// [����Ҫ��ֵ]��Ϊ�ͻ��˵�IP
        uint16_t	client_port;						// [����Ҫ��ֵ]��Ϊ�ͻ��˵Ķ˿�
        int32_t		client_ReceiveBuffer;				// ��Ϊ�ͻ��˵Ľ��ջ�����
        int32_t		client_SendBuffer;					// ��Ϊ�ͻ��˵ķ��ͻ�����
        uint32_t	server_ServerType;					// ��Ϊ����˵ķ���������
        uint32_t	server_ServerId;					// [����Ҫ��ֵ]��Ϊ����˵ķ�����ID
        char        server_ServerName[MAX_SERVER_NAME]; // ��Ϊ����˵ķ���������
        char		server_ip[MAX_IP];					// ��Ϊ����˵�IP
        uint16_t	server_port;						// ��Ϊ����˵Ķ˿�
        int32_t		nPackParserType;					// ������� 0:���� 1:������ 2:֧��TGW(����) 3:֧��TGW(������)
        char		cValidate[MAX_SESSION_KEY];			// [����Ҫ��ֵ]��֤��(��ܲ��æ��ֵ)
        bool		bAutoConnect;						// �Ƿ��������
        uint32_t	unSocketID;							// [����Ҫ��ֵ]SocketID(�ͻ��˷���������)
        int32_t		nTerminalType;						// [����Ҫ��ֵ]��������(�ͻ��˷���������)

        ServerLinkInfo()
            : client_ServerType(0)
            , client_ServerId(0)
            , client_port(0)
            , client_ReceiveBuffer(0)
            , client_SendBuffer(0)
            , server_ServerType(0)
            , server_ServerId(0)
            , server_port(0)
            , nPackParserType(1)
            , bAutoConnect(true)
            , unSocketID(0)
            , nTerminalType(0)
        {
            memset(server_ServerName, 0, sizeof(server_ServerName));
            memset(client_ServerName, 0, sizeof(client_ServerName));
            memset(client_ip, 0, sizeof(client_ip));
            memset(server_ip, 0, sizeof(server_ip));
            memset(cValidate, 0, sizeof(cValidate));
        }
    };

    struct MSG_SERVER_REGISTER : public MsgHead
    {
        ServerLinkInfo	    data;
        MSG_SERVER_REGISTER()
            : MsgHead(_MSG_SERVER_REGISTER, sizeof(*this))
        {
        }
    };
}
