/********************************************************************
    创建日期:	2021年7月17日
    文件名称:	TimeTools.h
    说    明:	时间类型工具

    当前版本:	1.00
    作    者:	shengjiechen
    概    述:	支持线程安全,可重入

*********************************************************************/
#pragma once

#include <time.h>
#include "TypeDefine.h"
#include <string>

namespace base
{
    namespace time_tool
    {
        const int32_t TT_MAX_BUFFER = 32;

        struct STime
        {
            uint32_t nYear;	// 年	[0, -)
            uint32_t nMon;	// 月	[1,12]
            uint32_t nDay;	// 日	[1,31]
            uint32_t nWDay;	// 周日 [1, 7]
            uint32_t nHour;	// 时	[0,23]
            uint32_t nMin;	// 分	[0,59]
            uint32_t nSec;	// 秒	[0,59]

            STime()
                : nYear(0)
                , nMon(0)
                , nDay(0)
                , nWDay(0)
                , nHour(0)
                , nMin(0)
                , nSec(0)
            {
            }
        };

        // 获得系统时区
        int32_t	getZoneTime();
        // 获取格林威治时间(世界时间)
        time_t	getGmtTime();
        // 获取格林威治时间(世界时间 单位：毫秒)
        time_t getGmtTimeMilli();
        // 获取格林威治TM(世界时间)
        STime* getGmtTm(STime* pTime);
        // 获取格林威治TM(世界时间),如果nTime > 0就会将这个时间转成TM格式，不然就用当前的格林威治时间
        STime* getGmtTm(STime* pTime, time_t nTime);

        // 获取格本地时间
        time_t	getLocalTime();
        // 获取格本地TM
        STime* getLocalTm(STime* pTime);
        // 获取格本地TM,如果nTime > 0就会将这个时间转成TM格式，不然就用当前的本地时间
        STime* getLocalTm(STime* pTime, time_t nTime);
        std::string	getLocalTimeStr();	// 2010-01-01 01:01:01(本地时间+运营偏移的时区)

        // 格式化格林威治时间为字符串 szFormat的格式 %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS
        size_t	formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat);
        // 格式化格林威治时间为字符串 szFormat的格式 %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS，如果nTime > 0就会将这个时间格式化，不然就用当前的格林威治时间
        size_t	formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime);

        // 格式化本地时间为字符串 szFormat的格式 %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS
        size_t	formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat);
        // 格式化本地时间为字符串 szFormat的格式 %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS，如果nTime > 0就会将这个时间格式化，不然就用当前的本地时间
        size_t	formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime);

        time_t	conversionStrTimeToTime_t(const char* pStr);				// 时间字符串转换成time_t格式(字符串格式：2010-01-01 01:01:01)

        time_t makeGmtTimeStamp(const STime& st);                           // 时间类型转换为time_t格式(字符串格式：2010-01-01 01:01:01)

        time_t getCurDayZeroTime();				                            // 获取今天的0点时间

    }	// endof namespace time_tool

};	// endof namespace base

#ifndef _WIN32
int32_t	timeGetTime();
#endif
