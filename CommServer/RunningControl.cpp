#include "RunningControl.h"
#include <signal.h>
#include <stdio.h>
#include "WriteLog.h"


bool	g_bRunning = false;
int		g_nStopSignNo = -1;
bool	g_bReload = false;


void CRunningControl::start()
{
	g_bRunning = true;
	LOG_INFO("Commsvr Start now !");
	fflush(stdout);
}

void CRunningControl::stop()
{
	LOG_INFO("Commsvr Stop now !");
	fflush(stdout);

}


void CRunningControl::initSignal()
{

	signal(SIGUSR1, CRunningControl::sigusr1Handle);
	signal(SIGUSR2, CRunningControl::sigusr2Handle);
	signal(SIGHUP, CRunningControl::sigusr2Handle);
	signal(SIGINT, CRunningControl::sigusr2Handle);
	signal(SIGQUIT, CRunningControl::sigusr2Handle);
	signal(SIGABRT, CRunningControl::sigusr2Handle);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, CRunningControl::sigusr2Handle);

}

// 信号处理函数
void CRunningControl::sigusr1Handle(int iSigVal)
{
	g_bReload = true;
}

void CRunningControl::sigusr2Handle(int iSigVal)
{
	g_nStopSignNo = iSigVal;
	g_bRunning = false;
}


void CRunningControl::setRunning(bool bRunning)
{
	g_bRunning = bRunning;
}

void CRunningControl::setReload(bool bReload)
{
	g_bReload = bReload;
}

bool CRunningControl::isRunning()
{
	return g_bRunning;
}

bool CRunningControl::isReload()
{
	return g_bReload;
}

void CRunningControl::setStopSignNo(int nStopSignNo)
{
	g_nStopSignNo = nStopSignNo;
}

int  CRunningControl::getStopSignNo()
{
	return g_nStopSignNo;
}
