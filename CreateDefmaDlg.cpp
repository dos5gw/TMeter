// CreateDefmaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "CreateDefmaDlg.h"
#include "DefineOneLayerDlg.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateDefmaDlg dialog


CCreateDefmaDlg::CCreateDefmaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDefmaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateDefmaDlg)
	m_tag = _T("");
	m_thickMode = _T("");
	//}}AFX_DATA_INIT
	m_bNewFile=TRUE;
	m_pDefMa = NULL;
	m_pWz = NULL;

}

void CCreateDefmaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateDefmaDlg)
	DDX_Control(pDX, IDC_LAYERTREE, m_LayerTree);
	DDX_Text(pDX, IDC_TAG, m_tag);
	DDV_MaxChars(pDX, m_tag, 48);
	DDX_Text(pDX, IDC_THICKMODE, m_thickMode);
	DDV_MaxChars(pDX, m_thickMode, 36);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateDefmaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateDefmaDlg)
	ON_BN_CLICKED(IDC_MODIFY, OnModifyLayer)
	ON_BN_CLICKED(IDC_NEW, OnNewLayer)
	ON_BN_CLICKED(IDC_CMPSA, OnSpectrumCompensate)
	ON_BN_CLICKED(IDC_CAILB, OnCailbSetting)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CANCL, OnExitCreating)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCreateDefmaDlg::LinkValue(BOOL bNew,CDefMa* ptr)
{
	m_bNewFile = bNew;
	m_pDefMa = ptr;
}

void CCreateDefmaDlg::LinkValue(BOOL bNew,WorkZoneDef* ptr)
{
	m_bNewFile = bNew;
	m_pWz = ptr;
}

/////////////////////////////////////////////////////////////////////////////
// CCreateDefmaDlg message handlers

BOOL CCreateDefmaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// update UI from CDefMa
	m_tag = m_pDefMa->m_tag;
	if(m_pDefMa->m_hv==30) ((CButton*)GetDlgItem(IDC_HV30KV))->SetCheck(TRUE);
	else if(m_pDefMa->m_hv==40) ((CButton*)GetDlgItem(IDC_HV40KV))->SetCheck(TRUE);
	else if(m_pDefMa->m_hv==50) ((CButton*)GetDlgItem(IDC_HV50KV))->SetCheck(TRUE);

	if(m_pDefMa->m_filter==0) ((CButton*)GetDlgItem(IDC_FILTER_NI))->SetCheck(TRUE);
	else if(m_pDefMa->m_filter==1) ((CButton*)GetDlgItem(IDC_FILTER_NONE))->SetCheck(TRUE);
	else if(m_pDefMa->m_filter==2) ((CButton*)GetDlgItem(IDC_FILTER_AL))->SetCheck(TRUE);

	if(m_pDefMa->bAllElemArray)
		((CButton*)GetDlgItem(IDC_ALLELEMLIST))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_ALLELEMLIST))->SetCheck(FALSE);

	if(m_pDefMa->bMqShown)
		((CButton*)GetDlgItem(IDC_ISMQ))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_ISMQ))->SetCheck(FALSE);

	if(m_pDefMa->bScaleCompensated)
		((CButton*)GetDlgItem(IDC_BECMSCALE))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_BECMSCALE))->SetCheck(FALSE);

	if(m_pDefMa->bOffsetCompensated)
		((CButton*)GetDlgItem(IDC_BECMOFFSET))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_BECMOFFSET))->SetCheck(FALSE);

	if(m_pDefMa->bFilterUsed)
		((CButton*)GetDlgItem(IDC_BEFILTER))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_BEFILTER))->SetCheck(FALSE);

	if(m_pDefMa->bNeedMeasureElem) 
		((CButton*)GetDlgItem(IDC_BEMEASURE))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_BEMEASURE))->SetCheck(FALSE);

	m_LayerTree.ModifyStyle(NULL,TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT);
	UpdateElemTree();
	
	UpdateData(FALSE); //Member Variables -> Interface
	return TRUE; 
}

