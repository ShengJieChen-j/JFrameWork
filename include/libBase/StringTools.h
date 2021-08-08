/********************************************************************
    �������ڣ� 2021��7��24��
    �ļ����ƣ� StringTools.h
    ˵    ���� �ַ�������

    ��ǰ�汾�� 1.00
    ��    �ߣ� shengjiechen
    ��    ���� ��Ҫ���ܣ�
               1. �ַ����ָ�
               2. �ַ����滻
               3. ���ַָ���ַ�����ʽ��ʾ
*********************************************************************/
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stdint.h>
using std::vector;
using std::string;
using std::map;
using std::basic_string;
using std::char_traits;
using std::allocator;
using std::basic_ostringstream;

namespace base
{

    namespace string_tool
    {
        // mysql �ַ���������ȫ����
        string& sql_escape(string& source);

        // ��ȡ�ַ������ż����ֵ �磺1��2��3
        void getIntValueSplitByComma(const char* pBuff, std::vector<int32_t>& vecValue);
        void getFloatValueSplitByComma(const char* pBuff, std::vector<float>& vecValue);
        void getUIntValueSplitByComma(const char* pBuff, std::vector<uint32_t>& vecValue);
        void getShortValueSplitByComma(const char* pBuff, std::vector<int16_t>& vecValue);
        void getStrValueSplitByComma(const char* pBuff, std::vector<string>& vecValue);
        void getInt64ValueSplitByComma(const char* pBuff, std::vector<int64_t>& vecValue);

        // ��ȡ�ַ������ż����ֵ �磺1,2,3, ���� 1,2,3
        void getIntValueSplitByCommaEx(const char* pBuff, std::vector<int32_t>& vecValue);
        void getFloatValueSplitByCommaEx(const char* pBuff, std::vector<float>& vecValue);
        void getUIntValueSplitByCommaEx(const char* pBuff, std::vector<uint32_t>& vecValue);
        void getShortValueSplitByCommaEx(const char* pBuff, std::vector<int16_t>& vecValue);
        void getStrValueSplitByCommaEx(const char* pBuff, std::vector<string>& vecValue);
        void getInt64ValueSplitByCommaEx(const char* pBuff, std::vector<int64_t>& vecValue);

        template <typename T>
        void getValueSplitByComma(const char* pBuff, std::vector<T>& vecValue)
        {
            if (NULL == pBuff)
            {
                return;
            }

            string str = pBuff;
            if (str.empty())
            {
                return;
            }

            string::size_type n = str.find(",");

            while (n != string::npos)
            {
                str.at(n) = '\0';
                vecValue.push_back((T)atoi(str.c_str()));
                str.erase(str.begin(), str.begin() + n + 1);
                n = str.find(",");
            }

            vecValue.push_back((T)atoi(str.c_str()));
        }

        // �滻ָ���ַ���
        template <typename T>
        inline T& replace(T& source, const T& repStr, const T& tgrStr)
        {
            typename T::size_type pos = 0;
            while (T::npos != (pos = source.find(repStr, pos)))
            {
                source.replace(pos, repStr.size(), tgrStr);
                pos += tgrStr.size();
            }

            return source;
        }

        // �ָ��ַ��� "1,,3," -- "1", "", "3", "" 
        template <typename T>
        inline void split(const T& source, const T& spStr, std::vector<T>& strs)
        {
            typename T::size_type orgPos = 0;
            typename T::size_type pos = source.find(spStr);

            while (T::npos != pos)
            {
                T temp(source.substr(orgPos, pos - orgPos));
                strs.push_back(temp);

                orgPos = pos + spStr.size();
                pos = source.find(spStr, orgPos);
            }

            if (!source.empty())
            {
                T temp(source.substr(orgPos));
                strs.push_back(temp);
            }
        }

        // �ָ��ַ��� "d=1&c=2" -- key-value "d"-"1"  "c"-"2"
        inline void split(std::string& strSrc,
            std::string& strSpliter,
            std::string& strPairSpliter,
            std::map<std::string, std::string>& mapStrSplit)
        {
            if (strSrc.empty())
            {
                return;
            }

            std::vector<std::string> vecSplits;
            split<std::string>(strSrc, strSpliter, vecSplits);
            std::vector<std::string>::iterator iter = vecSplits.begin();
            std::vector<std::string>::iterator iterEnd = vecSplits.end();
            for (; iter != iterEnd; ++iter)
            {
                std::vector<std::string> vecSplitPairs;
                split<std::string>((*iter).c_str(), strPairSpliter, vecSplitPairs);
                if (vecSplitPairs.size() == 2)
                {
                    mapStrSplit[vecSplitPairs[0]] = vecSplitPairs[1];
                }
            }
        }

