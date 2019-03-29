// GameLoader.h : main header file for the GAMELOADER application
//

#if !defined(AFX_GAMELOADER_H__5876624C_5F38_4E9C_AC29_81B2F9B1D99B__INCLUDED_)
#define AFX_GAMELOADER_H__5876624C_5F38_4E9C_AC29_81B2F9B1D99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderApp:
// See GameLoader.cpp for the implementation of this class
//

class CGameLoaderApp : public CWinApp
{
public:
	CGameLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGameLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CString	getResourceString( UINT uid );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMELOADER_H__5876624C_5F38_4E9C_AC29_81B2F9B1D99B__INCLUDED_)
