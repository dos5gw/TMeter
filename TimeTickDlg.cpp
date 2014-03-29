// TimeTickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "TimeTickDlg.h"
#include "XrfInstrument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

/////////////////////////////////////////////////////////////////////////////
// CTimeTickDlg dialog


CTimeTickDlg::CTimeTickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeTickDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeTickDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_SecondTick=10;
	m_bSampleFinished=FALSE;
	m_bSampleStarted=FALSE;
}


void CTimeTickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeTickDlg)
	DDX_Control(pDX, IDC_STATUS, m_StatusEx);
	DDX_Control(pDX, IDC_SEC, m_SecondsEx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeTickDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeTickDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeTickDlg message handlers
BOOL CTimeTickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// get Total Seconds from CTMeterApp::m_Instrument
	//m_SecondTick = ((CTMeterApp*)AfxGetApp())->m_Instrument.m_totalTimes;
	m_SecondTick = 10 * g_SpectrumInfo.GetTotalSampleTimes();
	m_SecondString.Format("%d",m_SecondTick);
	m_SecondsEx.Use3D(FALSE); 
	m_SecondsEx.align=2; //center align
	m_SecondsEx.SetWindowText(m_SecondString);
	m_StatusEx.SetWindowText("等待高压电源...");
	SetTimer(0,100,NULL);
	return TRUE; 
}

void CTimeTickDlg::OnOK() 
{
	// press 'Enter'
	QuitMeasure();
}

void CTimeTickDlg::OnCancel() 
{
	// Press 'ESC'
	QuitMeasure();
}

void CTimeTickDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int timerCount=0;
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	if(pApp->m_Instrument.GetInstrumentStatus() == Sampling)
	{
		m_bSampleFinished = FALSE;
		m_bSampleStarted = TRUE;
	}

	if(m_bSampleStarted && !m_bSampleFinished)
	{
		if((++timerCount)==10)
		{
			timerCount=0;
			m_SecondTick--;
			if(m_SecondTick<0) m_SecondTick=0;
			
			m_SecondString.Format("%d",m_SecondTick);
			m_SecondsEx.SetWindowText(m_SecondString);
			if(m_SecondTick==0) m_StatusEx.SetWindowText("等待测量结束...");
			else m_StatusEx.SetWindowText("测量中...");
		}
	}
	if((m_SecondTick==0) && (g_SpectrumInfo.m_isFinished==TRUE))
	{
		//DestroyWindow();
		//m_bSampleFinished = TRUE;
		//m_bSampleStarted = FALSE;
		//m_SecondsEx.SetWindowText("0");
		//m_StatusEx.SetWindowText("测量结束!");
		KillTimer(0);
		DestroyWindow();
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CTimeTickDlg::QuitMeasure()
{
	if(AfxMessageBox("确定终止测量?",MB_OKCANCEL)==IDOK)
	{
		g_pMeasureView->OnStopSample();
		DestroyWindow();
	}
}

void CTimeTickDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(0);
}

void CTimeTickDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}
