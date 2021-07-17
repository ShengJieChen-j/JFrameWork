#include "basecrt.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

namespace base
{
	namespace crt
	{

		int memcpy(
			void* dest,
			size_t sizeOfDest,
			const void* src,
			size_t count
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = memcpy_s(dest, sizeOfDest, src, count);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int strcpy(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = strcpy_s(strDest, sizeOfDest, strSource);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int strncpy(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource,
			size_t count
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = strncpy_s(strDest, sizeOfDest, strSource, count);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int strcat(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = strcat_s(strDest, sizeOfDest, strSource);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int sprintf(
			char* buffer,
			size_t sizeOfBuffer,
			const char* format,
			...
		)
		{
			int ret = 0;
#ifdef _WIN32
			va_list arglist;
			va_start(arglist, format);
			ret = _vsprintf_s_l(buffer, sizeOfBuffer, format, NULL, arglist);
			va_end(arglist);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int snprintf(
			char* buffer,
			size_t sizeOfBuffer,
			size_t count,
			const char* format,
			...
		)
		{
			int ret = 0;
#ifdef _WIN32
			va_list arglist;
			va_start(arglist, format);
			ret = _vsnprintf_s_l(
				buffer,
				sizeOfBuffer,
				count,
				format,
				NULL,
				arglist
			);
			va_end(arglist);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int vsprintf(
			char* buffer,
			size_t sizeOfBuffer,
			const char* format,
			va_list argptr
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = _vsprintf_s_l(buffer, sizeOfBuffer, format, NULL, argptr);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int vsnprintf(
			char* buffer,
			size_t sizeOfBuffer,
			size_t count,
			const char* format,
			va_list argptr
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = _vsnprintf_s_l(
				buffer,
				sizeOfBuffer,
				count,
				format,
				NULL,
				argptr
			);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		char* strtok(
			char* strToken,
			const char* strDelimit,
			char** context
		)
		{
			char* ptr = NULL;
#ifdef _WIN32
			ptr = strtok_s(strToken, strDelimit, context);
#else
#error Other platforms are not supported yet
#endif
			return ptr;
		}

		int localtime(
			struct tm* t,
			const time_t* time
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = localtime_s(t, time);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		int gmtime(
			struct tm* t,
			const time_t* time
		)
		{
			int ret = 0;
#ifdef _WIN32
			ret = gmtime_s(t, time);
#else
#error Other platforms are not supported yet
#endif
			return ret;
		}

		bool strtoi32(const char* str, int32_t& i32)
		{
			if (NULL == str)
			{
				return false;
			}

			char* endptr = NULL;
			errno = 0;
			i32 = strtol(str, &endptr, 10);
			if (endptr)
			{
				if (endptr == str ||
					*endptr != '\0')
				{
					return false;
				}
			}
			return (errno == 0);
		}

		bool strtou32(const char* str, uint32_t& u32)
		{
			if (NULL == str)
			{
				return false;
			}

			char* endptr = NULL;
			errno = 0;
			u32 = strtoul(str, &endptr, 10);
			if (endptr)
			{
				if (endptr == str ||
					*endptr != '\0')
				{
					return false;
				}
			}
			return (errno == 0);
		}

		bool strtoi64(const char* str, int64_t& i64)
		{
			if (NULL == str)
			{
				return false;
			}

			char* endptr = NULL;
			errno = 0;
			i64 = _strtoi64(str, &endptr, 10);
			if (endptr)
			{
				if (endptr == str ||
					*endptr != '\0')
				{
					return false;
				}
			}
			return (errno == 0);
		}

		bool strtou64(const char* str, uint64_t& u64)
		{
			if (NULL == str)
			{
				return false;
			}

			char* endptr = NULL;
			errno = 0;
			u64 = _strtoui64(str, &endptr, 10);
			if (endptr)
			{
				if (endptr == str ||
					*endptr != '\0')
				{
					return false;
				}
			}
			return (errno == 0);
		}

	}
}
