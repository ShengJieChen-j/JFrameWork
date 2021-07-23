/********************************************************************
	�������ڣ�  2021��7��24��
	�ļ����ƣ�  SingletonTemplate.h
	˵    ����  ����ģʽ�ķ���ʵ�֡�����Ԫ�ṩ�������͵����ࣺ

	�ֱ�ʵ�������������ϵĵ����� ���У�

	1. ��CisSingleInstance<T>��T��is a�Ĺ�ϵ��Ӧ�ô�ģʽ��˵��T���ǿɼ̳еģ�����T�������������������
	1.1 Ĭ�Ϲ��캯��������������public��protected�ķ��ʼ���
	1.2 ��������������virtual��

	2. ��ChaSingleInstance<T>��T��has a�Ĺ�ϵ����������ϵ��Ӧ�ô�ģʽ��˵��T��ChaSingleInstance<T>��
	   ��һ����̬��Ա�������ͣ�����T�������������������
	2.1 Ĭ�Ϲ��캯��������������public�ķ��ʼ���

	���飺
	1. ���ȿ���ʹ��ChaSingleInstance<T>
	2. typedef ��ʹ������߿ɶ��ԣ�
	   �磺typedef ChaSingleInstance<T> TSingleInstance;

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����  ����

*********************************************************************/
#pragma once

#include <stddef.h>		// for NULL declaration

namespace base {

	template <typename T>
	class CiaSingleton : public T
	{
	private:
		struct destroy
		{
			destroy(T** p) : m_p(p) {}
			~destroy()
			{
				if (*m_p)
				{
					delete (*m_p);
					*m_p = NULL;
					m_p = NULL;
				}
			}
			T** m_p;
		};
	public:
		typedef T SingletonType;

	public:
		/// <��ȡ����>
		static CiaSingleton* singleton();
		/// <�ͷŵ���>
		static void release();

	private:
		/// <����ʵ��ָ��>
		static CiaSingleton* pSingleInstance;

	private:
		CiaSingleton(const CiaSingleton&);
		const CiaSingleton& operator=(const CiaSingleton&);
	private:
		/// <���಻�ɼ̳У�����ʵ����>
		CiaSingleton() {}
		~CiaSingleton() {}
	};

	template <typename T>
	class ChaSingleton
	{
	private:
		struct destroy
		{
			destroy(T** p) : m_p(p) {}
			~destroy()
			{
				if (*m_p)
				{
					delete (*m_p);
					*m_p = NULL;
					m_p = NULL;
				}
			}
			T** m_p;
		};
	public:
		typedef T SingletonType;
	public:
		typedef T SingleInstanceType;

		/// <��ȡ����>
		static SingleInstanceType* singleton();
		/// <�ͷŵ���>
		static void release();

	private:
		/// <����ʵ��ָ��>
		static SingleInstanceType* pSingleInstance;

	private:
		ChaSingleton(const ChaSingleton&);
		const ChaSingleton& operator=(const ChaSingleton&);
	private:
		/// <���಻�ɼ̳У�����ʵ����>
		ChaSingleton() {}
		~ChaSingleton() {}
	};

	/// <��CiaSingleInstance�Ķ���>
	template <typename T>
	CiaSingleton<T>* CiaSingleton<T>::pSingleInstance = NULL;

	template <typename T>
	CiaSingleton<T>* CiaSingleton<T>::singleton()
	{
		if (NULL == pSingleInstance)
		{
			pSingleInstance = new CiaSingleton<T>();
			static destroy des(&pSingleInstance);
		}

		return pSingleInstance;
	}

	template <typename T>
	void CiaSingleton<T>::release()
	{
		if (pSingleInstance)
		{
			delete pSingleInstance;
			pSingleInstance = NULL;
		}
	}

	/// <��ChaSingleInstance�Ķ���>
	template <typename T>
	typename ChaSingleton<T>::SingleInstanceType* ChaSingleton<T>::pSingleInstance = NULL;

	template <typename T>
	typename ChaSingleton<T>::SingleInstanceType* ChaSingleton<T>::singleton()
	{
		if (NULL == pSingleInstance)
		{
			pSingleInstance = new SingleInstanceType();
			static destroy des(&pSingleInstance);
		}

		return pSingleInstance;
	}

	template <typename T>
	void ChaSingleton<T>::release()
	{
		if (pSingleInstance)
		{
			delete pSingleInstance;
			pSingleInstance = NULL;
		}
	}

};	// endof namespace base
