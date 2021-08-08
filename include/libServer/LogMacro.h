/****************************************************************

	创建日期：  2021年7月22日
    文件名称：  LogMacro.h
    说    明：  日志宏定义

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "ServerFramework.h"


//	控制台输出宏
#	define CONSOLE_WRITE(fmt, ...)					base::writeToConsole(fmt, __VA_ARGS__)

#	define CONSOLE_WRITE_EX(textColor, fmt, ...)	base::writeToConsole(textColor, fmt, __VA_ARGS__)

#   define LOG_EXCEPTION(fmt, ...)					base::saveErrorLog(__FILE__, __LINE__, fmt, __VA_ARGS__)

// while和for循环的安全控制
#	define FOR_OR_WHILE_DEFINE(maxSize)					\
	int32_t __nNum = 0;									\
	int32_t __nMaxSize = maxSize


#	define FOR_OR_WHILE_SAVE()							\
	if(__nNum++ >= __nMaxSize)							\
	{													\
		LOG_EXCEPTION(NULL);							\
		break;											\
	}
