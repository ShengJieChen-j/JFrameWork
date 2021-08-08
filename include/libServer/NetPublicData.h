/****************************************************************

    创建日期：  2021年7月24日
    文件名称：  NetPublicData.h
    说    明：

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：

******************************************************************/
#pragma once

#include "ServerData.h"

namespace server
{
    // 前置声明
    class CTerminal;
    class CConnection;

    enum  ePackParserType
    {
        PACK_PARSER_ENCRYPT = 0,	    // 加密
        PACK_PARSER_NOENCRYPT,			// 不加密
        PACK_PARSER_TGW_ENCRYPT,		// 支持TGW，加密。可以支持腾讯平台。
        PACK_PARSER_TGW_NOENCRYPT,		// 支持TGW，不加密。可以支持腾讯平台。
    };

    // ASIO client异步链接回调结构体
    typedef struct	client_connect_result
    {
        CTerminal* pTerminal;
        CConnection* pCon;
        ServerLinkInfo data;

        client_connect_result()
            : pTerminal(NULL)
            , pCon(NULL)
        {
        }

    } *PCLIENT_CONNECT_RESULT, CLIENT_CONNECT_RESULT;

}
