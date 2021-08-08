/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  IDBService.h
    ˵    ����  ���ݼ��ض���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include <functional>
#include <string>
#include <stdint.h>
#include "IDB.h"

// �ص���������
typedef std::tr1::function<void(dbtool::PSQL_RESULT result, int32_t nThreadId)>    ReadData_CallBack;
typedef std::tr1::function<void(int32_t nMaxRecord, int32_t nReadRecord)>          DataReady_CallBack;


// �첽���ݼ�������
struct DataLoadRequest
{
    std::string strIP;                      // ���ݿ�IP
    std::string strLoginName;               // ���ݿ��½�˺�
    std::string strPwd;                     // ���ݿ��½����
    std::string strDBName;                  // ���ݿ�����
    uint32_t uiPort;                        // ���ݿ�˿�

    int32_t nThreadNum;                     // �̸߳���
    std::string sql_select_max_record;      // ��ȡ����¼������SQL���
    std::string sql_select_data;            // ��ȡ���ݵ�SQL���
    ReadData_CallBack f_readdata;           // ���ݶ�ȡ�ص�
    DataReady_CallBack f_dataready;         // ���ݶ�ȡ��ϻص�

    DataLoadRequest()
        : uiPort(0)
        , nThreadNum(1)
        , f_readdata(NULL)
        , f_dataready(NULL)
    {
    }
};

// ���ݿ�ͬ����������
enum SynSqlOptType
{
    SSOT_SELECT,
    SSOT_INSERT,
    SSOT_UPDATE,
    SSOT_DELECT,
};

// ͬ�����ݼ�������
struct SynDataLoadRequest
{
    std::string strIP;                      // ���ݿ�IP
    std::string strLoginName;               // ���ݿ��½�˺�
    std::string strPwd;                     // ���ݿ��½����
    std::string strDBName;                  // ���ݿ�����
    uint32_t uiPort;                        // ���ݿ�˿�

    SynSqlOptType	optType;				// ��������
    std::string sql_select_data;            // ��ȡ���ݵ�SQL���
    ReadData_CallBack f_readdata;           // ���ݶ�ȡ�ص�

    SynDataLoadRequest()
        : uiPort(0)
        , optType(SSOT_SELECT)
        , f_readdata(NULL)
    {
    }
};

struct SynExecuteRequest
{
    std::string strIP;                      // ���ݿ�IP
    std::string strLoginName;               // ���ݿ��½�˺�
    std::string strPwd;                     // ���ݿ��½����
    std::string strDBName;                  // ���ݿ�����
    uint32_t uiPort;                        // ���ݿ�˿�

    std::string szSQL;						// SQL���

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

        virtual bool	createDataBase(int32_t nThreadNum) = 0;	// �������ݿ�����
        virtual void	AsyExecuteSQL(uint64_t n64Id, const char* pszSQL,
            void* pUser, dbtool::SQL_RESULT_CALLBACK callback) = 0;		// �첽ִ��SQL���
        virtual size_t  getRequestSize() = 0;
        virtual size_t  getResultSize() = 0;

        // �첽��������
        virtual bool AsyRequestDataLoad(DataLoadRequest& data) = 0;	// �첽��������
        virtual bool checkDataReady() = 0;	// ������������ݼ���

        // ͬ����������
        virtual bool SynRequestDataLoad(SynDataLoadRequest& data) = 0;	// ͬ����������
        virtual bool SynRequestExecute(SynExecuteRequest& request) = 0;	// ͬ��ִ��SQL
    };
}
