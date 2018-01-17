#include "MainProcess.h"
#include "WriteLog.h"
#include "RunningControl.h"
#include <unistd.h>
#include "MsgQManager.h"
#include "MonitorManager.h"



CMainProcess::CMainProcess()
{
}


CMainProcess::~CMainProcess()
{
}

bool CMainProcess::initialize()
{
	LOG_INFO("init OK!");
	CMonitorManager::getInstance().registerMoniModuel(m_oConnectionManager);
	return true;
}

void CMainProcess::run()
{
	LOG_INFO("Start MainProcess!");

	//m_oTcpListenerManager.startListen("127.0.0.1", 6000);
	m_oTcpListenerManager.startMultiListen();
	while (CRunningControl::getInstance().isRunning())
	{
		m_oConnectionManager.addNewConnections(m_oTcpListenerManager.getNewConnections());
		m_oConnectionManager.tryNetMsg2QMsg();


		//TEST
		CMsgQManager::getInstance().mvMsgFromSendQ2RecvQForTest();
		//TEST END


		m_oConnectionManager.tryQMsg2NetMsg();
		m_oConnectionManager.checkConnectionAviable();
		//sleep(1);
	}
	
}
