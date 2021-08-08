/****************************************************************

	�������ڣ�  2021��7��22��
    �ļ����ƣ�  LogMacro.h
    ˵    ����  ��־�궨��

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "ServerFramework.h"


//	����̨�����
#	define CONSOLE_WRITE(fmt, ...)					base::writeToConsole(fmt, __VA_ARGS__)

#	define CONSOLE_WRITE_EX(textColor, fmt, ...)	base::writeToConsole(textColor, fmt, __VA_ARGS__)

#   define LOG_EXCEPTION(fmt, ...)					base::saveErrorLog(__FILE__, __LINE__, fmt, __VA_ARGS__)

// while��forѭ���İ�ȫ����
#	define FOR_OR_WHILE_DEFINE(maxSize)					\
	int32_t __nNum = 0;									\
	int32_t __nMaxSize = maxSize


#	define FOR_OR_WHILE_SAVE()							\
	if(__nNum++ >= __nMaxSize)							\
	{													\
		LOG_EXCEPTION(NULL);							\
		break;											\
	}
