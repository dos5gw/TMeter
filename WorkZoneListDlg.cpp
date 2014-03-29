// WorkZoneListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "WorkZoneListDlg.h"
#include "WorkZoneDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkZoneListDlg dialog


CWorkZoneListDlg::CWorkZoneListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkZoneListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorkZoneListDlg)
	m_wzTag = _T("");
	//}}AFX_DATA_INIT
	beNewWz=TRUE;
}


void CWorkZoneListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkZoneListDlg)
	DDX_Control(pDX, IDC_WZLIST, m_WorkZoneList);
	DDX_Text(pDX, IDC_NAME, m_wzTag);
	DDV_MaxChars(pDX, m_wzTag, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkZoneListDlg, CDialog)
	//{{AFX_MSG_MAP(CWorkZoneListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkZoneListDlg message handlers

BOOL CWorkZoneListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD exstyle = m_WorkZoneList.GetExtendedStyle();
	m_WorkZoneList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	CRect rect;
	m_WorkZoneList.GetClientRect(&rect);
    int nColInterval = rect.Width()/3;
	m_WorkZoneList.InsertColumn(0, "编号",LVCFMT_LEFT,nColInterval);
	m_WorkZoneList.InsertColumn(1, "名称",LVCFMT_LEFT,nColInterval);
	m_WorkZoneList.InsertColumn(2, "日期",LVCFMT_LEFT,nColInterval);
	
	BOOL queryRet = TRUE;
	WorkZoneDataSet *pSet = new WorkZoneDataSet();
	if (!pSet->IsOpen()) 
		pSet->Open();
		//pSet->Open(CRecordset::dynaset,NULL);
	pSet->m_strSort=_T("ID ASC");
	if(!pSet->Requery()){
		AfxMessageBox(_T("CWorkZoneListDlg::OnInitDialog(), Requery failed!"));
		queryRet = FALSE;
	}
	
	CString text; int i=0;
	//m_MaListEx.DeleteAllItems(); //clean list controls
	while(!pSet->IsEOF()){
		text.Format("%04d",pSet->m_ID);
		m_WorkZoneList.InsertItem(i,text);
		m_WorkZoneList.SetItemText(i,1,pSet->m_Tag);
		text = pSet->m_ModifyTime.Format("%Y-%m-%d");
		m_WorkZoneList.SetItemText(i,2,text);
		pSet->MoveNext();
		i++;
	}
	
	return TRUE;  
}

void CWorkZoneListDlg::OnOK() 
{
	UpdateData(TRUE); //Interface -> Variable

	if(beNewWz) // new build a work-zone, input tag
	{
		if(m_wzTag.IsEmpty())
		{
			AfxMessageBox("请输入工作区名称");
			return;
		}
	}
	else   // switch to exist work-zone
	{
		int nId;
		POSITION pos=m_WorkZoneList.GetFirstSelectedItemPosition();
		if(pos!=NULL){
			nId=(int)m_WorkZoneList.GetNextSelectedItem(pos);
			//CString text = m_WorkZoneList.GetItemData();
			CString text = m_WorkZoneList.GetItemText(nId,0);
			//Index = atoi((LPTSTR)(LPCTSTR)text);
			Index = atoi((LPTSTR)(LPCTSTR)text);
		} 
	}
	
	CDialog::OnOK();
}

void CWorkZoneListDlg::OnCancel() 
{
	//UpdateData(TRUE); //Interface -> Variable
	CDialog::OnCancel();
}


