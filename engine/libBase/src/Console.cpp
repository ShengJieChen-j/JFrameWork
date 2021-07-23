#include "stdafx.h"
#include "Console.h"
#include "SyncObjs.h"
#include "basecrt.h"
#include "ExceptionHandler.h"

#include <Windows.h>
#include <conio.h>
#include <TlHelp32.h>

#include <string>
#include <vector>
#include <list>

static DWORD getMainThreadId()
{
	DWORD nProcessId = GetCurrentProcessId();

	DWORD nThreadId = 0;
	THREADENTRY32 threadEntry = { sizeof(threadEntry) };
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (Thread32First(hThreadSnap, &threadEntry))
	{
		do
		{
			if (nProcessId == threadEntry.th32OwnerProcessID)
			{
				nThreadId = threadEntry.th32ThreadID;
				break;
			}
		} while (Thread32Next(hThreadSnap, &threadEntry));
	}

	CloseHandle(hThreadSnap);

	return nThreadId;
}

namespace base
{
	static const int32_t nCommandSize = 256;

	struct SCommandCallbackInfo
	{
		std::string szArg;
		funCommand	pfCommand;

		SCommandCallbackInfo() : pfCommand(NULL) { }
	};

	struct SConsoleInfo
	{
		std::vector<SCommandInfo*>			vecCommandInfo;
		std::list<SCommandCallbackInfo>		listCommandCallbackInfo;
		CThread* pThread;
		CCriticalSection					mutex;
		std::string							szServerName;

		SConsoleInfo()
		{
			this->pThread = NULL;
		}

		~SConsoleInfo()
		{
			SAFE_DELETE(this->pThread);

			for (size_t i = 0; i < this->vecCommandInfo.size(); ++i)
			{
				SAFE_DELETE_ARRAY(this->vecCommandInfo[i]->szName);
				SAFE_DELETE_ARRAY(this->vecCommandInfo[i]->szDescription);
				SAFE_DELETE(this->vecCommandInfo[i]);
			}
		}
	};

	CConsole::CConsole()
	{
		this->m_pConsoleInfo = new SConsoleInfo();
		this->m_pConsoleInfo->pThread = nullptr;
		this->m_nKeyCount = 0;
		memset(this->m_szKeyBuffer, 0, sizeof(this->m_szKeyBuffer));
	}

	CConsole::~CConsole()
	{
		SAFE_DELETE(this->m_pConsoleInfo);
	}

	void CConsole::registCommand(const SCommandInfo& sCommandInfo)
	{
		if (sCommandInfo.szName == NULL || sCommandInfo.szDescription == NULL || sCommandInfo.pfCommand == NULL)
			return;

		SCommandInfo* pCommandInfo = new SCommandInfo();
		pCommandInfo->pfCommand = sCommandInfo.pfCommand;
		size_t nLen = strnlen(sCommandInfo.szName, 100) + 1;
		pCommandInfo->szName = new char[nLen + 1];
		base::crt::strncpy(pCommandInfo->szName, nLen, sCommandInfo.szName, _TRUNCATE);
		nLen = strnlen(sCommandInfo.szDescription, 1024) + 1;
		pCommandInfo->szDescription = new char[nLen + 1];
		base::crt::strncpy(pCommandInfo->szDescription, nLen, sCommandInfo.szDescription, _TRUNCATE);

		CSingleLock guard(&this->m_pConsoleInfo->mutex);
		this->m_pConsoleInfo->vecCommandInfo.push_back(pCommandInfo);
	}

	uint32_t CConsole::getCommandInfoCount() const
	{
		CSingleLock guard(&this->m_pConsoleInfo->mutex);

		return (uint32_t)this->m_pConsoleInfo->vecCommandInfo.size();
	}

	const SCommandInfo* CConsole::getCommandInfo(uint32_t nIndex) const
	{
		CSingleLock guard(&this->m_pConsoleInfo->mutex);

		if (nIndex >= (uint32_t)this->m_pConsoleInfo->vecCommandInfo.size())
			return NULL;

		return this->m_pConsoleInfo->vecCommandInfo[nIndex];
	}

	bool CConsole::init(const char* szServerName)
	{
		if (szServerName == NULL)
			return false;

		this->m_pConsoleInfo->szServerName = szServerName;
		this->m_pConsoleInfo->pThread = CThread::CreateNew(*this, false, 3);

		return true;
	}

	void CConsole::run()
	{
		while (true)
		{
			CSingleLock guard(&this->m_pConsoleInfo->mutex);
			if (this->m_pConsoleInfo->listCommandCallbackInfo.empty())
				return;

			SCommandCallbackInfo& sCommandCallbackInfo = this->m_pConsoleInfo->listCommandCallbackInfo.front();
			printf_s("\n");
			sCommandCallbackInfo.pfCommand(sCommandCallbackInfo.szArg.c_str());
			printf_s("\nCommand> ");
			this->m_pConsoleInfo->listCommandCallbackInfo.pop_front();
		}
	}

