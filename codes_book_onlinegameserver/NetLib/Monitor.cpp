#include "Precompile.h"
#include "Monitor.h"

Monitor::Monitor(void)
{
	InitializeCriticalSection(&m_csSyncObject);
}

Monitor::~Monitor(void)
{
	DeleteCriticalSection( &m_csSyncObject );
}

#if(_WIN32_WINNT >= 0x0400)
BOOL Monitor::TryEnter()
{
   return TryEnterCriticalSection(&m_csSyncObject);
}
#endif

void Monitor::Enter()
{
   ::EnterCriticalSection(&m_csSyncObject);
}

void Monitor::Leave()
{
   ::LeaveCriticalSection(&m_csSyncObject);
}

Monitor::Owner::Owner( Monitor &crit )
   : m_csSyncObject(crit)
{
	m_csSyncObject.Enter();
}

Monitor::Owner::~Owner()
{
   m_csSyncObject.Leave();
}