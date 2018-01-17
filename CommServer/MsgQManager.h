#pragma once
#include <iostream> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class CMsgQManager
{

public:
	static CMsgQManager& getInstance()
	{
		if (NULL == pInstance)
		{
			pInstance = new CMsgQManager();
		}
		return *pInstance;
	};

	bool initialize();
	void sendQMsg(void *pMsg, size_t msgLenth);
	bool tryGetOneQMsg(void *msgp, size_t msgsz, long msgtyp);

	void mvMsgFromSendQ2RecvQForTest();

	long getSendQId()
	{
		return m_nSendQId;
	}

	long getRecvQid()
	{
		return m_nRecvQId;
	}

private:
	static CMsgQManager* pInstance;
	long	m_nSendQId;
	long	m_nRecvQId;

	CMsgQManager();
	~CMsgQManager();
};

