#if !defined(AFX_TIMETICKDLG_H__7AF987FD_363B_4C5E_A02A_02384DC7251C__INCLUDED_)
#define AFX_TIMETICKDLG_H__7AF987FD_363B_4C5E_A02A_02384DC7251C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeTickDlg.h : header file
//
#include "Btnst.h"
/////////////////////////////////////////////////////////////////////////////
// CTimeTickDlg dialog

class CTimeTickDlg : public CDialog
{
// Construction
public:
	CTimeTickDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeTickDlg)
	enum { IDD = IDD_TIMETICKDLG };
	CStatic	m_StatusEx;
	CMyTextButton	m_SecondsEx;
	//}}AFX_DATA

private:
	int m_SecondTick;
	CString m_SecondString;
	BOOL m_bSampleFinished;
	BOOL m_bSampleStarted;
	void QuitMeasure();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeTickDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeTickDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMETICKDLG_H__7AF987FD_363B_4C5E_A02A_02384DC7251C__INCLUDED_)
