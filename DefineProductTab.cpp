// DefineProductTab.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "DefineProductTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefineProductTab dialog


CDefineProductTab::CDefineProductTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDefineProductTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefineProductTab)
	m_tag = _T("");
	m_measureTime = _T("");
	m_modifyTime = _T("");
	m_correctTime = _T("");
	m_index = 0;
	m_repeat = 0;
	m_sampleTimes = 0;
	//}}AFX_DATA_INIT
	pWz=NULL;
	m_thickUnit=0;
	m_contentUnit=0;
}


void CDefineProductTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefineProductTab)
	DDX_Control(pDX, IDC_PRECISION, m_percisionEx);
	DDX_Text(pDX, IDC_TAG, m_tag);
	DDV_MaxChars(pDX, m_tag, 64);
	DDX_Text(pDX, IDC_MEASURETIME, m_measureTime);
	DDX_Text(pDX, IDC_MODIFYTIME, m_modifyTime);
	DDX_Text(pDX, IDC_CORRECTTIME, m_correctTime);
	DDX_Text(pDX, IDC_INDEX, m_index);
	DDX_Text(pDX, IDC_REPEAT, m_repeat);
	DDV_MinMaxInt(pDX, m_repeat, 1, 99);
	DDX_Text(pDX, IDC_SAMPLETIME, m_sampleTimes);
	DDV_MinMaxInt(pDX, m_sampleTimes, 0, 999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefineProductTab, CDialog)
	//{{AFX_MSG_MAP(CDefineProductTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineProductTab message handlers
BOOL CDefineProductTab::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_percisionEx.AddString("0.0");
	m_percisionEx.AddString("0.00");
	m_percisionEx.AddString("0.000");
	m_percisionEx.AddString("0.0000");

	return TRUE;  
}

void CDefineProductTab::LinkWz(WorkZoneDef *p)
{
	pWz=p;
	m_percisionEx.SetCurSel(pWz->Precision);
	
	m_tag = pWz->Tag;
	m_measureTime = pWz->MeasureTime.Format("%Y-%m-%d %H:%M:%S");
	m_modifyTime = pWz->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
	m_correctTime = pWz->CorrectTime.Format("%Y-%m-%d %H:%M:%S");
	//m_index = pWz->Index;
	m_repeat = pWz->Repeat;
	m_sampleTimes = (pWz->SampleTimes)*10; // notice!

	if(pWz->ContentUnit==0) ((CButton*)GetDlgItem(IDC_CONTENTUNIT0))->SetCheck(TRUE);
	else if(pWz->ContentUnit==1) ((CButton*)GetDlgItem(IDC_CONTENTUNIT1))->SetCheck(TRUE);
	else if(pWz->ContentUnit==2) ((CButton*)GetDlgItem(IDC_CONTENTUNIT2))->SetCheck(TRUE);
	else if(pWz->ContentUnit==3) ((CButton*)GetDlgItem(IDC_CONTENTUNIT3))->SetCheck(TRUE);

	if(pWz->ThickUnit==0) ((CButton*)GetDlgItem(IDC_THICKUNIT0))->SetCheck(TRUE);
	else if(pWz->ThickUnit==1) ((CButton*)GetDlgItem(IDC_THICKUNIT1))->SetCheck(TRUE);
	else if(pWz->ThickUnit==2) ((CButton*)GetDlgItem(IDC_THICKUNIT2))->SetCheck(TRUE);
	else if(pWz->ThickUnit==3) ((CButton*)GetDlgItem(IDC_THICKUNIT3))->SetCheck(TRUE);
	
	UpdateData(FALSE); //Member Variables -> Interface
}

void CDefineProductTab::SaveToWz()
{
	UpdateData(TRUE); //Interface -> Variable
	pWz->Tag = m_tag;
	pWz->ModifyTime = CTime::GetCurrentTime();
	pWz->SampleTimes = m_sampleTimes/10; // notice!
	pWz->Repeat = m_repeat;

	// get value from radio control
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTUNIT0))->GetCheck())
		pWz->ContentUnit=0;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTUNIT1))->GetCheck())
		pWz->ContentUnit=1;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTUNIT2))->GetCheck())
		pWz->ContentUnit=2;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTUNIT3))->GetCheck())
		pWz->ContentUnit=3;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKUNIT0))->GetCheck())
		pWz->ThickUnit=0;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKUNIT1))->GetCheck())
		pWz->ThickUnit=1;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKUNIT2))->GetCheck())
		pWz->ThickUnit=2;
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKUNIT3))->GetCheck())
		pWz->ThickUnit=3;

	int n = m_percisionEx.GetCurSel();
	pWz->Precision = n<0? 1 : (n+1);
}


BOOL CDefineProductTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
    {
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
			pMsg->wParam=NULL ;
    }
	return CDialog::PreTranslateMessage(pMsg);
}
