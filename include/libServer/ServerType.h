/****************************************************************
    �������ڣ�  2021��7��23��
    �ļ����ƣ�  ServerType.h
    ˵    ����  ��ܲ����Ͷ���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once
#include <stdint.h>

// ������ϢЭ��
enum CORE_MESSAGE_TYPE
{
    _MSG_SERVER_REGISTER = 1,    // ��������ע��
    _MSG_KEEPLIVE = 2,    // ������
    _MSG_ROUTING_MESSAGE = 3,    // ��Ϣ·��
};


// IOCP����
const int32_t CLIENT_IOCP = 1;    // �ͻ���IOCP
const int32_t SERVER_IOCP = 2;	// ������IOCP

// NET����
static const int32_t NetType_Snet = 1;	// �ⲿ����
static const int32_t NetType_Inet = 2;	// �ڲ�����

//ͨ�����
const uint32_t MAX_SERVER_NAME = 64;	// ������������󳤶�
const uint32_t MAX_SESSION_KEY = 33;	// ��֤�����󳤶�
const uint32_t MAX_IP = 20;	// IP��󳤶�
const uint32_t CONNECT_DISCONNECT_TIME = 1200;	// 20��������û�������ͶϿ�����
const uint32_t CONNECT_KEEPLIVE_TIME = 60;	// ���������ʱ��(60�뷢һ��)
