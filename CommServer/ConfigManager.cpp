#include "ConfigManager.h"
#include <string>
#include "WriteLog.h"
using namespace std;



std::string CConfigManager::getLogConfigFilePath()
{
	string strSection = "LOG";
	string strKey = "conf";
	string value = "";
	svr_err_t ret = m_oIniFileReader.GetConfStr(strSection.c_str(), strKey.c_str(), value);
	if (ret != ERR_SUCCESS)
	{
		fprintf(stderr, "Config Error! file=[%s], section=[%s], key=[%s]\n", strSection.c_str(), strKey.c_str());
		return "";
	}
	return value;
}

std::list<int> CConfigManager::getListenPortList()
{
	list<int> lPortList;
	unsigned int nPortCount = 0;
	svr_err_t ret = m_oIniFileReader.GetConfUint("COMMSVR", "listenPortCount", nPortCount);
	if (ret != ERR_SUCCESS)
	{
		LOG_ERROR("getListenPortListCount COMMSVR--listenPortCount error!");
		return lPortList;
	}
	for (int i = 0; i < nPortCount;i++)
	{
		char szPortKey[10] = { 0 };
		snprintf(szPortKey, sizeof(szPortKey), "port_%d", i);
		unsigned int nPort = 0;
		svr_err_t ret = m_oIniFileReader.GetConfUint("COMMSVR", szPortKey, nPort);
		if (ret != ERR_SUCCESS)
		{
			LOG_ERROR("getListenPortList COMMSVR--%s error!", szPortKey);
		}
		else
		{
			lPortList.push_back(nPort);
		}
	}

	return lPortList;
}

std::string CConfigManager::getQKeyFilePath()
{
	string strSection = "IPCQ";
	string strKey = "keyFilePath";
	string value = "";
	svr_err_t ret = m_oIniFileReader.GetConfStr(strSection.c_str(), strKey.c_str(), value);
	if (ret != ERR_SUCCESS)
	{
		LOG_ERROR("Config Error! file=[%s], section=[%s], key=[%s]\n", strSection.c_str(), strKey.c_str());
		return "";
	}
	return value;
}

int CConfigManager::getSendQId()
{
	string strSection = "IPCQ";
	string strKey = "sendQId";
	int value = 0;
	svr_err_t ret = m_oIniFileReader.GetConfInt(strSection.c_str(), strKey.c_str(), value);
	if (ret != ERR_SUCCESS)
	{
		LOG_ERROR("Config Error! file=[%s], section=[%s], key=[%s]\n", strSection.c_str(), strKey.c_str());
		return -1;
	}
	return value;
}

int CConfigManager::getRecvQId()
{
	string strSection = "IPCQ";
	string strKey = "recvQId";
	int value = 0;
	svr_err_t ret = m_oIniFileReader.GetConfInt(strSection.c_str(), strKey.c_str(), value);
	if (ret != ERR_SUCCESS)
	{
		LOG_ERROR("Config Error! file=[%s], section=[%s], key=[%s]\n", strSection.c_str(), strKey.c_str());
		return -1;
	}
	return value;
}

int CConfigManager::getMonitorPort()
{
	string strSection = "MONITOR";
	string strKey = "monitorPort";
	int value = 0;
	svr_err_t ret = m_oIniFileReader.GetConfInt(strSection.c_str(), strKey.c_str(), value);
	if (ret != ERR_SUCCESS)
	{
		LOG_ERROR("Config Error! file=[%s], section=[%s], key=[%s]\n", strSection.c_str(), strKey.c_str());
		return -1;
	}
	return value;
}

bool CConfigManager::initialize(const std::string& strConfigFilePath)
{
	if (strConfigFilePath.empty())
	{
		fprintf(stderr, "No ConfigFilePath\n");
		return false;
	}

	if (m_oIniFileReader.LoadConf(strConfigFilePath.c_str()) != ERR_INIFILE_SUCCESS)
	{
		return false;
	}
	return true;

	//string strConfigFile = "/home/tbkk/comsvr.ini";
	//string strSection = "TEST";
	//string strKey = "key";
	//int value = 0;
	//svr_err_t ret = m_oIniFileReader.GetConfInt(strSection.c_str(), strKey.c_str(), value);
	//if (ret != ERR_SUCCESS)
	//{
	//	LOG_WARN("Config Error! file=[%s], section=[%s], key=[%s]\n", m_strConfigFile.c_str(), strSection.c_str(), strKey.c_str());
	//	return false;
	//}
	//LOG_INFO("value is [%d]\n", value);
	//return true;
}

CIniFileReader & CConfigManager::getReader()
{
	return m_oIniFileReader;
}

CConfigManager::CConfigManager()
{
}


CConfigManager::~CConfigManager()
{
}
