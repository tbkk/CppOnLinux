#pragma once
#include <iostream>
#include <string>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <sstream>
static const int MAX_BUFFE_LENTH = 81920;

enum CONN_TYPE						//连接类型
{
	CONN_TYPE_TCP_SVR = 0,			// tcp server
	CONN_TYPE_TCP_CLNT,				// tcp client
	CONN_TYPE_MSGQ_CLNT,			//msgq client
};

class CConnectionBase
{
public:
	CConnectionBase();
	virtual ~CConnectionBase();

	int			m_nConnSocketFd = 0;
	std::string m_strRemoteIp = "";
	int			m_nRemotePort = 0;

	time_t		m_tCreateTime = 0;
	time_t		m_tExpireTime = 0;
	time_t		m_tLastRecvTime = 0;
	time_t		m_tLastSendTime = 0;

	int			m_nCurrDataLenth = 0;
	char		m_szDatabuffer[MAX_BUFFE_LENTH] = { 0 };
	
public:
	bool			pushMsg(char * buffer, int buffSize);
	virtual void	doMsg();
	virtual std::string		toString();
	virtual CONN_TYPE getConnType() = 0;


};

static inline std::ostream& operator<<(std::ostream& s, CConnectionBase& m) {
		s << ", m_nConnSocketFd:" << m.m_nConnSocketFd
		<< ", m_strRemoteIp:" << m.m_strRemoteIp
		<< ", m_nRemotePort:" << m.m_nRemotePort
		<< ", m_tCreateTime:" << m.m_tCreateTime
		<< ", m_tExpireTime:" << m.m_tExpireTime
		<< ", m_tLastRecvTime:" << m.m_tLastRecvTime
		<< ", m_tLastSendTime:" << m.m_tLastSendTime
		<< ", m_nDataLenth:" << m.m_nCurrDataLenth;
	return s;
}
