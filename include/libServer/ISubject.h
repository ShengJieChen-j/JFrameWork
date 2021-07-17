/********************************************************************
    �������ڣ�  2021��7��17��
    �ļ����ƣ�  ISubject.h
    ˵    ����  �۲���ģʽ�ķ��Ϳ��֮����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

********************************************************************/
#pragma once

namespace server
{
    // �۲���ģʽ�е�������࣬TΪ�����������Ϊ
    template<typename T>
	class ISubject
	{
	public:
        typedef T SubjectType;

    public:
        virtual ~ISubject() {}

        virtual void onChange() = 0;        // ״̬�ı�ʱ�����(���ڼ�����Ϣ)
	};
}
