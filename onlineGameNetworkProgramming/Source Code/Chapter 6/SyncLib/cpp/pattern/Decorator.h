#ifndef  __Sync_Decorator_H
#define  __Sync_Decorator_H

#include <cpp/lang/Object.h>

namespace cpp
{
	namespace pattern
	{
		class Decorator : public Object
		{
		public:
					Decorator();
			virtual	~Decorator();

			virtual	bool		process();

					void		add( Decorator* d );
					void		addTail( Decorator* d );

		private:
					void		remove();

					Decorator*		prev;
					Decorator*		next;
		};
	}
}

#endif