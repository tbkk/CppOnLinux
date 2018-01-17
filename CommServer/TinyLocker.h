#pragma once

#include <pthread.h>
typedef pthread_mutex_t tLock;
/************************************************************************/
//	tLock m_oLock;	
//  CTinyLock::initlock(&m_oLock);
//
//
//  1. 
//	CTinyLock oLock(&m_oLock);                                         
//   2. 
//	CTinyLock::lock(&m_oLock);
//	CTinyLock::unlock(&m_oLock);
/************************************************************************/
class CTinyLock
{

public:
	CTinyLock(tLock* pLock)
	{
		m_pLock = pLock; pthread_mutex_lock(pLock); 
	};
	
	~CTinyLock()
	{
		if (m_pLock) pthread_mutex_unlock(m_pLock); 
	};

	static int initlock(tLock* pLock) 
	{
		int nRet;
		pthread_mutexattr_t ptype;
		nRet = pthread_mutexattr_init(&ptype);
		if (nRet != 0) 
		{
			return nRet;
		}
		nRet = pthread_mutexattr_settype(&ptype, PTHREAD_MUTEX_RECURSIVE);
		if (nRet != 0) 
		{
			return nRet;
		}
		nRet = pthread_mutex_init(pLock, &ptype);
		return nRet;
	}
	static void destorylock(tLock* pLock) { }  // no need to call pthread_mutex_destroy

	static void lock(tLock* pLock) 
	{
		pthread_mutex_lock(pLock); 
	}
	static void unlock(tLock* pLock)
	{
		pthread_mutex_unlock(pLock); 
	}

private:
	tLock* m_pLock;
};
