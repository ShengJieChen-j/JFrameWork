/******************************************************************

	创建日期：  2021年7月24日
	文件名称：  CommandService.h
	说    明：	命令行

	当前版本：  1.00
	作    者：  shengjiechen
	概    述：

*******************************************************************/

#pragma  once
#include <vector>
#include "libBase/noncopyable.h"
#include "libBase/Console.h"
#include "Ticker.h"

namespace server
{
	class CCommandService : public base::noncopyable_::noncopyable
	{
	public:
		CCommandService();
		~CCommandService();

		bool		init();
		void		registCommand(const base::SCommandInfo& sCommandInfo);
		void		startTicker(int32_t nCycle);
		void		release();

	public:
		// 命令
		static void cmd_Help(const char* pParam);			// 帮助信息
		static void cmd_Exit(const char* pParam);			// 保存退出服务器
		static void cmd_Quit(const char* pParam);			// 立即关闭服务器(不保存数据)

		static void cmd_ShowConnect(const char* pParam);	// 显示服务器链接信息
		static void cmd_ShowFPS(const char* pParam);		// 显示帧数
		static void cmd_Allocate(const char* pParam);		// 显示主要内存分配信息
		static void cmd_Performance(const char* pParam);	// 显示主要性能统计信息
		static void cmd_ReloadConfig(const char* pParam);	// 重新加载配置资源
		static void cmd_StartCheck(const char* pParam);		// 开始检查内存
		static void cmd_EndCheck(const char* pParam);		// 结束检查内存
		static void cmd_PrintBandwidth(const char* pParam);	// 打印带宽

	private:
		void		onUpdate(uint64_t nTickerID, uint64_t nContext);
		void        printfCommandInfo();

	private:
		base::CConsole* m_pConsole;
		CTicker				m_tickUpdate;
	};

}
