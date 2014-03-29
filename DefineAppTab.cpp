// DefineAppTab.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "DefineAppTab.h"
#include "CreateDefmaDlg.h"
#include "StdSampleDlg.h"
#include "CreateWorkZoneDlg.h"
#include "SelCollimatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefineAppTab dialog


CDefineAppTab::CDefineAppTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDefineAppTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefineAppTab)
	m_adjustRepeat = 0;
	m_adjustTimes = 0;
	m_mqMax = 0;
	m_index = 0;
	m_tag = _T("");
	m_current = 0;
	m_autoCurrent = FALSE;
	//}}AFX_DATA_INIT
	pWz=NULL;
	m_defMaName="";
	m_collimator=1;
}


void CDefineAppTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefineAppTab)
	DDX_Text(pDX, IDC_ADJUSTREPEAT, m_adjustRepeat);
	DDV_MinMaxInt(pDX, m_adjustRepeat, 1, 99);
	DDX_Text(pDX, IDC_ADJUSTTIME, m_adjustTimes);
	DDV_MinMaxInt(pDX, m_adjustTimes, 0, 999);
	DDX_Text(pDX, IDC_MQMAX, m_mqMax);
	DDX_Text(pDX, IDC_INDEX, m_index);
	DDX_Text(pDX, IDC_TAG, m_tag);
	DDX_Text(pDX, IDC_CURRENT, m_current);
	DDX_Check(pDX, IDC_AUTOCURRENT, m_autoCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefineAppTab, CDialog)
	//{{AFX_MSG_MAP(CDefineAppTab)
	ON_BN_CLICKED(IDC_DEFMA, OnEditDefMa)
	ON_BN_CLICKED(IDC_STD, OnEditStdSample)
	ON_BN_CLICKED(IDC_COLLSELECT, OnSelCollimator)
	ON_BN_CLICKED(IDC_AUTOCURRENT, OnAutoCurrent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineAppTab message handlers
BOOL CDefineAppTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/*
	CCreateWorkZoneDlg* pParent = (CCreateWorkZoneDlg*)GetParent()->GetParent()->GetParent();
	if(pParent->isNewFile())
	{
		(CButton*)GetDlgItem(IDC_STD)->EnableWindow(FALSE);
		(CButton*)GetDlgItem(IDC_DEFMA)->EnableWindow(FALSE);
	}*/

	
	return TRUE; 
}

void CDefineAppTab::LinkWz(WorkZoneDef *p,BOOL beNew)
{
	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	
	pWz=p;
	m_adjustRepeat=pWz->AdjustRepeat;
	m_adjustTimes=(pWz->AdjustTimes)*10;
	m_mqMax=pWz->MqMax;
	if(pWz->Current == 0){ //auto-calculate
		m_current = (int)(pApp->GetTubeCurrent(pWz->Collimator));
		m_autoCurrent = TRUE;
	}
	else{
		m_current = pWz->Current;
		m_autoCurrent = FALSE;
	}
	if(m_autoCurrent) GetDlgItem(IDC_CURRENT)->EnableWindow(FALSE);
	else GetDlgItem(IDC_CURRENT)->EnableWindow(TRUE);

	m_collimator = pWz->Collimator;
	CString text;
	switch(m_collimator){
	case 1:
		text.Format("%d=0.1",m_collimator);
		break;
	case 2:
		text.Format("%d=0.2",m_collimator);
		break;
	case 3:
		text.Format("%d=0.3",m_collimator);
		break;
	case 4:
		text.Format("%d=0.4",m_collimator);
		break;
	default:
		break;
	}
	((CButton *)GetDlgItem(IDC_COLLIMATOR))->SetWindowText(text);

	if(beNew) 
		((CButton *)GetDlgItem(IDC_STD))->EnableWindow(FALSE);

	UpdateData(FALSE); //Member Variables -> Interface
}


void CDefineAppTab::OnAutoCurrent() 
{
	m_autoCurrent = ((CButton*)GetDlgItem(IDC_AUTOCURRENT))->GetCheck();

	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	if(m_autoCurrent) {
		m_current = (int)(pApp->GetTubeCurrent(m_collimator));
		GetDlgItem(IDC_CURRENT)->EnableWindow(FALSE);
	}
	else 
		GetDlgItem(IDC_CURRENT)->EnableWindow(TRUE);

	UpdateData(FALSE); //Variable -> Interface
}

void CDefineAppTab::OnSelCollimator() 
{
	CString text;
	CSelCollimatorDlg dlg;
	dlg.LinkValue(FALSE,pWz);
	dlg.DoModal(); 

	m_collimator = dlg.collimator;
	switch(m_collimator){
	case 1:
		text.Format("%d=0.1",m_collimator);
		break;
	case 2:
		text.Format("%d=0.2",m_collimator);
		break;
	case 3:
		text.Format("%d=0.3",m_collimator);
		break;
	case 4:
		text.Format("%d=0.4",m_collimator);
		break;
	default:
		break;
	}
	((CButton *)GetDlgItem(IDC_COLLIMATOR))->SetWindowText(text);
	SaveToWz();
}


void CDefineAppTab::OnEditDefMa() 
{
	CCreateDefmaDlg dlg;
	dlg.LinkValue(FALSE,&(pWz->defMa));
	dlg.LinkValue(FALSE,pWz);
	dlg.DoModal(); 
}

void CDefineAppTab::OnEditStdSample() 
{
	CStdSampleDlg stdDlg;
	//stdDlg.LinkValue(TRUE,&(pWz->stdSample));
	stdDlg.LinkValue(TRUE,pWz);
	stdDlg.DoModal();
}

void CDefineAppTab::SaveToWz()
{
	UpdateData(TRUE); //Interface -> Variable
	pWz->AdjustTimes = m_adjustTimes/10;
	pWz->AdjustRepeat= m_adjustRepeat;
	pWz->MqMax = m_mqMax;
	pWz->Collimator = m_collimator;

	if(m_autoCurrent) pWz->Current = 0;
	else pWz->Current = m_current;
}



BOOL CDefineAppTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
    {
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
			pMsg->wParam=NULL ;
    }
	return CDialog::PreTranslateMessage(pMsg);
}



