/********************************************************************
    创建日期：  2021年7月17日
    文件名称：  ISubject.h
    说    明：  观察者模式的泛型框架之主题

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma once

namespace server
{
    // 观察者模式中的主题基类，T为具体的主题行为
    template<typename T>
	class ISubject
	{
	public:
        typedef T SubjectType;

    public:
        virtual ~ISubject() {}

        virtual void onChange() = 0;        // 状态改变时会调用(用于监听消息)
	};
}
