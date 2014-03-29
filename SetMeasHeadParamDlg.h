#if !defined(AFX_SETMEASHEADPARAMDLG_H__0232AA15_19F9_4D6B_9882_D94C65137B22__INCLUDED_)
#define AFX_SETMEASHEADPARAMDLG_H__0232AA15_19F9_4D6B_9882_D94C65137B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetMeasHeadParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetMeasHeadParamDlg dialog

class CSetMeasHeadParamDlg : public CDialog
{
// Construction
public:
	CSetMeasHeadParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetMeasHeadParamDlg)
	enum { IDD = IDD_SET_MEASHEARD_DLG };
	float	m_HV30;
	float	m_HV40;
	float	m_HV50;
	float	m_FilterThicknessAl;
	float	m_FilterThicknessNi;
	float	m_HV2500;
	float	m_XrayTargetTakeOffAngle;
	short	m_TargetAtomicNumber;
	float	m_XrayIncidentAngle;
	float	m_TubeWindowThickness;
	float	m_XrayEmergentAngle;
	float	m_CalibK;
	float	m_CalibB;
	float	m_CalibC1;
	float	m_CalibC2;
	float	m_HI30;
	float	m_HI40;
	float	m_HI50;
	float	m_HV2500Step;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetMeasHeadParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetMeasHeadParamDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETMEASHEADPARAMDLG_H__0232AA15_19F9_4D6B_9882_D94C65137B22__INCLUDED_)
