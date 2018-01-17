#pragma once
#include "TcpListenerManager.h"
#include "ConnectionManager.h"

class CMainProcess
{
public:
	CMainProcess();
	~CMainProcess();
	bool initialize();
	void run();
private:
	CTcpListenerManager m_oTcpListenerManager;
	CConnectionManager m_oConnectionManager;
};

