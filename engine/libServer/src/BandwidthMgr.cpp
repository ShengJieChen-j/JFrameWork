#include "stdafx.h"
#include "BandwidthMgr.h"
#include "libBase/logger.h"

namespace server
{
	CBandwidthMgr::CBandwidthMgr()
	{

	}

	void CBandwidthMgr::add(uint16_t usType, uint16_t usSize)
	{
		SNetData& data = m_map_Bandwidth[usType];
		data.lCapacity += usSize;		// ������Сͳ��
		data.lTurn++;					// ��Ϣ����ͳ��
	}

	void CBandwidthMgr::output()
	{
		std::unordered_map<uint16_t, SNetData>::iterator itor = m_map_Bandwidth.begin();
		std::unordered_map<uint16_t, SNetData>::iterator itore = m_map_Bandwidth.end();

		for (; itor != itore; ++itor)
		{
			SNetData& sData = itor->second;
			base::saveLogEx("bandwidth", false, "%d	%I64d	%I64d",
				itor->first, sData.lTurn, sData.lCapacity);
		}
	}
}
