#if !defined(AFX_MDIMEASUREVIEW_H__AA9C668F_4DCA_465B_835A_B616C3326294__INCLUDED_)
#define AFX_MDIMEASUREVIEW_H__AA9C668F_4DCA_465B_835A_B616C3326294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiMeasureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Btnst.h"
#include "ComWnd.h"
#include "OneTask.h"
#include "XrfInstrument.h"
//#include "OneResult.h"
#include "SpectrumDisplay.h"
#include "MyTextButton.h"

//#include "IntensityCalculator.h"

#include "WorkZoneDef.h"
#include "dvrsdk.h"

#define MVCARD_INDEX 0

void ConvertCmdBytes(COneCommand* pCmd);
void ConvertRecvBytes(COneCommand* pCmd);



class CMdiMeasureView : public CFormView
{
protected:
	CMdiMeasureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiMeasureView)

public:
	WorkZoneDef *m_pWorkZone; //added @ 2012-07-05, This is a pointer copy of CTMeter::m_pWorkZone, point to current work-zone for measurement
	//COneResult *m_pResult;
	BOOL m_beCalibMode;

	void PutToSpectrumInfo(CByteArray* buf);
	void ReadSystemParameter(CByteArray* buf);
	void SendSystemParameter();

	void OpenSpectrumFiles();
	void SaveSpectrumFiles();
	
	void WarmUpXTube();
	void CoolDownXTube();
	void OpenBeamShut();
	void CloseBeamShut();
	void OpenLampLight();
	void CloseLampLight();
	void OnMeasure();

	void CalibrateWorkZone(); // calibrate current work-zone
	void CalcPureIntensity(); // calculate current spectrum (g_SpectrumInfo)
	void CalculateQuantify(); // calculate current spectrum by current work-zone

	BOOL SpectrumDataReady(int &percent); //m_finishedPercent & m_bSpectrumReady
	void ResetDataReady();
	void OpenLastWorkZone();

	int m_ResultIndex;
	void UpdateArrayList();
	void AddNewResultToArray();
	void UpdateStatisticList();
	void UpdateWzTile();
	
	void ChangeMeasureCondition(WorkZoneDef*);

	BOOL bSystemParamSended;
	BOOL m_bAutoSaveSpec;
	BOOL m_bAutoSortResult;
	BOOL bBackSpectrumExist; //@2012-07-09, if true, new open spectrum should be added to CMdiSpectrumView::m_SpectrumDisplay::m_ContrastSpectrum array

private:
	BOOL m_bInitialized;
	int m_finishedPercent;
	BOOL m_bSpectrumReady;
	BOOL m_bResultAnalysis;
	CComWnd m_SerialPortInstance;
	//int m_SerialPortNum;
	//DWORD m_SerialPortBaud;
	
	CCommandTrack m_CommandTrack;
	COneCommand* m_pCurCommand;

	// Hardware
	CXrfInstrument* m_pInstrument;


	int m_nWinWidth;
	int m_nWinHeight;
// Form Data
public:
	//{{AFX_DATA(CMdiMeasureView)
	enum { IDD = IDD_MDIMEASUREVIEW_ZH };
	CButtonST	m_BtnSwitchWorkZone;
	CButtonST	m_BtnOpenWorkZone;
	CListCtrl	m_StatisList;
	CListCtrl	m_ArrayList;
	CMyTextButton m_BtnWorkZoneTitle;
	CButtonST	m_BtnStop;
	CButtonST	m_BtnStart;
	CStatic	m_VideoDisplay;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiMeasureView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMdiMeasureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMdiMeasureView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStartSample();
	afx_msg void OnStopSample();
	afx_msg void OnDestroy();
	afx_msg void CreateDefMa();
	afx_msg void CreateDefThick();
	afx_msg void TestDefMa();
	afx_msg void OnSwitchWz();
	afx_msg void OnOpenVideo();
	afx_msg void OnCloseVideo();
	afx_msg void OnEditWz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIMEASUREVIEW_H__AA9C668F_4DCA_465B_835A_B616C3326294__INCLUDED_)
