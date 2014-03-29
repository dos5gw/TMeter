// DefineOneLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "DefineOneLayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefineOneLayerDlg dialog


CDefineOneLayerDlg::CDefineOneLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDefineOneLayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefineOneLayerDlg)
	m_density = 0.0;
	m_thickness = 0.0;
	m_elemContent = 1.0;
	m_elemDensity = 0.0;
	m_isSaturated = FALSE;
	m_NeedCalculated = FALSE;
	m_elemLeft = 0;
	m_elemRight = 0;
	//}}AFX_DATA_INIT
	pApp = NULL;
}


void CDefineOneLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefineOneLayerDlg)
	DDX_Control(pDX, IDC_ELEMCOMBO, m_elemCombo);
	DDX_Control(pDX, IDC_ELEMLIST, m_listEx);
	DDX_Text(pDX, IDC_DENSITY, m_density);
	DDX_Text(pDX, IDC_THICKNESS, m_thickness);
	DDX_Text(pDX, IDC_ELEMCONTENT, m_elemContent);
	DDX_Text(pDX, IDC_ELEMDENSITY, m_elemDensity);
	DDX_Check(pDX, IDC_SATURATED, m_isSaturated);
	DDX_Check(pDX, IDC_CALCULATED, m_NeedCalculated);
	DDX_Text(pDX, IDC_ELEMLEFT, m_elemLeft);
	DDV_MinMaxInt(pDX, m_elemLeft, 0, 255);
	DDX_Text(pDX, IDC_ELEMRIGHT, m_elemRight);
	DDV_MinMaxInt(pDX, m_elemRight, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefineOneLayerDlg, CDialog)
	//{{AFX_MSG_MAP(CDefineOneLayerDlg)
	ON_CBN_SELCHANGE(IDC_ELEMCOMBO, OnElemComboChanged)
	ON_BN_CLICKED(IDC_ADD, OnAddNewElement)
	ON_BN_CLICKED(IDC_REPLACE, OnReplaceElement)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteElement)
	ON_BN_CLICKED(IDC_RECONTENT, OnRecontent)
	ON_NOTIFY(NM_CLICK, IDC_ELEMLIST, OnClickElemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefineOneLayerDlg message handlers
BOOL CDefineOneLayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!m_pOneLayer) return FALSE;
	pApp = (CTMeterApp*)AfxGetApp();

	switch(m_pOneLayer->m_thickMode){
	case 0:
		((CButton *)GetDlgItem(IDC_THICKMODE0))->SetCheck(TRUE);
		break;
	case 1:
		((CButton *)GetDlgItem(IDC_THICKMODE1))->SetCheck(TRUE);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_THICKMODE2))->SetCheck(TRUE);
		break;
	}
	switch(m_pOneLayer->m_contentMode){
	case 0:
		((CButton *)GetDlgItem(IDC_CONTENTMODE0))->SetCheck(TRUE);
		break;
	case 1:
		((CButton *)GetDlgItem(IDC_CONTENTMODE1))->SetCheck(TRUE);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_CONTENTMODE2))->SetCheck(TRUE);
		break;
	}

	m_density = m_pOneLayer->m_density;
	m_thickness = m_pOneLayer->m_thickness;
	m_isSaturated = m_pOneLayer->m_isSaturated;
	
	/*** initialize m_elemList ***/
	DWORD exstyle = m_listEx.GetExtendedStyle();
	m_listEx.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_listEx.InsertColumn(0, "序号",LVCFMT_LEFT,45);
	m_listEx.InsertColumn(1, "元素",LVCFMT_LEFT,45);
	m_listEx.InsertColumn(2, "含量",LVCFMT_LEFT,65);
	m_listEx.InsertColumn(3, "密度",LVCFMT_LEFT,65);
	m_listEx.InsertColumn(4, "计算",LVCFMT_LEFT,30);
	m_listEx.InsertColumn(5, "L",LVCFMT_LEFT,30);
	m_listEx.InsertColumn(6, "R",LVCFMT_LEFT,30);
	UpdateElemList();

	/*** initialize m_elemCombo  ***/
	UpdateElemCombo();

	UpdateData(FALSE); //Member Variables -> Interface
	return TRUE; 
}

