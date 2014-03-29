#if !defined(AFX_CALIBCHILDWND_H__9BCB5523_27ED_458A_AB40_AEA55CC79AA0__INCLUDED_)
#define AFX_CALIBCHILDWND_H__9BCB5523_27ED_458A_AB40_AEA55CC79AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalibChildWnd frame

class CCalibChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCalibChildWnd)
protected:
	CCalibChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibChildWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCalibChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CCalibChildWnd)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBCHILDWND_H__9BCB5523_27ED_458A_AB40_AEA55CC79AA0__INCLUDED_)
