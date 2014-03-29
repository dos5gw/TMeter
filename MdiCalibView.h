#if !defined(AFX_MDICALIBVIEW_H__60138E3E_CC1C_4F99_B53A_9BD9D2FDE6FE__INCLUDED_)
#define AFX_MDICALIBVIEW_H__60138E3E_CC1C_4F99_B53A_9BD9D2FDE6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiCalibView.h : header file
//

#include "Btnst.h"
#include "OneTask.h"
#include "XrfInstrument.h"
#include "MyTextButton.h"

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

typedef enum {
	CALIBRATE,
	NORMALIZE,
} CALIB_MODE;


class CMdiCalibView : public CFormView
{
protected:
	CMdiCalibView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiCalibView)

public:
	WorkZoneDef* m_pCalibWz;
	CXrfInstrument* m_pInstrument;
	CALIB_MODE m_calibMode;
	BOOL beNeedStd; 
	BOOL m_beCalibNewWz; // if TURE, calibrate new created work-zone, must delete this work-zone after finishing calibrating
                         // if FALSE, calibrate current opened  work-zone
	CTaskTrack m_TaskTrack;
	COneTask* m_pCurrTask;

	BOOL m_bInitialized;
	BOOL m_beCalibing;
	BOOL m_beCalibFinished;

// Form Data
public:
	//{{AFX_DATA(CMdiCalibView)
	enum { IDD = IDD_MDICALIBVIEW_FORM };
	CMyTextButton	m_BtnWorkZoneTitle;
	CStatic	m_VideoDisplay;
	CListCtrl	m_StatisList;
	CButtonST	m_BtnStop;
	CButtonST	m_BtnStart;
	CListCtrl	m_ArrayList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void CalibrateView(BOOL beNeedStd,BOOL beNewWz,WorkZoneDef* pWz);
	void ChangeMeasureCondition(WorkZoneDef* pWz);
	void CreateCalibTaskTrack(BOOL beNeedStd);
	void UpdateCalibTip(COneTask*);
	void ExecuteTask(COneTask*);

	void UpdateCpsArrayList();
	void AddNewCpsToArray();

	void CleanTaskTrack();
	void PreExitCalibView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiCalibView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMdiCalibView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMdiCalibView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnQuitCalib();
	afx_msg void OnCalibRedo();
	afx_msg void OnCalibLoadFromFile();
	afx_msg void OnCalibSkip();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICALIBVIEW_H__60138E3E_CC1C_4F99_B53A_9BD9D2FDE6FE__INCLUDED_)
