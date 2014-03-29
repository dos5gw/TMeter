#if !defined(AFX_ENERGYCALIBDLG_H__EEE92DE3_9413_4BFF_BAA6_D287162A0BCB__INCLUDED_)
#define AFX_ENERGYCALIBDLG_H__EEE92DE3_9413_4BFF_BAA6_D287162A0BCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnergyCalibDlg.h : header file
//

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

typedef enum CALIB_ELEM{
	Ag,
	Cu,
}CalibElem;

/////////////////////////////////////////////////////////////////////////////
// EnergyCalibDlg dialog

class EnergyCalibDlg : public CDialog
{
// Construction
public:
	EnergyCalibDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EnergyCalibDlg)
	enum { IDD = IDD_ENERGY_CALIB_DLG };
	float	m_LastCalibB;
	float	m_LastCalibK;
	float	m_LastH2500;
	float	m_CurrH2500;
	float	m_CurrPeakOfAg;
	int		m_CalibTime;
	float	m_Scale;
	//}}AFX_DATA

	int MaxCalibTime;
	BOOL m_CalibFinished;
	BOOL m_Calibrating;
	BOOL m_ParamBeSended;
	float m_TempH2500;
	float m_TempAgPeak;
	CalibElem m_CalibElem;

	float GetVolStep(float chs);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EnergyCalibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EnergyCalibDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnStartCalib();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENERGYCALIBDLG_H__EEE92DE3_9413_4BFF_BAA6_D287162A0BCB__INCLUDED_)
