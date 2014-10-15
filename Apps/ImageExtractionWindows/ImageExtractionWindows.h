// ImageExtractionWindows.h : main header file for the IMAGEEXTRACTIONWINDOWS application
//

#if !defined(AFX_IMAGEEXTRACTIONWINDOWS_H__277A4CF7_5037_42E0_8937_37777A66EB66__INCLUDED_)
#define AFX_IMAGEEXTRACTIONWINDOWS_H__277A4CF7_5037_42E0_8937_37777A66EB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsApp:
// See ImageExtractionWindows.cpp for the implementation of this class
//

class CImageExtractionWindowsApp : public CWinApp
{
public:
	CImageExtractionWindowsApp();

 	~CImageExtractionWindowsApp();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageExtractionWindowsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImageExtractionWindowsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEEXTRACTIONWINDOWS_H__277A4CF7_5037_42E0_8937_37777A66EB66__INCLUDED_)
