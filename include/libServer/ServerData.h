/****************************************************************
    创建日期：  2021年7月23日
    文件名称：  ServerData.h
    说    明：  框架层数据定义

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include <stdint.h>
#include <string.h>
#include "ServerType.h"

//消息头
struct  MsgHead
{
    uint16_t			usSize;                          // 消息内容的大小
    uint16_t			usType;                          // 消息的类型

    MsgHead() :usType(0), usSize(0) { ; }
    MsgHead(uint16_t _usType, uint16_t _usSize) :usType(_usType), usSize(_usSize) { ; }
};

struct MsgEncodeHead : public MsgHead
{
    unsigned char ucClientKey[4];	// 客户端加密的key

    MsgEncodeHead()
    {
    }

    MsgEncodeHead(uint16_t _usType, uint16_t _usSize)
        : MsgHead(_usType, _usSize)
    {
    }
};

// 注册消息头
struct REGISTER_HEAD : public MsgHead
{
    uint32_t          unType;							// 服务器类型
    uint32_t          unId;							    // 服务器id
    REGISTER_HEAD()
        : MsgHead(_MSG_SERVER_REGISTER, sizeof(*this))
        , unType(0)
        , unId(0)
    {

    }
};

struct MSG_ROUTING_MESSAGE : public MsgHead
{
    int32_t	nToServerType;		// 目标服务器类型
    int32_t	nToServerID;		// 目标服务器ID(-1:代表广播同一类型的所有服务器)
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

    // 服务器链路信息
    struct ServerLinkInfo
    {
        uint32_t	client_ServerType;					// [不需要赋值]做为客户端的服务器类型(框架层帮忙赋值)
        uint32_t	client_ServerId;					// [不需要赋值]做为客户端的服务器ID(框架层帮忙赋值)
        char        client_ServerName[MAX_SERVER_NAME]; // [不需要赋值]做为客户端的服务器名称(框架层帮忙赋值)
        char		client_ip[MAX_IP];					// [不需要赋值]做为客户端的IP
        uint16_t	client_port;						// [不需要赋值]做为客户端的端口
        int32_t		client_ReceiveBuffer;				// 做为客户端的接收缓冲区
        int32_t		client_SendBuffer;					// 做为客户端的发送缓冲区
        uint32_t	server_ServerType;					// 做为服务端的服务器类型
        uint32_t	server_ServerId;					// [不需要赋值]做为服务端的服务器ID
        char        server_ServerName[MAX_SERVER_NAME]; // 做为服务端的服务器名称
        char		server_ip[MAX_IP];					// 做为服务端的IP
        uint16_t	server_port;						// 做为服务端的端口
        int32_t		nPackParserType;					// 打包类型 0:加密 1:不加密 2:支持TGW(加密) 3:支持TGW(不加密)
        char		cValidate[MAX_SESSION_KEY];			// [不需要赋值]验证码(框架层帮忙赋值)
        bool		bAutoConnect;						// 是否断线重连
        uint32_t	unSocketID;							// [不需要赋值]SocketID(客户端服务器共用)
        int32_t		nTerminalType;						// [不需要赋值]端子类型(客户端服务器共用)

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
