#if !defined(AFX_SPECTRUMCHILDWND_H__0FA234B8_C2A4_4C6B_9EFD_C883D80A860D__INCLUDED_)
#define AFX_SPECTRUMCHILDWND_H__0FA234B8_C2A4_4C6B_9EFD_C883D80A860D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpectrumChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpectrumChildWnd frame

class CSpectrumChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSpectrumChildWnd)
protected:
	CSpectrumChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectrumChildWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSpectrumChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CSpectrumChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRUMCHILDWND_H__0FA234B8_C2A4_4C6B_9EFD_C883D80A860D__INCLUDED_)
