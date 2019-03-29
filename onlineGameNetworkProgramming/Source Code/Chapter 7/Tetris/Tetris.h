// Tetris.h : main header file for the TETRIS application
//

#if !defined(AFX_TETRIS_H__B9842E99_76EA_4E00_8458_4FEAF82F34B7__INCLUDED_)
#define AFX_TETRIS_H__B9842E99_76EA_4E00_8458_4FEAF82F34B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTetrisApp:
// See Tetris.cpp for the implementation of this class
//

class CTetrisApp : public CWinApp
{
public:
	CTetrisApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTetrisApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CTetrisApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TETRIS_H__B9842E99_76EA_4E00_8458_4FEAF82F34B7__INCLUDED_)
