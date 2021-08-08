/****************************************************************

    �������ڣ�  2015��6��15�� 15ʱ06��
    �ļ����ƣ�  DataBaseMgrNew.h
    ˵    ����  ���ݼ��ع���

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
        // ������������
        virtual bool AsyRequestDataLoad(DataLoadRequestNew& data);         // ���߳��첽��������(�����ڴ����ݼ���)
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
        //	����ǰ���ݼ���
        std::vector<CDataLoadServiceNew*>	m_vecDataLoadService;       // ���ݶ�ȡ����
        int32_t								m_MaxRecord;                // ��ļ�¼����
        dbtool::IDatabase* m_pDB;                      // ���ݿ����Ӿ��
        DataLoadRequestNew					m_CurDataLoadRequest;       // ��ǰ����ִ�е����ݼ�������
        std::vector<DataLoadRequestNew>		m_vecDataLoadRequest;       // ���ݼ��������б�
    };
}





