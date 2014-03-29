#if !defined(AFX_CREATEWORKZONEDLG_H__5FA18A46_A547_4532_B316_3B9D71E06CC0__INCLUDED_)
#define AFX_CREATEWORKZONEDLG_H__5FA18A46_A547_4532_B316_3B9D71E06CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateWorkZoneDlg.h : header file
//
#include "DefineProductTab.h"
#include "DefineAppTab.h"
#include "DefineUnitTab.h"
#include "WorkZoneDef.h"
/////////////////////////////////////////////////////////////////////////////
// CCreateWorkZoneDlg dialog

class CCreateWorkZoneDlg : public CDialog
{

	CDefineProductTab m_productTab;
	CDefineAppTab m_appTab;
	CDefineUnitTab m_unitTab;

private:
	WorkZoneDef* m_pWorkZone;
	BOOL m_bNewFile;

// Construction
public:
	CCreateWorkZoneDlg(CWnd* pParent = NULL);   // standard constructor
	void LinkValue(BOOL m_bNewFile,WorkZoneDef* ptr);
	void SaveToDatabase();
	BOOL isNewFile(){ return m_bNewFile;};
// Dialog Data
	//{{AFX_DATA(CCreateWorkZoneDlg)
	enum { IDD = IDD_CREAWORKZONEDLG };
	CTabCtrl	m_tabCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateWorkZoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateWorkZoneDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeWzTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAccept();
	afx_msg void OnExit();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEWORKZONEDLG_H__5FA18A46_A547_4532_B316_3B9D71E06CC0__INCLUDED_)
