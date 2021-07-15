/******************************************************************        
    创建日期：  2021年7月15日
    文件名称：  Base.h
    说    明：       
    
    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  支持win,linux平台。    

*******************************************************************/
#pargma once
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <logger.h>

#if defined WIN32
    #pragma warning(disable:4996)
	#pragma warning(disable:4819)
	#pragma warning(error:4244) ///< 把类型转换截断警告信息作为一个错误
	#pragma warning(error:4715) ///< 把没有返回值警告信息作为一个错误
    #include<windows.h>
    #include<crtdbg.h>
#elif defined __LINUX__
    #include "TypeDefine.h"
#endif

#undef	SAFE_DELETE
#define SAFE_DELETE(ptr)		{ if(ptr){ try{ delete ptr; }catch(...){ base::saveErrorLog(__FILE__, __LINE__, "CATCH: *** SAFE_DELETE() crash!"); } ptr = 0; } }

#undef	SAFE_RELEASE
#define SAFE_RELEASE(ptr)		{ if(ptr){ try{ ptr->release(); }catch(...){ base::saveErrorLog(__FILE__, __LINE__, "CATCH: *** SAFE_RELEASE() crash!"); } ptr = 0; } }

#undef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( p )	{ if(( p ) != NULL) { try{ delete[] ( p ); }catch(...){ base::saveErrorLog(__FILE__, __LINE__, "CATCH: *** SAFE_DELETE_ARRAY crash! ***"); } (p)=0; } }


#undef	ASSERT
#undef	IF_NOT

#ifdef	_DEBUG
#	define	ASSERT assert
#else
#	define	ASSERT(x) if(!(x)) { base::saveErrorLog(__FILE__, __LINE__, "ASSERT(%s)", #x);}
#endif


#define	IF_NOT(x)	if( (!(x)) ? ( base::saveErrorLog(__FILE__, __LINE__, "IF_NOT(%s)", #x),1 ) : 0 )

#define CHECK(exp)				do { if (!(exp)) { base::saveErrorLog(__FILE__, __LINE__, "CHECK(%s)", #exp); return; } } while (0)
#define CHECK_R(exp, ret)		do { if (!(exp)) { base::saveErrorLog(__FILE__, __LINE__, "CHECK(%s)", #exp); return ret; } } while (0)

#define CHECKF(exp)				CHECK_R(exp, false)
#define CHECKN(exp)				CHECK_R(exp, NULL)
#define CHECKZ(exp)				CHECK_R(exp, 0)

// functions...
namespace base
{
	uint32_t	hash(const char *szKey);

#ifndef _WIN32
	int32_t		timeGetTime();
#endif

}