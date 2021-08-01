#include "stdafx.h"
#if defined WIN32
#include <windows.h>
#include <mmsystem.h>
#include <io.h>
#include <tchar.h>
#pragma warning(disable : 4786)
#elif defined __LINUX__
#include <stdarg.h>
#include </usr/include/string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <set>
#include <string>
#include "Base.h"
#include "basecrt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace base {

	// JS Hash
	uint32_t hash(const char* szKey)
	{
		if (szKey == NULL)
			return 0;

		uint32_t nHash = 1315423911;

		while (*szKey)
		{
			nHash ^= ((nHash << 5) + (*szKey++) + (nHash >> 2));
		}

		return (nHash & 0x7FFFFFFF);
	}

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

}; // endof namespace base



