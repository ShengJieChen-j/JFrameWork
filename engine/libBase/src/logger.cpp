#include "stdafx.h"
#include "logger.h"
#include "Thread.h"
#include "ExceptionHandler.h"
#include <Mmsystem.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "Base.h"

// 日志功能是一个非常基础的功能，所以他不能依赖于项目中其他任何函数，不然可能会循环调用

#define _LOG_FILE_SIZE		1024*1024*20
#define _FLUSH_LOG_TIME		10

#define _LOG_BUF_SIZE		1024*8
#define _LOG_FILE_NAME_SIZE 32

static int32_t formatLog(char* szBuf, size_t nBufSize, const char* szSection, const char* szFormat, va_list arg, bool bEndLine, uint8_t* nDay)
{
	if (szBuf == nullptr || szSection == nullptr)
		return -1;

	SYSTEMTIME time;
	GetLocalTime(&time);
	if (nDay != NULL)
		*nDay = (uint8_t)time.wDay;

	// 这个绝对不会返回错误的，不作返回值判断
	char szTime[sizeof("2015-08-20 12:00:00.000") + 1] = { 0 };
	_snprintf_s(szTime, _countof(szTime), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	int32_t nRet = 0;
	if (szSection[0] != 0)
		nRet = _snprintf_s(szBuf, nBufSize, _TRUNCATE, "%s [%s] ", szTime, szSection);
	else
		nRet = _snprintf_s(szBuf, nBufSize, _TRUNCATE, "%s ", szTime);
	if (nRet < 0)
		return -1;

	int32_t nLen = nRet;

	if (szFormat != NULL)
	{
		nRet = _vsnprintf_s(szBuf + nLen, nBufSize - nLen, _TRUNCATE, szFormat, arg);
		if (nRet < 0)
			return -1;

		nLen += nRet;
	}

	if (bEndLine)
	{
		if (nLen >= _LOG_BUF_SIZE - 1)
		{
			// 截断
			strncpy_s(szBuf + nBufSize - 3, 3, "\r\n", _TRUNCATE);
			nLen = _LOG_BUF_SIZE - 1;
		}
		else
		{
			strncpy_s(szBuf + nLen, nBufSize - nLen, "\r\n", _TRUNCATE);
			nLen = nLen + 2;
		}
	}

	return nLen;
}

static std::string formatLogName(const std::string& szPath, const std::string& szPrefix, const std::string& szSuffix)
{
	time_t tt = time(NULL);
	struct tm ttm;
	errno_t err = localtime_s(&ttm, &tt);
	if (err != 0)
		return false;
	char szBuf[MAX_PATH] = { 0 };
	if (szSuffix.empty())
		_snprintf_s(szBuf, _countof(szBuf), _TRUNCATE, "%s%s.%4d_%02d_%02d_%02d_%02d_%02d.%d", szPath.c_str(), szPrefix.c_str(), ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday, ttm.tm_hour, ttm.tm_min, ttm.tm_sec, GetCurrentProcessId());
	else
		_snprintf_s(szBuf, _countof(szBuf), _TRUNCATE, "%s%s.%4d_%02d_%02d_%02d_%02d_%02d.%d.%s", szPath.c_str(), szPrefix.c_str(), ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday, ttm.tm_hour, ttm.tm_min, ttm.tm_sec, GetCurrentProcessId(), szSuffix.c_str());

	return szBuf;
}

struct SLogInfo
{
	char		szSuffix[_LOG_FILE_NAME_SIZE];
	char		szBuf[_LOG_BUF_SIZE];
	uint16_t	nBufSize;
	uint8_t		bConsole;
	uint8_t		nDay;

	SLogInfo() : nBufSize(0), bConsole(false), nDay(0)
	{

	}

	SLogInfo& operator = (const SLogInfo& lhs)
	{
		if (this == &lhs)
			return *this;

		if (lhs.nBufSize > _LOG_BUF_SIZE)
			return *this;

		strncpy_s(this->szBuf, _LOG_BUF_SIZE, lhs.szBuf, _TRUNCATE);
		this->nBufSize = lhs.nBufSize;

		strncpy_s(this->szSuffix, _LOG_FILE_NAME_SIZE, lhs.szSuffix, _TRUNCATE);
		this->bConsole = lhs.bConsole;
		this->nDay = lhs.nDay;

		return *this;
	}
};

class CLogger :
	public base::IThreadEvent
{
public:
	CLogger();
	virtual ~CLogger();

	bool				init(const char* szLogPath, const char* szPrefixName);
	void				uninit();
	void				pushLog(const SLogInfo& sLogInfo);

	void				saveErrorLog(const char* szBuf, size_t nSize);

	void				writeToConsole(uint32_t nColor, const char* szBuf);

private:
	virtual int32_t		OnThreadCreate() { return 0; }
	virtual int32_t		OnThreadDestroy() { return 0; }
	virtual int32_t		OnThreadEvent() { return 0; }
	virtual int32_t		OnThreadProcess();

	bool				createDir(const std::string& szPath);

	void				flushLog();
	void				saveLog(SLogInfo& sLogInfo);

private:
	struct SLogFileInfo
	{
		std::string	szFileName;
		uint32_t	nNextFileIndex;
		size_t		nFileSize;
		FILE* pFile;
		uint8_t		nLastDay;

		SLogFileInfo() : nNextFileIndex(0), nFileSize(0), pFile(NULL), nLastDay(0)
		{

		}
	};

	std::string								m_szPath;
	std::string								m_szPrefixName;
	std::map<std::string, SLogFileInfo*>	m_mapLogFileInfo;
	time_t									m_nLastFlushTime;
	std::list<SLogInfo>						m_logQueue;
	CRITICAL_SECTION						m_asyncLock;
	CRITICAL_SECTION						m_syncLock;
	base::CThread* m_pThreadBase;
	FILE* m_pErrorFile;
};

CLogger::CLogger()
{
	this->m_pThreadBase = NULL;
	this->m_pErrorFile = NULL;
	this->m_nLastFlushTime = time(NULL);
}

CLogger::~CLogger()
{
	for (auto iter = this->m_mapLogFileInfo.begin(); iter != this->m_mapLogFileInfo.end(); ++iter)
	{
		SLogFileInfo* pLogFileInfo = iter->second;
		if (pLogFileInfo != nullptr && pLogFileInfo->pFile != nullptr)
		{
			fflush(pLogFileInfo->pFile);
			fclose(pLogFileInfo->pFile);
			pLogFileInfo->pFile = nullptr;
		}

		delete pLogFileInfo;
	}

	this->m_mapLogFileInfo.clear();
}

bool CLogger::init(const char* szLogPath, const char* szPrefixName)
{
	if (szPrefixName == NULL || szLogPath == NULL)
		return false;

	this->m_szPath = szLogPath;
	this->m_szPrefixName = szPrefixName;

	if (this->m_szPath.empty())
		this->m_szPath = "log/";

	if ('\\' != this->m_szPath.at(this->m_szPath.size() - 1) && '/' != this->m_szPath.at(this->m_szPath.size() - 1))
		this->m_szPath += "/";

	if (!this->createDir(this->m_szPath))
		return false;

	time_t tt = time(NULL);
	struct tm ttm;
	errno_t err = localtime_s(&ttm, &tt);
	if (err != 0)
		return false;

	std::string szErrName = formatLogName(this->m_szPath.c_str(), this->m_szPrefixName.c_str(), "ERROR.log");

	this->m_pErrorFile = _fsopen(szErrName.c_str(), "w", _SH_DENYNO);
	if (NULL == this->m_pErrorFile)
		return false;

	InitializeCriticalSection(&this->m_asyncLock);
	InitializeCriticalSection(&this->m_syncLock);

	this->m_pThreadBase = base::CThread::CreateNew(*this, base::CThread::RUN, 10);
	if (NULL == this->m_pThreadBase)
		return false;

	return true;
}

bool CLogger::createDir(const std::string& szPath)
{
	std::string szTempPath = szPath;

	std::string::size_type pos = 0;
	while (std::string::npos != (pos = szTempPath.find("\\", pos)))
	{
		szTempPath.replace(pos, 1, "/");
		pos += 1;
	}

	pos = 0;
	while (std::string::npos != (pos = szTempPath.find("/", pos)))
	{
		std::string szSubPath = szTempPath.substr(0, pos);
		pos += 1;
		if (!::CreateDirectoryA(szSubPath.c_str(), NULL))
		{
			uint32_t nError = GetLastError();
			if (nError != ERROR_ALREADY_EXISTS)
				return false;
		}
	}

	return true;
}

void CLogger::uninit()
{
	if (this->m_pThreadBase == NULL)
		return;

	this->flushLog();

	SAFE_DELETE(m_pThreadBase);

	DeleteCriticalSection(&this->m_asyncLock);
	DeleteCriticalSection(&this->m_syncLock);
}

void CLogger::pushLog(const SLogInfo& sLogInfo)
{
	EnterCriticalSection(&this->m_asyncLock);
	this->m_logQueue.push_back(sLogInfo);
	LeaveCriticalSection(&this->m_asyncLock);
}

int32_t CLogger::OnThreadProcess()
{
	while (true)
	{
		EnterCriticalSection(&this->m_asyncLock);
		if (this->m_logQueue.empty())
		{
			LeaveCriticalSection(&this->m_asyncLock);
			break;
		}

		SLogInfo sLogInfo = this->m_logQueue.front();
		this->m_logQueue.pop_front();
		LeaveCriticalSection(&this->m_asyncLock);

		if (sLogInfo.nBufSize == 0)
		{
			this->flushLog();
			this->m_nLastFlushTime = time(NULL);
		}
		else
		{
			this->saveLog(sLogInfo);
		}
	}

	time_t nCurTime = time(NULL);
	if (nCurTime - this->m_nLastFlushTime >= _FLUSH_LOG_TIME)
	{
		this->flushLog();
		this->m_nLastFlushTime = nCurTime;
	}

	return 1;
}

void CLogger::flushLog()
{
	for (auto iter = this->m_mapLogFileInfo.begin(); iter != this->m_mapLogFileInfo.end(); ++iter)
	{
		SLogFileInfo* pLogFileInfo = iter->second;
		if (pLogFileInfo != nullptr && pLogFileInfo->pFile != nullptr)
		{
			fflush(pLogFileInfo->pFile);
		}
	}
}

void CLogger::saveLog(SLogInfo& sLogInfo)
{
	if (sLogInfo.bConsole)
		std::cout << sLogInfo.szBuf;

	SLogFileInfo* pLogFileInfo = nullptr;
	auto iter = this->m_mapLogFileInfo.find(sLogInfo.szSuffix);
	if (iter == this->m_mapLogFileInfo.end())
	{
		time_t tt = time(NULL);
		struct tm ttm;
		errno_t err = localtime_s(&ttm, &tt);
		if (err != 0)
			return;

		pLogFileInfo = new SLogFileInfo();
		if (pLogFileInfo == nullptr)
		{
			return;
		}

		pLogFileInfo->nNextFileIndex = 0;
		pLogFileInfo->nFileSize = 0;
		char szFullFileName[MAX_PATH] = { 0 };
		pLogFileInfo->pFile = nullptr;
		pLogFileInfo->nLastDay = sLogInfo.nDay;
		this->m_mapLogFileInfo[sLogInfo.szSuffix] = pLogFileInfo;
	}
	else
	{
		pLogFileInfo = iter->second;
		if (pLogFileInfo == nullptr)
		{
			return;
		}
	}

	if (pLogFileInfo->pFile == NULL || pLogFileInfo->nFileSize >= _LOG_FILE_SIZE || pLogFileInfo->nLastDay != sLogInfo.nDay)
	{
		std::string szFileName = formatLogName(this->m_szPath, this->m_szPrefixName, sLogInfo.szSuffix);
		char szFullFileName[MAX_PATH] = { 0 };
		_snprintf_s(szFullFileName, _countof(szFullFileName), _TRUNCATE, "%s_%d.log", szFileName.c_str(), pLogFileInfo->nNextFileIndex);
		FILE* pFile = _fsopen(szFullFileName, "w", _SH_DENYNO);
		if (pFile == NULL)
		{
			delete pLogFileInfo;
			this->m_mapLogFileInfo.erase(sLogInfo.szSuffix);
			return;
		}

		if (pLogFileInfo->pFile != nullptr)
		{
			fflush(pLogFileInfo->pFile);
			fclose(pLogFileInfo->pFile);
		}
		pLogFileInfo->pFile = pFile;
		pLogFileInfo->nFileSize = 0;
		pLogFileInfo->nLastDay = sLogInfo.nDay;
		++pLogFileInfo->nNextFileIndex;
	}
	size_t nSize = fwrite(sLogInfo.szBuf, 1, sLogInfo.nBufSize, pLogFileInfo->pFile);

	pLogFileInfo->nFileSize += nSize;
}

void CLogger::saveErrorLog(const char* szBuf, size_t nSize)
{
	EnterCriticalSection(&this->m_syncLock);
	fwrite(szBuf, 1, nSize, this->m_pErrorFile);

#ifdef _DEBUG
	std::cout << szBuf;
#endif

	fflush(this->m_pErrorFile);

	LeaveCriticalSection(&this->m_syncLock);
}

void CLogger::writeToConsole(uint32_t nColor, const char* szBuf)
{
	EnterCriticalSection(&this->m_syncLock);
	if (nColor != 0)
	{
		SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), (uint16_t)nColor);
		std::cout << szBuf;
		SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else
		std::cout << szBuf;
	LeaveCriticalSection(&this->m_syncLock);
}

