#ifndef  __Sync_Singleton_H
#define  __Sync_Singleton_H

#include <cpp/lang/Object.h>

namespace cpp
{
	namespace pattern
	{
		template < class T > class Singleton : public Object
		{
		public:
			static	T*		getInstance();
			static	void	releaseInstance();

			virtual	void	initialize() = 0;

		private:
			static	T*		selfInstance;
		};

		template < class T > T*		Singleton< T >::selfInstance = 0;

		template < class T > T* Singleton< T >::getInstance()
		{
			if( selfInstance == 0 )
			{
				selfInstance = new T();
				selfInstance->initialize();
			}

			return selfInstance;
		}

		template < class T > void Singleton< T >::releaseInstance()
		{
			if( selfInstance != 0 )
				delete selfInstance;

			selfInstance = 0;
		}
	}
}

#endif