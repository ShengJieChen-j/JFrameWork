/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  DataBaseMgr.h
    说    明：  数据加载管理

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "IDBService.h"
#include <vector>

namespace server
{
    class CDataLoadService;

    class CDataBaseMgr : public IDBService
    {
    public:
        CDataBaseMgr();
        virtual ~CDataBaseMgr();
        void			run();
        void			release();

    public:
        // 对外接口
        virtual bool	createDataBase(int32_t nThreadNum);				// 创建数据库链接
        virtual void	AsyExecuteSQL(uint64_t n64Id, const char* pszSQL,
            void* pUser, dbtool::SQL_RESULT_CALLBACK callback);			// 异步执行SQL语句
        virtual size_t  getRequestSize();
        virtual size_t  getResultSize();

        // 启服加载数据
        virtual bool AsyRequestDataLoad(DataLoadRequest& data);         // 多线程异步加载数据(适用于大数据加载)
        virtual bool checkDataReady();

        virtual bool SynRequestDataLoad(SynDataLoadRequest& data);		// 同步加载数据(适用于简单数据加载)
        virtual bool SynRequestExecute(SynExecuteRequest& request);     // 同步操作数据库

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
        std::vector<CDataLoadService*>  m_vecDataLoadService;       // 数据读取服务
        int32_t                         m_MaxRecord;                // 表的记录条数
        dbtool::IDatabase* m_pDB;                      // 数据库连接句柄
        DataLoadRequest                 m_CurDataLoadRequest;       // 当前正在执行的数据加载请求
        std::vector<DataLoadRequest>    m_vecDataLoadRequest;       // 数据加载请求列表

    private:
        //	数据库SQL语句操作
        std::vector<dbtool::IDatabase*>	m_vec_DBHandle;
        int32_t							m_ThreadNum;
    };
}





