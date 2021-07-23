/********************************************************************
	创建日期：  2021年7月24日
	文件名称：  SingletonTemplate.h
	说    明：  单例模式的泛型实现。本单元提供两个泛型单例类：

	分别实现了两个语义上的单例， 其中：

	1. 类CisSingleInstance<T>跟T是is a的关系，应用此模式则说明T类是可继承的，所以T类必需满足如下条件：
	1.1 默认构造函数和析构函数是public或protected的访问级别
	1.2 析构函数必需是virtual的

	2. 类ChaSingleInstance<T>跟T是has a的关系，即关联关系，应用此模式则说明T是ChaSingleInstance<T>里
	   的一个静态成员变量类型，所以T类必需满足如下条件：
	2.1 默认构造函数和析构函数是public的访问级别

	建议：
	1. 优先考虑使用ChaSingleInstance<T>
	2. typedef 将使代码更具可读性，
	   如：typedef ChaSingleInstance<T> TSingleInstance;

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：  创建

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
		/// <获取单例>
		static CiaSingleton* singleton();
		/// <释放单例>
		static void release();

	private:
		/// <单例实体指针>
		static CiaSingleton* pSingleInstance;

	private:
		CiaSingleton(const CiaSingleton&);
		const CiaSingleton& operator=(const CiaSingleton&);
	private:
		/// <此类不可继承，不可实例化>
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

		/// <获取单例>
		static SingleInstanceType* singleton();
		/// <释放单例>
		static void release();

	private:
		/// <单例实体指针>
		static SingleInstanceType* pSingleInstance;

	private:
		ChaSingleton(const ChaSingleton&);
		const ChaSingleton& operator=(const ChaSingleton&);
	private:
		/// <此类不可继承，不可实例化>
		ChaSingleton() {}
		~ChaSingleton() {}
	};

	/// <类CiaSingleInstance的定义>
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

	/// <类ChaSingleInstance的定义>
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
