/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  IDBService.h
    说    明：  数据加载定义

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include <functional>
#include <string>
#include <stdint.h>
#include "IDB.h"

// 回调函数定义
typedef std::tr1::function<void(dbtool::PSQL_RESULT result, int32_t nThreadId)>    ReadData_CallBack;
typedef std::tr1::function<void(int32_t nMaxRecord, int32_t nReadRecord)>          DataReady_CallBack;


// 异步数据加载请求
struct DataLoadRequest
{
    std::string strIP;                      // 数据库IP
    std::string strLoginName;               // 数据库登陆账号
    std::string strPwd;                     // 数据库登陆密码
    std::string strDBName;                  // 数据库名称
    uint32_t uiPort;                        // 数据库端口

    int32_t nThreadNum;                     // 线程个数
    std::string sql_select_max_record;      // 读取最大记录条数的SQL语句
    std::string sql_select_data;            // 读取数据的SQL语句
    ReadData_CallBack f_readdata;           // 数据读取回调
    DataReady_CallBack f_dataready;         // 数据读取完毕回调

    DataLoadRequest()
        : uiPort(0)
        , nThreadNum(1)
        , f_readdata(NULL)
        , f_dataready(NULL)
    {
    }
};

// 数据库同步操作类型
enum SynSqlOptType
{
    SSOT_SELECT,
    SSOT_INSERT,
    SSOT_UPDATE,
    SSOT_DELECT,
};

// 同步数据加载请求
struct SynDataLoadRequest
{
    std::string strIP;                      // 数据库IP
    std::string strLoginName;               // 数据库登陆账号
    std::string strPwd;                     // 数据库登陆密码
    std::string strDBName;                  // 数据库名称
    uint32_t uiPort;                        // 数据库端口

    SynSqlOptType	optType;				// 操作类型
    std::string sql_select_data;            // 读取数据的SQL语句
    ReadData_CallBack f_readdata;           // 数据读取回调

    SynDataLoadRequest()
        : uiPort(0)
        , optType(SSOT_SELECT)
        , f_readdata(NULL)
    {
    }
};

struct SynExecuteRequest
{
    std::string strIP;                      // 数据库IP
    std::string strLoginName;               // 数据库登陆账号
    std::string strPwd;                     // 数据库登陆密码
    std::string strDBName;                  // 数据库名称
    uint32_t uiPort;                        // 数据库端口

    std::string szSQL;						// SQL语句

    SynExecuteRequest()
        : uiPort(0)
    {
    }
};

namespace server
{
    class IDBService
    {
    public:
        virtual ~IDBService() {};

        virtual bool	createDataBase(int32_t nThreadNum) = 0;	// 创建数据库链接
        virtual void	AsyExecuteSQL(uint64_t n64Id, const char* pszSQL,
            void* pUser, dbtool::SQL_RESULT_CALLBACK callback) = 0;		// 异步执行SQL语句
        virtual size_t  getRequestSize() = 0;
        virtual size_t  getResultSize() = 0;

        // 异步加载数据
        virtual bool AsyRequestDataLoad(DataLoadRequest& data) = 0;	// 异步加载数据
        virtual bool checkDataReady() = 0;	// 驱动并检查数据加载

        // 同步加载数据
        virtual bool SynRequestDataLoad(SynDataLoadRequest& data) = 0;	// 同步加载数据
        virtual bool SynRequestExecute(SynExecuteRequest& request) = 0;	// 同步执行SQL
    };
}
