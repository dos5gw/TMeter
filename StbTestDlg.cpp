// StbTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "StbTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;
/////////////////////////////////////////////////////////////////////////////
// CStbTestDlg dialog
CStbTestDlg::CStbTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStbTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStbTestDlg)
	m_TestTimes = 10;
	m_SampleTimes = 0;
	//}}AFX_DATA_INIT
	m_pReport = NULL;
	m_bTesting=FALSE;
}


void CStbTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStbTestDlg)
	DDX_Control(pDX, IDC_STBREPORT, m_ReportList);
	DDX_Control(pDX, IDC_STBPROGRESS, m_sampleProgress);
	DDX_Text(pDX, IDC_TESTTIME, m_TestTimes);
	DDV_MinMaxInt(pDX, m_TestTimes, 0, 999);
	DDX_Text(pDX, IDC_SAMPLETIME, m_SampleTimes);
	DDV_MinMaxInt(pDX, m_SampleTimes, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStbTestDlg, CDialog)
	//{{AFX_MSG_MAP(CStbTestDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STARTSTB, OnStartStb)
	ON_BN_CLICKED(IDC_STOPSTB, OnStopStb)
	ON_BN_CLICKED(IDC_CLEANLIST, OnCleanList)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SAVELIST, OnSaveReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStbTestDlg message handlers

BOOL CStbTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sampleProgress.SetRange(0,100);
	m_SampleTimes = g_SpectrumInfo.GetTotalSampleTimes();
	
	//m_pReport=new CGridCtrl();
	DWORD exstyle = m_ReportList.GetExtendedStyle();
	m_ReportList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | WS_EX_STATICEDGE );
	CRect rect;
	m_ReportList.GetClientRect(&rect);
    int nColInterval = rect.Width()/4;

	m_ReportList.InsertColumn(0, "测量时间", LVCFMT_LEFT, nColInterval);
	m_ReportList.InsertColumn(1, "计数率",LVCFMT_LEFT, nColInterval);
	m_ReportList.InsertColumn(2, "峰位",LVCFMT_LEFT, nColInterval);
	m_ReportList.InsertColumn(3, "分辨率",LVCFMT_LEFT, nColInterval);

	UpdateData(FALSE);

	SetTimer(0,100,NULL);
	return TRUE;
}

CStbTestDlg::~CStbTestDlg()
{
	//if(m_pReport) delete m_pReport;
}

void CStbTestDlg::OnTimer(UINT nIDEvent) 
{
	int itemCount;
	int finishedPercent;
	CString text;
	if(m_bTesting)
	{	
		if(g_pMeasureView->SpectrumDataReady(finishedPercent))
		{
			//add new Row
			lastTestTime = CTime::GetCurrentTime();

			itemCount = m_ReportList.GetItemCount();
			m_ReportList.InsertItem(itemCount,lastTestTime.Format("%Y-%m-%d %H:%M:%S"));

			text.Format("%5.1lf",g_SpectrumInfo.GetCps());
			m_ReportList.SetItemText(itemCount,1,text);

			text.Format("%4.2lf",g_SpectrumInfo.GetPeakDword());
			m_ReportList.SetItemText(itemCount,2,text);

			text.Format("%5.1lf%c",g_SpectrumInfo.GetPrecision(),'%');
			m_ReportList.SetItemText(itemCount,3,text);

			UpdateData(FALSE);

			if(m_TestTimes-- <= 1)
			{
				m_sampleProgress.SetPos(0);
				m_bTesting=FALSE;
				GetDlgItem(IDC_TESTTIME)->EnableWindow(TRUE);
				GetDlgItem(IDC_STARTSTB)->EnableWindow(TRUE);
				GetDlgItem(IDC_STOPSTB)->EnableWindow(FALSE);
				GetDlgItem(IDC_CLEANLIST)->EnableWindow(TRUE);
				GetDlgItem(IDC_SAVELIST)->EnableWindow(TRUE);
				GetDlgItem(IDC_LOADLIST)->EnableWindow(TRUE);
				g_pMeasureView->OnStopSample(); // m_bSpectrumReady be changed to FALSE;
				CTMeterApp* pApp = (CTMeterApp*)AfxGetApp(); // newly added @2012-06-14
				pApp->m_Instrument.SetOccupied(FALSE,"NONE");
			}
			else
			{
				g_pMeasureView->OnMeasure();
			}
		}
		m_sampleProgress.SetPos(finishedPercent);
	}
	CDialog::OnTimer(nIDEvent);
}

void CStbTestDlg::ExitStbTest()
{
	if(AfxMessageBox("Exit Stable Test?",MB_OKCANCEL)==IDOK)
	{
		DestroyWindow();
	}
}

void CStbTestDlg::OnCancel()
{
	ExitStbTest();
}

void CStbTestDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp(); 
	if(m_bTesting)
	{
		CString text;
		text.LoadString(IDS_INFO_OCUUPY_STBWARN);
		if(AfxMessageBox(text,MB_OKCANCEL)==IDOK)
		{
			CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
			pApp->m_Instrument.SetOccupied(FALSE,"NONE");
		}
		else
		{
			return;
		}
	}
	CDialog::OnClose();
}

void CStbTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(0);
}

void CStbTestDlg::PostNcDestroy() 
{
	delete this;
	CDialog::PostNcDestroy();
}

void CStbTestDlg::OnSaveReport() 
{
	// TODO: Add your control notification handler code here
	CStdioFile iniFile;
	CString specFileName,writeString;

	CTime tm = CTime::GetCurrentTime();
	CString tmString = tm.Format("%Y-%m-%d %H-%M-%S");
	specFileName= "Stable " + tmString + ".csv";
	if(iniFile.Open(specFileName,CFile::modeCreate|CFile::modeWrite))
	{
		writeString="测量时间,计数率,峰位,分辨率,\n";
		iniFile.WriteString(writeString);

		int itemCount = m_ReportList.GetItemCount();
		for(int i=0;i<itemCount;i++)
		{
			CString itemString;
			for(int j=0;j<4;j++)
			{
				itemString += m_ReportList.GetItemText(i,j) + ",";
			}
			itemString += "\n";
			iniFile.WriteString(itemString);
		}
		iniFile.Close();
	}
}

void CStbTestDlg::OnStartStb() 
{
	// TODO: Add your control notification handler code here
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	if(pApp->m_Instrument.CheckOccupied())
	{
		return;
	}
	UpdateData(TRUE);
	m_sampleProgress.SetPos(0);
	m_bTesting=TRUE;
	GetDlgItem(IDC_TESTTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_STARTSTB)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOPSTB)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLEANLIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVELIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOADLIST)->EnableWindow(FALSE);
	g_pMeasureView->OnMeasure();
	
	CString ocp;
	ocp.LoadString(IDS_INFO_OCUUPY_STBTEST);
	pApp->m_Instrument.SetOccupied(TRUE,ocp);
}

void CStbTestDlg::OnStopStb() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_sampleProgress.SetPos(0);
	m_bTesting=FALSE;
	GetDlgItem(IDC_TESTTIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_STARTSTB)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOPSTB)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLEANLIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVELIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADLIST)->EnableWindow(TRUE);
	g_pMeasureView->OnStopSample();
	
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	pApp->m_Instrument.SetOccupied(FALSE,"NONE");
}

void CStbTestDlg::OnCleanList() 
{
	// TODO: Add your control notification handler code here
	m_ReportList.DeleteAllItems();
}

