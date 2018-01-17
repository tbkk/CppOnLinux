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

	void initSignal();

	void start();
	void setRunning(bool bRunning);
	void setReload(bool bReload);
	void stop();

	bool isRunning();
	bool isReload();

	void setStopSignNo(int nStopSignNo);
	int  getStopSignNo();

protected:

	static void sigusr1Handle(int iSigVal);
	static void sigusr2Handle(int iSigVal);

};

