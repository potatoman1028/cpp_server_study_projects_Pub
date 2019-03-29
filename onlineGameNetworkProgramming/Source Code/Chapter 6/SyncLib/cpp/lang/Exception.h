#ifndef  __Sync_Exception_H
#define  __Sync_Exception_H

#include <cpp/lang/String.h>

namespace cpp
{
	namespace lang
	{
		class Exception
		{
		public:
					Exception();
					Exception( String msg );
			virtual ~Exception();

					String			message;
		};
	}
}

#endif
