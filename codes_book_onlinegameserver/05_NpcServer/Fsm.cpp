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
			"SYSTEM | Fsm::FsmState::InsertState() | �̺�Ʈ(%d)�� m_mapState�� �̹� �����ϰ� �ִ�.",
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
			"SYSTEM | Fsm::FsmState::RemoveState() | �̺�Ʈ(%d)�� m_mapState�� �������� �ʴ´�.",
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
			"SYSTEM | Fsm::FsmState::GetNextState() | �̺�Ʈ(%d)�� m_mapState�� �������� �ʴ´�.",
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
	//������ ���°� �̹� ���� �Ѵٸ� �� ���� ��ü�� �����ͼ� �̺�Ʈ�� ���� ���¸� �߰�
	if( fsmstate_it != m_mapFsmState.end() )
	{
		pFsmState = (FsmState*)fsmstate_it->second;
		pFsmState->InsertState( inputEvent , inputNextState );
		return;

	}
	//������ ���°� ���ٸ� ���� ��ü�� �����ϰ� �̺�Ʈ�� ���� ���¸� �߰�
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
	//�ش� ���°�ü�� �������� �ʴ´ٸ�
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | Fsm::FsmClass::RemoveFsmState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
			inputState );
		return;
	}

	FsmState* pFsmState = fsmstate_it->second;
	//�̺�Ʈ�� �ش��ϴ� ���� ������ �����
	pFsmState->RemoveState( inputEvent );
	//���� ��ü�� ���� ������ ���ٸ� ��ü�� �����.
	if( pFsmState->GetStateCount() == 0 )
	{
		delete pFsmState;
		m_mapFsmState.erase( fsmstate_it );
	}
}

void Fsm::FsmClass::SetCurFsmState(eState inputState)
{
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//�ش� ���°�ü�� �������� �ʴ´ٸ�
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | Fsm::FsmClass::SetCurFsmState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
			inputState );
		return;
	}
	m_pCurFsmState = fsmstate_it->second;
}

eState Fsm::FsmClass::GetCurState()
{
	//���� ���°� �����Ǿ� �ִٸ�
	if( m_pCurFsmState )
		return m_pCurFsmState->GetState();
	return NPC_NONE;
}

eState Fsm::FsmClass::GetNextState(eEvent inputEvent)
{
	//���� ���°� �����Ǿ� �ִٸ�
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
		//�ش� ���°�ü�� �������� �ʴ´ٸ�
		if( fsmstate_it == m_mapFsmState.end() )
		{
			LOG( LOG_ERROR_NORMAL ,
				"SYSTEM | Fsm::FsmClass::TranslateState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
				enumNextState );
			return;
		}
		m_pCurFsmState = fsmstate_it->second;
		m_eCurEvent = inputEvent;
		LOG( LOG_INFO_LOW,
			"SYSTEM | Fsm::FsmClass::TranslateState() | NPC���� ���� %s -> %s"
			, strtableState[ enumCurState ] , strtableState[ enumNextState ] );
	}
}