#if !defined(AFX_CONDITIONMONITORDLG_H__B6A6F800_52D0_49D5_B5B2_20DFAC71B474__INCLUDED_)
#define AFX_CONDITIONMONITORDLG_H__B6A6F800_52D0_49D5_B5B2_20DFAC71B474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XrfInstrument.h"
/////////////////////////////////////////////////////////////////////////////
// CConditionMonitorDlg dialog

class CConditionMonitorDlg : public CDialog
{
// Construction
public:
	CConditionMonitorDlg(CWnd* pParent = NULL);   // standard constructor
	~CConditionMonitorDlg();
	CXrfInstrument* p_Instrument;
// Dialog Data
	//{{AFX_DATA(CConditionMonitorDlg)
	enum { IDD = IDD_CONDITIONMONITORDLG };
	CStatic	currentTemp2;
	CStatic	currentTemp1;
	CStatic	currentStdVol;
	CStatic	currentMA;
	CStatic	currentHV;
	CStatic	currentH2500Vol;
	int	SampleTime;
	float	WorkHV;
	float	WorkMA;
	float	H2500Vol;
	float	UTemp;
	float	LTemp;
	int		BaseLine;
	int		TopLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConditionMonitorDlg)
	afx_msg void OnOpenTube();
	afx_msg void OnCloseTube();
	afx_msg void OnOpenShut();
	afx_msg void OnCloseShut();
	afx_msg void OnOpenLamp();
	afx_msg void OnCloseLamp();
	afx_msg void OnBnClickedSetparam();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONMONITORDLG_H__B6A6F800_52D0_49D5_B5B2_20DFAC71B474__INCLUDED_)
