#include "RunningControl.h"
#include <signal.h>
#include <stdio.h>
#include "WriteLog.h"


bool	g_bRunning = false;
int		g_nStopSignNo = -1;
bool	g_bReload = false;


void CRunningControl::Start()
{
	g_bRunning = true;
	LOG_INFO("Commsvr Start now !");
	fflush(stdout);
}

void CRunningControl::Stop()
{
	LOG_INFO("Commsvr Stop now !");
	fflush(stdout);

}


void CRunningControl::InitSignal()
{

	signal(SIGUSR1, CRunningControl::sigusr1_handle);
	signal(SIGUSR2, CRunningControl::sigusr2_handle);
	signal(SIGHUP, CRunningControl::sigusr2_handle);
	signal(SIGINT, CRunningControl::sigusr2_handle);
	signal(SIGQUIT, CRunningControl::sigusr2_handle);
	signal(SIGABRT, CRunningControl::sigusr2_handle);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, CRunningControl::sigusr2_handle);

}

// 信号处理函数
void CRunningControl::sigusr1_handle(int iSigVal)
{
	g_bReload = true;
}

void CRunningControl::sigusr2_handle(int iSigVal)
{
	g_nStopSignNo = iSigVal;
	g_bRunning = false;
}


void CRunningControl::SetRunning(bool bRunning)
{
	g_bRunning = bRunning;
}

void CRunningControl::SetReload(bool bReload)
{
	g_bReload = bReload;
}

bool CRunningControl::IsRunning()
{
	return g_bRunning;
}

bool CRunningControl::IsReload()
{
	return g_bReload;
}

void CRunningControl::SetStopSignNo(int nStopSignNo)
{
	g_nStopSignNo = nStopSignNo;
}

int  CRunningControl::GetStopSignNo()
{
	return g_nStopSignNo;
}
