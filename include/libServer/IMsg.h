/********************************************************************
    �������ڣ�  2021��7��17��
    �ļ����ƣ�  IMsg.h
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

		virtual const char*  getBuf          () const = 0;
		virtual uint16_t     getSize         () const = 0;
		virtual uint16_t     getType         () const = 0;
		virtual uint16_t     getHeadSize     () const = 0;
		virtual bool         isValid         () const = 0;
		virtual uint32_t     getSocketID     () const = 0;
		virtual int32_t      getConnectType  () const = 0;
		virtual int64_t      getID           () const = 0;
		virtual int32_t      getNetType      () const = 0;
    };
}