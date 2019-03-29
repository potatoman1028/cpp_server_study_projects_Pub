#include "StdAfx.h"
#include "CriticalSection.h"
#include "StaticSyncParent.h"
#include "SyncParent.h"
#include "MemoryPool.h"
#include "ManagedBuf.h"

CManagedBuf::CManagedBuf(VOID)
{
	ZeroMemory(m_aucBuf, sizeof(m_aucBuf));
}

CManagedBuf::~CManagedBuf(VOID)
{
}
