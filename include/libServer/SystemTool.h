/********************************************************************
    �������ڣ�  2021��7��24��
    �ļ����ƣ�  SystemTool.h
    ˵    ����  ϵͳ��������

    ��ǰ�汾��  1.00
    ��    �ߣ�  shengjiechen
    ��    ����  ��ȡ���̼�ϵͳ�������Ϣ (���̰߳�ȫ)

********************************************************************/
#pragma once
#include <stdint.h>

namespace server
{

    class SystemTool
    {
    public:
        /// ��������ʼ����ʱ��������
        static void init();

    public:
        /// ��ȡCPU�ĺ���
        static int32_t getProcessorNumber();

        /// ��ȡ��ǰ���̵�cpu usage������-1ʧ��
        static int32_t getCpuUsage();

        /// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ�
        static int32_t getMemoryUsage(uint64_t* mem, uint64_t* vmem);

        /// ��ȡ��ǰ�����ܹ�����д��IO�ֽ���������-1ʧ�ܣ�0�ɹ�
        static int32_t getIOBytes(uint64_t* read_bytes, uint64_t* write_bytes);

        /// ���δ��ڹرհ�ť
        static void forbidWindowClose();

        /// ����Ctrl + C ��ݼ�
        static void	forbidCtrlC();

    private:
        static uint64_t filetimeToUTC(const FILETIME* ftime);
        static BOOL WINAPI ConsoleHandler(DWORD msgType);
    };

};