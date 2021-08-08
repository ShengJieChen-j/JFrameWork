#include "stdafx.h"
#include "ServerNetEvent.h"
#include "ServerFramework.h"
#include "MessageSubject.h"

namespace server
{

	CServerNetEvent::CServerNetEvent()
	{
	}

	CServerNetEvent::~CServerNetEvent()
	{
	}

	void CServerNetEvent::doResolveMsg(IMsg* pMsg)
	{
		CServerFramework::Inst()->getMessageSubject()->resolveMsg(pMsg);
	}

	bool CServerNetEvent::doOnConnect(IMsg* pMsg)
	{
		return CServerFramework::Inst()->doOnConnect(pMsg);
	}

	void CServerNetEvent::doOnClose(uint32_t unSocketID)
	{
		CServerFramework::Inst()->doOnClose(unSocketID);
	}

	void CServerNetEvent::doDoConnectSuccess(ServerLinkInfo& data)
	{
	}

	void CServerNetEvent::doDoConnectFail(ServerLinkInfo& data)
	{
		CONSOLE_WRITE("Connect to %s[%s:%d] error!\n", data.server_ServerName,
			data.server_ip, data.server_port);
	}

	bool CServerNetEvent::doWebConnect(IMsg* pMsg)
	{
		return true;
	}

}
