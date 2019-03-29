#include <cpp/util/List.h>

using namespace cpp::util;

List::List()
{
	headPosition();
}

List::~List()
{
	clear();
}

void List::clear()
{
	elementList.clear();
}

List::Position List::headPosition()
{
	return elementList.begin();
}

bool List::isEnd( Position pos )
{
	if( pos == elementList.end() )
		return true;

	return false;
}

int List::size()
{
	return elementList.size();
}

List::Position List::find( Object* arg )
{
	Position pos = headPosition();

	while( !isEnd( pos ) )
	{
		if( getItem( pos ) == arg )
			return pos;

		pos++;
	}

	return elementList.end();
}

void List::pushHead( Object* arg )
{
	elementList.push_front( arg );
}

void List::pushTail( Object* arg )
{
	elementList.push_back( arg );
}

Object* List::popHead()
{
	Object* returnValue;

	if( size() == 0 )
		return 0;

	returnValue = getItem( headPosition() );

	elementList.pop_front();

	return returnValue;
}

void List::popTail()
{
	if( size() == 0 )
		return;

	elementList.pop_back();
}

Object* List::removeItem( Position pos )
{
	Object* item = 0;

	if( isEnd( pos ) )
		return 0;

	item = getItem( pos );
	elementList.erase( pos );

	return item;
}

Object* List::getItem( Position pos )
{
	return *pos;
}

Object* List::removeHead()
{
	return removeItem( headPosition() );
}
