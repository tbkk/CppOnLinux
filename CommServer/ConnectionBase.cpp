#include "ConnectionBase.h"
#include <string.h>
#include "WriteLog.h"
#include <iostream>
#include <string>
#include <netinet/in.h>
#include "Common.h"
#include <unistd.h>
#include "MsgQManager.h"

using namespace std;



CConnectionBase::CConnectionBase()
{
}


CConnectionBase::~CConnectionBase()
{
}

bool CConnectionBase::pushMsg(char * buffer, int buffSize)
{
	//LOG_DEBUG("current bytes is %d",  m_nCurrDataLenth);
	if (buffSize + m_nCurrDataLenth > MAX_BUFFE_LENTH)
	{
		LOG_WARN("Not enough space, MAX_BUFFE_LENTH is %d ,current data lenth is %d , recv buffer size is %d", MAX_BUFFE_LENTH, m_nCurrDataLenth, buffSize);
		return false;
	}

	memcpy(m_szDatabuffer + m_nCurrDataLenth, buffer, buffSize);
	m_nCurrDataLenth += buffSize;
	//LOG_DEBUG("push : %d bytes, current bytes is %d", buffSize, m_nCurrDataLenth);
	return true;
}

void CConnectionBase::doMsg()
{
	LOG_DEBUG("CConnectionBase::doMsg()");
	//LOG_DEBUG("current bytes is %d", m_nCurrDataLenth);
	//if (m_nCurrDataLenth < sizeof(short))
	//{
	//	return false;
	//}

	//S_NetMsg * pMsg = (S_NetMsg *)m_szDatabuffer;
	//if (m_nCurrDataLenth < pMsg->getTotalLen() )
	//{
	//	LOG_INFO("Not enouth Msg!");
	//	return false;
	//}

	//S_RequestMsg oRequestMsg(m_strRemoteIp.c_str(),m_nReplyMtype, pMsg);
	//

	//int nSurplusLenth = m_nCurrDataLenth - oRequestMsg.m_nMsgLen - sizeof(short);
	//memmove(m_szDatabuffer, m_szDatabuffer + sizeof(short) + oRequestMsg.m_nMsgLen, nSurplusLenth);
	//m_nCurrDataLenth = nSurplusLenth;
	//
	////write(m_nConnSocketFd, oRequestMsg.toString().c_str(), oRequestMsg.toString().length());

	////LOG_DEBUG(oRequestMsg.toString().c_str());
	//CMsgQManager::getInstance().sendQMsg(&oRequestMsg, oRequestMsg.getMsgQSendLenth());
	//LOG_DEBUG("Send Msg to Q : %d Msg : %s", CMsgQManager::getInstance().getSendQId(), oRequestMsg.toString().c_str());
	return;
}

std::string CConnectionBase::toString()
{
	ostringstream oss(ostringstream::out);
	oss << *this;
	return oss.str();
}
