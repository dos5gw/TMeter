#if !defined(AFX_SELCOLLIMATORDLG_H__2DE96B1C_733D_40E1_A119_12976841EB65__INCLUDED_)
#define AFX_SELCOLLIMATORDLG_H__2DE96B1C_733D_40E1_A119_12976841EB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelCollimatorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelCollimatorDlg dialog

class CSelCollimatorDlg : public CDialog
{
// Construction
public:
	CSelCollimatorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelCollimatorDlg)
	enum { IDD = IDD_COLLDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void LinkValue(BOOL beNew,WorkZoneDef* ptr);
	int collimator;  // 1,2,3,4

private:
	WorkZoneDef* pWz;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelCollimatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelCollimatorDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELCOLLIMATORDLG_H__2DE96B1C_733D_40E1_A119_12976841EB65__INCLUDED_)
