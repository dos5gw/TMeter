#if !defined(AFX_WORKZONELISTDLG_H__03A75CBF_554A_4C0A_9C96_8F145E355397__INCLUDED_)
#define AFX_WORKZONELISTDLG_H__03A75CBF_554A_4C0A_9C96_8F145E355397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkZoneListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkZoneListDlg dialog

class CWorkZoneListDlg : public CDialog
{
// Construction
public:
	CWorkZoneListDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CWorkZoneListDlg)
	enum { IDD = IDD_WORKZONELISTDLG };
	CListCtrl	m_WorkZoneList;
	CString	m_wzTag;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkZoneListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL beNewWz;
	int Index;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorkZoneListDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKZONELISTDLG_H__03A75CBF_554A_4C0A_9C96_8F145E355397__INCLUDED_)
