#pragma once

class CCriticalSection
{
public:
	CCriticalSection(VOID) {InitializeCriticalSection(&m_csObject);}
	~CCriticalSection(VOID)	{DeleteCriticalSection(&m_csObject);}
	
	inline VOID Enter(VOID) {EnterCriticalSection(&m_csObject);}
	inline VOID Leave(VOID)	{LeaveCriticalSection(&m_csObject);}

private:
	CRITICAL_SECTION m_csObject;
};