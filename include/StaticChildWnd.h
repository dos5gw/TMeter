#if !defined(AFX_STATICCHILDWND_H__94204617_64C1_4707_8758_1E32697A285F__INCLUDED_)
#define AFX_STATICCHILDWND_H__94204617_64C1_4707_8758_1E32697A285F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticChildWnd frame

class CStaticChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CStaticChildWnd)
protected:
	CStaticChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticChildWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStaticChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CStaticChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCHILDWND_H__94204617_64C1_4707_8758_1E32697A285F__INCLUDED_)
