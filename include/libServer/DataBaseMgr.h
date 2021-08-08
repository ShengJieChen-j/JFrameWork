/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  DataBaseMgr.h
    ˵    ����  ���ݼ��ع���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
        // ����ӿ�
        virtual bool	createDataBase(int32_t nThreadNum);				// �������ݿ�����
        virtual void	AsyExecuteSQL(uint64_t n64Id, const char* pszSQL,
            void* pUser, dbtool::SQL_RESULT_CALLBACK callback);			// �첽ִ��SQL���
        virtual size_t  getRequestSize();
        virtual size_t  getResultSize();

        // ������������
        virtual bool AsyRequestDataLoad(DataLoadRequest& data);         // ���߳��첽��������(�����ڴ����ݼ���)
        virtual bool checkDataReady();

        virtual bool SynRequestDataLoad(SynDataLoadRequest& data);		// ͬ����������(�����ڼ����ݼ���)
        virtual bool SynRequestExecute(SynExecuteRequest& request);     // ͬ���������ݿ�

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
        //	����ǰ���ݼ���
        std::vector<CDataLoadService*>  m_vecDataLoadService;       // ���ݶ�ȡ����
        int32_t                         m_MaxRecord;                // ��ļ�¼����
        dbtool::IDatabase* m_pDB;                      // ���ݿ����Ӿ��
        DataLoadRequest                 m_CurDataLoadRequest;       // ��ǰ����ִ�е����ݼ�������
        std::vector<DataLoadRequest>    m_vecDataLoadRequest;       // ���ݼ��������б�

    private:
        //	���ݿ�SQL������
        std::vector<dbtool::IDatabase*>	m_vec_DBHandle;
        int32_t							m_ThreadNum;
    };
}





