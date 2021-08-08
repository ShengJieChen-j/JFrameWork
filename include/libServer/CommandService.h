/******************************************************************

	�������ڣ�  2021��7��24��
	�ļ����ƣ�  CommandService.h
	˵    ����	������

	��ǰ�汾��  1.00
	��    �ߣ�  shengjiechen
	��    ����

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
		// ����
		static void cmd_Help(const char* pParam);			// ������Ϣ
		static void cmd_Exit(const char* pParam);			// �����˳�������
		static void cmd_Quit(const char* pParam);			// �����رշ�����(����������)

		static void cmd_ShowConnect(const char* pParam);	// ��ʾ������������Ϣ
		static void cmd_ShowFPS(const char* pParam);		// ��ʾ֡��
		static void cmd_Allocate(const char* pParam);		// ��ʾ��Ҫ�ڴ������Ϣ
		static void cmd_Performance(const char* pParam);	// ��ʾ��Ҫ����ͳ����Ϣ
		static void cmd_ReloadConfig(const char* pParam);	// ���¼���������Դ
		static void cmd_StartCheck(const char* pParam);		// ��ʼ����ڴ�
		static void cmd_EndCheck(const char* pParam);		// ��������ڴ�
		static void cmd_PrintBandwidth(const char* pParam);	// ��ӡ����

	private:
		void		onUpdate(uint64_t nTickerID, uint64_t nContext);
		void        printfCommandInfo();

	private:
		base::CConsole* m_pConsole;
		CTicker				m_tickUpdate;
	};

}
