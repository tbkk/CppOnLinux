#include<iostream>  
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "RunningControl.h"
#include "MainProcess.h"
#include "ConfigManager.h"
#include "WriteLog.h"
#include "MsgQManager.h"

using namespace std;

void Usage(const char* pProgrameName)
{
	printf("\nUsage: %s [-h|--help] [-v|--verion] [-c|--config]\n", pProgrameName);
	printf("\t-h --help             help\n");
	printf("\t-v --version          printf version\n");
	return;
}

struct option longopts[] = 
{
	{ "help",0,NULL,'h' },
	{ "version",0,NULL,'v' },
	{ "config",0,NULL,'c' },
	{ 0,0,0,0 },
};

int main(int argc, char *argv[])
{	
	int iOpt = 0;
	while ((iOpt = getopt_long(argc, argv, "hvc:", longopts, NULL)) != -1)
	{
		switch (iOpt)
		{
		case 'c':
		{
			if (!CONFIG_INIT(optarg))
			{
				printf("error config file\n");
				return 0;
			}
		}
			break;
		case 'h':
			Usage(argv[0]);
			return 0;
		case 'v':
			printf("Version 1.0\n");
			break;
		default:
			//Usage(argv[0]);
			printf("error input\n");
			return 0;
		}
	}
	srand((unsigned int)time(NULL));

	if (!LOG_INIT(CConfigManager::getInstance().getLogConfigFilePath().c_str()))
	{
		printf("error log config file\n");
		return 0;
	}

	if (!CMsgQManager::getInstance().initialize())
	{
		LOG_ERROR("CMsgQManager init Error!\n");
		return 0;
	}

	CMainProcess oMainProcess;
	if (!oMainProcess.initialize())
	{
		LOG_ERROR("MainProcess init Error!\n");
		return 0;
	}

	CRunningControl::getInstance().initSignal();
	CRunningControl::getInstance().start();

	oMainProcess.run();
	LOG_WARN("Warning log test!");
	LOG_ERROR("ERROR log test!");
//	oMainControl.exit();

	CRunningControl::getInstance().stop();
	LOG_EDN;
	return 0;
}