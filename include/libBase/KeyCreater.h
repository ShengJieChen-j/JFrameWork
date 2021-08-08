/******************************************************************

    �������ڣ�  2021��7��24��
    �ļ����ƣ�  keycreater.h
    ˵    ����  key ��������guidkey, session key

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  �̰߳�ȫ(createKey������֧��linux)

*******************************************************************/
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace base
{
    // ����ΨһKey(���л��չ��ܵ�key)
    const int32_t UKC_MAX_KEY_BUFFER = 10000;

    class CUniqueKeyCreater
    {
    public:
        CUniqueKeyCreater();
        ~CUniqueKeyCreater();

    public:
        int32_t					createKey();                // ����key�� ����-1��ʾ����ʧ��
        void					freeKey(int32_t nKey);      // ����key

        int32_t                 getUsedKeys();              // ��ǰ�Ѿ������ȥ��key����
        int32_t                 getMaxKeys();               // ���key����

    private:
        void					growKey();

    private:
        std::vector<int32_t*>	m_vec_KeyList;              // key�ķֶ���������
        int32_t					m_CurrentBlock;
        int32_t					m_CurrentIndex;
        int32_t					m_MaxBlock;
    };

};	// endof namespace base
