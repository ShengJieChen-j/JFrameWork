/********************************************************************
    创建日期：  2021年7月18日
    文件名称：  Subject.h
    说    明：  观察者模式的泛型框架之主题

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

********************************************************************/
#pragma once

namespace server
{

    // 观察者模式中的主题基类, T为具体的主题行为
    template <typename T>
    class Subject : public T
    {
    public:
        typedef T SubjectType;

    public:
        virtual ~Subject() {}

        // 当状态改变时调用(拉数据方式)
        virtual void onChange() = 0;
    };

}
