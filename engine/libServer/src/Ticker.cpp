#include "Ticker.h"
#include "libBase/Base.h"
#include <windows.h>

#pragma comment( lib,"winmm.lib" )

server::CTicker::CTicker(uint32_t nTickerID) 
	:m_pTickerNode(nullptr)
	,m_nTickerID(nTickerID)
{
}

server::CTicker::~CTicker()
{
	// todo serverframe unRegists
}

bool server::CTicker::isRegist() const
{
	return this->m_pTickerNode != nullptr;
}

uint32_t server::CTicker::getTickerID() const
{
	return this->m_nTickerID;
}

int64_t server::CTicker::getIntervalTime() const
{
	return this->m_nIntervalTime;
}

int64_t server::CTicker::getNextTickTime() const
{
	return this->m_nNextTickTime;
}

int64_t server::CTicker::getContext() const
{
	return this->m_nContext;
}

int64_t server::CTicker::getRemainTime() const
{
	return this->m_nRemainTime;
}

void server::CTicker::setCallback(const std::function<void(uint32_t, uint64_t)>& func)
{
	this->m_callback = func;
}

int64_t server::getNaturePassTime()
{
	static int64_t s_nNaturePassTime = timeGetTime();

	int64_t nCurTime = timeGetTime();

	if (nCurTime < s_nNaturePassTime)
	{
		nCurTime += 0x10000000;
	}

	s_nNaturePassTime = nCurTime;
	return nCurTime;
}

int64_t server::getHighPrecisionTime()
{
	struct  Frequency
	{
		Frequency()
		{
			memset(&liPrefFreq, 0, sizeof(liPrefFreq));
			QueryPerformanceFrequency(&liPrefFreq);
			if (liPrefFreq.QuadPart <= 0)
			{
				liPrefFreq.QuadPart = 1;
			}
		}
		LARGE_INTEGER liPrefFreq;
	};

	static Frequency frequency;

	LARGE_INTEGER liPrefNow = { 0 };
	QueryPerformanceCounter(&liPrefNow);
	return liPrefNow.QuadPart * 1000000 / frequency.liPrefFreq.QuadPart; // ms
}
