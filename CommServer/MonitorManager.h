#pragma once
#include <string.h>
#include "ConnectionManager.h"



class CMonitorManager
{
public:
	static CMonitorManager& getInstance()
	{
		if (NULL == pInstance)
		{
			pInstance = new CMonitorManager();
		}
		return *pInstance;
	};



	void registerMoniModuel(CConnectionManager & oConnectionManager)
	{
		m_pConnectionManager = &oConnectionManager;
	}

	std::map<long, CConnectionBase*> & getConnDetailsMap()
	{
		return m_pConnectionManager->getConnDetailsMap();
	}

private:
	static CMonitorManager* pInstance;

	//��Ҫ��صĶ����б�
	CConnectionManager * m_pConnectionManager = NULL;

public:
	CMonitorManager();
	~CMonitorManager();

};

