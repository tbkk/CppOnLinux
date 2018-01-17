#pragma once
#include "IniFileReader.h"
#include <list>

//#define ConfManager    CConfigManager::getInstance()

#define CONFIG_INIT(configFile)		CConfigManager::getInstance().initialize(configFile)

class CConfigManager
{
public:
	std::string getLogConfigFilePath();
	std::list<int> getListenPortList();

	std::string getQKeyFilePath();
	int			getSendQId();
	int			getRecvQId();

	int			getMonitorPort();
public:
	static CConfigManager& getInstance()
	{
		static CConfigManager oConfigManager;
		return oConfigManager;
	};

	bool initialize(const std::string& strConfigFilePath);
	CIniFileReader & getReader();


private:
	CConfigManager();
	~CConfigManager();
	CIniFileReader m_oIniFileReader;
};

