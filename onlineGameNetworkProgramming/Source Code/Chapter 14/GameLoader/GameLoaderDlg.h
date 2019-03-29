#if !defined(AFX_GAMELOADERDLG_H__9D42E591_67FA_4581_82A4_D065F60A8B32__INCLUDED_)
#define AFX_GAMELOADERDLG_H__9D42E591_67FA_4581_82A4_D065F60A8B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AfxTempl.h>

#include "FTPClient.h"
#include "ClientSocket.h"
#include "Protocol.h"


#define  WM_CONNECTUPDATESERVER	( WM_USER + 1 )
#define  WM_CONNECTFTPSERVER	( WM_USER + 2 )

#define  LOADERVERSION			1
#define  GAMECLIENTFILE			_T( "Client.exe" )
#define  GAMECLIENT_ARGUMENT	_T( "RunClient" )

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderDlg dialog

class CGameLoaderDlg : public CDialog
{
private:
	typedef struct
	{
		CString		directory;
		CString		remoteFile;
		CString		localFile;
		DWORD		fileSize;
	} FILEINFO;

public:
	CGameLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGameLoaderDlg)
	enum { IDD = IDD_GAMELOADER_DIALOG };
	CProgressCtrl	m_ctrlTotalProgress;
	CProgressCtrl	m_ctrlCurrentProgress;
	CString	m_strDestFile;
	CString	m_strMessage;
	CString	m_strRemainCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameLoaderDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameLoaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnFTPCallback(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCancelPatch();
	afx_msg void OnConnectUpdateServer(WPARAM wParam, LPARAM lParam);
	afx_msg void OnConnectFTPServer(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSocketConnect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSocketClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static	void		packetParsing( Packet& packet, ClientSocket* parent = 0, void* arg = 0 );

			int			sendNetMessage( Packet packet );
			bool		isEnoughDiskSpace( DWORD requireSpace );

			bool		connectToFTPServer( CString address, CString account, CString password );

			void		removeDir( LPCTSTR dir );
			void		createFullPath( CString fullPath );

			void		splitFullPath( CString fullPath, CString* directory, CString* fileName );
			DWORD		addFileToList( CString fileName, CString localFile );
			bool		killFileAttr( CString fileName );

			void		sendLoaderVersion();
			void		sendClientVersion();
			void		requestUpdateList();

			void		requestUpdateLoaderList();
			void		requestUpdateClientList();

			void		onUCConnectionSuccessAck( Packet& packet );
			void		onUCLoaderVersionAck( Packet& packet );
			void		onUCClientVersionAck( Packet& packet );
			void		onUCUpdateLoaderListAck( Packet& packet );
			void		onUCUpdateClientListAck( Packet& packet );

			bool		getNextFile();


			CList	< FILEINFO*, FILEINFO* >		listFile;
			CList	< FILEINFO*, FILEINFO* >		listDownloadFile;

			ClientSocket	clientSocket;

			FTPClient		ftpClient;

			CString			FTPAddress, FTPAccount, FTPPasswd;

			CString			localDir, tempDir;
			bool			destroyDlg;
			bool			updateLoader;
			bool			updateClient;
			bool			loadClient;
			bool			receivedPatchInfo;

			FILEINFO*		currentFileInfo;
			DWORD			transBytesCurrent;
			DWORD			transBytesTotal;
			DWORD			currentWorkFileNo, totalFileCount;

			DWORD			loaderVersion;
			DWORD			clientVersion;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMELOADERDLG_H__9D42E591_67FA_4581_82A4_D065F60A8B32__INCLUDED_)
