// SetMeasHeadParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "SetMeasHeadParamDlg.h"
#include "XrfInstrument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetMeasHeadParamDlg dialog


CSetMeasHeadParamDlg::CSetMeasHeadParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetMeasHeadParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetMeasHeadParamDlg)
	m_HV30 = 0.0f;
	m_HV40 = 0.0f;
	m_HV50 = 0.0f;
	m_FilterThicknessAl = 0.0f;
	m_FilterThicknessNi = 0.0f;
	m_HV2500 = 0.0f;
	m_XrayTargetTakeOffAngle = 0.0f;
	m_TargetAtomicNumber = 74;
	m_XrayIncidentAngle = 0.0f;
	m_TubeWindowThickness = 0.0f;
	m_XrayEmergentAngle = 0.0f;
	m_CalibK = 0.0f;
	m_CalibB = 0.0f;
	m_CalibC1 = 0.0f;
	m_CalibC2 = 0.0f;
	m_HI30 = 0.0f;
	m_HI40 = 0.0f;
	m_HI50 = 0.0f;
	m_HV2500Step = 0.0f;
	//}}AFX_DATA_INIT
}


void CSetMeasHeadParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetMeasHeadParamDlg)
	DDX_Text(pDX, IDC_HV30, m_HV30);
	DDX_Text(pDX, IDC_HV40, m_HV40);
	DDX_Text(pDX, IDC_HV50, m_HV50);
	DDX_Text(pDX, IDC_FLTAL, m_FilterThicknessAl);
	DDX_Text(pDX, IDC_FLTNI, m_FilterThicknessNi);
	DDX_Text(pDX, IDC_H2500, m_HV2500);
	DDX_Text(pDX, IDC_TAKEOFF, m_XrayTargetTakeOffAngle);
	DDX_Text(pDX, IDC_TARGET, m_TargetAtomicNumber);
	DDX_Text(pDX, IDC_INCIDENT, m_XrayIncidentAngle);
	DDX_Text(pDX, IDC_BETHICK, m_TubeWindowThickness);
	DDX_Text(pDX, IDC_EMGERMENT, m_XrayEmergentAngle);
	DDX_Text(pDX, IDC_ENGK, m_CalibK);
	DDX_Text(pDX, IDC_ENGB, m_CalibB);
	DDX_Text(pDX, IDC_ENGC1, m_CalibC1);
	DDX_Text(pDX, IDC_ENGC2, m_CalibC2);
	DDX_Text(pDX, IDC_HI30, m_HI30);
	DDX_Text(pDX, IDC_HI40, m_HI40);
	DDX_Text(pDX, IDC_HI50, m_HI50);
	DDX_Text(pDX, IDC_H2500STEP, m_HV2500Step);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetMeasHeadParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetMeasHeadParamDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetMeasHeadParamDlg message handlers

BOOL CSetMeasHeadParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	CXrfInstrument *pIns = &(pApp->m_Instrument);

	m_HV30 = pIns->Hv30HardCode;
	m_HV40 = pIns->Hv40HardCode;
	m_HV50 = pIns->Hv50HardCode;
	m_FilterThicknessAl = pIns->FilterAlThick;
	m_FilterThicknessNi = pIns->FilterNiThick;
	m_HV2500 = pIns->m_H2500WorkingHv;
	m_HV2500Step = pIns->m_H2500Step;
	m_XrayTargetTakeOffAngle = pIns->TargetTakeOffAngle;
	m_TargetAtomicNumber = pIns->TargetAtomicNumber;
	m_XrayIncidentAngle = pIns->IncidentAngle;
	m_XrayEmergentAngle = pIns->EmergentAngle;
	m_TubeWindowThickness = pIns->WindowThickness;
	
	m_CalibK = pApp->CalibK;
	m_CalibB = pApp->CalibB;
	UpdateData(FALSE); //Variable -> Interface

	return TRUE;  
}


void CSetMeasHeadParamDlg::OnOK() 
{
	UpdateData(TRUE); //Interface -> Variable

	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	CXrfInstrument *pIns = &(pApp->m_Instrument);

	pIns->Hv30HardCode = m_HV30;
	pIns->Hv40HardCode = m_HV40;
	pIns->Hv50HardCode = m_HV50;
	pIns->FilterAlThick = m_FilterThicknessAl;
	pIns->FilterNiThick = m_FilterThicknessNi;
	pIns->m_H2500WorkingHv = m_HV2500;
	pIns->m_H2500Step = m_HV2500Step;
	pIns->TargetTakeOffAngle = m_XrayTargetTakeOffAngle;
	pIns->TargetAtomicNumber = m_TargetAtomicNumber;
	pIns->IncidentAngle = m_XrayIncidentAngle;
	pIns->EmergentAngle = m_XrayEmergentAngle;
	pIns->WindowThickness = m_TubeWindowThickness;
	pApp->CalibK = m_CalibK;
	pApp->CalibB = m_CalibB;

	if(pApp->m_pWorkZone != NULL) // @2012-09-03
		pApp->m_pWorkZone->FpCalibrate();

	//SAVE TO FILE
	pIns->WriteSettingsToFiles();

	//SEND TO DCT
	if(pApp->m_pWorkZone != NULL)
	{
		//Change HV
		pIns->m_WorkingHv = pApp->GetTubeVoltage(pApp->m_pWorkZone->defMa.m_hv);

		//Change HI
		if(pApp->m_pWorkZone->Current == 0)
			pIns->m_WorkingMa = pApp->GetTubeCurrent(pApp->m_pWorkZone->Collimator);
		else
			pIns->m_WorkingMa = (float)(pApp->m_pWorkZone->Current);

		//Send
		g_pMeasureView->SendSystemParameter();
	}
	
	CDialog::OnOK();
}

void CSetMeasHeadParamDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


