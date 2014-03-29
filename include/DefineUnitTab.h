#if !defined(AFX_DEFINEUNITTAB_H__1561EE53_69FC_4681_AC50_D7E1ADA332B0__INCLUDED_)
#define AFX_DEFINEUNITTAB_H__1561EE53_69FC_4681_AC50_D7E1ADA332B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineUnitTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDefineUnitTab dialog
#include "WorkZoneDef.h"
class CDefineUnitTab : public CDialog
{
	WorkZoneDef* pWz;
// Construction
public:
	CDefineUnitTab(CWnd* pParent = NULL);   // standard constructor
	void LinkWz(WorkZoneDef *p);
	void SaveToWz();
// Dialog Data
	//{{AFX_DATA(CDefineUnitTab)
	enum { IDD = IDD_TAB_DEFUNIT };
	int		m_arraySize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineUnitTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefineUnitTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEUNITTAB_H__1561EE53_69FC_4681_AC50_D7E1ADA332B0__INCLUDED_)
