#pragma once

template <class T>
class CSyncParent
{
friend class CSyncObj;
public:
	class CSyncObj
	{
	private:
		const CSyncParent &m_rOwner;

	public:
		CSyncObj(const CSyncParent &rOwner) : m_rOwner(rOwner) {m_rOwner.m_csSyncObj.Enter();}
		~CSyncObj(VOID) {m_rOwner.m_csSyncObj.Leave();}
	};

private:
	mutable CCriticalSection m_csSyncObj;
};

#define Sync Sync(*this)