/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  IDBServiceNew.h
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
struct DataLoadRequestNew
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

        // �첽��������
        virtual bool AsyRequestDataLoad(DataLoadRequestNew& data) = 0;	// �첽��������
        virtual bool checkDataReady() = 0;	// ������������ݼ���

    };
}
