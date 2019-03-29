#include "Decorator.h"

using namespace cpp::pattern;

Decorator::Decorator()
	: prev( 0 ), next( 0 )
{
}

Decorator::~Decorator()
{
	remove();
}

bool Decorator::process()
{
	while( next != 0 && next->process() == false )
		delete next;

	return true;
}

void Decorator::add( Decorator* d )
{
	if( d == 0 )
		return;

	d->prev = this;
	d->next = next;
	if( next != 0 )
		next->prev = d;
	next = d;
}

void Decorator::addTail( Decorator* d )
{
	if( next == 0 )
	{
		next = d;
		next->prev = this;
	}
	else
		next->addTail( d );
}

void Decorator::remove()
{
	if( prev != 0 )
		prev->next = next;
	if( next != 0 )
		next->prev = prev;
}