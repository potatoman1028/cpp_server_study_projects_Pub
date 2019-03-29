#ifndef  __Sync_List_H
#define  __Sync_List_H

#pragma warning (disable: 4786) // for set (microsoft compilers only)

#include <list>

#include <cpp/lang/Object.h>

namespace cpp
{
	namespace util
	{
		class List : public Object
		{
		public:
			typedef  std::list < Object* >::iterator	Position;

					List();
			virtual	~List();

			virtual	void		clear();
			virtual	Position	headPosition();

			virtual	bool		isEnd( Position pos );
			virtual	int			size();
			virtual	Position	find( Object* arg );

			virtual	void		pushHead( Object* arg );
			virtual	void		pushTail( Object* arg );
			virtual	Object*		popHead();
			virtual	void		popTail();

			virtual	Object*		removeItem( Position pos );
			virtual	Object*		getItem( Position pos );
			virtual	Object*		removeHead();

		private:
					std::list	< Object* >		elementList;
		};
	}
}

#endif