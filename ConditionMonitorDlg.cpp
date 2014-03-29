// ConditionMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "ConditionMonitorDlg.h"
#include "SpectrumInfo.h"
#include "MdiMeasureView.h"

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConditionMonitorDlg dialog


CConditionMonitorDlg::CConditionMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConditionMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConditionMonitorDlg)
	SampleTime = 0;
	WorkHV = 0;
	WorkMA = 0;
	H2500Vol = 0;
	UTemp = 0;
	LTemp = 0.0f;
	BaseLine = 0;
	TopLine = 0;
	//}}AFX_DATA_INIT
	p_Instrument=NULL;
}


void CConditionMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConditionMonitorDlg)
	DDX_Control(pDX, IDC_TEMP2_STATIC, currentTemp2);
	DDX_Control(pDX, IDC_TEMP1_STATIC, currentTemp1);
	DDX_Control(pDX, IDC_STD_STATIC, currentStdVol);
	DDX_Control(pDX, IDC_MA_STATIC, currentMA);
	DDX_Control(pDX, IDC_HV_STATIC, currentHV);
	DDX_Control(pDX, IDC_H2500_STATIC, currentH2500Vol);
	DDX_Text(pDX, IDC_SETTIME, SampleTime);
	DDV_MinMaxInt(pDX, SampleTime, 0, 999);
	DDX_Text(pDX, IDC_SETHV, WorkHV);
	DDV_MinMaxFloat(pDX, WorkHV, 0.f, 55.f);
	DDX_Text(pDX, IDC_SETMA, WorkMA);
	DDV_MinMaxFloat(pDX, WorkMA, 0.f, 2999.f);
	DDX_Text(pDX, IDC_SET2500, H2500Vol);
	DDV_MinMaxFloat(pDX, H2500Vol, 0.f, 2599.f);
	DDX_Text(pDX, IDC_UTEMP, UTemp);
	DDV_MinMaxFloat(pDX, UTemp, 0.f, 99.f);
	DDX_Text(pDX, IDC_LTEMP, LTemp);
	DDV_MinMaxFloat(pDX, LTemp, 0.f, 99.f);
	DDX_Text(pDX, IDC_SETBASELINE, BaseLine);
	DDV_MinMaxInt(pDX, BaseLine, 0, 255);
	DDX_Text(pDX, IDC_SETTOPLINE, TopLine);
	DDV_MinMaxInt(pDX, TopLine, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConditionMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CConditionMonitorDlg)
	ON_BN_CLICKED(IDC_OPENTUBE, OnOpenTube)
	ON_BN_CLICKED(IDC_CLOSETUBE, OnCloseTube)
	ON_BN_CLICKED(IDC_OPENSHUT, OnOpenShut)
	ON_BN_CLICKED(IDC_CLOSESHUT, OnCloseShut)
	ON_BN_CLICKED(IDC_OPENLAMP, OnOpenLamp)
	ON_BN_CLICKED(IDC_CLOSELAMP, OnCloseLamp)
	ON_BN_CLICKED(IDC_SETPARAM, OnBnClickedSetparam)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionMonitorDlg message handlers

void CConditionMonitorDlg::OnOpenTube() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->WarmUpXTube();
}

void CConditionMonitorDlg::OnCloseTube() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->CoolDownXTube();
}

void CConditionMonitorDlg::OnOpenShut() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->OpenBeamShut();
}

void CConditionMonitorDlg::OnCloseShut() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->CloseBeamShut();
}

void CConditionMonitorDlg::OnOpenLamp() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->OpenLampLight();
}

void CConditionMonitorDlg::OnCloseLamp() 
{
	// TODO: Add your control notification handler code here
	g_pMeasureView->CloseLampLight();
}

void CConditionMonitorDlg::OnBnClickedSetparam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_SpectrumInfo.SetTotalSampleTimes(SampleTime);
	p_Instrument->m_WorkingHv=WorkHV;
	p_Instrument->m_WorkingMa=WorkMA;
	p_Instrument->m_H2500WorkingHv=H2500Vol;
	p_Instrument->m_UThresholdTemp=UTemp;
	p_Instrument->m_LThresholdTemp=LTemp;

	if(TopLine>BaseLine)
	{
		g_SpectrumInfo.m_topline=TopLine;
		g_SpectrumInfo.m_baseline=BaseLine;
	}
	else
	{
		AfxMessageBox("Exception: TopLine<=BaseLine");
		//return;
	}
	UpdateData(FALSE);
	g_pMeasureView->SendSystemParameter();

	//CString text;
	//text.LoadString(IDS_SYSPARAMHASBEENSENT);
	//AfxMessageBox(text);
	//AfxMessageBox("System Parameter has been Sent");
}

void CConditionMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString text;
	
	text.Format("%6.2f",p_Instrument->m_CurrentHv);
	currentHV.SetWindowText(text);
	
	text.Format("%6.2f",p_Instrument->m_CurrentMa);
	currentMA.SetWindowText(text);
	
	text.Format("%6.2f",p_Instrument->m_H2500CurrentHv);
	currentH2500Vol.SetWindowText(text);
	
	text.Format("%6.2f",p_Instrument->m_CurrentHtStdVol);
	currentStdVol.SetWindowText(text);
	CDialog::OnTimer(nIDEvent);
}

BOOL CConditionMonitorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	p_Instrument=&(((CTMeterApp*)AfxGetApp())->m_Instrument);
	if(!p_Instrument) return FALSE;
	SampleTime = g_SpectrumInfo.GetTotalSampleTimes();
	WorkHV=p_Instrument->m_WorkingHv;
	WorkMA=p_Instrument->m_WorkingMa;
	H2500Vol=p_Instrument->m_H2500WorkingHv;
	UTemp=p_Instrument->m_UThresholdTemp;
	LTemp=p_Instrument->m_LThresholdTemp;

	//TopLine=g_SpectrumInfo.m_topline;
	//BaseLine=g_SpectrumInfo.m_baseline;
	TopLine =g_pSpectrumView->m_SpectrumDisplay.m_topLine;
	BaseLine=g_pSpectrumView->m_SpectrumDisplay.m_baseLine;
	UpdateData(FALSE);
	SetTimer(0,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CConditionMonitorDlg::~CConditionMonitorDlg()
{
	// release resource here
}

void CConditionMonitorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(!p_Instrument->WriteSettingsToFiles())
		AfxMessageBox("–¥»ÎXrfSettings.ini ß∞‹");
	KillTimer(0);
}
