#ifndef __Sync_InetAddress_H
#define __Sync_InetAddress_H

#include <cpp/lang/String.h>

namespace cpp
{
	namespace net
	{
		class InetAddress : public Object
		{
		public:
					InetAddress();
			virtual	~InetAddress();

			static	String		getHostAddress();
			static	String		getHostName();
			static	String		getHostByName( String& name );
		};
	}
}

#endif
