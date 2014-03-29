#if !defined(AFX_DEFINEAPPTAB_H__E717ABB8_5821_437C_AC01_382546A2269B__INCLUDED_)
#define AFX_DEFINEAPPTAB_H__E717ABB8_5821_437C_AC01_382546A2269B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineAppTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDefineAppTab dialog
#include "WorkZoneDef.h"
class CDefineAppTab : public CDialog
{
	WorkZoneDef* pWz;
// Construction
public:
	CDefineAppTab(CWnd* pParent = NULL);   // standard constructor
	void LinkWz(WorkZoneDef *p,BOOL beNew);
	void SaveToWz();
// Dialog Data
	//{{AFX_DATA(CDefineAppTab)
	enum { IDD = IDD_TAB_DEFAPP };
	int		m_adjustRepeat;
	int		m_adjustTimes;
	int		m_mqMax;
	int		m_index;
	CString	m_tag;
	int		m_current;
	BOOL	m_autoCurrent;
	//}}AFX_DATA
	CString m_defMaName;
	int m_collimator;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineAppTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefineAppTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditDefMa();
	afx_msg void OnEditStdSample();
	afx_msg void OnSelCollimator();
	afx_msg void OnAutoCurrent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEAPPTAB_H__E717ABB8_5821_437C_AC01_382546A2269B__INCLUDED_)