static CLogger* g_pLogger;

namespace base
{
	bool initLog(const char* szLogPath, const char* szPrefixName)
	{
		if (g_pLogger != NULL)
			return true;

		g_pLogger = new CLogger();
		if (!g_pLogger->init(szLogPath, szPrefixName))
		{
			delete g_pLogger;
			g_pLogger = NULL;

			return false;
		}

		return true;
	}

	void uninitLog()
	{
		if (NULL == g_pLogger)
			return;

		g_pLogger->uninit();
		delete g_pLogger;
		g_pLogger = NULL;
	}

	void saveLog(const char* szSection, bool bConsole, const char* szFormat, ...)
	{
		if (szFormat == NULL || szSection == NULL || g_pLogger == NULL)
			return;

		SLogInfo sLogInfo;
		va_list arg;
		va_start(arg, szFormat);
		int32_t nSize = formatLog(sLogInfo.szBuf, _countof(sLogInfo.szBuf), szSection, szFormat, arg, true, &sLogInfo.nDay);
		va_end(arg);
		if (nSize < 0)
			return;

		sLogInfo.nBufSize = (uint16_t)nSize;
		sLogInfo.szSuffix[0] = 0;
		sLogInfo.bConsole = bConsole;

		g_pLogger->pushLog(sLogInfo);
	}

