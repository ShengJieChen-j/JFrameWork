/******************************************************************
	�������ڣ�  2021��7��15��
	�ļ����ƣ�  ExceptionHandler.h
	˵    ����	�쳣����
				include SEH, invalid parameter, signal etc.

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����  

*******************************************************************/
#pragma once

namespace base
{
	void InitProcessExceptionHandler();									// ��ʼ�����쳣����

	void InitThreadExceptionHandler();									// ��ʼ�߳��쳣����

	void GetCallStackString(void* thread, char* buffer, size_t size);	// ��õ��ö�ջ�ַ���������
}
