#pragma once
#include "ConnectionBase.h"
#include "TcpSvrConnection.h"

class CMonitorConnection : public CTcpSvrConnection
{
public:
	CMonitorConnection();
	virtual ~CMonitorConnection();

	virtual void	doMsg();
};

