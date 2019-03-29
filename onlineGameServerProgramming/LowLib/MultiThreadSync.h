#pragma once

template <class T>
class CMultiThreadSync
{
friend class CThreadSync;
public:
	class CThreadSync
	{
	public:
		CThreadSync(VOID)
		{
			T::mSync.Enter();
		}

		~CThreadSync(VOID)
		{
			T::mSync.Leave();
		}
	};

private:
	static CCriticalSection mSync;
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;