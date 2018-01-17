#pragma once
#include "ConnectionBase.h"
#include <list>
#include <string>
#include <map>


class CConnectionManager
{
public:
	CConnectionManager();
	~CConnectionManager();
	void initialize();
	void addNewConnections(std::list<CConnectionBase*>);
	void tryNetMsg2QMsg();
	void tryQMsg2NetMsg();
	void checkConnectionAviable();

	std::map<long, CConnectionBase*> & getConnDetailsMap()
	{
		return m_mpConnDetails;
	}
private:
	int m_nMtype;
	//std::list<CConnectionDetail> m_lConnDetails;
	std::map<long, CConnectionBase*> m_mpConnDetails;
};

