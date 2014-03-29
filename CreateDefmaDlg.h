#if !defined(AFX_CREATEDEFMADLG_H__0E5C9B76_6C3B_45EA_9FDD_CA629AADAA5F__INCLUDED_)
#define AFX_CREATEDEFMADLG_H__0E5C9B76_6C3B_45EA_9FDD_CA629AADAA5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateDefmaDlg.h : header file
//

#include "DefMa.h"
#include "DefMADataSet.h"
/////////////////////////////////////////////////////////////////////////////
// CCreateDefmaDlg dialog

class CCreateDefmaDlg : public CDialog
{
	
// Construction
public:
	CCreateDefmaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateDefmaDlg)
	enum { IDD = IDD_DEFMADLG };
	CTreeCtrl	m_LayerTree;
	CString	m_tag;
	CString	m_thickMode;
	//}}AFX_DATA

private:
	CDefMa* m_pDefMa; //Data struct for defMA
	WorkZoneDef *m_pWz;
	BOOL m_bNewFile; //if this dialog is run from CDefineAppTab::OnEditDefMa(), m_bNewFile=F, and must point out "m_wzDir"
public:
	int index;
	void UpdateElemTree();
	void LinkValue(BOOL bNew,CDefMa* p);
	void LinkValue(BOOL bNew,WorkZoneDef* p);
	void SaveDataToPtr();  // save data from UI to m_pDefMa

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateDefmaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateDefmaDlg)
	afx_msg void OnModifyLayer();
	afx_msg void OnNewLayer();
	afx_msg void OnSpectrumCompensate();
	afx_msg void OnCailbSetting();
	afx_msg void OnSave();
	afx_msg void OnExitCreating();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEDEFMADLG_H__0E5C9B76_6C3B_45EA_9FDD_CA629AADAA5F__INCLUDED_)
