#if !defined(AFX_PERIODICTABLEDLG_H__6FA95D35_552B_464B_B562_73A6248FE78B__INCLUDED_)
#define AFX_PERIODICTABLEDLG_H__6FA95D35_552B_464B_B562_73A6248FE78B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPeriodicTableDlg dialog
#include "Btnst.h"

class CPeriodicTableDlg : public CDialog
{
// Construction
public:
	CPeriodicTableDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPeriodicTableDlg)
	enum { IDD = IDD_PERIODICTABLEDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeriodicTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPeriodicTableDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnClickElement(UINT uID);
	afx_msg void OnCleanAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERIODICTABLEDLG_H__6FA95D35_552B_464B_B562_73A6248FE78B__INCLUDED_)
