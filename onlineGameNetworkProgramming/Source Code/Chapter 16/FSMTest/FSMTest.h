// FSMTest.h : main header file for the FSMTEST application
//

#if !defined(AFX_FSMTEST_H__9B408007_A9ED_4520_81FC_FD3A15EC8B21__INCLUDED_)
#define AFX_FSMTEST_H__9B408007_A9ED_4520_81FC_FD3A15EC8B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFSMTestApp:
// See FSMTest.cpp for the implementation of this class
//

class CFSMTestApp : public CWinApp
{
public:
	CFSMTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFSMTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CFSMTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSMTEST_H__9B408007_A9ED_4520_81FC_FD3A15EC8B21__INCLUDED_)
