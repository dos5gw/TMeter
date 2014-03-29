#if !defined(AFX_DEFMALISTDLG_H__AD988F8F_1CEE_4A10_B94D_6F5C61C92B74__INCLUDED_)
#define AFX_DEFMALISTDLG_H__AD988F8F_1CEE_4A10_B94D_6F5C61C92B74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefMAListDlg.h : header file
//

#include "DefMADataSet.h"
/////////////////////////////////////////////////////////////////////////////
// CDefMAListDlg dialog

class CDefMAListDlg : public CDialog
{
// Construction
public:
	CDefMAListDlg(CWnd* pParent = NULL);   // standard constructor
	CString defMaName;
	CString defMaTag;
	int defMaSum;
// Dialog Data
	//{{AFX_DATA(CDefMAListDlg)
	enum { IDD = IDD_DEFMALISTDLG };
	CListCtrl	m_MaListEx;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefMAListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefMAListDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFMALISTDLG_H__AD988F8F_1CEE_4A10_B94D_6F5C61C92B74__INCLUDED_)
