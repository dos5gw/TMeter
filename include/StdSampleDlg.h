#if !defined(AFX_STDSAMPLEDLG_H__38A775D9_D75F_4E98_B789_33D37D3C9895__INCLUDED_)
#define AFX_STDSAMPLEDLG_H__38A775D9_D75F_4E98_B789_33D37D3C9895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdSample.h"

// StdSampleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStdSampleDlg dialog

class CStdSampleDlg : public CDialog
{
// Construction
public:
	CStdSampleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStdSampleDlg)
	enum { IDD = IDD_STDSAMPLEDLG };
	CListCtrl	m_SampleList;
	//}}AFX_DATA
	
private:
	BOOL m_bNewFile;
	BOOL m_beChanged;
	CStandardSamples* m_pStdSamples;
	WorkZoneDef* m_pWzDef;

public:
	void LinkValue(BOOL bNew, CStandardSamples* ptr);
	void LinkValue(BOOL bNew, WorkZoneDef* ptr);
	void UpdateSampleList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStdSampleDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnNewAndEdit();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnDeleteAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenFromFile();
	afx_msg void OnSaveToFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDSAMPLEDLG_H__38A775D9_D75F_4E98_B789_33D37D3C9895__INCLUDED_)
