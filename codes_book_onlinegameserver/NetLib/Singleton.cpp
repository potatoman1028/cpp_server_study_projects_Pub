#include "Precompile.h"

Singleton::Singleton()
{
	m_listSingleton.push_back( this );
}

Singleton::~Singleton()
{
	
}

void Singleton::releaseAll()
{
	SINGLETON_LIST::reverse_iterator singleton_rit = m_listSingleton.rbegin();
	while ( singleton_rit != m_listSingleton.rend() )
	{
		(*singleton_rit)->releaseInstance();
		++singleton_rit;
	}
	m_listSingleton.clear();
}

Singleton::SINGLETON_LIST Singleton::m_listSingleton;