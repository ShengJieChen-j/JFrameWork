#pragma once
#include <stdint.h>
#include <functional>
#include "libBase/TinyList.h"
#include "libBase/noncopyable.h"

namespace server
{
	class CTicker;
	struct STickerNode
	{
		CTicker* pTicker;
		STickerNode() :pTicker(nullptr) {}
	};

	typedef base::STinyListNode<STickerNode> TickerNode;

	class CTicker
		: public base::noncopyable_::noncopyable
	{
		friend class CTickerMgr;
		public:
			CTicker(uint32_t nTickerID);
			~CTicker();

			bool isRegist() const;
			uint32_t getTickerID() const;
			int64_t getIntervalTime() const;
			int64_t getNextTickTime() const;
			int64_t getContext() const;
			int64_t getRemainTime() const;

			void setCallback(const std::function<void(uint32_t, uint64_t)>& callback);

		private:
			TickerNode*								m_pTickerNode;
			uint32_t								m_nTickerID;
			int64_t									m_nIntervalTime;
			int64_t									m_nNextTickTime;
			int64_t									m_nRemainTime;
			uint64_t								m_nContext;
			std::function<void(uint32_t, uint64_t)> m_callback;
	};

	int64_t getNaturePassTime();
	int64_t getHighPrecisionTime();
}