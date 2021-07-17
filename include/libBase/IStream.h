/********************************************************************
    �������ڣ�  2021��7��17��
    �ļ����ƣ�  IStream.h
    ˵    ����  ������

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  �ַ���������ʽ ��Ҫ������Ϣͨ��

********************************************************************/
#pragma once

#include <stdint.h>
#include <string>

namespace base
{
    // ���󷵻�
    enum ISTREAM_ERROR_TYPE
    {
        ISTREAM_ERROR_NULL = 0,			// �ɹ�
        ISTREAM_ERROR_INPUT_TYPE,		// �����������
        ISTREAM_ERROR_INPUT_STRING,		// string�������
    };

    class CIStream
    {
    public:
        CIStream(char* ptr, uint16_t len)
        {
            m_ptr = ptr;
            m_len = len;
            m_pos = 0;
            m_LastError = ISTREAM_ERROR_NULL;
        }

        ~CIStream() {};

        char* getBuf() { return m_ptr; }

        uint16_t	    getPos() { return m_pos; }

        void			setPos(uint16_t pos) { m_pos = pos; };

        ISTREAM_ERROR_TYPE	getLastError() { return m_LastError; };

        uint16_t	setMsgSize()
        {
            if (NULL == m_ptr || m_pos < 2)
                return 0;

            uint16_t* pSize = (uint16_t*)m_ptr;
            *pSize = m_pos;
            return m_pos;
        }

        void	setMsgSize(uint16_t pos)
        {
            if (NULL == m_ptr || pos + 2 > m_pos)
                return;

            uint16_t* pSize = (uint16_t*)(m_ptr + pos);
            *pSize = (m_pos - pos);
        }

        inline void resetPosition()
        {
            m_pos = 0;
            m_LastError = ISTREAM_ERROR_NULL;
        }

        void set(const char* pData, uint16_t length)
        {
            if (m_LastError != ISTREAM_ERROR_NULL)
                return;

            if (m_pos + length > m_len)
            {
                m_LastError = ISTREAM_ERROR_INPUT_TYPE;
            }
            else
            {
#ifdef __linux__
                memcpy(m_ptr + m_pos, pData, length);
#else
                memcpy_s(m_ptr + m_pos, length, pData, length);
#endif
                m_pos += length;
            }
        }

        // any type ��ֵ
        template<typename T>
        CIStream& operator<<(T f)
        {
            if (m_LastError != ISTREAM_ERROR_NULL)
                return *this;

            uint16_t datalen = sizeof(T);
            if (m_pos + datalen > m_len)
            {
                m_LastError = ISTREAM_ERROR_INPUT_TYPE;
            }
            else
            {
#ifdef __linux__
                memcpy(m_ptr + m_pos, &f, datalen);
#else
                memcpy_s(m_ptr + m_pos, datalen, &f, datalen);
#endif
                m_pos += datalen;
            }

            return *this;
        }

        // string ��ֵ
        CIStream& operator<<(const std::string& str)
        {
            if (m_LastError != ISTREAM_ERROR_NULL)
                return *this;

            if (m_pos + str.length() + 2 > m_len || str.length() > 65535)
            {
                m_LastError = ISTREAM_ERROR_INPUT_STRING;
            }
            else
            {
                *(uint16_t*)(m_ptr + m_pos) = (uint16_t)str.length();
#ifdef __linux__
                memcpy(m_ptr + m_pos + 2, str.c_str(), str.length() > (m_len - m_pos - 2) ? m_len - m_pos - 2 : str.length());
#else
                memcpy_s(m_ptr + m_pos + 2, m_len - m_pos - 2, str.c_str(), str.length());
#endif
                m_pos += (uint16_t)str.length() + 2;
            }

            return *this;
        }

        bool append(void* pData, uint16_t nLen)
        {
            if (m_LastError != ISTREAM_ERROR_NULL)
                return false;

            if (NULL == pData)
            {
                m_LastError = ISTREAM_ERROR_INPUT_TYPE;
                return false;
            }

            if (m_pos + nLen > m_len || nLen > 65535)
            {
                m_LastError = ISTREAM_ERROR_INPUT_TYPE;
                return false;
            }
#ifdef __linux__
            memcpy(m_ptr + m_pos, pData, nLen > (m_len - m_pos) ? (m_len - m_pos) : nLen);
#else   
            memcpy_s(m_ptr + m_pos, m_len - m_pos, pData, nLen);
#endif
            m_pos += nLen;

            return true;
        }

    private:
        char* m_ptr;
        uint16_t		    m_len;
        uint16_t		    m_pos;
        ISTREAM_ERROR_TYPE	m_LastError;	// ��ȷ���� 0

    };	// endof class CIStream

};	// endof namespace base;

