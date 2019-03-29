#include <cpp/util/Stack.h>

using namespace cpp::util;

Stack::Stack()
{
}

Stack::~Stack()
{
	elementStack.empty();
}

void Stack::push( Object* obj )
{
	elementStack.push( obj );
}

Object* Stack::pop()
{
	if( size() == 0 )
		return 0;

	Object* obj = elementStack.top();

	elementStack.pop();

	return obj;
}

int Stack::size()
{
	return elementStack.size();
}