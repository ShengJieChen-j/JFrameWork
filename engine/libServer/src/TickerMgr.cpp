#include "TickerMgr.h"
#include "libBase/Base.h"
#include "libBase/TimeTools.h"
#include "libBase/logger.h"
#include "Profiling.h"

server::CTickerMgr::CTickerMgr()
{
	this->m_nCurTickTime = getNaturePassTime();
}

server::CTickerMgr::~CTickerMgr()
{
	//todo
}

void server::CTickerMgr::registTicker(CTicker* pTicker, uint64_t nStartTime, uint64_t nIntervalTime, uint64_t nContext)
{
	CHECK(pTicker != nullptr);

	if (pTicker->isRegist())
	{
		LogError("!pTicker->isRegist() TickerID: %u", pTicker->getTickerID());
		return;
	}

	CHECK(pTicker->m_callback != nullptr);

	TickerNode* pTickerNode = new TickerNode();
	CHECK(pTickerNode);

	pTickerNode->m_Val.pTicker = pTicker;
	pTicker->m_nIntervalTime = nIntervalTime;
	pTicker->m_nNextTickTime = this->m_nCurTickTime + nStartTime;
	pTicker->m_nContext = nContext;
	pTicker->m_pTickerNode = pTickerNode;

	this->insertTicker(pTickerNode);
}

void server::CTickerMgr::unRegistTicker(CTicker* pTicker)
{
	CHECK(pTicker != nullptr);

	if (!pTicker->isRegist())
	{
		return;
	}

	if (pTicker->m_pTickerNode->isHang())
	{
		pTicker->m_pTickerNode->remove();
		pTicker->m_pTickerNode->m_Val.pTicker = nullptr;
		SAFE_DELETE(pTicker->m_pTickerNode);
	}
	else
	{
		pTicker->m_pTickerNode->m_Val.pTicker = nullptr;
		pTicker->m_pTickerNode = nullptr;
	}
}

void server::CTickerMgr::setCurTickTime()
{
	int64_t nDiffTime = getNaturePassTime() - this->m_nCurTickTime;
	if (nDiffTime > 0)
	{
		this->update(nDiffTime);
	}
}

void server::CTickerMgr::update(int64_t nTime)
{
	int64_t nEndTime = this->m_nCurTickTime + nTime;

	for (; this->m_nCurTickTime < nEndTime; ++this->m_nCurTickTime)
	{
		uint32_t nPos = (uint32_t)(this->m_nCurTickTime & __Time_Mask) / __Time_Precision;
		auto& listTicker = this->m_listTicker[nPos];
		this->m_vecTempTickerNode.clear();

		while (!listTicker.isEmpty())
		{
			TickerNode* pTickerNode = listTicker.getFront();
			pTickerNode->remove();
			CTicker* pTicker = pTickerNode->m_Val.pTicker;

			if (pTicker->m_nNextTickTime <= this->m_nCurTickTime)
			{
#ifdef __PROFILING_OPEN
				uint32_t nTickerID = pTicker->m_nTickerID;
				profilingBeginByTickerID(nTickerID);
#endif

				if (pTicker->m_nIntervalTime == 0)
				{
					this->unRegistTicker(pTicker);
				}

				if (pTicker->m_callback != nullptr)
				{
					pTicker->m_callback(pTicker->m_nTickerID, pTicker->m_nContext);
				}

#ifdef __PROFILING_OPEN
				profilingEndByTickerID(nTickerID);
#endif

				if (pTickerNode->m_Val.pTicker == nullptr)
				{
					SAFE_DELETE(pTickerNode);
					continue;
				}

				pTicker->m_nNextTickTime += pTicker->m_nIntervalTime;
				this->m_vecTempTickerNode.push_back(pTickerNode);
			}
			else
			{
				this->m_vecTempTickerNode.push_back(pTickerNode);
			}
		}

		size_t nCount = this->m_vecTempTickerNode.size();

		for (size_t i = 0; i < nCount;++i)
		{
			TickerNode* pTickerNode = this->m_vecTempTickerNode[i];
			if (pTickerNode->m_Val.pTicker == nullptr)
			{
				SAFE_DELETE(pTickerNode);
				continue;
			}

			this->insertTicker(pTickerNode);
		}

	}
}

void server::CTickerMgr::insertTicker(TickerNode* pTickerNode)
{
	CHECK(pTickerNode != nullptr && pTickerNode->m_Val.pTicker != nullptr);

	uint32_t nPos = (uint32_t)(pTickerNode->m_Val.pTicker->m_nNextTickTime & __Time_Mask) / __Time_Precision;
	auto& listTicker = this->m_listTicker[nPos];
	listTicker.pushBack(pTickerNode);
}
