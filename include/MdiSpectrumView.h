#if !defined(AFX_MDISPECTRUMVIEW_H__089BC3B4_2D4C_4D58_B941_7BFADA44A243__INCLUDED_)
#define AFX_MDISPECTRUMVIEW_H__089BC3B4_2D4C_4D58_B941_7BFADA44A243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiSpectrumView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//#include "TMeter.h"
#include "Btnst.h"
#include "ColorListCtrl.h"
#include "SpectrumDisplay.h"
#include "MyTextButton.h"
#include "PeriodicTableDlg.h"

class CMdiSpectrumView : public CFormView
{
protected:
	CMdiSpectrumView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiSpectrumView)

// Form Data
public:
	//{{AFX_DATA(CMdiSpectrumView)
	enum { IDD = IDD_MDISPECTRUMVIEW_ZH };
	CColorListCtrl	m_colorListEx;
	CStatic	m_CurrentChannelInfo;
	CStatic	m_AllChannelsInfo;
	CSpinButtonCtrl	m_CursorSpin;
	CButtonST	m_BtnMoveRight;
	CButtonST	m_BtnMoveLeft;
	CSpectrumDisplay	m_SpectrumDisplay;
	int		m_CursorChannel;
	//}}AFX_DATA

// Attributes
public:
	BOOL m_bAutoScale;
	CPeriodicTableDlg* m_pPeriodicTableDlg;

// Operations
public:
	void UpdateStaticControls();
	void UpdateColorControls();
	void CleanContrastSpec();

private:
	//CTMeterApp* m_pApp;
	BOOL m_bInitialized;
	CXrfInstrument* m_pInstrument;
	//CImageList m_imageList;
	//CTypedPtrArray<CObArray,CSpectrumInfo<DWORD,SPEC_SIZE>*> m_ContrastSpectrum;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiSpectrumView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMdiSpectrumView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: 
	// Generated message map functions
	//{{AFX_MSG(CMdiSpectrumView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasure();
	afx_msg void OnStbSample();
	afx_msg void OnYContract();
	afx_msg void OnYExtend();
	afx_msg void OnXExtend();
	afx_msg void OnXContract();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnSetZone();
	afx_msg void OnAutoScale();
	afx_msg void OnFillSpectrum();
	afx_msg void OnOpenPeriodicTable();
	afx_msg void OnChangeChannels();
	afx_msg void OnClickUserScale();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickSpectrumEx();
	afx_msg void CleanAllContrastSpec(); 
	afx_msg void CleanLastContrastSpec();
	afx_msg void OnClickColorList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColorListChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDISPECTRUMVIEW_H__089BC3B4_2D4C_4D58_B941_7BFADA44A243__INCLUDED_)
