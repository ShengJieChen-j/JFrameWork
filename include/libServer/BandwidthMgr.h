/****************************************************************
    创建日期：  2021年7月15日
    文件名称：  BandwidthMgr.h
    说    明：  带宽统计类    
    
    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  

******************************************************************/
#pragma once

#include <hash_map>
#include "libBase/TypeDefine.h"

namespace server
{
	class CBandwidthMgr
	{
		//流量数据
		struct SNetData
		{
			int64_t lTurn;		// 消息次数
			int64_t lCapacity;	// 流量大小

			SNetData() : lTurn(0), lCapacity(0) { }
		};

	public:
		CBandwidthMgr();

	public:
		void add(uint16_t usType, uint16_t usSize);

		void output();

	private:
		std::hash_map<uint16_t, SNetData>  m_map_Bandwidth;
	};
}