void CDefineOneLayerDlg::OnElemComboChanged() 
{
	int iPos=m_elemCombo.GetCurSel();
	if(iPos<0) return;

	CClassicalElement* pElem = pApp->m_PeriodicTable.GetAt(iPos);
	m_elemContent = pElem->Content;
	m_elemDensity = pElem->Density;
	m_NeedCalculated = pElem->beActive;
	m_elemLeft = pElem->WinLeft;
	m_elemRight= pElem->WinRight;

	UpdateData(FALSE); //Member Variables -> Interface
}

void CDefineOneLayerDlg::OnAddNewElement() 
{
	UpdateData(TRUE);
	int iPos=m_elemCombo.GetCurSel();
	if(iPos<0) return;

	int num = m_pOneLayer->m_elementList.GetSize();
	CClassicalElement* pElem;
	for(int i=0;i<num;i++)
	{
		pElem = m_pOneLayer->m_elementList.GetAt(i);
		if(pElem->Id == iPos+1) // pElem has already exist 
			return;
	}

	// copy from m_PeriodicTable, modified content/density
	pElem = pApp->m_PeriodicTable.GetAt(iPos);
	CClassicalElement* pNewElem = new CClassicalElement(pElem);
	pNewElem->Content = m_elemContent;
	pNewElem->Density = m_elemDensity;
	pNewElem->beActive= m_NeedCalculated;
	pNewElem->WinLeft = m_elemLeft;
	pNewElem->WinRight= m_elemRight;
	m_pOneLayer->m_elementList.Add(pNewElem);

	m_density = m_pOneLayer->CalcAverageDensity(); //m_pOneLayer->CalcDensity();
	UpdateElemList();
	UpdateData(FALSE); //Variable -> Interface
}

void CDefineOneLayerDlg::OnReplaceElement() 
{
	UpdateData(TRUE);
	int iPos=m_elemCombo.GetCurSel();
	if(iPos<0) return;
	
	// find duplicate
	int num = m_pOneLayer->m_elementList.GetSize();
	CClassicalElement* pElem;
	for(int i=0;i<num;i++)
	{
		pElem = m_pOneLayer->m_elementList.GetAt(i);
		if(pElem->Id == iPos+1) 
		{
			pElem->Density=m_elemDensity;
			pElem->Content=m_elemContent;
			pElem->beActive=m_NeedCalculated;
			pElem->WinLeft = m_elemLeft;
			pElem->WinRight= m_elemRight;
			m_density = m_pOneLayer->CalcAverageDensity(); 
			UpdateElemList();
			return;
		}	
	}

	// free memory of selected element
	int nId;
	POSITION pos=m_listEx.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	nId=(int)m_listEx.GetNextSelectedItem(pos);
	pElem = m_pOneLayer->m_elementList.GetAt(nId);
	delete pElem;

	// copy CClassicalElement from m_PeriodicTable
	pElem = pApp->m_PeriodicTable.GetAt(iPos);
	CClassicalElement* pNewElem = new CClassicalElement(pElem);
	pNewElem->Content = m_elemContent;
	pNewElem->Density = m_elemDensity;
	pNewElem->beActive= m_NeedCalculated;
	pNewElem->WinLeft = m_elemLeft;
	pNewElem->WinRight= m_elemRight;
	m_pOneLayer->m_elementList[nId] = pNewElem;

	m_density = m_pOneLayer->CalcAverageDensity(); //m_pOneLayer->CalcDensity();
	UpdateElemList();
	UpdateData(FALSE); //Variable -> Interface
}

