#include "stdafx.h"
#include "Timer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace base
{
    CTimer::CTimer()
		: m_nInterval(0)
		, m_tUpdateTime(0)
    {
    }

    CTimer::~CTimer()
    {
    }

    void CTimer::startup( int32_t nSecs )
    {
        m_nInterval = nSecs;
        update();
    }

    bool CTimer::isTimeOut(void)
    { 
        int64_t tCurrentTime = time(NULL);
        return m_tUpdateTime != 0 && (tCurrentTime >= m_tUpdateTime + m_nInterval); 
    }

    void CTimer::update( void )
    {
        m_tUpdateTime = time(NULL);
    }

    void CTimer::clear( void )
    {
        m_tUpdateTime = 0;
        m_nInterval = 0;
    }

    int64_t CTimer::getRemainTime()
    {
        return m_tUpdateTime ? __min(__max(m_nInterval - ((int64_t)time(NULL)- m_tUpdateTime), 0), m_nInterval) : 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CTimerMS::CTimerMS()
		: m_nInterval(0)
		, m_tUpdateTime(0)
    {
    }

    CTimerMS::~CTimerMS()
    {

    }

    void CTimerMS::startup( int32_t nMilliSecs )
    {
        m_nInterval = nMilliSecs;
        update();
    }

    bool CTimerMS::isTimeOut( void )
    {
        int64_t tCurrentTime = getNowMillisecond();
        return m_tUpdateTime != 0 && (tCurrentTime >= m_tUpdateTime + m_nInterval);
    }

    void CTimerMS::update( void )
    {
        m_tUpdateTime = getNowMillisecond();
    }

    void CTimerMS::clear( void )
    {
        m_tUpdateTime = 0;
        m_nInterval = 0;
    }

    int64_t CTimerMS::getRemainTime()
    {
        return m_tUpdateTime ? __min(__max(m_nInterval - (getNowMillisecond()- m_tUpdateTime), 0), m_nInterval) : 0;
    }

    int64_t CTimerMS::getNowMillisecond()
    {

#if defined WIN32
        return clock() / (CLOCKS_PER_SEC / 1000);
#elif defined __LINUX__
        return base::timeGetTime();
#endif

    }

	int32_t	CTimerMS::getInterval() const
	{
		return m_nInterval;
	}

};	// endof namespace base
