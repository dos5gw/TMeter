// StdSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "StdSampleDlg.h"
#include "ConfOneStdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStdSampleDlg dialog


CStdSampleDlg::CStdSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStdSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStdSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStdSamples = NULL;
	m_pWzDef = NULL;
	m_bNewFile = FALSE;
	m_beChanged = FALSE;
}


void CStdSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdSampleDlg)
	DDX_Control(pDX, IDC_STDLIST, m_SampleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStdSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CStdSampleDlg)
	ON_BN_CLICKED(ID_EDIT_STD, OnNewAndEdit)
	ON_BN_CLICKED(ID_INSERT_STD, OnInsert)
	ON_BN_CLICKED(ID_DELETE_STD, OnDelete)
	ON_BN_CLICKED(ID_DELETE_ALL, OnDeleteAll)
	ON_BN_CLICKED(ID_OPEN_FILE, OnOpenFromFile)
	ON_BN_CLICKED(ID_SAVE_FILE, OnSaveToFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStdSampleDlg message handlers

void CStdSampleDlg::LinkValue(BOOL bNew, CStandardSamples* ptr)
{
	m_bNewFile = bNew;
	m_pStdSamples = ptr;
}

void CStdSampleDlg::LinkValue(BOOL bNew, WorkZoneDef* ptr)
{
	m_bNewFile = bNew;
	m_pWzDef = ptr;
	m_pStdSamples = &(ptr->stdSample);
}

BOOL CStdSampleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_pWzDef && !m_pStdSamples) return FALSE;

	BOOL beAdmin = TRUE;
	if(beAdmin)
	{
		((CButton*)GetDlgItem(ID_OPEN_FILE))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(ID_SAVE_FILE))->EnableWindow(TRUE);
	}

	DWORD exstyle = m_SampleList.GetExtendedStyle();
	m_SampleList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	CRect rect;
	m_SampleList.GetClientRect(&rect);
	int nColInterval = rect.Width()/4;

	m_SampleList.InsertColumn(0, "序号",LVCFMT_LEFT,45);
	m_SampleList.InsertColumn(1, "标样名",LVCFMT_LEFT,nColInterval);

	int nLayer = m_pWzDef->defMa.m_layerArray.GetSize();
	OneLayerDefine* pOneLayer;
	CString column,columnTheory,columnCorrect;
	for(int i=0;i<nLayer;i++)
	{
		pOneLayer = m_pWzDef->defMa.m_layerArray.GetAt(i);

		column.Format("d%d ",i+1);
		column += pOneLayer->GetLayerName(); 
		m_SampleList.InsertColumn(2+i*3, column+" 标准 [um]",LVCFMT_LEFT,nColInterval);

		columnTheory = column + " 理论 [um]";
		m_SampleList.InsertColumn(3+i*3, columnTheory,LVCFMT_LEFT,nColInterval);
		
		columnCorrect = column + " 修正 [um]";
		m_SampleList.InsertColumn(4+i*3, columnCorrect,LVCFMT_LEFT,nColInterval);
	}

	UpdateSampleList();

	return TRUE;  
}

void CStdSampleDlg::UpdateSampleList()
{
	//CStandardSamples* pStds = &(m_pWzDef->stdSample);
	CStandardSamples* pStds = m_pStdSamples;
	CStandardSample* pOneStd;
	OneLayerDefine* pOneLayer;
	CString text;
	
	m_SampleList.DeleteAllItems();
	int nStd = pStds->GetSize();
	int nLayer = m_pWzDef->defMa.m_layerArray.GetSize(); /* all standard samples have same layers */

	for(int i=0;i<nStd;i++)
	{
		pOneStd = pStds->GetAt(i);
		//CString stdName = pOneStd->GetStdSampleName();
		CString stdName = pOneStd->m_tag;

		//insert index and name of standard sample
		text.Format("%d",i+1);
		m_SampleList.InsertItem(i,text);
		m_SampleList.SetItemText(i,1,stdName);

		for(int j=0;j<nLayer;j++)
		{
			pOneLayer = pOneStd->m_layerArray.GetAt(j);

			text.Format("%6.3f",pOneLayer->m_thickness);
			m_SampleList.SetItemText(i,2+j*3,text);

			text.Format("%6.3f",pOneLayer->m_theoryness);
			m_SampleList.SetItemText(i,3+j*3,text);

			text.Format("%6.3f",pOneLayer->m_correctness);
			m_SampleList.SetItemText(i,4+j*3,text);
		}
	}
}



void CStdSampleDlg::OnNewAndEdit() 
{
	UpdateData(TRUE);
	int nId;
	POSITION pos=m_SampleList.GetFirstSelectedItemPosition();

	CStandardSample* pOneStd = NULL;
	BOOL beNew;
	if(pos==NULL) /* new standard */
	{
		beNew=TRUE;
		pOneStd = new CStandardSample();
		pOneStd->CopyFrom(&(m_pWzDef->defMa));
		//m_pStdSamples->Add(pOneStd);
	}
	else   /* modify standard */
	{
		beNew=FALSE;
		nId=(int)m_SampleList.GetNextSelectedItem(pos);
		pOneStd = m_pStdSamples->GetAt(nId);
		if(pOneStd==NULL) return;
	}
	
	CConfOneStdDlg dlg;
	dlg.LinkValue(beNew,pOneStd);
	if(dlg.DoModal()==IDOK)  /* if CConfOneStdDlg click OK */
	{
		if(beNew) m_pStdSamples->Add(pOneStd);
	}
	
	m_beChanged = TRUE;
	UpdateSampleList();
}

void CStdSampleDlg::OnInsert() 
{
	
}

void CStdSampleDlg::OnDelete() 
{
	UpdateData(TRUE);
	int nId;
	POSITION pos=m_SampleList.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	nId=(int)m_SampleList.GetNextSelectedItem(pos);

	int nStd = m_pStdSamples->GetSize();
	if((nId<0) || (nId>=nStd))
	{
		// exception
		AfxMessageBox("无效的行号");
		return;
	}

	CStandardSample *pOneStd = m_pStdSamples->GetAt(nId);
	delete pOneStd;
	m_pStdSamples->RemoveAt(nId);

	m_beChanged = TRUE;
	UpdateSampleList();
}

void CStdSampleDlg::OnDeleteAll() 
{
	
}



void CStdSampleDlg::OnOpenFromFile() 
{
	CString stdName;
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"standard sample(.std)|*.std||");
	fileDlg.m_ofn.lpstrInitialDir = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\";
	fileDlg.m_ofn.lpstrTitle = "选择一个标样存档并载入";
	if(fileDlg.DoModal() == IDOK) stdName = fileDlg.GetFileName();
	else return;

	m_pStdSamples->LoadSourceData(stdName);

	UpdateSampleList();
}

void CStdSampleDlg::OnSaveToFile() 
{
	// TODO: Add your control notification handler code here
	
}

void CStdSampleDlg::OnOK() 
{
	// TODO: Add extra validation here

	//calculate theory thickness , and save to standard sample(OneLayerDefine::m_theoryness)


	//if changed standard sample, must re-calibrate work-zone, re-build temp.cal
	if(m_beChanged) // @ 2012-09-03
	{
		m_pWzDef->FpCalibrate();
	}
	

	CString folderName;
	folderName.Format("%04d",m_pWzDef->GetDirIndex());
	CString fullPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName + "\\sample.std";
	m_pStdSamples->SaveSourceData(fullPathName);



	
	CDialog::OnOK();
}

void CStdSampleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
