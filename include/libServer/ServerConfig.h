/********************************************************************

    创建日期：  2021年7月24日
    文件名称：  ServerConfig.h
    说    明：  服务器配置管理类

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  非线程安全(子线程需要使用可以直接创建对象)

********************************************************************/
#pragma once

#include <string>
#include <map>
#include "libBase/TypeDefine.h"
#include "IServerConfig.h"

namespace server
{

    class CServerConfig : public IServerConfig
    {
    public:
        CServerConfig();
        virtual ~CServerConfig();

        bool	                init(const char* pConfigName);

    public:
        virtual bool	        reload();
        virtual std::string	    getString(const char* pPathName);
        virtual int32_t	        getInt32(const char* pPathName);
        virtual int64_t	        getInt64(const char* pPathName);
        virtual bool	        getBool(const char* pPathName);
        virtual int32_t	        getElementCount(const char* pParentElementName);

        std::string getString(const std::string& pathName);
        int32_t getInt32(const std::string& pathName);
        int64_t getInt64(const std::string& pathName);
        bool getBool(const std::string& pathName);

    private:
        bool	                loadConfig(std::string& strConfigName);
        void	                checkName(std::string& strName);
        bool					loadServerConfig(const char* filename);

    private:
        std::map<std::string, std::string>	m_map_Attribute;
        std::map<std::string, int32_t>		m_map_SiblingElementCount;
        std::string                         m_strConfigName;
    };

}
