/********************************************************************
    �������ڣ�  2021��7��24��
    �ļ����ƣ�  MsgInterface.h
    ˵    ����  ��Ϣ�ӿڵĳ���API

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

********************************************************************/
#pragma once
#include "libBase/TypeDefine.h"

namespace server
{

    class IMsg
    {
    public:
        virtual ~IMsg() {}

        virtual const char* GetBuf() const = 0;
        virtual uint16_t		    GetSize() const = 0;
        virtual uint16_t		    GetType() const = 0;
        virtual uint16_t		    GetHeadSize() const = 0;
        virtual bool				IsValid() const = 0;
        virtual uint32_t		    GetSocketID() const = 0;
        virtual int32_t			    GetConnectType() const = 0;
        virtual int64_t				GetID() const = 0;
        virtual int32_t				getNetType() const = 0;
    };

}
