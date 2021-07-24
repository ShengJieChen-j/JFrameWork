/********************************************************************
    创建日期：  2021年7月24日
    文件名称：  SystemTool.h
    说    明：  系统环境工具

    当前版本：  1.00
    作    者：  shengjiechen
    概    述：  获取进程及系统方面的信息 (非线程安全)

********************************************************************/
#pragma once
#include <stdint.h>

namespace server
{

    class SystemTool
    {
    public:
        /// 服务器初始化的时候必须调用
        static void init();

    public:
        /// 获取CPU的核数
        static int32_t getProcessorNumber();

        /// 获取当前进程的cpu usage，返回-1失败
        static int32_t getCpuUsage();

        /// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
        static int32_t getMemoryUsage(uint64_t* mem, uint64_t* vmem);

        /// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
        static int32_t getIOBytes(uint64_t* read_bytes, uint64_t* write_bytes);

        /// 屏蔽窗口关闭按钮
        static void forbidWindowClose();

        /// 屏蔽Ctrl + C 快捷键
        static void	forbidCtrlC();

    private:
        static uint64_t filetimeToUTC(const FILETIME* ftime);
        static BOOL WINAPI ConsoleHandler(DWORD msgType);
    };

};