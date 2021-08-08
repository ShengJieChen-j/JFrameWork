/****************************************************************
    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MsgAdapter.h
    ˵    ����  ��Ϣ����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "MsgInterface.h"
#include "libBase/TypeDefine.h"

namespace server
{
    class MsgAdapter : public IMsg
    {
    public:
        MsgAdapter();
        ~MsgAdapter();

        // ����Ϣ
        void bind(const char*, uint32_t unSocketID, int32_t nType, int64_t n64Id, int32_t netType);

        virtual const char* GetBuf() const;
        virtual uint16_t GetSize() const;
        virtual uint16_t GetType() const;
        virtual uint16_t GetHeadSize() const;
        virtual bool IsValid() const;
        virtual uint32_t GetSocketID() const;
        virtual int32_t GetConnectType() const;
        virtual int64_t	GetID() const;
        virtual int32_t getNetType() const;

    private:
        const char* m_pBuffer;		    // ��Ϣ��ַ
        uint32_t m_SocketID;	        // SocketID
        int32_t m_ConnectType;			// ��������
        int64_t m_n64ID;				// ID(���Է�����ID Ҳ���������DBID)
        int32_t m_NetType;				// ��������
    };

}
