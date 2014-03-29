// TMeter.h : main header file for the TMETER application
//

#if !defined(AFX_TMETER_H__5D48F37B_57E2_4CA7_9CEE_2634BD3F300F__INCLUDED_)
#define AFX_TMETER_H__5D48F37B_57E2_4CA7_9CEE_2634BD3F300F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "XrfInstrument.h"
#include "MdiSpectrumView.h"
#include "MdiMeasureView.h"
#include "ClassicalElement.h"
#include "TMeterTypeDefine.h"
#include "FpCalculator.h"
#include "IntensityCalculator.h"
#include "TMeterLog.h"

extern CMdiMeasureView*  g_pMeasureView;
extern CMdiSpectrumView* g_pSpectrumView;
BOOL IsSamplingFinished(int& percent);

/////////////////////////////////////////////////////////////////////////////
// CTMeterApp:
// See TMeter.cpp for the implementation of this class
//
class EnergyCalib
{
public:
	EnergyCalib(){
		AgEnergy=22.10f;
		AgChannel=170.00f;
		CuEnergy=8.04f;
		CuChannel=80.00f;
	}
	~EnergyCalib(){}
	float AgEnergy,AgChannel;
	float CuEnergy,CuChannel;
	void SetPeakOfCu(float peak){CuChannel = peak;};
	void CalcEnergyCalib(float &k,float &b); //
	BOOL LoadEnergyCalib();
	void SaveEnergyCalib(float k,float b);
	
};

class CTMeterApp : public CWinApp
{
public:
	CTMeterApp();
	~CTMeterApp();
	CMultiDocTemplate* m_pMeasureTemplate;
	CMultiDocTemplate* m_pSpectrumTemplate;
	CMultiDocTemplate* m_pCalibTemplate;

	CXrfInstrument m_Instrument;   /* Hardware Parameter */
	PeriodicTable m_PeriodicTable; /* PeriodicTable.xml  */
	FpCalculator m_FpCalculator;   /* FP-Pro Dynamic Library */
	IntensityCalculator m_IntensityCalculator; /* Intensity Calculate Dynamic Library */

	WorkZoneDef *m_pWorkZone;  /* Current Opened Work-Zone */
	void OpenWorkZone(int id);
	void LoadWorkZonePureElem();
	void ReloadPureElemList(WorkZoneDef*);

	void CalcEnergyCalib();  
	EnergyCalib m_energyCalib;
	float CalibK;
	float CalibB;

	CString m_AppWorkDirectory;

	inline CString GetApplicationDir()
	{
		return m_AppWorkDirectory;
	}

	// PeriodicTable CTMeterApp::m_PeriodicTable
	double GetEnergyByChannel(double ch);
	double GetChannelByEnergy(double eng);
	void GetEnergyByEleName(CString ele,double &ka,double &kb);
	void GetEnergyByAtomicNum(int num,CString &name,double &ka,double &kb,double &kaChannel,double &kbChannel); 


	// Get hardware parameter
	short GetTargetAtomicNumber();
	float GetTargetTakeOffAngle();
	float GetTubeVoltage(int index); // index=30,40,50
	float GetTubeCurrent(int coll);  // coll=1,2,3,4
	float GetWindowThickness();
	float GetIncidentAngle();
	float GetEmergentAngle();
	short GetFilterAtomicNumber(int index); // 0:Ni, 1:--, 2:Al
	float GetFilterThickness(int index);

	void TryCloseAllDoc();
	BOOL HasDocUnclosed();

	CTMeterLog AppLog;
	void WriteSwitchWz2Log();
	void WriteCalibWz2Log();
	void WriteMeasureWz2Log();
	void WriteCalcIntensity2Log();

private:
	HANDLE m_hOneInstance;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMeterApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTMeterApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMETER_H__5D48F37B_57E2_4CA7_9CEE_2634BD3F300F__INCLUDED_)
