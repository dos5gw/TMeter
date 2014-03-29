#if !defined(AFX_ADJUSTZONEDLG_H__63DD3B61_065F_4A8B_8E5C_69CCAC2DB2FE__INCLUDED_)
#define AFX_ADJUSTZONEDLG_H__63DD3B61_065F_4A8B_8E5C_69CCAC2DB2FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustZoneDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjustZoneDlg dialog

class CAdjustZoneDlg : public CDialog
{
// Construction
public:
	CAdjustZoneDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bInitialized;
// Dialog Data
	//{{AFX_DATA(CAdjustZoneDlg)
	enum { IDD = IDD_SETZONE };
	CSpinButtonCtrl	m_topSpin;
	CSpinButtonCtrl	m_baseSpin;
	int		m_baseLine;
	int		m_topLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustZoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustZoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeBase();
	afx_msg void OnChangeTop();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTZONEDLG_H__63DD3B61_065F_4A8B_8E5C_69CCAC2DB2FE__INCLUDED_)
