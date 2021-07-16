/******************************************************************
    创建日期：  2021年7月15日
    文件名称：  TypeDefine.h
    说    明：  类型定义
    
    当前版本：  1.00
    作    者：  shengjiechen
    概    述：      

*******************************************************************/
#pragma once

#include <stdint.h>

// 跨平台的类型统一
#if defined __LINUX__
typedef uint32_t			DWORD;
#endif

// buf大小定义
const int32_t		BUF_SIZE_4			= 4;
const int32_t		BUF_SIZE_8			= 8;
const int32_t		BUF_SIZE_16			= 16;
const int32_t		BUF_SIZE_32			= 32;
const int32_t		BUF_SIZE_64			= 64;
const int32_t		BUF_SIZE_128		= 128;
const int32_t		BUF_SIZE_256		= 256;
const int32_t		BUF_SIZE_512		= 512;
const int32_t		BUF_SIZE_1024		= 1024;
const int32_t		BUF_SIZE_2048		= 2048;
const int32_t		BUF_SIZE_3072		= 3072;
const int32_t		BUF_SIZE_4096		= 4096;
const int32_t		BUF_SIZE_5120		= 5120;
const int32_t		BUF_SIZE_8192		= 8192;
const int32_t		BUF_SIZE_16384		= 16384;
const int32_t		BUF_SIZE_32768		= 32768;
