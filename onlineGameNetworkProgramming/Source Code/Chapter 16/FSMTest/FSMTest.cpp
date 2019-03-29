// FSMTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FSMTest.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFSMTestApp

BEGIN_MESSAGE_MAP(CFSMTestApp, CWinApp)
	//{{AFX_MSG_MAP(CFSMTestApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFSMTestApp construction

CFSMTestApp::CFSMTestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFSMTestApp object

CFSMTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFSMTestApp initialization

BOOL CFSMTestApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);



	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}
