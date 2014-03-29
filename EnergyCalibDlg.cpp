// EnergyCalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "EnergyCalibDlg.h"
//#include "SpectrumInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EnergyCalibDlg dialog


EnergyCalibDlg::EnergyCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EnergyCalibDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EnergyCalibDlg)
	m_LastCalibB = 0.0f;
	m_LastCalibK = 0.0f;
	m_LastH2500 = 0.0f;
	m_CurrH2500 = 0.0f;
	m_CurrPeakOfAg = 0.0f;
	m_CalibTime = 0;
	m_Scale = 0.0f;
	//}}AFX_DATA_INIT
	m_ParamBeSended = FALSE; 
	m_Calibrating = FALSE;
	m_CalibFinished = FALSE;
	MaxCalibTime = 10;
}


void EnergyCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EnergyCalibDlg)
	DDX_Text(pDX, IDC_LAST_B, m_LastCalibB);
	DDX_Text(pDX, IDC_LAST_K, m_LastCalibK);
	DDX_Text(pDX, IDC_LAST_H2500, m_LastH2500);
	DDX_Text(pDX, IDC_CURR_H2500, m_CurrH2500);
	DDX_Text(pDX, IDC_CURR_PEAK, m_CurrPeakOfAg);
	DDX_Text(pDX, IDC_CALIB_TIME, m_CalibTime);
	DDX_Text(pDX, IDC_SCALE, m_Scale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EnergyCalibDlg, CDialog)
	//{{AFX_MSG_MAP(EnergyCalibDlg)
	ON_BN_CLICKED(IDC_START, OnStartCalib)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EnergyCalibDlg message handlers

BOOL EnergyCalibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	m_LastH2500  = pApp->m_Instrument.m_H2500WorkingHv;
	m_CurrH2500  = pApp->m_Instrument.m_H2500CurrentHv;
	m_LastCalibB = pApp->CalibB;
	m_LastCalibK = pApp->CalibK;
	m_Scale = pApp->m_Instrument.m_H2500Step;

	// Set Temp Value for the first Calibrating
	m_CalibElem = Ag;
	m_TempH2500 = m_LastH2500;
	m_TempAgPeak = 170.f; 
	

	GetDlgItem(IDC_CALIBTIP)->SetWindowText("请放入Ag标样");
	UpdateData(FALSE);
	SetTimer(0,100,NULL);
	return TRUE;  
}

void EnergyCalibDlg::OnTimer(UINT nIDEvent) 
{
	int finishedPercent;
	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();

	if(m_Calibrating) // adjust peak of Ag to 170
	{
		if(g_pMeasureView->SpectrumDataReady(finishedPercent))
		{
			if(m_CalibElem == Ag)
			{
				m_CurrH2500 = pApp->m_Instrument.m_H2500WorkingHv;
				m_CurrPeakOfAg = (float)(g_SpectrumInfo.GetPeakDword());
				UpdateData(FALSE); // update ui

				if((m_CalibTime>MaxCalibTime) || ((170.f-m_CurrPeakOfAg)<0.5 && (170.f-m_CurrPeakOfAg)>-0.5))
				{
					// force interrupt calibrating Ag
					m_CalibElem = Cu;
					g_SpectrumInfo.ResetSpectrum(); // clean SpectrumDataReady() status
					GetDlgItem(IDC_START)->EnableWindow(TRUE);
					GetDlgItem(IDC_CALIBTIP)->SetWindowText("请放入Cu标样");
				}
				else
				{
					// Avoid Run SendSystemParameter() More Then Once
					if(!m_ParamBeSended) 
					{
						//pApp->m_Instrument.m_H2500WorkingHv += m_Scale*(170.f-m_CurrPeakOfAg);
						pApp->m_Instrument.m_H2500WorkingHv += GetVolStep(170.f - m_CurrPeakOfAg);
						g_pMeasureView->SendSystemParameter();
						m_ParamBeSended = TRUE; 
						//ActionStart = CurrentTimeTick();
					}
					//else if(CurrentTimeTick() - ActionStart > MAX_WAIT_SECOND) CTMeterErrorLog("H2500 Changed Timeout");

					//make sure vol already reach target, and measure again
					float differ = pApp->m_Instrument.m_H2500WorkingHv - pApp->m_Instrument.m_H2500CurrentHv;
					if((differ<1.0) && (differ>-1.0))
					{
						//m_TempAgPeak = m_CurrPeakOfAg;
						//m_TempH2500 = m_CurrH2500;
						m_CalibTime++;
						m_ParamBeSended = FALSE; 
						g_pMeasureView->OnMeasure();  //m_bSpectrumReady be changed to FALSE
						CString text;
						text.Format("正在测试Ag标样, 第%d次...",m_CalibTime);
						GetDlgItem(IDC_CALIBTIP)->SetWindowText(text);
					}
				}
			} 
			else if(m_CalibElem == Cu)
			{
				// Calculate CalibK / CalibB, And Save Value To EnergyCalib.ini
				float PeakOfCu = (float)(g_SpectrumInfo.GetPeakDword());
				pApp->m_energyCalib.SetPeakOfCu(PeakOfCu);
				pApp->CalcEnergyCalib(); 
				m_Calibrating = FALSE;
				KillTimer(0);
				GetDlgItem(IDC_CALIBTIP)->SetWindowText("能量刻度校准完成");
			}
		}
		// Updating UI
	}
	
	CDialog::OnTimer(nIDEvent);
}

float EnergyCalibDlg::GetVolStep(float chs)
{
	return m_Scale*chs;
}

void EnergyCalibDlg::OnStartCalib() 
{
	if(m_CalibTime==0) // start Ag Calibrating ... 
	{
		m_Calibrating = TRUE;
		g_pMeasureView->OnMeasure();
		m_CalibElem = Ag;
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
	}
	else // start sampling once , and record peak of Cu
	{
		g_pMeasureView->OnMeasure();
		m_CalibElem = Cu;
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
	}
}

void EnergyCalibDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_CalibFinished)
	{
		CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
		// Set New Value of H2500
		pApp->m_Instrument.m_H2500WorkingHv = m_CurrH2500;
		// Restore Sample Time of Current Opend Work-Zone
		if(pApp->m_pWorkZone)
			g_SpectrumInfo.SetTotalSampleTimes(pApp->m_pWorkZone->SampleTimes);

		g_pMeasureView->SendSystemParameter();

		// Save to File, H2500 Vol 
		pApp->m_Instrument.WriteSettingsToFiles();
	}
	
	CDialog::OnOK();
}

void EnergyCalibDlg::OnCancel() 
{
	// DO SOMETHING
	CDialog::OnCancel();
}



