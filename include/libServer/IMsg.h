/********************************************************************
    创建日期：  2021年7月17日
    文件名称：  IMsg.h
    说    明：  信息接口的抽象API

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

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