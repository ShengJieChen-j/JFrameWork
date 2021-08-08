/******************************************************************

    创建日期：  2021年7月24日
    文件名称：  keycreater.h
    说    明：  key 生成器：guidkey, session key

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  线程安全(createKey函数不支持linux)

*******************************************************************/
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace base
{
    // 生成唯一Key(具有回收功能的key)
    const int32_t UKC_MAX_KEY_BUFFER = 10000;

    class CUniqueKeyCreater
    {
    public:
        CUniqueKeyCreater();
        ~CUniqueKeyCreater();

    public:
        int32_t					createKey();                // 创建key， 返回-1表示创建失败
        void					freeKey(int32_t nKey);      // 回收key

        int32_t                 getUsedKeys();              // 当前已经分配出去的key个数
        int32_t                 getMaxKeys();               // 最大key个数

    private:
        void					growKey();

    private:
        std::vector<int32_t*>	m_vec_KeyList;              // key的分段连续容器
        int32_t					m_CurrentBlock;
        int32_t					m_CurrentIndex;
        int32_t					m_MaxBlock;
    };

};	// endof namespace base
