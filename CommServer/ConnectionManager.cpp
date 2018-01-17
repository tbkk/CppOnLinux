#include "ConnectionManager.h"
#include <unistd.h>
#include <list>
#include "WriteLog.h"
#include "Common.h"
#include "MsgQManager.h"
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include "TcpSvrConnection.h"

using namespace std;

CConnectionManager::CConnectionManager()
{
	m_nMtype = 1;
}


CConnectionManager::~CConnectionManager()
{
	//! close all socket
	//BOOST_FOREACH(const DF_ITEM &item, fds_list)
	//{
	//	close(item.first);
	//}
}

void CConnectionManager::initialize()
{
}

void CConnectionManager::tryNetMsg2QMsg()
{
	fd_set connFds;
	FD_ZERO(&connFds);
	int nMaxFds = 0;

	for (map<long,CConnectionBase *>::iterator itorConn = m_mpConnDetails.begin(); itorConn != m_mpConnDetails.end(); itorConn++)
	{
		//LOG_DEBUG(itorConn->toString().c_str());
		if (itorConn->second->m_nConnSocketFd > nMaxFds)
		{
			nMaxFds = itorConn->second->m_nConnSocketFd;
		}
		FD_SET(itorConn->second->m_nConnSocketFd, &connFds);
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;//500ms
	int nSelectRet = select(nMaxFds + 1, &connFds, NULL, NULL, &tv);
	if (nSelectRet > 0)
	{
		for (map<long, CConnectionBase * >::iterator itorConn = m_mpConnDetails.begin(); itorConn != m_mpConnDetails.end(); )
		{
			CConnectionBase * pConnection = itorConn->second;
			if (!FD_ISSET(pConnection->m_nConnSocketFd, &connFds))
			{
				itorConn++;
				continue;
			}
			//此处Read
			const int buffer_size = 1024;
			char buffer[buffer_size] = { 0 };
			int nLen = read(pConnection->m_nConnSocketFd, buffer, buffer_size);
			if (nLen <= 0 && errno != EINTR)
			{
				LOG_DEBUG("Close Connection : %s", pConnection->toString().c_str());
				close(pConnection->m_nConnSocketFd);
				delete pConnection;//释放
				pConnection = NULL;
				itorConn = m_mpConnDetails.erase(itorConn);
			}
			else
			{
				
				pConnection->pushMsg(buffer, nLen);
				pConnection->doMsg();
				//while (pConnection->doMsg()) {};
				itorConn++;
			}
		}
	}
}

void CConnectionManager::tryQMsg2NetMsg()
{
	S_ReplyMsg oMsg;
	while (CMsgQManager::getInstance().tryGetOneQMsg(&oMsg, sizeof(oMsg), 0))
	{
		LOG_DEBUG("Read Msg from Q: %d, Msg: %s", CMsgQManager::getInstance().getRecvQid(), oMsg.toString().c_str());
		if ((getpid() & 0xffff) != (oMsg.m_nMtype >> 16))
		{
			LOG_WARN("find inviald msg from Q: %d, Msg: %s", CMsgQManager::getInstance().getRecvQid(), oMsg.toString().c_str());
			continue;
		}
		if (m_mpConnDetails.count(oMsg.m_nMtype) <= 0)
		{
			LOG_WARN("find inviald msg from Q: %d, Msg: %s", CMsgQManager::getInstance().getRecvQid(), oMsg.toString().c_str());
			continue;
		}

		//remove sycn head
		int nSyncHeadLen = oMsg.m_szBuf[0];
		memmove(oMsg.m_szBuf, oMsg.m_szBuf + nSyncHeadLen + 1, oMsg.m_nMsgLen - nSyncHeadLen - 1);
		oMsg.m_nMsgLen = oMsg.m_nMsgLen - nSyncHeadLen - 1;


		//begin write to socket
		CConnectionBase * pConnection = m_mpConnDetails[oMsg.m_nMtype];
		char szSendBuffer[8192] = { 0 };
		S_NetMsg * pSendMsg = (S_NetMsg *)szSendBuffer;
		pSendMsg->m_nMsgLen = htons(oMsg.m_nMsgLen);
		memcpy((char *)pSendMsg + sizeof(pSendMsg->m_nMsgLen), oMsg.m_szBuf, oMsg.m_nMsgLen);

		write(pConnection->m_nConnSocketFd, pSendMsg, oMsg.m_nMsgLen + sizeof(pSendMsg->m_nMsgLen));
		//write(oConnectionDetail.m_nConnSocketFd, &oMsg, oMsg.toString().length());
		//write(oConnectionDetail.m_nConnSocketFd, oMsg.toString().c_str(), oMsg.toString().length());
	}
}

void CConnectionManager::checkConnectionAviable()
{
	//LOG_INFO("checkConnectionAviable");
}

void CConnectionManager::addNewConnections(std::list<CConnectionBase*> lNewConnectionList)
{
	for (list<CConnectionBase*>::iterator itorConn = lNewConnectionList.begin(); itorConn != lNewConnectionList.end(); itorConn++)
	{
		if ((*itorConn)->getConnType() == CONN_TYPE::CONN_TYPE_TCP_SVR)
		{
			CTcpSvrConnection * pConn = (CTcpSvrConnection *)(*itorConn);
			if (!m_mpConnDetails.count(pConn->m_nReplyMtype))
			{
				m_mpConnDetails[pConn->m_nReplyMtype] = *itorConn;
			}
		}
		else
		{
			//增加其他类型
		}

		//m_lConnDetails.push_back(*itorConn);
	}
}
