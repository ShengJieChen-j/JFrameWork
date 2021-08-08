/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  IServerConfig.h
    说    明：  服务器配置接口

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include <string>
#include <stdint.h>


namespace server
{

    class IServerConfig
    {
    public:
        virtual ~IServerConfig() {}

        virtual bool	        reload() = 0;
        virtual std::string	    getString(const char* pPathName) = 0;
        virtual int32_t	        getInt32(const char* pPathName) = 0;
        virtual int64_t	        getInt64(const char* pPathName) = 0;
        virtual bool	        getBool(const char* pPathName) = 0;
        virtual int32_t	        getElementCount(const char* pParentElementName) = 0;

    };

}
