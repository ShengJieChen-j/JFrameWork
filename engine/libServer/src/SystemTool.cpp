#include "stdafx.h"
#include <windows.h>
#include <psapi.h>
#include "SystemTool.h"

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Psapi.lib")

namespace server
{

	void SystemTool::init()
	{
		SystemTool::getCpuUsage();
	}

	int32_t SystemTool::getProcessorNumber()
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return (int32_t)info.dwNumberOfProcessors;
	}

	int32_t SystemTool::getCpuUsage()
	{
		//cpu数量
		static int32_t processor_count_ = -1;
		//上一次的时间
		static int64_t last_time_ = 0;
		static int64_t last_system_time_ = 0;


		FILETIME now;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		int64_t system_time;
		int64_t time;
		int64_t system_time_delta;
		int64_t time_delta;
		int32_t cpu = -1;

		if (-1 == processor_count_)
		{
			processor_count_ = getProcessorNumber();
		}

		GetSystemTimeAsFileTime(&now);

		if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time))
		{
			// 不要在一些情况下assert，例如任务管理器中
			// 可以对刚刚退出但没收到通知的进程调用此函数
			return -1;
		}

		system_time = (filetimeToUTC(&kernel_time) + filetimeToUTC(&user_time)) / processor_count_;
		time = filetimeToUTC(&now);
		if ((last_system_time_ == 0) || (last_time_ == 0))
		{
			// 第一次调用，只需设置last值.
			last_system_time_ = system_time;
			last_time_ = time;
			return -1;
		}

		system_time_delta = system_time - last_system_time_;
		time_delta = time - last_time_;

		if (time_delta == 0)
			return -1;

		// 四舍五入
		cpu = (int32_t)((system_time_delta * 100 + time_delta / 2) / time_delta);
		last_system_time_ = system_time;
		last_time_ = time;

		return cpu;
	}

	int32_t SystemTool::getMemoryUsage(uint64_t* mem, uint64_t* vmem)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			if (mem)
				*mem = pmc.WorkingSetSize;

			if (vmem)
				*vmem = pmc.PagefileUsage;

			return 0;
		}

		return -1;
	}

	int32_t SystemTool::getIOBytes(uint64_t* read_bytes, uint64_t* write_bytes)
	{
		IO_COUNTERS io_counter;
		if (GetProcessIoCounters(GetCurrentProcess(), &io_counter))
		{
			if (read_bytes)
				*read_bytes = io_counter.ReadTransferCount;

			if (write_bytes)
				*write_bytes = io_counter.WriteTransferCount;

			return 0;
		}

		return -1;
	}

	uint64_t SystemTool::filetimeToUTC(const FILETIME* ftime)
	{
		if (NULL == ftime)
			return 0;

		LARGE_INTEGER li;
		li.LowPart = ftime->dwLowDateTime;
		li.HighPart = ftime->dwHighDateTime;
		return li.QuadPart;
	}

	void SystemTool::forbidWindowClose()
	{
		HWND hWnd = GetConsoleWindow();
		if (hWnd)
		{
			HMENU hMenu = GetSystemMenu(hWnd, FALSE);
			DeleteMenu(hMenu, SC_MAXIMIZE, MF_GRAYED | MF_DISABLED);
			DeleteMenu(hMenu, SC_SIZE, MF_GRAYED | MF_DISABLED);
			DeleteMenu(hMenu, SC_CLOSE, MF_GRAYED | MF_DISABLED);
		}
	}

	void SystemTool::forbidCtrlC()
	{
		SetConsoleCtrlHandler(SystemTool::ConsoleHandler, true);
	}

	BOOL WINAPI SystemTool::ConsoleHandler(DWORD msgType)
	{
		switch (msgType)
		{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:
		{
			return TRUE;
		}
		break;
		default:
			break;
		}

		return FALSE;
	}

}