	bool CConsole::parseCommand(const char* szCommandExpr)
	{
		if (nullptr == szCommandExpr)
			return false;

		// 移除末端的\n
		char szExpr[nCommandSize] = { 0 };
		base::crt::strncpy(szExpr, _countof(szExpr), szCommandExpr, _TRUNCATE);
		size_t nLen = strnlen(szExpr, sizeof(szExpr));
		if (nLen <= 0)
			return false;

		if (szExpr[nLen - 1] == '\n')
			szExpr[nLen - 1] = '\0';

		char* szContext = NULL;
		char* szCommand = base::crt::strtok(szExpr, " ", &szContext);
		if (NULL == szContext)
			return false;

		if (szCommand == NULL)
		{
			printf_s("Command> ");
			return false;
		}

#ifdef WIN32
		if (strncmp(szCommand, "callstack", 10) == 0)
		{
			DWORD dwThreadID = 0;
			dwThreadID = atoi(szContext);
			if (dwThreadID == 0)
				dwThreadID = getMainThreadId();

			// 打印调用栈
			HANDLE hThread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadID);
			if (NULL != hThread)
			{
				char szBuf[4096] = { 0 };
				base::GetCallStackString((void*)hThread, szBuf, _countof(szBuf));
				printf_s("\nCommand> %s\n", szBuf);
				::CloseHandle(hThread);
			}
			return true;
		}

		if (strncmp(szCommand, "cls", 4) == 0)
		{
			// 清屏
			system("cls");
			printf_s("\nCommand> ");
			return true;
		}

		/*if (strncmp(szCommand, "exit", 5) == 0 || strncmp(szCommand, "quit", 5) == 0)
		{
#ifdef _DEBUG

#else
			if (MessageBoxA(NULL, "Do you want to exit?", this->m_pConsoleInfo->szServerName.c_str(), MB_YESNO | MB_DEFBUTTON2) != IDYES)
				return true;
#endif

		}*/
#endif

		CSingleLock guard(&this->m_pConsoleInfo->mutex);
		SCommandInfo* pCommandInfo = NULL;
		for (size_t i = 0; i < this->m_pConsoleInfo->vecCommandInfo.size(); ++i)
		{
			if (strcmp(this->m_pConsoleInfo->vecCommandInfo[i]->szName, szCommand) == 0)
			{
				pCommandInfo = this->m_pConsoleInfo->vecCommandInfo[i];
				break;
			}
		}
		if (pCommandInfo == NULL)
		{
			printf_s("     ...[Unknown command!] : %s", szCommand);
			printf_s("\nCommand> ");
			return true;
		}
		SCommandCallbackInfo sCommandCallbackInfo;
		sCommandCallbackInfo.pfCommand = pCommandInfo->pfCommand;
		sCommandCallbackInfo.szArg = szContext;
		this->m_pConsoleInfo->listCommandCallbackInfo.push_back(sCommandCallbackInfo);

		return true;
	}

	int32_t	CConsole::OnThreadCreate()
	{
		return 1;
	}

	int32_t	CConsole::OnThreadDestroy()
	{
		return 1;
	}

	int32_t CConsole::OnThreadEvent()
	{
		return 1;
	}

	int32_t CConsole::OnThreadProcess()
	{
		if (kbhit())
		{
			// 	#ifdef WIN32
			// 			HANDLE hStdInput = ::GetStdHandle(STD_INPUT_HANDLE);
			// 			INPUT_RECORD key;
			// 			DWORD dwRet;
			// 			while (true)
			// 			{
			// 				::ReadConsoleInput(hStdInput, &key, 1, &dwRet);
			// 				if (key.EventType == KEY_EVENT)	//如果当前事件是键盘事件  
			// 				{  
			// 					if (key.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && key.Event.KeyEvent.bKeyDown) // 当前事件的虚拟键为Esc键
			// 						printf_s("\nCommand> ");
			// // 					else
			// // 						::WriteConsoleInput(hStdInput, &key, 1, &dwRet);
			// 					break;
			// 				}
			// 			}
			// 	#endif

			// 			char szExp[nCommandSize] = { 0 };
			// 			if (fgets( szExp, _countof(szExp), stdin) != NULL )
			// 				parseCommand( szExp  );

			int32_t key = getch();
			switch (key)
			{
			case 13:			// Enter
			{
				if (this->m_nKeyCount == 0)
				{
					printf_s("\nCommand> ");
					break;
				}
				parseCommand(this->m_szKeyBuffer);
				this->m_nKeyCount = 0;
				memset(this->m_szKeyBuffer, 0, sizeof(this->m_szKeyBuffer));
			}
			break;
			case 8:				// BackSpace
			{
				if (this->m_nKeyCount > 0)
				{
					printf("\b \b");
					this->m_szKeyBuffer[this->m_nKeyCount] = 0;
					this->m_nKeyCount--;
				}
			}
			break;
			case 27:			// ESC
			{
				this->m_nKeyCount = 0;
				memset(this->m_szKeyBuffer, 0, sizeof(this->m_szKeyBuffer));
				parseCommand(this->m_szKeyBuffer);
			}
			break;
			default:
			{
				if (this->m_nKeyCount >= sizeof(this->m_szKeyBuffer) - 1)
					break;

				this->m_szKeyBuffer[this->m_nKeyCount] = (char)key;
				this->m_nKeyCount++;
				printf("%c", key);
			}
			break;
			}
		}

		Sleep(50);

		return 1;
	}

	void CConsole::release()
	{
		SAFE_DELETE(this->m_pConsoleInfo->pThread);

		delete this;
	}

	CConsole* CConsole::createNew(const char* szServerName)
	{
		if (szServerName == NULL)
			return NULL;

		CConsole* pConsole = new CConsole();
		if (pConsole == NULL)
		{
			return NULL;
		}

		if (!pConsole->init(szServerName))
		{
			SAFE_DELETE(pConsole);
			return NULL;
		}

		return pConsole;
	}
}