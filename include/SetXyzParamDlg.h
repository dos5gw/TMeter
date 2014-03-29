#if !defined(AFX_SETXYZPARAMDLG_H__68F2DE6F_E313_42F7_A0BA_B3E25AB7BF8F__INCLUDED_)
#define AFX_SETXYZPARAMDLG_H__68F2DE6F_E313_42F7_A0BA_B3E25AB7BF8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetXyzParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetXyzParamDlg dialog

class CSetXyzParamDlg : public CDialog
{
// Construction
public:
	CSetXyzParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetXyzParamDlg)
	enum { IDD = IDD_SET_XYZ_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetXyzParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetXyzParamDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETXYZPARAMDLG_H__68F2DE6F_E313_42F7_A0BA_B3E25AB7BF8F__INCLUDED_)
