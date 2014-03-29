// AdjustZoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "AdjustZoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;
/////////////////////////////////////////////////////////////////////////////
// CAdjustZoneDlg dialog


CAdjustZoneDlg::CAdjustZoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustZoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdjustZoneDlg)
	m_baseLine = 0;
	m_topLine = 0;
	//}}AFX_DATA_INIT
	m_bInitialized=FALSE;
}


void CAdjustZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustZoneDlg)
	DDX_Control(pDX, IDC_TOPSPIN, m_topSpin);
	DDX_Control(pDX, IDC_BASESPIN, m_baseSpin);
	DDX_Text(pDX, IDC_BASE, m_baseLine);
	DDV_MinMaxInt(pDX, m_baseLine, 0, 255);
	DDX_Text(pDX, IDC_TOP, m_topLine);
	DDV_MinMaxInt(pDX, m_topLine, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjustZoneDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustZoneDlg)
	ON_EN_CHANGE(IDC_BASE, OnChangeBase)
	ON_EN_CHANGE(IDC_TOP, OnChangeTop)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustZoneDlg message handlers

BOOL CAdjustZoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_topSpin.SetRange(0,255);
    m_topSpin.SetBuddy(GetDlgItem(IDC_TOP));
	m_baseSpin.SetRange(0,255);
    m_baseSpin.SetBuddy(GetDlgItem(IDC_BASE));

	//m_baseLine = g_SpectrumInfo.m_baseline;
	//m_topLine  = g_SpectrumInfo.m_topline;
	m_baseLine = g_pSpectrumView->m_SpectrumDisplay.m_baseLine;
	m_topLine = g_pSpectrumView->m_SpectrumDisplay.m_topLine;
	UpdateData(FALSE); // Member Variables -> Interface
	g_pSpectrumView->m_SpectrumDisplay.SetZoneMode(TRUE);
	g_pSpectrumView->m_SpectrumDisplay.Update();
	m_bInitialized = TRUE;
	return TRUE;  
}

void CAdjustZoneDlg::OnChangeBase() 
{
	if(!m_bInitialized) return;
	m_baseLine = m_baseSpin.GetPos();
	if(m_baseLine>m_topLine) return;
	g_pSpectrumView->m_SpectrumDisplay.m_baseLine = m_baseLine; //g_SpectrumInfo.m_baseline = m_baseLine;
	g_pSpectrumView->m_SpectrumDisplay.Update();
}

void CAdjustZoneDlg::OnChangeTop() 
{
	if(!m_bInitialized) return;
	m_topLine = m_topSpin.GetPos();
	if(m_baseLine>m_topLine) return;
	g_pSpectrumView->m_SpectrumDisplay.m_topLine = m_topLine; //g_SpectrumInfo.m_topline = m_topLine;
	g_pSpectrumView->m_SpectrumDisplay.Update();
}

void CAdjustZoneDlg::OnDestroy() 
{
	g_pSpectrumView->m_SpectrumDisplay.SetZoneMode(FALSE);
	g_pSpectrumView->m_SpectrumDisplay.Update();
	CDialog::OnDestroy();
}
