// SelCollimatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "SelCollimatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelCollimatorDlg dialog


CSelCollimatorDlg::CSelCollimatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelCollimatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelCollimatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pWz = NULL;
	collimator = 1;
}


void CSelCollimatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelCollimatorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelCollimatorDlg, CDialog)
	//{{AFX_MSG_MAP(CSelCollimatorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelCollimatorDlg message handlers

BOOL CSelCollimatorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!pWz) return FALSE; 
	
	collimator = pWz->Collimator; 
	switch(collimator){
	case 1:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		break;
	case 3:
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
		break;
	case 4:
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
		break;
	default:
		break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelCollimatorDlg::LinkValue(BOOL beNew,WorkZoneDef* ptr)
{
	pWz = ptr;
}

void CSelCollimatorDlg::OnOK() 
{
	UpdateData(TRUE); //Interface -> Variable
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		collimator=1;
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		collimator=2;
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		collimator=3;
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		collimator=4;
	}
	
	CDialog::OnOK();
}

void CSelCollimatorDlg::OnCancel() 
{
	//do nothing 
	
	CDialog::OnCancel();
}
