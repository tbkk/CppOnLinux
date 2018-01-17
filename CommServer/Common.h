#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <iomanip>
#include "MsgQManager.h"
#include <sstream>


#define 	MSGMAX 						8192
#define 	MAX_NODE_ID_LEN 			20



#pragma pack(1) //设置字节对齐
struct S_NetMsg	//包体在这个结构体后面跟着
{
	short m_nMsgLen;
	char  m_szMsgType[2];
	char  m_szSenderId[4];
	unsigned int m_unSeqNo;
	unsigned short m_usPkgNo;
	unsigned char m_ucPkgfalg;
	unsigned short m_usRecordNumber;
	unsigned char m_ucCompressFlag;

	S_NetMsg()
	{
	
	}

	int getTotalLen()
	{
		return ntohs(m_nMsgLen) + sizeof(m_nMsgLen);
	}

	int getMsgLen()
	{
		return ntohs(m_nMsgLen);
	}

	int getPkgNo()
	{
		return ntohs(m_usPkgNo);
	}

	const char * getMsgHeadBody()
	{
		return (char*)this + sizeof(m_nMsgLen);
	}

	bool checkValid()
	{
		return true;
	}
};
#pragma pack() //取消设置的字节对齐方式

struct S_RequestMsg
{
	long		m_nMtype = 0;
	int			m_nMsgLen = 0;
	int			m_nPkgNo = 0;
	char		m_szIpAddr[16] = { 0 }; // if msg is outgoing msg, this field is dest host ip, if msg is incoming msg, this field is source host ip
	char		m_nReqReplyFlag = 0; // 0 - request, 1 - reply
	int			m_nLastPkg = 0;
	char		m_nSaveFile = 0;
	char		m_szOnlyNode[MAX_NODE_ID_LEN + 1] = {0};
	long		m_nReplyMType = 0;
	int			m_nReplyQId = -1; // if replyQId = -1, this request need not be replied
	int			m_nPort = 0;	// if msg is outgoing msg, this field is dest host server port, otherwise not filled
	char		m_szBuf[MSGMAX] = {0};
	S_RequestMsg()
	{
		m_nMsgLen = sizeof(S_RequestMsg) - sizeof(long) - MSGMAX;
	}

	S_RequestMsg(const char * pIpAddr, long nReplyMtype, S_NetMsg * pMsg)
	{
		memset(this, 0, sizeof(S_RequestMsg));
		m_nMsgLen = pMsg->getMsgLen();
		memcpy(m_szBuf, pMsg->getMsgHeadBody(), pMsg->getMsgLen());
		m_nLastPkg = pMsg->m_ucPkgfalg & 1;
		m_nPkgNo = pMsg->getPkgNo();
		m_nMtype = 1;
		//m_nReplyQId = -1;
		m_nReplyQId = CMsgQManager::getInstance().getRecvQid();
		m_nReplyMType = nReplyMtype;
		strncpy(m_szIpAddr, pIpAddr, sizeof(m_szIpAddr));

	}


	friend inline std::ostream& operator<<(std::ostream& s, S_RequestMsg& m)
	{
		s << "m_nMtype:" << m.m_nMtype
			<< ", m_nMsgLen:" << m.m_nMsgLen
			<< ", m_szIpAddr:" << m.m_szIpAddr
			<< ", m_nReqReplyFlag:" << (int)m.m_nReqReplyFlag
			<< ", m_nLastPkg:" << m.m_nLastPkg
			<< ", m_nSaveFile:" << (int)m.m_nSaveFile
			<< ", m_szOnlyNode:" << m.m_szOnlyNode
			<< ", m_nReplyMType:" << m.m_nReplyMType
			<< ", m_nReplyQId:" << m.m_nReplyQId
			<< ", m_nPort:" << m.m_nPort
			<< ", m_szBuf:";

		for (int i = 0; i < m.m_nMsgLen; i++)
		{
			s << std::hex << std::setw(2) << std::setfill('0') << ((unsigned int)(m.m_szBuf[i]) & 0xff) << " ";
		}
		//s << std::endl;
		return s;
	}

	std::string		toString()
	{
		std::ostringstream oss(std::ostringstream::out);
		oss << *this;
		return oss.str();
	}

	int			getMsgQSendLenth()
	{
		return sizeof(S_RequestMsg) - sizeof(long) - sizeof(m_szBuf) + m_nMsgLen;
	}
};

struct S_ReplyMsg
{
	long		m_nMtype;
	int			m_nMsgLen;
	int			m_nPkgNo;
	char		m_szIpAddr[16]; // if msg is incoming msg, this field is source host ip, if msg is outgoing msg, this field is not filled, and also not checked
	char		m_nReqReplyFlag; // 0 - request, 1 - reply
	int			m_nLastPkg;
	char		m_szBuf[MSGMAX];
	S_ReplyMsg()
	{
		m_nMsgLen = sizeof(S_ReplyMsg) - sizeof(long);
	}


	friend inline std::ostream& operator<<(std::ostream& s, S_ReplyMsg& m)
	{
		s << "m_nMtype:" << m.m_nMtype
			<< ", m_nMsgLen:" << m.m_nMsgLen
			<< ", m_nPkgNo:" << m.m_nPkgNo
			<< ", m_szIpAddr:" << m.m_szIpAddr
			<< ", m_nReqReplyFlag:" << (int)m.m_nReqReplyFlag
			<< ", m_nLastPkg:" << m.m_nLastPkg
			<< ", m_szBuf:";

		for (int i = 0; i < m.m_nMsgLen; i++)
		{
			s << std::hex << std::setw(2) << std::setfill('0') << ((unsigned int)(m.m_szBuf[i]) & 0xff) << " ";
		}
		//s << std::endl;
		return s;
	}

	std::string		toString()
	{
		std::ostringstream oss(std::ostringstream::out);
		oss << *this;
		return oss.str();
	}
};