void CDefineOneLayerDlg::OnDeleteElement() 
{
	UpdateData(TRUE);
	int nId;
	POSITION pos=m_listEx.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	nId=(int)m_listEx.GetNextSelectedItem(pos);
	CClassicalElement* pElem = m_pOneLayer->m_elementList.GetAt(nId);
	delete pElem;
	m_pOneLayer->m_elementList.RemoveAt(nId);

	m_density = m_pOneLayer->CalcAverageDensity(); //m_pOneLayer->CalcDensity();
	UpdateElemList();
	UpdateData(FALSE); //Variable -> Interface
}

void CDefineOneLayerDlg::OnRecontent() 
{
	UpdateData(TRUE);
	CClassicalElement* pElem;
	int num = m_pOneLayer->m_elementList.GetSize();
	if(num == 0) return;

	for(int i=0;i<num;i++)
	{
		pElem = m_pOneLayer->m_elementList.GetAt(i);
		pElem->Content = (float)(100.0/num);
	}

	UpdateElemList();

	m_density = m_pOneLayer->CalcAverageDensity(); //m_pOneLayer->CalcDensity();
	UpdateData(FALSE); //Variable -> Interface
}


void CDefineOneLayerDlg::OnClickElemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	int nId;
	POSITION pos=m_listEx.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	nId=(int)m_listEx.GetNextSelectedItem(pos);
	CClassicalElement* pElem = m_pOneLayer->m_elementList.GetAt(nId);

	m_elemCombo.SetCurSel((pElem->Id)-1);
	OnElemComboChanged();
	//UpdateElemList();
	*pResult = 0;
}

void CDefineOneLayerDlg::UpdateElemList()
{
	CClassicalElement* pElem;
	CString text;

	//float densityTemp = 0.0;
	m_listEx.DeleteAllItems();
	int num = m_pOneLayer->m_elementList.GetSize();
	for(int i=0;i<num;i++){
		pElem = m_pOneLayer->m_elementList.GetAt(i);

		text.Format("%2d",pElem->Id); // insert new 
		m_listEx.InsertItem(i,text);

		text.Format("%s",pElem->Name);
		m_listEx.SetItemText(i,1,text);

		text.Format("%4.2lf%%",pElem->Content); 
		m_listEx.SetItemText(i,2,text);

		text.Format("%4.2lf",pElem->Density);
		m_listEx.SetItemText(i,3,text);

		text.Format("%d",pElem->beActive);
		m_listEx.SetItemText(i,4,text);

		text.Format("%d",pElem->WinLeft);  // new 
		m_listEx.SetItemText(i,5,text);

		text.Format("%d",pElem->WinRight); // new 
		m_listEx.SetItemText(i,6,text);
		
	}
}

void CDefineOneLayerDlg::UpdateElemCombo()
{
	CString text;
	CClassicalElement* pElem;

	if(!pApp) return;
	//m_elemCombo.ResetContent();
	int num = pApp->m_PeriodicTable.GetSize();
	for(int i=0;i<num;i++)
	{
		pElem = pApp->m_PeriodicTable.GetAt(i);
		text.Format("%2d %s",pElem->Id,(LPTSTR)(LPCTSTR)(pElem->Name));
		m_elemCombo.AddString(text);
	}
	//m_elemCombo.SetReadOnly(TRUE);
}

void CDefineOneLayerDlg::OnOK() 
{
	UpdateData(TRUE); //Interface -> Variable
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKMODE0))->GetCheck())
		m_pOneLayer->m_thickMode = 0;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKMODE1))->GetCheck())
		m_pOneLayer->m_thickMode = 1;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_THICKMODE2))->GetCheck())
		m_pOneLayer->m_thickMode = 2;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTMODE0))->GetCheck())
		m_pOneLayer->m_contentMode = 0;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTMODE1))->GetCheck())
		m_pOneLayer->m_contentMode = 1;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTENTMODE2))->GetCheck())
		m_pOneLayer->m_contentMode = 2;
	
	m_pOneLayer->m_density = m_density;
	m_pOneLayer->m_thickness = m_thickness;
	m_pOneLayer->m_isSaturated = m_isSaturated;

	CDialog::OnOK();
}

void CDefineOneLayerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//AfxMessageBox("");
	CDialog::OnCancel();
}



