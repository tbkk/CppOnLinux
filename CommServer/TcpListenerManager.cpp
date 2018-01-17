#include "TcpListenerManager.h"
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <list>
#include "ConnectionBase.h"
#include "ConfigManager.h"
#include "WriteLog.h"
#include "MonitorConnection.h"
#include <string.h>


using namespace std;

static int g_nMtype = 1;
#define		NEXT_MTYPE					(((getpid() & 0xffff) << 16) | (g_nMtype++ & 0xffff))

CTcpListenerManager::CTcpListenerManager()
{
}


CTcpListenerManager::~CTcpListenerManager()
{

}

bool CTcpListenerManager::startMultiListen()
{
	list<int> lListenPortList = CConfigManager::getInstance().getListenPortList();
	LOG_DEBUG("lListenPortList size %d", lListenPortList.size());
	for (list<int>::iterator itor = lListenPortList.begin(); itor != lListenPortList.end(); itor++)
	{
		int listenSocket = startListen("0.0.0.0", *itor);
		if (listenSocket > 0)
		{
			m_lListenSocketList.push_back(listenSocket);
		}
	}
}

int CTcpListenerManager::startListen(std::string strlocalIp, int nPort)
{
	LOG_DEBUG("start listen %s - %d", strlocalIp.c_str(), nPort);
	int nListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nListenSocket == -1)
	{
		LOG_ERROR("create socket error");
		return 0;
	}

	const int val = 1;

	if (setsockopt(nListenSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
	{
		LOG_ERROR("setsockopt socket error");
		close(nListenSocket);
		return 0;
	}

	struct linger soLinger;
	soLinger.l_onoff = 1;
	soLinger.l_linger = 0;
	if (setsockopt(nListenSocket, SOL_SOCKET, SO_LINGER, (char *)&soLinger, sizeof(struct linger)) < 0)
	{
		LOG_ERROR("setsockopt socket error");
		close(nListenSocket);
		return 0;
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(strlocalIp.c_str());
	server_addr.sin_port = htons(nPort);

	if (bind(nListenSocket, (const struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		LOG_ERROR("bind socket error");
		close(nListenSocket);
		return 0;
	}

	listen(nListenSocket, 10);
	return nListenSocket;

}

std::list<CConnectionBase *> CTcpListenerManager::getNewConnections()
{
	list<CConnectionBase *> lConnections;

	fd_set listenFds;
	FD_ZERO(&listenFds);
	int nMaxFds = 0;
	for (list<int>::iterator itorFd = m_lListenSocketList.begin(); itorFd != m_lListenSocketList.end(); itorFd++)
	{
		//LOG_DEBUG("itorFd %d", *itorFd);
		if (*itorFd > nMaxFds)
		{
			nMaxFds = *itorFd;
		}
		FD_SET(*itorFd, &listenFds);
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int nSelectRet = select(nMaxFds + 1, &listenFds, NULL, NULL, &tv);
	//LOG_DEBUG("nSelectRet %d", nSelectRet);
	if (nSelectRet > 0)
	{
		for (list<int>::iterator itorFd = m_lListenSocketList.begin(); itorFd != m_lListenSocketList.end(); itorFd++)
		{
			int nCurrListenSocket = *itorFd;
			if (FD_ISSET(nCurrListenSocket, &listenFds))
			{
				struct sockaddr_in clientAddr;
				socklen_t clientAddrLen;
				int nNewConnectionSocket = accept(nCurrListenSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
				if (nNewConnectionSocket <= 0)
				{
					LOG_WARN("Accept Error!");
					continue;
					//LOG_DEBUG("New Connection form ip:%s, port:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
				}

				struct sockaddr_in sa;
				socklen_t len = sizeof(sa);
				if (getsockname(nCurrListenSocket, (struct sockaddr *)&sa, &len))
				{
					LOG_WARN("Can not fand listenIp and listenPort!");
					continue;
				}

				CTcpSvrConnection * pNewConn = NULL;
				if (CConfigManager::getInstance().getMonitorPort() == ntohs(sa.sin_port))
				{
					pNewConn = new CMonitorConnection();
				}
				else
				{
					pNewConn = new CTcpSvrConnection();
				}

				pNewConn->m_strListenIp = inet_ntoa(sa.sin_addr);
				pNewConn->m_nListenPort = ntohs(sa.sin_port);
				pNewConn->m_nListenSocketFd = nCurrListenSocket;
				pNewConn->m_nConnSocketFd = nNewConnectionSocket;
				pNewConn->m_tCreateTime = time(NULL);
				pNewConn->m_nReplyMtype = NEXT_MTYPE;

				if (!getpeername(nNewConnectionSocket, (struct sockaddr *)&sa, &len))
				{
					pNewConn->m_nRemotePort = ntohs(sa.sin_port);
					pNewConn->m_strRemoteIp = inet_ntoa(sa.sin_addr);
				}

				lConnections.push_back(pNewConn);
				
			}
		}

	}

	//return nMaxFds;



	//处理列表省略
	return lConnections;
}