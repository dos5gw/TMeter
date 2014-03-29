// CreateWorkZoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "CreateWorkZoneDlg.h"
#include "FpCalculator.h"
#include "WorkZoneDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateWorkZoneDlg dialog


CCreateWorkZoneDlg::CCreateWorkZoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateWorkZoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateWorkZoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bNewFile = TRUE;
}


void CCreateWorkZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateWorkZoneDlg)
	DDX_Control(pDX, IDC_WZTAB, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateWorkZoneDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateWorkZoneDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_WZTAB, OnSelchangeWzTab)
	ON_BN_CLICKED(IDC_ACCEPT, OnAccept)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateWorkZoneDlg message handlers

int CCreateWorkZoneDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CCreateWorkZoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_tabCtrl.InsertItem(0,"产品程式");
	m_tabCtrl.InsertItem(1,"应用程式");
	m_tabCtrl.InsertItem(2,"表达方式");
	//m_tabCtrl.InsertItem(3,"备忘");
	
	m_productTab.Create(IDD_TAB_DEFPRODUCT,&m_tabCtrl);//GetDlgItem(IDC_WZTAB));
	m_appTab.Create(IDD_TAB_DEFAPP,&m_tabCtrl);//GetDlgItem(IDC_WZTAB));
	m_unitTab.Create(IDD_TAB_DEFUNIT,&m_tabCtrl);//GetDlgItem(IDC_WZTAB));
	
	m_productTab.LinkWz(m_pWorkZone);
	m_appTab.LinkWz(m_pWorkZone,m_bNewFile);
	m_unitTab.LinkWz(m_pWorkZone);

	CRect rs;
	m_tabCtrl.GetClientRect(&rs);
	rs.top+=24;
	rs.bottom-=2;
	rs.left+=2;
	rs.right-=2;
	m_productTab.MoveWindow(&rs);
	m_appTab.MoveWindow(&rs);
	m_unitTab.MoveWindow(&rs);

	m_productTab.ShowWindow(TRUE);
	m_appTab.ShowWindow(FALSE);
	m_unitTab.ShowWindow(FALSE);
	m_tabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateWorkZoneDlg::LinkValue(BOOL bNew, WorkZoneDef* ptr)
{
	m_bNewFile = bNew;
	m_pWorkZone= ptr;

}

void CCreateWorkZoneDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	//KillTimer(0);
	// and free memory here
}

void CCreateWorkZoneDlg::SaveToDatabase()
{
	WorkZoneDataSet *pSet = new WorkZoneDataSet();
	pSet->Open(); 

	pSet->AddNew();
	pSet->m_Tag = m_pWorkZone->Tag;
	pSet->m_ModifyTime = m_pWorkZone->ModifyTime;
	pSet->m_MeasureTime = m_pWorkZone->MeasureTime;
	pSet->m_CorrectTime = m_pWorkZone->CorrectTime;
	pSet->m_SampleTimes = m_pWorkZone->SampleTimes;
	pSet->m_SampleRepeat = m_pWorkZone->Repeat;
	pSet->m_ContentUnit = m_pWorkZone->ContentUnit;
	pSet->m_ThickUnit = m_pWorkZone->ThickUnit;
	pSet->m_Precision = m_pWorkZone->Precision;
	pSet->m_AdjustTimes = m_pWorkZone->AdjustTimes;
	pSet->m_AdjustRepeat = m_pWorkZone->AdjustRepeat;
	pSet->m_MqMax = m_pWorkZone->MqMax;
	pSet->m_Collimator = m_pWorkZone->Collimator;
	pSet->m_Current = m_pWorkZone->Current;
	pSet->Update();

	pSet->Close();
	delete pSet;

}

