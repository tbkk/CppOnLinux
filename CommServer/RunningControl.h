#pragma once

class CRunningControl {
public:
	CRunningControl() {}
	~CRunningControl() {};

	//»ñÈ¡ÊµÀý
	static CRunningControl& getInstance() 
	{
		static CRunningControl oRunningControl;
		return oRunningControl;
	};

	void InitSignal();

	void Start();
	void SetRunning(bool bRunning);
	void SetReload(bool bReload);
	void Stop();

	bool IsRunning();
	bool IsReload();

	void SetStopSignNo(int nStopSignNo);
	int  GetStopSignNo();

protected:

	static void sigusr1_handle(int iSigVal);
	static void sigusr2_handle(int iSigVal);

};

