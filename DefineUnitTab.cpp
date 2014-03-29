// DefineUnitTab.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "DefineUnitTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefineUnitTab dialog


CDefineUnitTab::CDefineUnitTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDefineUnitTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefineUnitTab)
	m_arraySize = 0;
	//}}AFX_DATA_INIT
	pWz=NULL;
}


void CDefineUnitTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefineUnitTab)
	DDX_Text(pDX, IDC_ARRAYSIZE, m_arraySize);
	DDV_MinMaxInt(pDX, m_arraySize, 1, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefineUnitTab, CDialog)
	//{{AFX_MSG_MAP(CDefineUnitTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineUnitTab message handlers

BOOL CDefineUnitTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	return TRUE;  
}

void CDefineUnitTab::LinkWz(WorkZoneDef *p)
{
	pWz=p;

	if(pWz->ArrayVariable) ((CButton*)GetDlgItem(IDC_VARIABLE0))->SetCheck(TRUE);
	else ((CButton*)GetDlgItem(IDC_VARIABLE1))->SetCheck(TRUE);
	
	if(pWz->ReportStyle==0) ((CButton*)GetDlgItem(IDC_STYLED0))->SetCheck(TRUE);
	else if(pWz->ReportStyle==1) ((CButton*)GetDlgItem(IDC_STYLED1))->SetCheck(TRUE);
	else if(pWz->ReportStyle==2) ((CButton*)GetDlgItem(IDC_STYLED2))->SetCheck(TRUE);

	m_arraySize = pWz->ArraySize;
	UpdateData(FALSE); //Member Variables -> Interface
}

void CDefineUnitTab::SaveToWz()
{
	UpdateData(TRUE); //Interface -> Variable
	pWz->ArraySize = m_arraySize;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_VARIABLE0))->GetCheck())
		pWz->ArrayVariable = TRUE;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_VARIABLE1))->GetCheck())
		pWz->ArrayVariable = FALSE;
}

BOOL CDefineUnitTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
    {
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
			pMsg->wParam=NULL ;
    }
	return CDialog::PreTranslateMessage(pMsg);
}
