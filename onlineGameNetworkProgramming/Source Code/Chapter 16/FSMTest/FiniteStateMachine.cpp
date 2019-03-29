#include "stdafx.h"
#include "FiniteStateMachine.h"

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- Exception objects ----------------------------------------------------

InvalidCurrentStateException::InvalidCurrentStateException( int curStateID )
	: currentStateID( curStateID )
{
}

InvalidCurrentStateException::~InvalidCurrentStateException()
{
}


InvalidInputEventException::InvalidInputEventException( int event )
	: inputEvent( event )
{
}

InvalidInputEventException::~InvalidInputEventException()
{
}




//------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- Finite State Machine ----------------------------------------------------

FiniteStateMachine::FiniteStateMachine()
	: currentState( 0 )
{
}

FiniteStateMachine::~FiniteStateMachine()
{
	std::map < DWORD, FiniteState* >::iterator iter, iterPrev;

	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		iterPrev = iter++;
		delete iterPrev->second;
		mapState.erase( iterPrev );
	}
}

void FiniteStateMachine::addStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID )
{
	std::map < DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//  ������ State�� �̹� �����ϴ����� ã�Ƴ���.
	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		state = iter->second;

		if( state->getStateID() == stateID )
			break;

		iter++;
	}

	//  ���� ������ State�� �������� �ʴ´ٸ� ���� �����Ѵ�.
	if( iter == mapState.end() )
	{
		state = new FiniteState( stateID );
		mapState[state->getStateID()] = state;
	}

	//  ���� ���� ������ �߰��Ѵ�.
	state->addTransition( inputEvent, outputStateID );
}

void FiniteStateMachine::deleteTransition( DWORD stateID, DWORD inputEvent )
{
	std::map < DWORD, FiniteState* >::iterator iter, iterPrev;
	FiniteState* state;

	//  �ش� State�� ã�´�.
	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		iterPrev = iter;
		state = iter->second;

		if( state->getStateID() == stateID )
			break;

		iter++;
	}

	if( iter == mapState.end() )
		return;

	//  State�� ���� ������ �����.
	state->deleteTransition( inputEvent );
	if( state->getCount() == 0 )
	{
		delete state;

		mapState.erase( iterPrev );
	}
}

DWORD FiniteStateMachine::getOutputState( DWORD inputEvent ) throw ( InvalidCurrentStateException* )
{
	std::map < DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//  ���� �������� �ʴ� State�� �ԷµǾ��ٸ�, Exception�� �߻���Ų��.
	iter = mapState.find( getCurrentStateID() );
	if( iter == mapState.end() )
		throw ( new InvalidCurrentStateException( getCurrentStateID() ) );

	state = mapState[getCurrentStateID()];

	return state->outputState( inputEvent );
}

void FiniteStateMachine::setCurrentState( DWORD stateID ) throw ( InvalidCurrentStateException* )
{
	std::map < DWORD, FiniteState* >::iterator iter;

	iter = mapState.find( stateID );
	if( iter == mapState.end() )
		throw ( new InvalidCurrentStateException( -1 ) );

	currentState = iter->second;
}

DWORD FiniteStateMachine::getCurrentStateID() throw ( InvalidCurrentStateException* )
{
	if( currentState == 0 )
		throw ( new InvalidCurrentStateException( -1 ) );

	return currentState->getStateID();
}

void FiniteStateMachine::stateTransition( int event ) throw ( InvalidCurrentStateException* )
{
	if( currentState == 0 )
		throw ( new InvalidCurrentStateException( -1 ) );

	DWORD outputState;

	try
	{
		outputState = currentState->outputState( event );
	}
	catch( InvalidInputEventException* ex )
	{
		delete ex;

		return;
	}

	currentState = mapState[outputState];
}