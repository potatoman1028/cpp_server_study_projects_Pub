#include "stdafx.h"
#include "Fsm.h"

Fsm::FsmState::FsmState( eState enumState ): m_eState(enumState)
{
}

Fsm::FsmState::~FsmState()
{ 
	m_mapState.clear();
}

void Fsm::FsmState::InsertState( eEvent inputEvent, eState inputState )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it != m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | Fsm::FsmState::InsertState() | 이벤트(%d)는 m_mapState에 이미 존재하고 있다.",
			inputEvent );
		return;

	}
	m_mapState.insert( STATE_PAIR( inputEvent , inputState ) );
}

void Fsm::FsmState::RemoveState( eEvent inputEvent )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it == m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | Fsm::FsmState::RemoveState() | 이벤트(%d)는 m_mapState에 존재하지 않는다.",
			inputEvent );
		return;

	}
	m_mapState.erase( inputEvent );
}

eState Fsm::FsmState::GetNextState( eEvent inputEvent )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it == m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | Fsm::FsmState::GetNextState() | 이벤트(%d)는 m_mapState에 존재하지 않는다.",
			inputEvent );
		return NPC_NONE;

	}
	return state_it->second;
}

DWORD Fsm::FsmState::GetStateCount()
{
	return (DWORD)m_mapState.size();
}


/////////////////////////////////////////////////////////////////////////////
// cFSM::cFsmClass class
//
Fsm::FsmClass::FsmClass()
	: m_pCurFsmState(0), m_eCurEvent( EVENT_NONE )
{

}

Fsm::FsmClass::~FsmClass()
{
	FSMSTATE_IT fsmstate_it;
	for( fsmstate_it = m_mapFsmState.begin() ; fsmstate_it != m_mapFsmState.end() ; fsmstate_it++ )
	{
		cFsmState* pFsmState = fsmstate_it->second;
		delete pFsmState;
	}
	m_mapFsmState.clear();
}


void Fsm::FsmClass::InsertFsmState( eState inputState, eEvent inputEvent, eState inputNextState )
{
	FsmState* pFsmState = NULL;
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//동일한 상태가 이미 존재 한다면 그 상태 객체를 가져와서 이벤트와 다음 상태를 추가
	if( fsmstate_it != m_mapFsmState.end() )
	{
		pFsmState = (FsmState*)fsmstate_it->second;
		pFsmState->InsertState( inputEvent , inputNextState );
		return;

	}
	//동일한 생태가 없다면 상태 객체를 생성하고 이벤트와 다음 상태를 추가
	else
	{
		pFsmState = new FsmState( inputState );
		m_mapFsmState.insert( FSMSTATE_PAIR( inputState , pFsmState ) );
	}

	pFsmState->InsertState( inputEvent , inputNextState );
}

void Fsm::FsmClass::RemoveFsmState( eState inputState, eEvent inputEvent )
{
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//해당 상태객체가 존재하지 않는다면
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | Fsm::FsmClass::RemoveFsmState | 상태(%d) 객체는 m_mapFsmState에 존재하지 않는다.",
			inputState );
		return;
	}

	FsmState* pFsmState = fsmstate_it->second;
	//이벤트에 해당하는 상태 정보를 지운다
	pFsmState->RemoveState( inputEvent );
	//상태 객체에 상태 정보가 없다면 객체를 지운다.
	if( pFsmState->GetStateCount() == 0 )
	{
		delete pFsmState;
		m_mapFsmState.erase( fsmstate_it );
	}
}

void Fsm::FsmClass::SetCurFsmState(eState inputState)
{
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//해당 상태객체가 존재하지 않는다면
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | Fsm::FsmClass::SetCurFsmState | 상태(%d) 객체는 m_mapFsmState에 존재하지 않는다.",
			inputState );
		return;
	}
	m_pCurFsmState = fsmstate_it->second;
}

eState Fsm::FsmClass::GetCurState()
{
	//현재 상태가 설정되어 있다면
	if( m_pCurFsmState )
		return m_pCurFsmState->GetState();
	return NPC_NONE;
}

eState Fsm::FsmClass::GetNextState(eEvent inputEvent)
{
	//현재 상태가 설정되어 있다면
	if( m_pCurFsmState )
	{
		m_pCurFsmState->GetNextState( inputEvent );
		return m_pCurFsmState->GetState();
	}
	return NPC_NONE;
}

void Fsm::FsmClass::TranslateState(eEvent inputEvent)
{
	if( m_pCurFsmState )
	{
		eState enumCurState = m_pCurFsmState->GetState();
		eState enumNextState = m_pCurFsmState->GetNextState( inputEvent );

		FSMSTATE_IT fsmstate_it = m_mapFsmState.find( enumNextState );
		//해당 상태객체가 존재하지 않는다면
		if( fsmstate_it == m_mapFsmState.end() )
		{
			LOG( LOG_ERROR_NORMAL ,
				"SYSTEM | Fsm::FsmClass::TranslateState | 상태(%d) 객체는 m_mapFsmState에 존재하지 않는다.",
				enumNextState );
			return;
		}
		m_pCurFsmState = fsmstate_it->second;
		m_eCurEvent = inputEvent;
		LOG( LOG_INFO_LOW,
			"SYSTEM | Fsm::FsmClass::TranslateState() | NPC상태 변경 %s -> %s"
			, strtableState[ enumCurState ] , strtableState[ enumNextState ] );
	}
}