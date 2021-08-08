/****************************************************************
    创建日期：  2021年7月24日
    文件名称：  MessageService.h
    说    明：  信息服务开放接口API

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once
#include <stdint.h>

namespace server
{
    // 前置定义：观信息类型
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