        // �ָ��ַ��� "1,1;2,30;" --  1,1,2,30
        inline void split(std::string& strSrc,
            std::string& strFirstSpliter,
            std::string& strSecondSpliter,
            std::vector<int32_t>& vecValue)
        {
            if (strSrc.empty())
            {
                return;
            }

            std::vector<std::string> vecFirstSplits;
            split<std::string>(strSrc, strFirstSpliter, vecFirstSplits);
            std::vector<std::string>::iterator iter = vecFirstSplits.begin();
            std::vector<std::string>::iterator iterEnd = vecFirstSplits.end();
            for (; iter != iterEnd; ++iter)
            {
                std::vector<std::string> vecSecondSplits;
                split<std::string>((*iter).c_str(), strSecondSpliter, vecSecondSplits);
                for (size_t i = 0; i < vecSecondSplits.size(); ++i)
                {
                    vecValue.push_back(atoi(vecSecondSplits[i].c_str()));
                }
            }
        }

        // ʵ�����ֵĶ��ŷָ���ʾģʽ���� 1234567 �����Ϊ 1,234,567
        template<typename ELEM>
        inline basic_string<ELEM, char_traits<ELEM>, allocator<ELEM> > comma(uint32_t number)
        {
            bool bHighbit = false;
            typedef basic_ostringstream< ELEM, char_traits<ELEM>, allocator<ELEM> > streamtype;
            streamtype stream;
            uint16_t field1 = (uint16_t)(number % 1000);
            uint16_t field2 = (uint16_t)((number / 1000) % 1000);
            uint16_t field3 = (uint16_t)((number / 1000000) % 1000);
            uint16_t field4 = (uint16_t)((number / 1000000000) % 1000);
            if (field4 > 0)
            {
                stream << field4 << ELEM(',');
                stream.width(3);
                stream.fill(ELEM('0'));
                bHighbit = true;
            }
            if (field3 > 0 || bHighbit)
            {
                stream << field3 << ELEM(',');
                stream.width(3);
                stream.fill(ELEM('0'));
                bHighbit = true;
            }
            if (field2 > 0 || bHighbit)
            {
                stream << field2 << ELEM(',');
                stream.width(3);
                stream.fill(ELEM('0'));
                bHighbit = true;
            }
            stream << field1;

            return stream.str();
        }


        // ��ȡ�ַ������ż����ֵ �磺1,2,3 �����þ�̬�������
        // ������������ַ��� 1,,3 �м���λ��ʹ�ô�������Ĭ��ֵ
        template<typename T, uint32_t arraySize>
        inline void splitStringToArray(const char* str, T(&array)[arraySize])
        {
            string tmpStr = str;
            size_t pos = tmpStr.find_first_of(',');
            int32_t index = 0;
            stringstream sstream;
            while (pos != string::npos)
            {
                if (index >= arraySize)
                    break;

                sstream << tmpStr.substr(0, pos);
                sstream >> array[index++];
                sstream.clear();
                sstream.str("");
                tmpStr = tmpStr.substr(pos + 1);
                pos = tmpStr.find_first_of(',');
            }

            if (index < arraySize)
            {
                sstream << tmpStr.substr(0, pos);
                sstream >> array[index++];
            }
        }

        // ͬ�ϣ�char�����ػ�
        template<uint32_t arraySize>
        inline void splitStringToArray(const char* str, char(&array)[arraySize])
        {
            std::vector<std::string> tmpArray;
            split(std::string(str), std::string(","), tmpArray);
            for (uint32_t i = 0; i < arraySize && i < tmpArray.size(); ++i)
            {
                array[i] = atoi(tmpArray[i].c_str());
            }
        }

        // ͬ�ϣ�unsigned char�����ػ�
        template<uint32_t arraySize>
        inline void splitStringToArray(const char* str, unsigned char(&array)[arraySize])
        {
            std::vector<std::string> tmpArray;
            split<std::string>(std::string(str), std::string(","), tmpArray);
            for (uint32_t i = 0; i < arraySize && i < tmpArray.size(); ++i)
            {
                array[i] = atoi(tmpArray[i].c_str());
            }
        }

    };	// endof namespace StringTools

};	// endof namespace base

