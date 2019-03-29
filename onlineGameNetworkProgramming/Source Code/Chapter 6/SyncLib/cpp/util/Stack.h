#ifndef  __Sync_Stack_H
#define  __Sync_Stack_H

#pragma warning (disable: 4786) // for set (microsoft compilers only)

#include <stack>

#include <cpp/lang/Object.h>


namespace cpp
{
	namespace util
	{
		class Stack : public Object
		{
		public:
					Stack();
			virtual	~Stack();

					void		push( Object* obj );
					Object*		pop();

					int			size();
					void		empty();

		private:
					std::stack		< Object* >		elementStack;
		};
	}
}

#endif