// GameLoader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GameLoader.h"
#include "GameLoaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderApp

BEGIN_MESSAGE_MAP(CGameLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CGameLoaderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderApp construction

CGameLoaderApp::CGameLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGameLoaderApp object

CGameLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderApp initialization

BOOL CGameLoaderApp::InitInstance()
{
	HANDLE hMutex;

	if( ( hMutex = ::OpenMutex( MUTEX_ALL_ACCESS, FALSE, _T( "Client Loader" ) ) ) != 0 )
	{
		::CloseHandle( hMutex );

		return FALSE;
	}

	hMutex = ::CreateMutex( 0, 0, _T( "Client Loader" ) );

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CGameLoaderDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	::CloseHandle( hMutex );

	return FALSE;
}

CString getResourceString( UINT uid )
{
	TCHAR buffer[1024];

	LoadString( AfxGetApp()->m_hInstance, uid, buffer, 1024 );

	return buffer;
}
