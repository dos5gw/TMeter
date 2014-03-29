#if !defined(AFX_PUREINTENSITYDLG_H__785957A7_B3D1_40BE_A8FA_15C55A80ECA5__INCLUDED_)
#define AFX_PUREINTENSITYDLG_H__785957A7_B3D1_40BE_A8FA_15C55A80ECA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PureIntensityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPureIntensityDlg dialog

//extern "C"{
typedef void (WINAPI *SETELEMENTWINDOWS)(int count, int* win);
typedef void (WINAPI *SETELEMENTDATA)(int length, double* datas);
//}

#include <afxtempl.h>

class OnePureElement : public CObject
{
public:
	CString Name;
	int left;
	int right;
public:
	OnePureElement(CString name,int l,int r):Name(name),left(l),right(r){ }
	~OnePureElement(){ }
};

class PureElementPtrList : public CTypedPtrArray<CObArray,OnePureElement*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
	
};

class CPureIntensityDlg : public CDialog
{
// Construction
public:
	CPureIntensityDlg(CWnd* pParent = NULL);   // standard constructor
	//~CPureIntensityDlg();
// Dialog Data
	//{{AFX_DATA(CPureIntensityDlg)
	enum { IDD = IDD_PUREELEINTENSITYDLG };
	CComboBox	m_HvComboBox;
	CListCtrl	m_ListControl;
	CProgressCtrl	m_Progress;
	CString	m_ElementName;
	int		m_Left;
	int		m_Right;
	//}}AFX_DATA

private:
	void CleanPureElementList();
	BOOL LoadSettings();
	BOOL m_bTesting;
	PureElementPtrList pureElementList;

	CString CurrentHv;
	CString CurrentEle;
	int m_nCheckIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPureIntensityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPureIntensityDlg)
	virtual void OnCancel();
	afx_msg void OnStart();
	afx_msg void OnStop();	
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetCfg();
	afx_msg void OnClickPureList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSavePureWindows();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUREINTENSITYDLG_H__785957A7_B3D1_40BE_A8FA_15C55A80ECA5__INCLUDED_)
