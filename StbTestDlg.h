#if !defined(AFX_STBTESTDLG_H__727CA25F_D25B_4EFB_B737_FE100BB51DC0__INCLUDED_)
#define AFX_STBTESTDLG_H__727CA25F_D25B_4EFB_B737_FE100BB51DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StbTestDlg.h : header file
//

#include "Gridctrl.h"

class StableTestResult
{
public:
	StableTestResult()
	{
		cps=0.0;
		peakPos=0;
		precision=0.0;
	}
	~StableTestResult();
	CTime time;
	double cps;
	int peakPos;
	double precision;
};

/////////////////////////////////////////////////////////////////////////////
// CStbTestDlg dialog

class CStbTestDlg : public CDialog
{
// Construction
public:
	CStbTestDlg(CWnd* pParent = NULL);   // standard constructor
	~CStbTestDlg();

// Dialog Data
	//{{AFX_DATA(CStbTestDlg)
	enum { IDD = IDD_STBTESTDLG };
	CListCtrl	m_ReportList;
	CProgressCtrl	m_sampleProgress;
	int		m_TestTimes;
	int		m_SampleTimes;
	//}}AFX_DATA
private:
	CGridCtrl* m_pReport;
	BOOL m_bTesting;
	//StableTestResult currentResult;
	CTime lastTestTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStbTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ExitStbTest();

	// Generated message map functions
	//{{AFX_MSG(CStbTestDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStartStb();
	afx_msg void OnStopStb();
	afx_msg void OnCleanList();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSaveReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STBTESTDLG_H__727CA25F_D25B_4EFB_B737_FE100BB51DC0__INCLUDED_)
