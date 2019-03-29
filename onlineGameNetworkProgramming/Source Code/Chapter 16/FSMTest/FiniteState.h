#ifndef  __FiniteState_H
#define  __FiniteState_H

#pragma warning (disable: 4786) // for set (microsoft compilers only)

#include <map>
#include <functional>


class FiniteStateMachine;
class InvalidInputEventException;

class FiniteState
{
	friend class FiniteStateMachine;
private:
			FiniteState( DWORD state );
	virtual ~FiniteState();

			DWORD		getStateID(){ return stateID; }
			void		addTransition( DWORD inputEvent, DWORD outputStateID );
			void		deleteTransition( DWORD inputEvent );
			DWORD		outputState( DWORD inputEvent ) throw ( InvalidInputEventException* );

			DWORD		getCount();

private:
			DWORD		stateID;
			std::map	< DWORD, DWORD >		mapTransition;
};

#endif