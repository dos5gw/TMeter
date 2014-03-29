// DefMAListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "DefMAListDlg.h"
#include "DefMADataSet.h"
//#include "WorkZoneDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefMAListDlg dialog


CDefMAListDlg::CDefMAListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDefMAListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefMAListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	defMaName = "";
	defMaTag  = "";
}


void CDefMAListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefMAListDlg)
	DDX_Control(pDX, IDC_DEFMALIST, m_MaListEx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefMAListDlg, CDialog)
	//{{AFX_MSG_MAP(CDefMAListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefMAListDlg message handlers

BOOL CDefMAListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD exstyle = m_MaListEx.GetExtendedStyle();
	m_MaListEx.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	CRect rect;
	m_MaListEx.GetClientRect(&rect);
    int nColInterval = rect.Width()/3;
	m_MaListEx.InsertColumn(0, "编号",LVCFMT_LEFT,nColInterval);
	m_MaListEx.InsertColumn(1, "标签",LVCFMT_LEFT,nColInterval);
	m_MaListEx.InsertColumn(2, "厚度模式",LVCFMT_LEFT,nColInterval);

	BOOL queryRet = TRUE;
	DefMADataSet *pSet = new DefMADataSet();
	if (!pSet->IsOpen()) pSet->Open();
	pSet->m_strSort=_T("ID ASC");
	if(!pSet->Requery()){
		AfxMessageBox(_T("CDefMAListDlg::OnInitDialog(), Requery failed!"));
		queryRet = FALSE;
	}

	CString text;
	int i=0;
	//m_MaListEx.DeleteAllItems(); //clean list controls
	while(!pSet->IsEOF()){
		text.Format("%04d",pSet->m_ID);
		m_MaListEx.InsertItem(i,text);
		m_MaListEx.SetItemText(i,1,pSet->m_Tag);
		m_MaListEx.SetItemText(i,2,pSet->m_Mode);
		pSet->MoveNext();
		i++;
	}
	defMaSum = i+1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDefMAListDlg::OnOK() 
{
	// TODO: Add extra validation here
	// get current selected row
	POSITION pos=m_MaListEx.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem = m_MaListEx.GetNextSelectedItem(pos);
	if(nItem<0 || nItem>defMaSum-1) return;
	defMaName= m_MaListEx.GetItemText(nItem,0) + ".xma";
	defMaTag = m_MaListEx.GetItemText(nItem,1);
	CDialog::OnOK();
}

void CDefMAListDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