void CCreateDefmaDlg::UpdateElemTree()
{
	HTREEITEM hLayer;
	//HTREEITEM hRoot=m_LayerTree.InsertItem("Root");
	m_LayerTree.DeleteAllItems();
	
	CString layerName,layerIndex;
	CString layerText;

	OneLayerDefine* pOneLayer;
	int num = m_pDefMa->m_layerArray.GetSize(); 
	for(int i=0;i<num;i++)
	{
		pOneLayer = m_pDefMa->m_layerArray.GetAt(i);
		layerName = pOneLayer->GetLayerName();
		layerIndex.Format("%2d",i+1);
		layerName = "镀层  " + layerIndex + "    " + layerName;
		hLayer = m_LayerTree.InsertItem(layerName,TVI_ROOT);
		m_LayerTree.SetItemData(hLayer,(DWORD)i); //added @ 2012-07-03
		
		if(pOneLayer->m_thickMode==0) layerText = "镀层的厚度将会被测量";
		else if(pOneLayer->m_thickMode==1) layerText = "镀层的厚度将可能被更正";
		else if(pOneLayer->m_thickMode==2) layerText = "厚度已知";	
		m_LayerTree.InsertItem(layerText,hLayer);

		if(pOneLayer->m_contentMode==0) layerText = "镀层的成份将会被测量";
		else if(pOneLayer->m_contentMode==1) layerText = "镀层的成份将可能被更正";
		else if(pOneLayer->m_contentMode==2) layerText = "成份已知";	
		m_LayerTree.InsertItem(layerText,hLayer);

		CClassicalElement* pElem;
		int nEle = pOneLayer->m_elementList.GetSize();
		for(int j=0;j<nEle;j++)
		{
			pElem = pOneLayer->m_elementList.GetAt(j);
			layerText.Format("%2d  %s  (%4.2lf%%)  (%4.2lfg/cm^3)",pElem->Id,pElem->Name,pElem->Content,pElem->Density);
			m_LayerTree.InsertItem(layerText,hLayer);
		}
	}
}

void CCreateDefmaDlg::OnModifyLayer() 
{
	CDefineOneLayerDlg dlg;
	OneLayerDefine* pOneLayer = NULL;
	
	HTREEITEM item = m_LayerTree.GetSelectedItem();
	if(!item) return;
	int index = m_LayerTree.GetItemData(item); 
	int sum   = m_pDefMa->m_layerArray.GetSize();
	if(index<0 || index>sum-1) return;
	pOneLayer = m_pDefMa->m_layerArray.GetAt(index);

	dlg.LinkData(pOneLayer);
	dlg.DoModal();

	UpdateElemTree();
}

void CCreateDefmaDlg::OnNewLayer() 
{
	OneLayerDefine* pOneLayer = new OneLayerDefine();
	CDefineOneLayerDlg dlg;
	dlg.LinkData(pOneLayer);
	dlg.DoModal();

	m_pDefMa->m_layerArray.Add(pOneLayer);
	UpdateElemTree();
}

void CCreateDefmaDlg::OnSpectrumCompensate() 
{
	// TODO: Add your control notification handler code here
	
}

void CCreateDefmaDlg::OnCailbSetting() 
{
	// TODO: Add your control notification handler code here
	
}

void CCreateDefmaDlg::SaveDataToPtr()
{
	m_pDefMa->m_tag = m_tag;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_HV30KV))->GetCheck())
		m_pDefMa->m_hv=30;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_HV40KV))->GetCheck())
		m_pDefMa->m_hv=40;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_HV50KV))->GetCheck())
		m_pDefMa->m_hv=50;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_FILTER_NI))->GetCheck())
		m_pDefMa->m_filter=0;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_FILTER_NONE))->GetCheck())
		m_pDefMa->m_filter=1;
	else if(BST_CHECKED==((CButton*)GetDlgItem(IDC_FILTER_AL))->GetCheck())
		m_pDefMa->m_filter=2;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_ALLELEMLIST))->GetCheck())
		m_pDefMa->bAllElemArray=TRUE;
	else
		m_pDefMa->bAllElemArray=FALSE;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_ISMQ))->GetCheck())
		m_pDefMa->bMqShown=TRUE;
	else
		m_pDefMa->bMqShown=FALSE;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_BECMSCALE))->GetCheck())
		m_pDefMa->bScaleCompensated=TRUE;
	else
		m_pDefMa->bScaleCompensated=FALSE;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_BECMOFFSET))->GetCheck())
		m_pDefMa->bOffsetCompensated=TRUE;
	else
		m_pDefMa->bOffsetCompensated=FALSE;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_BEFILTER))->GetCheck())
		m_pDefMa->bFilterUsed=TRUE;
	else
		m_pDefMa->bFilterUsed=FALSE;
	
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_BEMEASURE))->GetCheck())
		m_pDefMa->bNeedMeasureElem=TRUE;
	else
		m_pDefMa->bNeedMeasureElem=FALSE;
}

