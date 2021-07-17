/********************************************************************
    创建日期：  2021年7月17日
    文件名称：  IObserver.h
    说    明：  通信管道观察者

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma once
#include "libBase/Base.h"

namespace server
{
    // 前置声明
    class IMsg;

    // <外部通信管道观察者>
    class IObserver
    {
    public:
        virtual ~IObserver() {}

        // 普通的消息响应回调
        virtual void doOnMessage(const IMsg* pMsg) = 0;
        // 超过64K的消息响应回调，一般这种大消息内容都以额外数据的形式给出
        virtual void doOnMessage(const IMsg* pMsg, const void* pExtraData, uint32_t unExtraSize) = 0;
    };
}
