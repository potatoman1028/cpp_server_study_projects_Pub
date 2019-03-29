#pragma once

template <class T>
class CStaticSyncParent
{
friend class CStaticSyncObj;
public:
	class CStaticSyncObj
	{
	public:
		CStaticSyncObj(VOID) {T::m_scsSyncObj.Enter();}
		~CStaticSyncObj(VOID) {T::m_scsSyncObj.Leave();}
	};

private:
	static CCriticalSection m_scsSyncObj;
};

template <class T>
CCriticalSection CStaticSyncParent<T>::m_scsSyncObj;