#pragma once

template < class TYPE >
class Queue : public Monitor
{
public:
	Queue( int nMaxSize = MAX_QUEUESIZE );
	virtual ~Queue(void);

	bool	PushQueue( TYPE typeQueueItem );		//queue에 데이타를 넣는다.
	void	PopQueue();								//queue에서 데이타를 지운다.

	bool	IsEmptyQueue();							//queue가 비었는지 알려준다.
	TYPE	GetFrontQueue();						//queue에서 데이타를 얻어온다.
	int		GetQueueSize();							//queue에 현재 들어있는 item의 갯수를 알려준다.
	int		GetQueueMaxSize() { return m_nQueueMaxSize; }  //최대 queue사이즈를 알려준다
	void	SetQueueMaxSize( int nMaxSize ) { m_nQueueMaxSize = nMaxSize; }   //최대 queue size를 정해 준다
	void	ClearQueue();						//queue에 있는 모든 item을 삭제한다.

private:

	TYPE*			m_arrQueue;
	int				m_nQueueMaxSize;				//큐에대한 사이즈 제한
	Monitor		m_csQueue;
	
	int				m_nCurSize;
	int				m_nEndMark;
	int				m_nBeginMark;
	
};
template <class TYPE>
Queue< TYPE >::Queue( int nMaxSize )
{
	m_arrQueue = new TYPE[ nMaxSize ];
	m_nQueueMaxSize = nMaxSize;
	ClearQueue();
	
}

template <class TYPE>
Queue< TYPE >::~Queue< TYPE >(void)
{
	delete [] m_arrQueue;
}

template <class TYPE>
bool Queue< TYPE >::PushQueue( TYPE typeQueueItem )
{
	Monitor::Owner lock( m_csQueue );
	{
		
		//정해놓은 사이즈를 넘었다면 더이상 큐에 넣지 않는다.
		if( m_nCurSize >= m_nQueueMaxSize )
			return false;
		
		m_nCurSize++;
		if( m_nEndMark == m_nQueueMaxSize )
			m_nEndMark = 0;
		m_arrQueue[ m_nEndMark++ ] = typeQueueItem;
		
	}
	return true;


}

template <class TYPE>
TYPE Queue< TYPE >::GetFrontQueue()	
{
	TYPE typeQueueItem;
	Monitor::Owner lock( m_csQueue );
	{
		if( m_nCurSize <= 0 )
			return NULL;
		if( m_nBeginMark == m_nQueueMaxSize )
			m_nBeginMark = 0;
		typeQueueItem = m_arrQueue[ m_nBeginMark++ ];
		
	}
	return typeQueueItem;

}

template <class TYPE>
void Queue< TYPE >::PopQueue()
{
	Monitor::Owner lock( m_csQueue );
	{
		m_nCurSize--;
	}
}

template <class TYPE>
bool Queue< TYPE >::IsEmptyQueue()
{
	bool bFlag = false;
	Monitor::Owner lock( m_csQueue );
	{
		bFlag = ( m_nCurSize > 0 ) ? true : false;
	}

	return flag;
}

template <class TYPE>
int Queue< TYPE >::GetQueueSize()
{
	int nSize;
	Monitor::Owner lock( m_csQueue );
	{
		nSize = m_nCurSize;
	}
	
	return nSize;
}

template <class TYPE>
void Queue< TYPE >::ClearQueue()
{
	Monitor::Owner lock( m_csQueue );
	{
		m_nCurSize = 0;
		m_nEndMark = 0;
		m_nBeginMark = 0;
	}
}
