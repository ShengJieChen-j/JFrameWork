/********************************************************************
    �������ڣ�  2021��7��18��
    �ļ����ƣ�  Subject.h
    ˵    ����  �۲���ģʽ�ķ��Ϳ��֮����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

********************************************************************/
#pragma once

namespace server
{

    // �۲���ģʽ�е��������, TΪ�����������Ϊ
    template <typename T>
    class Subject : public T
    {
    public:
        typedef T SubjectType;

    public:
        virtual ~Subject() {}

        // ��״̬�ı�ʱ����(�����ݷ�ʽ)
        virtual void onChange() = 0;
    };

}
