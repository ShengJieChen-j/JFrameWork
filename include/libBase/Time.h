/******************************************************************
	创建日期：  2021年7月15日
    文件名称：  timer.h
    说    明：       
    
    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  支持win,linux平台。    

*******************************************************************/
#pragma once
#include <time.h>
#include <stdlib.h>
#include <stdint.h>


#if defined WIN32
    #include <windows.h>
#elif defined __LINUX__
    #define __max(a,b)  (((a) > (b)) ? (a) : (b))
    #define __min(a,b)  (((a) < (b)) ? (a) : (b))
    #include "Base.h"
#endif

namespace base
{
	//////////////////////////////////////////////////////////////////////
	// 超时类，以“秒”为单位
	//////////////////////////////////////////////////////////////////////
	class CTimer  
	{
    public:
		CTimer();
        ~CTimer();

	public:
		void	    startup(int32_t nSecs);             // 启动计时器
		bool	    isTimeOut(void);                    // 检测超时
        void	    update(void);                       // 更新计时器(重新计时)
        void	    clear(void);                        // 清空计时器

		int64_t	    getRemainTime();                    // 获取剩余时间
        
    private:
		int32_t		m_nInterval;
		int64_t	    m_tUpdateTime;
	};
	
	
	//////////////////////////////////////////////////////////////////////
	// 超时类，以“毫秒”为单位
	//////////////////////////////////////////////////////////////////////
	class CTimerMS
	{
	public:
		CTimerMS();
		~CTimerMS();

	public:
        void	    startup(int32_t nMilliSecs);        // 启动计时器
        bool	    isTimeOut(void);                    // 检测超时
		void	    update(void);                       // 更新计时器(重新计时)
		void	    clear(void);                        // 清空计时器

        int64_t	    getRemainTime();                    // 获取剩余时间
		int32_t		getInterval() const;				// 获取定时间隔

	private:
		int64_t     getNowMillisecond();			

    private:
		int32_t		m_nInterval;
		int64_t	    m_tUpdateTime;
	};

};	// endof namespace base

