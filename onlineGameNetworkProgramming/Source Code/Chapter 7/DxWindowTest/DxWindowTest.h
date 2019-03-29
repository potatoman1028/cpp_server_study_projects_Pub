// DxWindowTest.h : main header file for the DXWINDOWTEST application
//

#if !defined(AFX_DXWINDOWTEST_H__3C7B57A5_46FF_4009_806B_E16E9DC7D0B1__INCLUDED_)
#define AFX_DXWINDOWTEST_H__3C7B57A5_46FF_4009_806B_E16E9DC7D0B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDxWindowTestApp:
// See DxWindowTest.cpp for the implementation of this class
//

class CDxWindowTestApp : public CWinApp
{
public:
	CDxWindowTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDxWindowTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CDxWindowTestApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXWINDOWTEST_H__3C7B57A5_46FF_4009_806B_E16E9DC7D0B1__INCLUDED_)
