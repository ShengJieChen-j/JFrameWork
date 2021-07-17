#include "stdafx.h"
#include "TimeTools.h"
#include "basecrt.h"
#include <stdio.h>
#include <string>
#include <Windows.h>

namespace base {

	namespace time_tool
	{
		#define MILLISECONDS_PER_SECOND 1000
		int32_t getZoneTime()
		{
			struct SZoneTime
			{
				SZoneTime()
				{
					_tzset();
					long nTimeZone = 0;
					_get_timezone(&nTimeZone);
					this->nZoneTime = (int32_t)nTimeZone;
				}

				int32_t nZoneTime;
			};

			static SZoneTime s_sZoneTime;
			return s_sZoneTime.nZoneTime;
		}

		time_t getGmtTime()
		{
			return time(NULL);
		}

		time_t getGmtTimeMilli()
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			struct tm tm;
			tm.tm_year = st.wYear - 1900;
			tm.tm_mon = st.wMonth - 1;
			tm.tm_mday = st.wDay;
			tm.tm_hour = st.wHour;
			tm.tm_min = st.wMinute;
			tm.tm_sec = st.wSecond;
			tm.tm_isdst = -1;

			time_t t = mktime(&tm);
			t = t * MILLISECONDS_PER_SECOND + st.wMilliseconds;
			return t;
		}

		STime* getGmtTm(STime* pTime)
		{
			if (pTime == NULL)
				return NULL;

			struct tm ttm;
			time_t nTime = getGmtTime();
			errno_t err = gmtime_s(&ttm, &nTime);
			if (err != 0)
				return NULL;

			pTime->nSec = ttm.tm_sec;
			pTime->nMin = ttm.tm_min;
			pTime->nHour = ttm.tm_hour;
			pTime->nDay = ttm.tm_mday;
			pTime->nWDay = ttm.tm_wday + 1;
			pTime->nMon = ttm.tm_mon + 1;
			pTime->nYear = (uint16_t)(1900 + ttm.tm_year);

			return pTime;
		}

		STime* getGmtTm(STime* pTime, time_t nTime)
		{
			if (pTime == NULL)
				return NULL;

			if (nTime <= 0)
				nTime = getGmtTime();
			struct tm ttm;
			errno_t err = gmtime_s(&ttm, &nTime);
			if (err != 0)
				return NULL;

			pTime->nSec = ttm.tm_sec;
			pTime->nMin = ttm.tm_min;
			pTime->nHour = ttm.tm_hour;
			pTime->nDay = ttm.tm_mday;
			pTime->nWDay = ttm.tm_wday + 1;
			pTime->nMon = ttm.tm_mon + 1;
			pTime->nYear = (uint16_t)(1900 + ttm.tm_year);

			return pTime;
		}

		time_t getLocalTime()
		{
			return getGmtTime() - getZoneTime();
		}

		STime* getLocalTm(STime* pTime)
		{
			if (pTime == NULL)
				return NULL;

			time_t nTime = getGmtTime();
			struct tm ttm;
			errno_t err = localtime_s(&ttm, &nTime);
			if (err != 0)
				return NULL;

			pTime->nSec = ttm.tm_sec;
			pTime->nMin = ttm.tm_min;
			pTime->nHour = ttm.tm_hour;
			pTime->nDay = ttm.tm_mday;
			pTime->nWDay = ttm.tm_wday + 1;
			pTime->nMon = ttm.tm_mon + 1;
			pTime->nYear = (uint16_t)(1900 + ttm.tm_year);

			return pTime;
		}

		STime* getLocalTm(STime* pTime, time_t nTime)
		{
			if (pTime == NULL)
				return NULL;

			if (nTime <= 0)
				nTime = getGmtTime();
			struct tm ttm;
			errno_t err = localtime_s(&ttm, &nTime);
			if (err != 0)
				return NULL;

			pTime->nSec = ttm.tm_sec;
			pTime->nMin = ttm.tm_min;
			pTime->nHour = ttm.tm_hour;
			pTime->nDay = ttm.tm_mday;
			pTime->nWDay = ttm.tm_wday + 1;
			pTime->nMon = ttm.tm_mon + 1;
			pTime->nYear = (uint16_t)(1900 + ttm.tm_year);

			return pTime;
		}

		std::string getLocalTimeStr()
		{
			char cTime[TT_MAX_BUFFER] = { 0 };
			formatLocalTime(cTime, sizeof(cTime), "%4d-%02d-%02d %02d:%02d:%02d");

			return cTime;
		}

