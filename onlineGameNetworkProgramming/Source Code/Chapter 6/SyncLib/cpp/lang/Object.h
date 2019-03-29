#ifndef __Sync_Object_H
#define __Sync_Object_H

#pragma warning( disable : 4290 )
#pragma warning( disable : 4291 )


#include <Windows.h>
#include <TChar.h>

#include <cpp/lang/Synchronized.h>

namespace cpp
{
	namespace lang
	{
#ifdef _DEBUG
		#define  DebugAssert( exp ) { if( exp == false ) __asm int 3 }
#else
		#define  DebugAssert( exp ) { exp; }
#endif

		class Object
		{
			friend Synchronized;

		public:
					Object();
			virtual ~Object();

					bool	equals( Object* obj );

		private:
					CRITICAL_SECTION			cs;
		};
	}
}

using namespace cpp::lang;

#endif
