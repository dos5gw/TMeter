//{{AFX_INCLUDES()
#include "_grid.h"
//}}AFX_INCLUDES
#if !defined(AFX_CONFONESTDDLG_H__7710DA9F_DD37_4064_990D_A1AA5E8E293F__INCLUDED_)
#define AFX_CONFONESTDDLG_H__7710DA9F_DD37_4064_990D_A1AA5E8E293F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfOneStdDlg.h : header file
//
#include "StdSample.h"

/////////////////////////////////////////////////////////////////////////////
// CConfOneStdDlg dialog

class CConfOneStdDlg : public CDialog
{
// Construction
public:
	CConfOneStdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfOneStdDlg)
	enum { IDD = IDD_CONFSAMPLEDLG };
	C_Grid	m_StdGrid;
	CString	m_StdName;
	//}}AFX_DATA

	BOOL m_bNewFile;
	CStandardSample* m_pStdSample;

	void FillGrid();
	void LinkValue(BOOL bNew,CStandardSample* ptr); /* bNew=TURE insert new sample, bNew=FALSE update exist sample */

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfOneStdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfOneStdDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFONESTDDLG_H__7710DA9F_DD37_4064_990D_A1AA5E8E293F__INCLUDED_)
