/****************************************************************
	创建日期：  2021年7月23日
	文件名称：  Console.h
	说    明：  控制台输入服务

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

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