#include "MsgQManager.h"
#include "ConfigManager.h"
#include "WriteLog.h"
#include <exception>
#include <string.h>
#include "Common.h"
using namespace std;

CMsgQManager* CMsgQManager::pInstance = NULL;
bool CMsgQManager::initialize()
{
	try
	{
		string strKeyFilePath = CConfigManager::getInstance().getQKeyFilePath();
		int nSendQId = CConfigManager::getInstance().getRecvQId();
		int nRecvQId = CConfigManager::getInstance().getSendQId();
		m_nSendQId = msgget(ftok(strKeyFilePath.c_str(), nSendQId), 0660 | IPC_CREAT);
		m_nRecvQId = msgget(ftok(strKeyFilePath.c_str(), nRecvQId), 0660 | IPC_CREAT);
		return true;
	}
	catch (const std::exception& e)
	{
		LOG_ERROR("initialize %s", e.what());
		return false;
	}
}

void CMsgQManager::sendQMsg(void *pMsg, size_t msgLenth)
{
	if (msgsnd(m_nSendQId, pMsg, msgLenth, IPC_NOWAIT) == -1)
	{
		LOG_ERROR("msgsnd : %s", strerror(errno));
	}
}

bool CMsgQManager::tryGetOneQMsg(void *pMsg, size_t msgLenth, long mType)
{
	if ((msgrcv(m_nRecvQId, pMsg, msgLenth - sizeof(long), mType, IPC_NOWAIT | MSG_NOERROR)) == -1)
	{
		return false;
	}
	//LOG_DEBUG("");
	return true;
}

void CMsgQManager::mvMsgFromSendQ2RecvQForTest()
{
	S_RequestMsg oReqMsg;
	while ((msgrcv(m_nSendQId, &oReqMsg, sizeof(oReqMsg) - sizeof(long), 0, IPC_NOWAIT | MSG_NOERROR)) != -1)
	{
		S_ReplyMsg oReplyMsg;
		oReplyMsg.m_nMtype = oReqMsg.m_nReplyMType;
		oReplyMsg.m_nMsgLen = 0;
		oReplyMsg.m_nPkgNo = oReqMsg.m_nPkgNo;
		strncpy(oReplyMsg.m_szIpAddr, oReqMsg.m_szIpAddr, sizeof(oReplyMsg.m_szIpAddr));
		oReplyMsg.m_nReqReplyFlag = 1;
		oReplyMsg.m_nLastPkg = oReqMsg.m_nLastPkg;

		//增加同步头
		char syncHeadLen = 0;
		memcpy(oReplyMsg.m_szBuf + oReplyMsg.m_nMsgLen, &syncHeadLen, 1);
		oReplyMsg.m_nMsgLen++;
		//增加包头
		int nMsgHeadLen = sizeof(S_NetMsg) - sizeof(short);
		memcpy(oReplyMsg.m_szBuf + oReplyMsg.m_nMsgLen, oReqMsg.m_szBuf, nMsgHeadLen);
		oReplyMsg.m_nMsgLen = oReplyMsg.m_nMsgLen + nMsgHeadLen;//减去长度字段
		//增加应答包体
		char replyCode = 1;
		memcpy(oReplyMsg.m_szBuf + oReplyMsg.m_nMsgLen, &replyCode, 1);
		oReplyMsg.m_nMsgLen++;

		if (msgsnd(m_nRecvQId, &oReplyMsg, sizeof(S_ReplyMsg) - sizeof(long) - sizeof(oReplyMsg.m_szBuf) + oReplyMsg.m_nMsgLen, IPC_NOWAIT) == -1)
		{
			LOG_ERROR("msgsnd : %s", strerror(errno));
		}
	}
}

CMsgQManager::CMsgQManager()
{
}


CMsgQManager::~CMsgQManager()
{
}
