/****************************************************************
	�������ڣ�  2021��7��23��
	�ļ����ƣ�  Console.h
	˵    ����  ����̨�������

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����

******************************************************************/
#pragma once
#include "Thread.h"
#include "Base.h"
#include "noncopyable.h"

namespace base
{
	typedef void(*funCommand)(const char* szArg);
	struct SCommandInfo
	{
		char* szName;
		char* szDescription;
		funCommand	pfCommand;
	};

	struct SConsoleInfo;
	class CConsole
		: public IThreadEvent
		, public base::noncopyable_::noncopyable
	{

	public:
		void				run();
		void				release();

		static CConsole* createNew(const char* szServerName);
		void				registCommand(const SCommandInfo& sCommandInfo);
		const SCommandInfo* getCommandInfo(uint32_t nIndex) const;
		uint32_t			getCommandInfoCount() const;

	private:
		virtual int32_t			OnThreadCreate();
		virtual int32_t			OnThreadDestroy();
		virtual int32_t			OnThreadEvent();
		virtual int32_t			OnThreadProcess();

	private:
		bool				parseCommand(const char* szCommand);
		bool				init(const char* szServerName);

	private:
		CConsole();
		~CConsole();

	private:
		SConsoleInfo*		m_pConsoleInfo;
		char				m_szKeyBuffer[256];
		uint32_t			m_nKeyCount;
	};
}