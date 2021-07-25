/******************************************************************
	�������ڣ�  2021��7��24��
	�ļ����ƣ�  noncopyable.h
	˵    ����

	��ǰ�汾��  1.00
	��    �ߣ�	shengjiechen
	��    ����  �������ิ������

*******************************************************************/
#pragma once

namespace base
{
	namespace noncopyable_ 
	{
		class noncopyable {
		protected:
			noncopyable() = default;
			~noncopyable() = default;
		private:
			noncopyable(const noncopyable&) = delete;
			const noncopyable& operator=(const noncopyable&) = delete;
		};
	}
}