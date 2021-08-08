/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  IDBServiceNew.h
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
struct DataLoadRequestNew
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

    DataLoadRequestNew()
        : uiPort(0)
        , nThreadNum(1)
        , f_readdata(NULL)
        , f_dataready(NULL)
    {
    }
};


namespace server
{
    class IDBServiceNew
    {
    public:
        virtual ~IDBServiceNew() {};

        // 异步加载数据
        virtual bool AsyRequestDataLoad(DataLoadRequestNew& data) = 0;	// 异步加载数据
        virtual bool checkDataReady() = 0;	// 驱动并检查数据加载

    };
}
