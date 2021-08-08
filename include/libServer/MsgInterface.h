/********************************************************************
    创建日期：  2021年7月24日
    文件名称：  MsgInterface.h
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