		size_t formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat)
		{
			if (NULL == szBuf || szFormat == NULL)
				return 0;

			STime sTime;
			STime* pTime = getGmtTm(&sTime);
			if (NULL != pTime)
				return base::crt::snprintf(szBuf, nBufSize, _TRUNCATE, szFormat, pTime->nYear, pTime->nMon, pTime->nDay, pTime->nHour, pTime->nMin, pTime->nSec);

			return 0;
		}

		size_t formatGmtTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime)
		{
			if (NULL == szBuf || szFormat == NULL)
				return 0;

			STime sTime;
			STime* pTime = getGmtTm(&sTime, nTime);
			if (NULL != pTime)
				return base::crt::snprintf(szBuf, nBufSize, _TRUNCATE, szFormat, pTime->nYear, pTime->nMon, pTime->nDay, pTime->nHour, pTime->nMin, pTime->nSec);

			return 0;
		}

		size_t formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat)
		{
			if (NULL == szBuf || szFormat == NULL)
				return 0;

			STime sTime;
			STime* pTime = getLocalTm(&sTime);
			if (NULL != pTime)
				return base::crt::snprintf(szBuf, nBufSize, _TRUNCATE, szFormat, pTime->nYear, pTime->nMon, pTime->nDay, pTime->nHour, pTime->nMin, pTime->nSec);

			return 0;
		}

		size_t formatLocalTime(char* szBuf, size_t nBufSize, const char* szFormat, time_t nTime)
		{
			if (NULL == szBuf || szFormat == NULL)
				return 0;

			STime sTime;
			STime* pTime = getLocalTm(&sTime, nTime);
			if (NULL != pTime)
				return base::crt::snprintf(szBuf, nBufSize, _TRUNCATE, szFormat, pTime->nYear, pTime->nMon, pTime->nDay, pTime->nHour, pTime->nMin, pTime->nSec);

			return 0;
		}

		time_t conversionStrTimeToTime_t(const char* pStr)
		{
			if (NULL == pStr)
			{
				return -1;
			}

			// 数字判断
			std::string str = pStr;
			if (-1 != str.find_first_not_of("0123456789-: "))
			{
				return -1;
			}

			int count = 0;
			std::string::size_type pos;
			while (-1 != (pos = str.find_first_of("-: ")))
			{
				++count;
				if (count >= 1 && count <= 2)
				{
					if ('-' != str[pos])
					{
						return -1;
					}
				}
				else if (3 == count)
				{
					if (' ' != str[pos])
					{
						return -1;
					}
				}
				else if (count >= 4)
				{
					if (':' != str[pos])
					{
						return -1;
					}
				}
				str.erase(0, pos + 1);
			}

			if (count != 5)
			{
				return -1;
			}

			// unix stamp
			struct tm t;
			sscanf_s(pStr, "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
			t.tm_year -= 1900;
			t.tm_mon -= 1;
			time_t result = _mkgmtime(&t);
			return result;
		}

		time_t makeGmtTimeStamp(const STime& st)
		{
			struct tm time_info;
			time_info.tm_year = st.nYear - 1900;
			time_info.tm_mon = st.nMon - 1;
			time_info.tm_mday = st.nDay;
			time_info.tm_hour = st.nHour;
			time_info.tm_min = st.nMin;
			time_info.tm_sec = st.nSec;
			time_info.tm_wday = 0;
			time_info.tm_yday = 0;
			time_info.tm_isdst = -1;
#ifdef _WIN32
			return _mkgmtime(&time_info);
#elif defined(linux)
			return timegm(&time_info);
#else
#error platform is not supported.
#endif
		}

		time_t getCurDayZeroTime()
		{
			time_t locTime = base::time_tool::getGmtTime();
			struct tm stTm;
			gmtime_s(&stTm, &locTime);
			stTm.tm_hour = 0;
			stTm.tm_min = 0;
			stTm.tm_sec = 0;
			return _mkgmtime(&stTm);
		}

	} // endof namespace time_tool

};	// endof namespace base


#ifndef _WIN32
int32_t timeGetTime()
{
	int32_t uptime = 0;
	struct timespec on;
	if (clock_gettime(CLOCK_MONOTONIC, &on) == 0)
		uptime = on.tv_sec * 1000 + on.tv_nsec / 1000000;
	return uptime;

}
#endif
