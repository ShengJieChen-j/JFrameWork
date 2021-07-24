/********************************************************************
	�������ڣ�  2021��7��24��
	�ļ����ƣ�  Profiling.h
	˵    ����  ���ܷ���

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����  

********************************************************************/
#pragma once

#include "libBase/Base.h"
#include "libBase/ScopeExit.h"

namespace server
{
	bool initProfiling();
	void uninitProfiling();
	void enableProfiling(bool bEnable);
	void profilingBeginByLabel(const char* szLabel);
	void profilingEndByLabel(const char* szLabel);
	void profilingBeginByMessageID(uint32_t nID);
	void profilingEndByMessageID(uint32_t nID);
	void profilingBeginByTickerID(uint32_t nID);
	void profilingEndByTickerID(uint32_t nID);
	void profiling(int64_t nTotalTime);
}

//#define __PROFILING_OPEN

#ifndef __PROFILING_OPEN
#	define PROFILING_BEGIN(Label)
#	define PROFILING_END(Label)
#	define PROFILING_GUARD(Label)
#else
#	define PROFILING_BEGIN(Label)					server::profilingBeginByLabel(#Label);
#	define PROFILING_END(Label)						server::profilingEndByLabel(#Label);
#	define PROFILING_GUARD(Label)					server::profilingBeginByLabel(#Label); SCOPE_EXIT(server::profilingEndByLabel(#Label););
#endif
