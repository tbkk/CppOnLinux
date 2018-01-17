#include "MonitorConnection.h"
#include "WriteLog.h"
#include "MonitorManager.h"
#include <unistd.h>

using namespace std;
CMonitorConnection::CMonitorConnection()
{
}


CMonitorConnection::~CMonitorConnection()
{
}

void CMonitorConnection::doMsg()
{
	std::map<long, CConnectionBase *> & mpConnDetailsMap = CMonitorManager::getInstance().getConnDetailsMap();

	for (map<long, CConnectionBase *>::iterator itorConn = mpConnDetailsMap.begin(); itorConn != mpConnDetailsMap.end(); itorConn++)
	{
		LOG_DEBUG("Monitor : %s", itorConn->second->toString().c_str());
		write(m_nConnSocketFd, itorConn->second->toString().c_str(), itorConn->second->toString().length());
	}

	LOG_DEBUG("Current Monitor buffer is [%s]!!!!!!!!!!!!", m_szDatabuffer);
	m_nCurrDataLenth = 0;
	LOG_DEBUG("Monitor !!!!!!!!!!!!");
	return;
}
