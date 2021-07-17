/****************************************************************
	创建日期：  2021年7月15日
	文件名称：  basecrt.h
	说    明：  安全的c运行时库

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

******************************************************************/
#pragma once

#include <stdarg.h>
#include <time.h>
#include <stdint.h>

namespace base
{
	namespace crt
	{
		/********************************************************************
		* method: memcpy
		* return: Zero if successful; an error code on failure.(EINVAL, ERANGE)
		*********************************************************************/
		int memcpy(
			void* dest,
			size_t sizeDest,
			const void* src,
			size_t sizeSrc
		);

		/********************************************************************
		* method: strcpy
		* return: Zero if successful; otherwise, an error.(EINVAL, ERANGE)
		* NOTE: [sizeOfDest] should NOT less than strlen([strSource]) + 1.
		*********************************************************************/
		int strcpy(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource
		);

		/********************************************************************
		* method: strncpy
		* return: Zero if successful, otherwise an error code.(EINVAL, ERANGE)
		* NOTE: strncpy will append '\0',
		*		so [sizeOfDest] should NOT less than [count] + 1.
		*       Don't use _TRUNCATE for param [count](for compatible with POSIX)
		*********************************************************************/
		int strncpy(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource,
			size_t count
		);

		/********************************************************************
		* method: strcat
		* return: Zero if successful; an error code on failure.(EINVAL, ERANGE)
		* NOTE:	[sizeOfDest] should NOT less than
		*		strlen(strDest) + strlen(strSource) + 1.
		*********************************************************************/
		int strcat(
			char* strDest,
			size_t sizeOfDest,
			const char* strSource
		);

		/********************************************************************
		* method: sprintf
		* return: The number of characters written(not include null terminator),
		*		  or –1 if an error occurred.
		*		  If fail, check errno.
		* NOTE: [sizeOfBuffer] should include the length of null terminator.
		*********************************************************************/
		int sprintf(
			char* buffer,
			size_t sizeOfBuffer,
			const char* format,
			...
		);

		/********************************************************************
		* method: snprintf
		* return: The number of characters written(not include null terminator),
		*		  or –1 if an error occurred.
		*		  If fail, check errno.
		* NOTE: snprintf will append '\0',
		*		so [sizeOfBuffer] should NOT less than [count] + 1.
		*       Don't use _TRUNCATE for param [count](for compatible with POSIX)
		*********************************************************************/
		int snprintf(
			char* buffer,
			size_t sizeOfBuffer,
			size_t count,
			const char* format,
			...
		);

		/********************************************************************
		* method: vsprintf
		* return: the number of characters written(not include null terminator),
		*		  or a negative value if an output error occurs.
		* NOTE: [sizeOfBuffer] should include the length of null terminator.
		*********************************************************************/
		int vsprintf(
			char* buffer,
			size_t sizeOfBuffer,
			const char* format,
			va_list argptr
		);

		/********************************************************************
		* method: vsnprintf
		* return: the number of characters written(not include null terminator),
		*		  or a negative value if an output error occurs.
		* NOTE: vsnprintf will append '\0',
		*		so [sizeOfBuffer] should NOT less than [count] + 1.
		*       Don't use _TRUNCATE for param [count](for compatible with POSIX)
		*********************************************************************/
		int vsnprintf(
			char* buffer,
			size_t sizeOfBuffer,
			size_t count,
			const char* format,
			va_list argptr
		);

		/********************************************************************
		* method: strtok
		* return: Return a pointer to the next token found in strToken,
		*         or return NULL when no more tokens are found.
		*********************************************************************/
		char* strtok(
			char* strToken,
			const char* strDelimit,
			char** context
		);

		/********************************************************************
		* method: localtime
		* return: Zero if successful,  an error code on failure.(EINVAL)
		*********************************************************************/
		int localtime(
			struct tm* t,
			const time_t* time
		);

		/********************************************************************
		* method: gmtime
		* return: Zero if successful,  an error code on failure.(EINVAL)
		*********************************************************************/
		int gmtime(
			struct tm* t,
			const time_t* time
		);

		/********************************************************************
		* method: strtoi32
		*********************************************************************/
		bool strtoi32(
			const char* str,
			int32_t& i32
		);

		/********************************************************************
		* method: strtou32
		*********************************************************************/
		bool strtou32(
			const char* str,
			uint32_t& u32
		);

		/********************************************************************
		* method: strtoi64
		*********************************************************************/
		bool strtoi64(
			const char* str,
			int64_t& i64
		);

		/********************************************************************
		* method: strtou64
		*********************************************************************/
		bool strtou64(
			const char* str,
			uint64_t& u64
		);
	}
}