	void saveLogEx(const char* szFileName, bool bConsole, const char* szFormat, ...)
	{
		if (szFormat == NULL || g_pLogger == NULL)
			return;

		SLogInfo sLogInfo;
		va_list arg;
		va_start(arg, szFormat);
		int32_t nSize = formatLog(sLogInfo.szBuf, _countof(sLogInfo.szBuf), "", szFormat, arg, true, &sLogInfo.nDay);
		va_end(arg);
		if (nSize < 0)
			return;

		sLogInfo.nBufSize = (uint16_t)nSize;
		strncpy_s(sLogInfo.szSuffix, _LOG_FILE_NAME_SIZE, szFileName, _TRUNCATE);
		sLogInfo.bConsole = bConsole;

		g_pLogger->pushLog(sLogInfo);
	}

	void saveErrorLog(const char* szFile, int32_t nLine, const char* szFormat, ...)
	{
		if (g_pLogger == NULL)
			return;

		static char szBuf[1024 * 1024] = { 0 };
		int32_t nSize = 0;

		va_list arg;
		va_start(arg, szFormat);
		nSize = formatLog(szBuf, _countof(szBuf), "", szFormat, arg, false, NULL);
		va_end(arg);

		if (nSize < 0)
			return;

		int32_t nRet = _snprintf_s(szBuf + nSize, _countof(szBuf) - nSize, _TRUNCATE, " in %s, %d\r\n", szFile, nLine);
		if (nRet < 0)
		{
			g_pLogger->saveErrorLog(szBuf, nSize);
			return;
		}

		nSize += nRet;

#ifdef _WIN32
		//if (nSize < _countof(szBuf) )
		//{
		//	// 打印调用栈
		//	base::GetCallStackString(NULL, szBuf + nSize, _countof(szBuf) - nSize - 1);
		//	nSize = (uint32_t)strnlen(szBuf, _countof(szBuf));
		//}
#endif

		g_pLogger->saveErrorLog(szBuf, nSize);
	}

	void writeToConsole(const char* szFormat, ...)
	{
		if (NULL == szFormat || g_pLogger == NULL)
			return;

		char szBuf[4096] = { 0 };
		va_list arg;
		va_start(arg, szFormat);
		int32_t nSize = formatLog(szBuf, _countof(szBuf), "", szFormat, arg, true, NULL);
		va_end(arg);
		if (nSize < 0)
			return;

		g_pLogger->writeToConsole(0, szBuf);
	}

	void writeToConsole(uint32_t nColor, const char* szFormat, ...)
	{
		if (NULL == szFormat || g_pLogger == NULL)
			return;

		char szBuf[4096] = { 0 };
		va_list arg;
		va_start(arg, szFormat);
		int32_t nSize = formatLog(szBuf, _countof(szBuf), "", szFormat, arg, true, NULL);
		va_end(arg);
		if (nSize < 0)
			return;

		g_pLogger->writeToConsole(nColor, szBuf);
	}

	void flushLog()
	{
		if (g_pLogger == NULL)
			return;


	}
}