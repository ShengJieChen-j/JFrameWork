/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  IServerConfig.h
    ˵    ����  ���������ýӿ�

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

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
