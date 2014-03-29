// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__51EC3D03_AC9C_47AE_A663_A252297F5DF5__INCLUDED_)
#define AFX_MAINFRM_H__51EC3D03_AC9C_47AE_A663_A252297F5DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	CMenu *pMDIFrameMenu;
	CMenu *pSpectrumSubMenu;
	CMenu *pMeasureSubMenu;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SwitchCalibrateView(BOOL beNeedStd,BOOL beNew,WorkZoneDef* pWz); //if beNew=TURE, Calibrate new created work-zone, else Calibrate current work-zone
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOpenDaemonDlg();
	afx_msg void OnClickOpenTube();
	afx_msg void OnClickCloseTube();
	afx_msg void OnClickOpenSpec();
	afx_msg void OnClickSaveSpec();
	afx_msg void OnClose();
	afx_msg void OnClickAutoSave();
	afx_msg void OnClickAutoSort();
	afx_msg void OnSwitchSpectrumView();
	afx_msg void OnSwitchMeasureView();
	afx_msg void OnClickStbTest();
	afx_msg void OnUpdateAutoSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAutoSort(CCmdUI* pCmdUI);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnClickTestFunction();
	afx_msg void OnCreateWorkZone();
	afx_msg void OnModifyWorkZone();
	afx_msg void OnSwitchWorkZone();
	afx_msg void OnUpdateFillSpec(CCmdUI* pCmdUI);
	afx_msg void OnClickFillSpec();
	afx_msg void OnMenuNormalize();
	afx_msg void OnMenuCorrect();
	afx_msg void OnMenuCorrectBase();
	afx_msg void OnMenuCorrectStd();
	afx_msg void OnSetMeasHeadParam();
	afx_msg void OnSetXYZParam();
	afx_msg void OnSetCrossParam();
	afx_msg void OnMenuEnergyCalib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__51EC3D03_AC9C_47AE_A663_A252297F5DF5__INCLUDED_)
