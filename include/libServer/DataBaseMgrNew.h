/****************************************************************

    创建日期：  2015年6月15日 15时06分
    文件名称：  DataBaseMgrNew.h
    说    明：  数据加载管理

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "IDBServiceNew.h"
#include <vector>

namespace server
{
    class CDataLoadServiceNew;

    class CDataBaseMgrNew : public IDBServiceNew
    {
    public:
        CDataBaseMgrNew();
        virtual ~CDataBaseMgrNew();
        void			run();
        void			release();

    public:
        // 启服加载数据
        virtual bool AsyRequestDataLoad(DataLoadRequestNew& data);         // 多线程异步加载数据(适用于大数据加载)
        virtual bool checkDataReady();

    private:
        bool    loadData();
        bool    dataReady();
        bool    initDBConnection();
        bool    readMaxRecord();
        bool    averageThreadRead();
        void    clearDataLoad();

        int32_t getMaxRecord();
        int32_t getReadRecord();

    private:
        //	启服前数据加载
        std::vector<CDataLoadServiceNew*>	m_vecDataLoadService;       // 数据读取服务
        int32_t								m_MaxRecord;                // 表的记录条数
        dbtool::IDatabase* m_pDB;                      // 数据库连接句柄
        DataLoadRequestNew					m_CurDataLoadRequest;       // 当前正在执行的数据加载请求
        std::vector<DataLoadRequestNew>		m_vecDataLoadRequest;       // 数据加载请求列表
    };
}





