/****************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  NetPublicData.h
    ˵    ����

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����

******************************************************************/
#pragma once

#include "ServerData.h"

namespace server
{
    // ǰ������
    class CTerminal;
    class CConnection;

    enum  ePackParserType
    {
        PACK_PARSER_ENCRYPT = 0,	    // ����
        PACK_PARSER_NOENCRYPT,			// ������
        PACK_PARSER_TGW_ENCRYPT,		// ֧��TGW�����ܡ�����֧����Ѷƽ̨��
        PACK_PARSER_TGW_NOENCRYPT,		// ֧��TGW�������ܡ�����֧����Ѷƽ̨��
    };

    // ASIO client�첽���ӻص��ṹ��
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
