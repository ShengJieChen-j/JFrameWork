/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  MessageSubject.h
    说    明：  系统主消息分发框架的主题

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "Subject.h"
#include "Observer.h"
#include "MessageService.h"
#include "MessageMapManager.h"
#include "MsgAdapter.h"

namespace server
{
    class CMessageSubject : public Subject<IMessageService>
    {
    private:
        typedef void (CMessageSubject::* pMsgChangeFunc)(const IMsg* pMsg);

    public:
        CMessageSubject();
        ~CMessageSubject();

    public:
        // <Subject接口实现>
        virtual void regInsideObserver(uint16_t usMessageType, IInsideObserver* observer);
        virtual void regObserver(uint16_t usMessageType, IObserver* observer);

        // <##实现来自Subject申明的方法>
        // <当状态改变时调用(拉数据方式)>
        virtual void onChange();

    public:
        // <解析消息包>
        void resolveMsg(IMsg* pMsg);
        // 解析大消息包
        void resolveMsg(IMsg* pMsg, const void* pExtraData, uint32_t nExtraSize);

    private:
        // <当状态改变时调用(推数据方式)>
        void onInsideChange(const IMsg* pMsg);
        void onChange(const IMsg* pMsg, const void* pExtraData, uint32_t nExtraSize);


    private:
        // <消息观察者容器>
        MessageMapManager<IInsideObserver>				m_map_InsideMsg;
        MessageMapManager<IObserver>					m_map_OutsideMsg;
    };

}


