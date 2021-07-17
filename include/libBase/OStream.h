/********************************************************************
    创建日期：  2021年7月17日
    文件名称：  OStream.h
    说    明：  输出流

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  字符流操作方式 主要用于消息通信

********************************************************************/
#pragma once

#include <stdint.h>
#include <string>

namespace base
{
    // 错误返回
    enum OSTREAM_ERROR_TYPE
    {
        OSTREAM_ERROR_NULL = 0,			// 成功
        OSTREAM_ERROR_OUTPUT_TYPE,		// 类型输出错误
        OSTREAM_ERROR_OUTPUT_STRING,	// string输出错误
        OSTREAM_ERROR_OUTPUT_STRING1,	// string解析长度出错
    };

    class COStream
    {
    public:
        COStream() {};
        COStream(const char* ptr, uint16_t len)
        {
            m_ptr = ptr;
            m_len = len;
            m_pos = 0;
            m_LastError = OSTREAM_ERROR_NULL;
        }

        void init(const char* ptr, uint16_t len)
        {
            m_ptr = ptr;
            m_len = len;
            m_pos = 0;
            m_LastError = OSTREAM_ERROR_NULL;
        }

        ~COStream() {};

        const char* getBuf() { return m_ptr; }

        uint16_t	    getPos() { return m_pos; }

        void			setPos(uint16_t pos) { m_pos = pos; };

        OSTREAM_ERROR_TYPE	getLastError() { return m_LastError; };

        inline void resetPosition()
        {
            m_pos = 0;
            m_LastError = OSTREAM_ERROR_NULL;
        }

        void get(char* pData, uint16_t length)
        {
            if (m_pos + length > m_len)
            {
                m_LastError = OSTREAM_ERROR_OUTPUT_TYPE;
            }
            else
            {
#ifdef __linux__
                memcpy(pData, m_ptr + m_pos, length);
#else
                memcpy_s(pData, length, m_ptr + m_pos, length);
#endif
                m_pos += length;
            }
        }

        // any type 输出
        template<typename T>
        COStream& operator>>(T& f)
        {
            if (m_LastError != OSTREAM_ERROR_NULL)
                return *this;

            uint16_t datalen = sizeof(T);
            if (m_pos + datalen > m_len)
            {
                m_LastError = OSTREAM_ERROR_OUTPUT_TYPE;
            }
            else
            {
#ifdef __linux__
                memcpy(&f, m_ptr + m_pos, datalen);
#else
                memcpy_s(&f, datalen, m_ptr + m_pos, datalen);
#endif
                m_pos += datalen;
            }

            return *this;
        }


        // string 输出
        COStream& operator>>(std::string& str)
        {
            if (m_LastError != OSTREAM_ERROR_NULL)
                return *this;

            if (m_pos + 2 > m_len)
            {
                m_LastError = OSTREAM_ERROR_OUTPUT_STRING;
            }
            else
            {
                uint16_t slen = *(uint16_t*)(m_ptr + m_pos);
                if (m_pos + 2 + slen > m_len)
                {
                    m_LastError = OSTREAM_ERROR_OUTPUT_STRING1;
                }
                else
                {
                    str = std::string(m_ptr + m_pos + 2, slen);
                    m_pos += slen + 2;
                }
            }

            return *this;
        }

    private:
        const char* m_ptr;
        uint16_t		    m_len;
        uint16_t		    m_pos;
        OSTREAM_ERROR_TYPE	m_LastError;	// 正确返回 0

    };	// endof class COStream

};	// endof namespace base;