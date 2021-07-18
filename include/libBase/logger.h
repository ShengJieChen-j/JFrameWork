/****************************************************************
    创建日期：  2021年7月15日
    文件名称：  logger.h
    说    明：  日志函数
    
    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  

******************************************************************/

#pragma once

#include <stdint.h>

namespace base
{
	bool	initLog(const char* szLogPath, const char* szPrefixName);
	void	uninitLog();
	void	saveLog(const char* szSection, bool bConsole, const char* szFormat, ...);
	void	saveLogEx(const char* szFileName, bool bConsole, const char* szFormat, ...);
	void	saveErrorLog(const char* szFile, int32_t nLine, const char* szFormat, ...);
	void	writeToConsole(const char *szFormat, ...);
	void	writeToConsole(uint32_t nColor, const char *szFormat, ...);
	void	flushLog();
}

#define LOG_FOREGROUND_BLUE				0x0001 // 蓝色
#define LOG_FOREGROUND_GREEN			0x0002 // 绿色
#define LOG_FOREGROUND_RED				0x0004 // 红色
#define LOG_FOREGROUND_INTENSITY		0x0008 // 颜色加深

#ifdef _DEBUG
#define PrintDebug( szFormat, ... )			base::saveLog( "DEBUG", true, szFormat, __VA_ARGS__ )
#else
#define PrintDebug( szFormat, ... )
#endif

#define PrintInfo( szFormat, ... )			base::saveLog( "INFO", false, szFormat, __VA_ARGS__ )
#define PrintWarning( szFormat, ... )		base::saveLog( "WARNING", true, szFormat, __VA_ARGS__ )
#define LogError( szFormat, ... )			base::saveErrorLog(__FILE__, __LINE__, szFormat, __VA_ARGS__)

#define LogSF(szFormat, ... )				base::saveLog("SF", false, szFormat, __VA_ARGS__)
