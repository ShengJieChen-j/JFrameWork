/******************************************************************
	创建日期：  2021年7月24日
	文件名称：  noncopyable.h
	说    明：

	当前版本：  1.00
	作    者：	shengjiechen
	概    述：  不允许类复制语义

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