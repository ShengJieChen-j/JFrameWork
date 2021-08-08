/********************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  ServerConfig.h
    ˵    ����  ���������ù�����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  ���̰߳�ȫ(���߳���Ҫʹ�ÿ���ֱ�Ӵ�������)

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
