#ifndef  __Sync_ThreadManager_H
#define  __Sync_ThreadManager_H

#include <list>

#include <cpp/lang/Object.h>

namespace cpp
{
	namespace lang
	{
		class ThreadManager : public Object
		{
		public:
			static	ThreadManager*	getInstance();
			static	void			releaseInstance();
			
					void			join();
					HANDLE			spawn( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* identifier );

		private:
					ThreadManager();
			virtual ~ThreadManager();

			static	ThreadManager*		selfInstance;

					std::list	< HANDLE >				listThreadHandle;
			typedef	std::list	< HANDLE >::iterator	ListPosition;
		};
	}
}

#endif
