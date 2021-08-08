/********************************************************************
    创建日期： 2021年7月24日
    文件名称： StringTools.h
    说    明： 字符串操作

    当前版本： 1.00
    作    者： shengjiechen
    概    述： 主要功能：
               1. 字符串分割
               2. 字符串替换
               3. 数字分割成字符串格式显示
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
        // mysql 字符串参数安全处理
        string& sql_escape(string& source);

        // 获取字符串逗号间隔的值 如：1，2，3
        void getIntValueSplitByComma(const char* pBuff, std::vector<int32_t>& vecValue);
        void getFloatValueSplitByComma(const char* pBuff, std::vector<float>& vecValue);
        void getUIntValueSplitByComma(const char* pBuff, std::vector<uint32_t>& vecValue);
        void getShortValueSplitByComma(const char* pBuff, std::vector<int16_t>& vecValue);
        void getStrValueSplitByComma(const char* pBuff, std::vector<string>& vecValue);
        void getInt64ValueSplitByComma(const char* pBuff, std::vector<int64_t>& vecValue);

        // 获取字符串逗号间隔的值 如：1,2,3, 或者 1,2,3
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

        // 替换指定字符串
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

        // 分割字符串 "1,,3," -- "1", "", "3", "" 
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

        // 分割字符串 "d=1&c=2" -- key-value "d"-"1"  "c"-"2"
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

        // 分割字符串 "1,1;2,30;" --  1,1,2,30
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

        // 实现数字的逗号分隔显示模式例如 1234567 输出成为 1,234,567
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


        // 获取字符串逗号间隔的值 如：1,2,3 可以用静态数组接收
        // 如果出现这种字符串 1,,3 中间那位将使用传进来的默认值
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

        // 同上，char类型特化
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

        // 同上，unsigned char类型特化
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

