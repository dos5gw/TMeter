#if !defined(AFX_DEFINEONELAYERDLG_H__E44AE914_19D2_4D0A_BB4C_35B74ED5F617__INCLUDED_)
#define AFX_DEFINEONELAYERDLG_H__E44AE914_19D2_4D0A_BB4C_35B74ED5F617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineOneLayerDlg.h : header file
//

#include "OneLayerDefine.h"
/////////////////////////////////////////////////////////////////////////////
// CDefineOneLayerDlg dialog

class CDefineOneLayerDlg : public CDialog
{
	OneLayerDefine* m_pOneLayer;
	CTMeterApp* pApp;
// Construction
public:
	CDefineOneLayerDlg(CWnd* pParent = NULL);   // standard constructor
	void LinkData(OneLayerDefine* p){m_pOneLayer=p;}
	void UpdateElemCombo();
	void UpdateElemList();
// Dialog Data
	//{{AFX_DATA(CDefineOneLayerDlg)
	enum { IDD = IDD_CONFLAYERDLG };
	CComboBox	m_elemCombo;
	CListCtrl	m_listEx;
	float	m_density;   //镀层的密度
	float	m_thickness; //镀层的厚度
	float	m_elemContent; // 选中元素的含量
	float	m_elemDensity; // 选中元素的密度
	BOOL	m_isSaturated;
	BOOL	m_NeedCalculated;
	int		m_elemLeft;
	int		m_elemRight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefineOneLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefineOneLayerDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnElemComboChanged();
	afx_msg void OnAddNewElement();
	afx_msg void OnReplaceElement();
	afx_msg void OnDeleteElement();
	afx_msg void OnRecontent();
	afx_msg void OnClickElemList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEONELAYERDLG_H__E44AE914_19D2_4D0A_BB4C_35B74ED5F617__INCLUDED_)
