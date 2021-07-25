#include "Ticker.h"
#include <vector>

#pragma once

namespace server
{
	class CTickerMgr
		: public base::noncopyable_::noncopyable
	{
	public:
		CTickerMgr();
		~CTickerMgr();

		void				registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext);
		void				unRegistTicker(CTicker* pTicker);
		inline	int64_t		getCurLogicTime() const { return m_nCurTickTime; }
		void				setCurTickTime();
		void				update(int64_t nTime);

	private:
		void				insertTicker(TickerNode* pTickerNode);
	private:
		enum
		{
			__Time_Size = 0x00010000,   // 时间刻度尺长度 2^16 
			__Time_Mask = 0x0000ffff,   // 时间掩码 &
			__Time_Precision = 10,      // 精度10ms
		};
	private:
		base::CTinyList<TickerNode>			m_listTicker[__Time_Size];
		std::vector<TickerNode*>			m_vecTempTickerNode;
		int64_t								m_nCurTickTime;
	};
}