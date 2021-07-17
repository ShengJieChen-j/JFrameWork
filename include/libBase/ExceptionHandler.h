/******************************************************************
	创建日期：  2021年7月15日
	文件名称：  ExceptionHandler.h
	说    明：	异常处理
				include SEH, invalid parameter, signal etc.

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：  

*******************************************************************/
#pragma once

namespace base
{
	void InitProcessExceptionHandler();									// 初始进程异常处理

	void InitThreadExceptionHandler();									// 初始线程异常处理

	void GetCallStackString(void* thread, char* buffer, size_t size);	// 获得调用堆栈字符串缓冲区
}