void CCreateDefmaDlg::OnSave()
{
	BOOL queryRet = TRUE;
	UpdateData(TRUE); //Interface -> Variable

	if(m_tag.IsEmpty()){
		AfxMessageBox("请输入标签描述\n");
		return;
	}

	CString fileName,folderName,fullPathName;
	if(m_bNewFile) {
		//(1) Add new record to database
		DefMADataSet *pNewSet = new DefMADataSet();
		pNewSet->Open();
		pNewSet->AddNew();
		pNewSet->m_Mode = this->m_thickMode;
		pNewSet->m_Tag  = this->m_tag;
		pNewSet->Update();
		Sleep(200);

		//(2) Query last 
		pNewSet->m_strSort=_T("ID DESC");
		if(!pNewSet->Requery()){
			AfxMessageBox(_T("CCreateDefmaDlg::OnSave(), Requery failed!"));
			queryRet = FALSE;
		}
		int index = pNewSet->m_ID; // default new file name
		pNewSet->Close();
		delete pNewSet;
		
		if(queryRet){
			//(3) Save defMA to 00xx.xma
			SaveDataToPtr();
			fileName.Format("%04d",index);
			fullPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineMA\\" + fileName + ".xma";
			
			TiXmlDocument doc;
			m_pDefMa->XMLSerialize(TRUE,&doc);
			if(!doc.SaveFile((LPTSTR)(LPCTSTR)fullPathName))
			{
				CString text;
				text.Format("%s 保存失败",fullPathName);
				AfxMessageBox(text);
				return;
			}
		}
	} 
	else 
	{
		// for debugging watch, delete the codes when release
		OneLayerDefine* pOneLayer;
		CClassicalElement* pElem;
		CString eleName; double content; BOOL active;
		int winLeft,winRight;
		int nLayers = m_pDefMa->m_layerArray.GetSize();
		for(int m=0;m<nLayers;m++)
		{
			pOneLayer = m_pDefMa->m_layerArray.GetAt(m);
			int nEle = pOneLayer->m_elementList.GetSize();
			for(int n=0;n<nEle;n++)
			{
				pElem = pOneLayer->m_elementList.GetAt(n);
				eleName = pElem->Name;    //watch
				content = pElem->Content; //watch
				active = pElem->beActive; //watch
				winLeft = pElem->WinLeft;
				winRight = pElem->WinRight;
			}
		}
		// end : debugging watch

		// Rewrite defMA to 00xx.xma
		SaveDataToPtr();
		folderName.Format("%04d",m_pWz->GetDirIndex());
		fullPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName + "\\analysis.xma"; 
		
		TiXmlDocument doc;
		m_pDefMa->XMLSerialize(TRUE,&doc);
		if(!doc.SaveFile((LPTSTR)(LPCTSTR)fullPathName))
		{
			CString text;
			text.Format("%s 保存失败",fullPathName);
			AfxMessageBox(text);
			return;
		}

	}
	
	CDialog::EndDialog(TRUE);
	
}

void CCreateDefmaDlg::OnExitCreating() 
{
	if(AfxMessageBox("确定退出?",MB_YESNOCANCEL)==IDYES)
		CDialog::EndDialog(TRUE);
}

void CCreateDefmaDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	//if(m_bNewFile) delete m_pDefMa;
}
