#pragma once
#include "ConnectionBase.h"
#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <iomanip>

class CTcpSvrConnection : public CConnectionBase
{
public:
	CTcpSvrConnection();
	~CTcpSvrConnection();

	int			m_nListenSocketFd = 0;
	std::string	m_strListenIp = "";
	int			m_nListenPort = 0;
	long		m_nReplyMtype = 0;


	virtual void	doMsg();
	bool	tryDoMsg();


	virtual CONN_TYPE getConnType()
	{
		return CONN_TYPE::CONN_TYPE_TCP_SVR;
	}

	friend inline std::ostream& operator<<(std::ostream& s, CTcpSvrConnection& m) {
		s << "m_nListenSocketFd:" << m.m_nListenSocketFd
			<< ", m_strListenIp:" << m.m_strListenIp
			<< ", m_nListenPort:" << m.m_nListenPort
			<< ", m_nConnSocketFd:" << m.m_nConnSocketFd
			<< ", m_strRemoteIp:" << m.m_strRemoteIp
			<< ", m_nRemotePort:" << m.m_nRemotePort
			<< ", m_tCreateTime:" << m.m_tCreateTime
			<< ", m_tExpireTime:" << m.m_tExpireTime
			<< ", m_tLastRecvTime:" << m.m_tLastRecvTime
			<< ", m_tLastSendTime:" << m.m_tLastSendTime
			<< ", m_nReplyMtype:" << m.m_nReplyMtype
			<< ", m_nDataLenth:" << m.m_nCurrDataLenth;
		return s;
	}

	virtual std::string		toString()
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}
};


