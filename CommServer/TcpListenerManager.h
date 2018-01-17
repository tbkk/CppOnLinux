#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "TcpSvrConnection.h"

class CTcpListenerManager
{
public:
	CTcpListenerManager();
	~CTcpListenerManager();
	
	bool startMultiListen();
	int startListen(std::string strlocalIp, int nPort);
	std::list<CConnectionBase *> getNewConnections();

private:
	//int m_listenSocket;
	std::list<int> m_lListenSocketList;
	//std::unordered_map<int, CConnectionBase> m_mpListenSocketMap;
};