//void CCreateWorkZoneDlg::OnSaveWorkZone() 
void CCreateWorkZoneDlg::OnOK()
{
	// save data, from dialog to WorkZoneDef
	m_productTab.SaveToWz();
	m_appTab.SaveToWz();
	m_unitTab.SaveToWz();

	// create file and serialize
	if(m_pWorkZone->Tag.IsEmpty()){
		AfxMessageBox("请输入工作区描述\n");
		return;
	}
	//else m_pWorkZone->Tag.Replace("\\","/"); 
	//after conversion, Tag contain no escape character '\', like "24Ni/MoMn/Ceramic"

	CString folderName,fullFpPathName;
	CString defMaName,stdSampleName;

	if(m_bNewFile)
	{
		//(1) insert new recorder to database @2012-08-17
		WorkZoneDataSet *pNewSet = new WorkZoneDataSet();
		pNewSet->Open();
		pNewSet->AddNew();
		pNewSet->m_Tag = m_pWorkZone->Tag;
		pNewSet->m_ModifyTime = m_pWorkZone->ModifyTime;
		pNewSet->m_MeasureTime= m_pWorkZone->MeasureTime;
		pNewSet->m_CorrectTime= m_pWorkZone->CorrectTime;
		pNewSet->m_SampleTimes= m_pWorkZone->SampleTimes;
		pNewSet->m_SampleRepeat=m_pWorkZone->Repeat;
		pNewSet->m_ContentUnit =m_pWorkZone->ContentUnit;
		pNewSet->m_ThickUnit = m_pWorkZone->ThickUnit;
		pNewSet->m_Precision = m_pWorkZone->Precision;
		pNewSet->m_AdjustTimes = m_pWorkZone->AdjustTimes;
		pNewSet->m_AdjustRepeat= m_pWorkZone->AdjustRepeat;
		pNewSet->m_MqMax = m_pWorkZone->MqMax;
		pNewSet->m_Collimator = m_pWorkZone->Collimator;
		pNewSet->m_Current = m_pWorkZone->Current;
		pNewSet->Update();
		
		//(2) query last record from database
		Sleep(200);
		pNewSet->m_strSort=_T("ID DESC");
		if(!pNewSet->Requery())
		{
			AfxMessageBox(_T("CCreateDefmaDlg::OnSave(), Requery failed!"));
		}
		else
		{
			// default new folder name
			m_pWorkZone->DirNameIndex = pNewSet->m_ID; 
		
			//(3) create work-zone folder
			folderName.Format("%04d",m_pWorkZone->DirNameIndex);
			fullFpPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName;
			if(GetFileAttributes(fullFpPathName) == -1)
			{ 
				CreateDirectory(fullFpPathName,NULL);
			}
			else
			{
				CString text;
				text.Format("创建目录失败%s",fullFpPathName);
				AfxMessageBox("text");
				return;
			}
			
			Sleep(200);
			//(4) serialize def.ma to this folder
			defMaName = fullFpPathName + "\\analysis.xma";
			TiXmlDocument doc;
			m_pWorkZone->defMa.XMLSerialize(TRUE,&doc);
			if(!doc.SaveFile((LPTSTR)(LPCTSTR)defMaName))
			{
				CString text;
				text.Format("%s 保存失败",defMaName);
				AfxMessageBox(text);
				return;
			}

		}
		pNewSet->Close();
		delete pNewSet;
		
	} 
	else //update recorder
	{
		folderName.Format("%04d",m_pWorkZone->GetDirIndex());
		fullFpPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName;

		//(1) update recorder which ID==m_pWorkZone->GetDirIndex()
		WorkZoneDataSet *pUpdateSet = new WorkZoneDataSet();
		pUpdateSet->m_strFilter.Format("ID = %d",m_pWorkZone->GetDirIndex());
		pUpdateSet->Open();
		
		//CString sql;
		//sql.Format("select * from WorkZone where ID=%d",m_pWorkZone->GetDirIndex());
		//pUpdateSet->Open(AFX_DB_USE_DEFAULT_TYPE,sql,0);

		if(pUpdateSet->GetRecordCount() <= 0)
		{
			CString text;
			text.Format("%04d Not Found, 更新没有被执行",m_pWorkZone->GetDirIndex());
			AfxMessageBox(text);
		}
		else
		{
			pUpdateSet->Edit();
			pUpdateSet->m_Tag = m_pWorkZone->Tag;
			pUpdateSet->m_ModifyTime = m_pWorkZone->ModifyTime;
			pUpdateSet->m_MeasureTime= m_pWorkZone->MeasureTime;
			pUpdateSet->m_CorrectTime= m_pWorkZone->CorrectTime;
			pUpdateSet->m_SampleTimes= m_pWorkZone->SampleTimes;
			pUpdateSet->m_SampleRepeat=m_pWorkZone->Repeat;
			pUpdateSet->m_ContentUnit =m_pWorkZone->ContentUnit;
			pUpdateSet->m_ThickUnit = m_pWorkZone->ThickUnit;
			pUpdateSet->m_Precision = m_pWorkZone->Precision;
			pUpdateSet->m_AdjustTimes = m_pWorkZone->AdjustTimes;
			pUpdateSet->m_AdjustRepeat= m_pWorkZone->AdjustRepeat;
			pUpdateSet->m_MqMax = m_pWorkZone->MqMax;
			pUpdateSet->m_Collimator = m_pWorkZone->Collimator;
			pUpdateSet->m_Current = m_pWorkZone->Current;
			pUpdateSet->Update();

		}

		//(2) re-serialize def.ma
		defMaName = fullFpPathName + "\\analysis.xma";

		//(3) re-serialize std.sample
		stdSampleName = fullFpPathName + "\\sample.std";

		pUpdateSet->Close();
		delete pUpdateSet;

	}
	

	//(1)save *.xwz to work-zone directory
	//CString fileName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineWZ\\" + m_pWorkZone->DirName + ".xwz";
	//TiXmlDocument doc;
	//m_pWorkZone->XMLSerialize(TRUE,&doc);
	//doc.SaveFile(fileName);

	//(2)save "sample.std" and "condition.ini" to folder "DefineFP"
	//FP_SOURCE_DATA source;
	//InitSourceData(&source,&m_WorkZone);

	//FP_SAMPLE_DATA sample;
	//InitSampleData(&sample,&m_WorkZone);
	
	/*********************************************************
	CFile file;
	if(file.Open(fileName,CFile::modeWrite|CFile::modeCreate))
	{
		CArchive ar(&file,CArchive::store);
		m_pWorkZone->Serialize(ar);
		ar.Close();
		file.Close();
	}
	*********************************************************/
	
	//CDialog::EndDialog(TRUE);
	CDialog::OnOK();
}

void CCreateWorkZoneDlg::OnSelchangeWzTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_tabCtrl.GetCurSel();
	switch(CurSel)
	{
	case 0:
		m_productTab.ShowWindow(TRUE);
		m_appTab.ShowWindow(FALSE);
		m_unitTab.ShowWindow(FALSE);
		break;
	case 1:
		m_productTab.ShowWindow(FALSE);
		m_appTab.ShowWindow(TRUE);
		m_unitTab.ShowWindow(FALSE);
		break;
	case 2:
		m_productTab.ShowWindow(FALSE);
		m_appTab.ShowWindow(FALSE);
		m_unitTab.ShowWindow(TRUE);
		break;
	default:
		break;
	}
	
	*pResult = 0;
}

void CCreateWorkZoneDlg::OnAccept() 
{
	CDialog::EndDialog(TRUE);	
}

void CCreateWorkZoneDlg::OnExit() 
{
	CDialog::EndDialog(TRUE);	
}


void CCreateWorkZoneDlg::OnCancel() 
{
	// TODO: Add your message handler code here and/or call default
	if(AfxMessageBox("Exit Creating WorkZone Process?",MB_OKCANCEL)==IDOK)
		CDialog::OnCancel();
}