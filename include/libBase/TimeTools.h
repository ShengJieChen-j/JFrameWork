/********************************************************************
    ��������:	2021��7��17��
    �ļ�����:	TimeTools.h
    ˵    ��:	ʱ�����͹���

    ��ǰ�汾:	1.00
    ��    ��:	shengjiechen
    ��    ��:	֧���̰߳�ȫ,������

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
            uint32_t nYear;	// ��	[0, -)
            uint32_t nMon;	// ��	[1,12]
            uint32_t nDay;	// ��	[1,31]
            uint32_t nWDay;	// ���� [1, 7]
            uint32_t nHour;	// ʱ	[0,23]
            uint32_t nMin;	// ��	[0,59]
            uint32_t nSec;	// ��	[0,59]

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

        // ���ϵͳʱ��
        int32_t	getZoneTime();
        // ��ȡ��������ʱ��(����ʱ��)
        time_t	getGmtTime();
        // ��ȡ��������ʱ��(����ʱ�� ��λ������)
        time_t getGmtTimeMilli();
        // ��ȡ��������TM(����ʱ��)
        STime* getGmtTm(STime* pTime);
        // ��ȡ��������TM(����ʱ��),���nTime > 0�ͻὫ���ʱ��ת��TM��ʽ����Ȼ���õ�ǰ�ĸ�������ʱ��
        STime* getGmtTm(STime* pTime, time_t nTime);

        // ��ȡ�񱾵�ʱ��
        time_t	getLocalTime();
        // ��ȡ�񱾵�TM
        STime* getLocalTm(STime* pTime);
        // ��ȡ�񱾵�TM,���nTime > 0�ͻὫ���ʱ��ת��TM��ʽ����Ȼ���õ�ǰ�ı���ʱ��
        STime* getLocalTm(STime* pTime, time_t nTime);
        std::string	getLocalTimeStr();	// 2010-01-01 01:01:01(����ʱ��+��Ӫƫ�Ƶ�ʱ��)

        // ��ʽ����������ʱ��Ϊ�ַ��� szFormat�ĸ�ʽ %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS
        size_t	formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat);
        // ��ʽ����������ʱ��Ϊ�ַ��� szFormat�ĸ�ʽ %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS�����nTime > 0�ͻὫ���ʱ���ʽ������Ȼ���õ�ǰ�ĸ�������ʱ��
        size_t	formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime);

        // ��ʽ������ʱ��Ϊ�ַ��� szFormat�ĸ�ʽ %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS
        size_t	formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat);
        // ��ʽ������ʱ��Ϊ�ַ��� szFormat�ĸ�ʽ %dY-%dm-%dd %dH:%dM:%dS %dY%dm%dd%dH%dM%dS�����nTime > 0�ͻὫ���ʱ���ʽ������Ȼ���õ�ǰ�ı���ʱ��
        size_t	formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime);

        time_t	conversionStrTimeToTime_t(const char* pStr);				// ʱ���ַ���ת����time_t��ʽ(�ַ�����ʽ��2010-01-01 01:01:01)

        time_t makeGmtTimeStamp(const STime& st);                           // ʱ������ת��Ϊtime_t��ʽ(�ַ�����ʽ��2010-01-01 01:01:01)

        time_t getCurDayZeroTime();				                            // ��ȡ�����0��ʱ��

    }	// endof namespace time_tool

};	// endof namespace base

#ifndef _WIN32
int32_t	timeGetTime();
#endif
