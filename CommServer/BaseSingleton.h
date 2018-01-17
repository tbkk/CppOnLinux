#pragma once

template <class TYPE>
class CBaseSingleton
{
public:
	static TYPE * instance(void)
	{
		if (m_singleton == NULL)
		{
			m_singleton = new CBaseSingleton;
		}
		return m_singleton->m_instance;
	}
	static void setInstance(TYPE* p) {
		if (m_singleton == NULL) {
			m_singleton = new CBaseSingleton(p);
		}
		else {
			TYPE* pSaved = m_singleton->m_instance;
			m_singleton->m_instance = p;
			if (pSaved != NULL) {
				// delete pSaved;	// Do not delete the old one
			}
		}
	}
public:
	CBaseSingleton() { m_instance = new TYPE; }
	CBaseSingleton(TYPE* p) { m_instance = p; }
	TYPE* m_instance;
	static CBaseSingleton<TYPE>* m_singleton;
};

template<class TYPE>
CBaseSingleton<TYPE>* CBaseSingleton<TYPE>::m_singleton = NULL;

