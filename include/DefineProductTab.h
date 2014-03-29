#if !defined(AFX_DEFINEPRODUCTTAB_H__D2925415_72D7_41E0_BD3C_078E87D3A88C__INCLUDED_)
#define AFX_DEFINEPRODUCTTAB_H__D2925415_72D7_41E0_BD3C_078E87D3A88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineProductTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDefineProductTab dialog
#include "WorkZoneDef.h"
class CDefineProductTab : public CDialog
{
	WorkZoneDef* pWz;
// Construction
public:
	CDefineProductTab(CWnd* pParent = NULL);   // standard constructor
	void LinkWz(WorkZoneDef *p);
	void SaveToWz();
// Dialog Data
	//{{AFX_DATA(CDefineProductTab)
	enum { IDD = IDD_TAB_DEFPRODUCT };
	CComboBox	m_percisionEx;
	CString	m_tag;
	CString	m_measureTime;
	CString	m_modifyTime;
	CString	m_correctTime;
	int		m_index;
	int		m_repeat;
	int		m_sampleTimes;
	//}}AFX_DATA

	int m_thickUnit;
	int m_contentUnit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineProductTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefineProductTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEPRODUCTTAB_H__D2925415_72D7_41E0_BD3C_078E87D3A88C__INCLUDED_)
