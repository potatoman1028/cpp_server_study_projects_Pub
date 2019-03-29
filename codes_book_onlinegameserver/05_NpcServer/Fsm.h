#pragma once

#define MAX_DISCOMPORT_INDEX 5
enum eState
{
	NPC_NONE		= 0,    //����
	NPC_NORMAL		= 1,	//�Ϲ�			
	NPC_DISCOMPORT	= 2,	//����
    NPC_ANGRY		= 3,	//ȭ��

	NPC_STATE_CNT			//���� ����
};

enum eEvent
{
	EVENT_NONE	=	0,			//����
	EVENT_PLAYER_APPEAR	=	1,  //�÷��̾ ����
	EVENT_PLAYER_ATTACK	=	2,  //�÷��̾ ������
	EVENT_PLAYER_RUNAWAY=	3,  //�÷��̾ ������
	EVENT_DISCOMPORT_INDEX = 4, //�÷��̾ ��� �����ϰ���
};

static char strtableState[][20] = 
{
	"NPC_NONE",
	"NPC_NORMAL",
	"NPC_DISCOMPORT",
	"NPC_ANGRY"
};

class Fsm
{
public:
	Fsm(void);
	~Fsm(void);
	class FsmClass;

	/////////////////////////////////////////////////////////////////////////
	// ���¿� �̺�Ʈ�� �����ϴ� ��ü
	class FsmState
	{
		friend class cFsmClass;
	private:
		typedef pair< eEvent , eState >  STATE_PAIR;
		typedef map< eEvent , eState >	 STATE_MAP;
		typedef STATE_MAP::iterator	STATE_IT;
		
		eState				m_eState;
		STATE_MAP			m_mapState;
	
		eState	GetState(){ return m_eState; }
		void	InsertState( eEvent inputEvent, eState inputState );
		void	RemoveState( eEvent inputEvent );
		eState	GetNextState( eEvent inputEvent );
		DWORD	GetStateCount();

	private:
		FsmState(eState inputState );
		virtual ~FsmState();
	};

	/////////////////////////////////////////////////////////////////////////
	// ���°�ü�� ���¸� �����ϴ� ��ü
	class FsmClass
	{
	private:
		typedef pair< eState , FsmState* >  FSMSTATE_PAIR;
		typedef map< eState , FsmState* >	 FSMSTATE_MAP;
		typedef FSMSTATE_MAP::iterator	FSMSTATE_IT;

		FsmState*					m_pCurFsmState;
		FSMSTATE_MAP				m_mapFsmState;
		eEvent						m_eCurEvent;
		
	public:
		void	InsertFsmState( eState inputState, eEvent inputEvent, eState inputNextState  );
		void	RemoveFsmState( eState inputState, eEvent inputEvent );
	
		void	SetCurFsmState( eState inputState );
		eState	GetCurState();
		eState	GetNextState( eEvent inputEvent );
		void	TranslateState( eEvent inputEvent );
		inline	eEvent	GetCurEvent() { return m_eCurEvent; }
		
	public:
		FsmClass();
		virtual ~FsmClass();
	};
};
