#ifndef  __FiniteStateMachine_H
#define  __FiniteStateMachine_H

#include "FiniteState.h"

class InvalidCurrentStateException
{
public:
			InvalidCurrentStateException( int curStateID );
	virtual	~InvalidCurrentStateException();

			DWORD	currentStateID;
};

class InvalidInputEventException
{
public:
			InvalidInputEventException( int event );
	virtual	~InvalidInputEventException();

			DWORD	inputEvent;
};

class FiniteStateMachine
{
public:
			FiniteStateMachine();
	virtual ~FiniteStateMachine();

			void		addStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID );
			void		deleteTransition( DWORD stateID, DWORD inputEvent );
			DWORD		getOutputState( DWORD inputEvent ) throw ( InvalidCurrentStateException* );

			void		setCurrentState( DWORD stateID ) throw ( InvalidCurrentStateException* );
			DWORD		getCurrentStateID() throw ( InvalidCurrentStateException* );
			void		stateTransition( int event ) throw ( InvalidCurrentStateException* );

private:
			std::map	< DWORD, FiniteState* >		mapState;
			FiniteState*	currentState;
};

#endif