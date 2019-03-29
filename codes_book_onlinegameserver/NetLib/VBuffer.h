#pragma once

class VBuffer :  public Singleton
{
	DECLEAR_SINGLETON( VBuffer );

public:
	VBuffer( int nMaxBufSize = 1024 * 50 );
	virtual ~VBuffer(void);
	
	void GetChar( char& cCh );
	void GetShort( short& sNum );
	void GetInteger( int& nNum );
	void GetString( char* pszBuffer );
	void GetStream( char* pszBuffer, short sLen);	
	void SetInteger( int nI );
	void SetShort( short sShort );
	void SetChar( char cCh);
	void SetString( char* pszBuffer );
	void SetStream( char* pszBuffer , short sLen);

	void SetBuffer( char* pVBuffer );

	inline int	GetMaxBufSize() { return m_nMaxBufSize; }
	inline int	GetCurBufSize() { return m_nCurBufSize; }
	inline char*	GetCurMark() { return m_pCurMark; }
	inline char*    GetBeginMark() { return m_pszVBuffer; }

	bool CopyBuffer( char* pDestBuffer );
	void Init();			
	 
private:

	char* m_pszVBuffer;		//실제 버퍼
	char* m_pCurMark;		//현재 버퍼 위치

	int	m_nMaxBufSize;		//최대 버퍼 사이즈
	int m_nCurBufSize;		//현재 사용된 버퍼 사이즈

	VBuffer(const VBuffer &rhs);
	VBuffer &operator=(const VBuffer &rhs);
};

CREATE_FUNCTION( VBuffer , g_GetVBuffer );