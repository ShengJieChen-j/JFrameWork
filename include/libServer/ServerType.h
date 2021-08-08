/****************************************************************
    创建日期：  2021年7月23日
    文件名称：  ServerType.h
    说    明：  框架层类型定义

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once
#include <stdint.h>

// 核心消息协议
enum CORE_MESSAGE_TYPE
{
    _MSG_SERVER_REGISTER = 1,    // 服务器间注册
    _MSG_KEEPLIVE = 2,    // 心跳包
    _MSG_ROUTING_MESSAGE = 3,    // 消息路由
};


// IOCP类型
const int32_t CLIENT_IOCP = 1;    // 客户端IOCP
const int32_t SERVER_IOCP = 2;	// 服务器IOCP

// NET类型
static const int32_t NetType_Snet = 1;	// 外部网络
static const int32_t NetType_Inet = 2;	// 内部网络

//通信相关
const uint32_t MAX_SERVER_NAME = 64;	// 服务器名称最大长度
const uint32_t MAX_SESSION_KEY = 33;	// 验证码的最大长度
const uint32_t MAX_IP = 20;	// IP最大长度
const uint32_t CONNECT_DISCONNECT_TIME = 1200;	// 20分钟链接没有心跳就断开链接
const uint32_t CONNECT_KEEPLIVE_TIME = 60;	// 心跳包间隔时间(60秒发一次)
