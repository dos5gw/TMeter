// PureIntensityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "PureIntensityDlg.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "SpectrumInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

CPureIntensityDlg::CPureIntensityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPureIntensityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPureIntensityDlg)
	m_ElementName = _T("");
	m_Left = 10;
	m_Right = 245;
	//}}AFX_DATA_INIT

	m_bTesting=FALSE;
	m_nCheckIndex=0;
}

void CPureIntensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPureIntensityDlg)
	DDX_Control(pDX, IDC_HV, m_HvComboBox);
	DDX_Control(pDX, IDC_PURE, m_ListControl);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_ELENAME, m_ElementName);
	DDX_Text(pDX, IDC_LEFT, m_Left);
	DDV_MinMaxInt(pDX, m_Left, 0, 255);
	DDX_Text(pDX, IDC_RIGHT, m_Right);
	DDV_MinMaxInt(pDX, m_Right, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPureIntensityDlg, CDialog)
	//{{AFX_MSG_MAP(CPureIntensityDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SETCFG, OnSetCfg)
	ON_NOTIFY(NM_CLICK, IDC_PURE, OnClickPureList)
	ON_BN_CLICKED(IDC_SAVEPUREWIN, OnSavePureWindows)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPureIntensityDlg::LoadSettings()
{
	TiXmlDocument doc("PureElenemtList.xml");
	if(!doc.LoadFile()) return FALSE;

	TiXmlElement* root = doc.RootElement(); // <Element>

	TiXmlElement* OneElement;
	TiXmlAttribute* ElementAttribute;
	CString ElementName;
	int Left, Right;
	CString test;
	
	OneElement = root->FirstChildElement(); //Elem
	
	while(OneElement)
	{
		ElementAttribute = OneElement->FirstAttribute();
		test = ElementAttribute->Name();   //Name
		ElementName = ElementAttribute->Value();
		ElementAttribute = ElementAttribute->Next();
		
		test = ElementAttribute->Name();   //Left
		Left = ElementAttribute->IntValue();
		ElementAttribute = ElementAttribute->Next();
		
		test = ElementAttribute->Name();   //Right
		Right = ElementAttribute->IntValue();
		ElementAttribute = ElementAttribute->Next();
		
		OnePureElement* pEle=new OnePureElement(ElementName,Left,Right);
		pureElementList.Add(pEle);
		
		OneElement = OneElement->NextSiblingElement();
	}
	return TRUE;
}

BOOL CPureIntensityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!LoadSettings())
		return FALSE;
	m_Progress.SetRange(0,100);
	
	//m_pReport=new CGridCtrl();
	DWORD exstyle = m_ListControl.GetExtendedStyle();
	m_ListControl.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	CRect rect;
	m_ListControl.GetClientRect(&rect);
    int nColInterval = rect.Width()/3;
	
	m_ListControl.InsertColumn(0, "元素", LVCFMT_LEFT, nColInterval);
	m_ListControl.InsertColumn(1, "左边界",LVCFMT_LEFT, nColInterval);
	m_ListControl.InsertColumn(2, "右边界",LVCFMT_LEFT, nColInterval);
	//m_ListControl.InsertColumn(3, "管压",LVCFMT_LEFT, nColInterval);

	CString text;
	int sum = pureElementList.GetSize();
	for(int i=0;i<sum;i++)
	{	
		OnePureElement* pEle = pureElementList.GetAt(i);
		m_ListControl.InsertItem(i,pEle->Name);
		text.Format("%d",pEle->left);
		m_ListControl.SetItemText(i,1,text);
		text.Format("%d",pEle->right);
		m_ListControl.SetItemText(i,2,text);
		//text.Format("%f",pEle->right);
		m_ListControl.SetItemText(i,3,"0.0");
	}
	m_HvComboBox.AddString("30");
	m_HvComboBox.AddString("40");
	m_HvComboBox.AddString("50");
	m_HvComboBox.SetCurSel(0);

	UpdateData(FALSE);

	SetTimer(0,100,NULL);
	return TRUE;  
}

int CPureIntensityDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CPureIntensityDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CPureIntensityDlg::OnCancel()
{
	if(AfxMessageBox("Exit Pure Element Measurement?",MB_OKCANCEL)==IDOK)
	{
		//SaveElementWindows();
		DestroyWindow();
	}
}

void CPureIntensityDlg::OnClose() 
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp(); 
	if(m_bTesting)
	{
		CString text;
		text.LoadString(IDS_INFO_OCUUPY_STBWARN);
		if(AfxMessageBox(text,MB_OKCANCEL)==IDOK)
		{
			CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
			pApp->m_Instrument.SetOccupied(FALSE,"NONE");
		}
		else
		{
			return;
		}
	}
	CDialog::OnClose();
}



void CPureIntensityDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(0);
	CleanPureElementList();
}

void CPureIntensityDlg::PostNcDestroy() 
{
	delete this;
	CDialog::PostNcDestroy();
}

void CPureIntensityDlg::OnSetCfg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Left>m_Right) 
		m_Right=m_Left;
	g_SpectrumInfo.m_baseline=m_Left;
	g_SpectrumInfo.m_topline=m_Right;

	OnePureElement* pEle = pureElementList.GetAt(m_nCheckIndex);
	pEle->left=m_Left;
	pEle->right=m_Right;
	UpdateData(FALSE);
	g_pSpectrumView->m_SpectrumDisplay.Update();	
}

void CPureIntensityDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_HvComboBox.GetLBText(m_HvComboBox.GetCurSel(),CurrentHv);
	CurrentEle = m_ElementName;
	CString text = "将要测定纯元素" + CurrentEle + ", 管压" + CurrentHv + "HV" + ", 是否确定?";
	if(AfxMessageBox(text,MB_OKCANCEL)==IDCANCEL)
	{
		return;
	}
	else
	{

	}
}

void CPureIntensityDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	
}

void CPureIntensityDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	
}

void CPureIntensityDlg::OnClickPureList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex; 
	HD_NOTIFY *phdn = (HD_NOTIFY*)pNMHDR; 
	nIndex = phdn-> iItem; 
	if(nIndex!=-1)
	{
		m_nCheckIndex = nIndex;
		m_ElementName= m_ListControl.GetItemText(m_nCheckIndex,0);
		m_Left = atoi(m_ListControl.GetItemText(m_nCheckIndex,1));
		m_Right= atoi(m_ListControl.GetItemText(m_nCheckIndex,2));
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CPureIntensityDlg::OnSavePureWindows() 
{
	// TODO: Add your control notification handler code here
		HINSTANCE hDllInst = LoadLibrary("Fack.dll");
		//if(hDllInst) return;

		SETELEMENTWINDOWS SetElementWindows = (SETELEMENTWINDOWS)GetProcAddress(hDllInst,"_SetElementsWindow@8");
		SETELEMENTDATA SetPureElementsData  = (SETELEMENTDATA)GetProcAddress(hDllInst,"_SetPureElementsData@8");
		if(SetElementWindows==NULL ||SetPureElementsData==NULL) return;
		
		CSpectrumInfo<DWORD,SPEC_SIZE> specInfo;
		int i,j;
		OnePureElement* pEle;
		CString eleName,specFileName;

		int length = specInfo.maxsize;
		int no = pureElementList.GetSize();
		int* win = new int[no*2];
		double* datas= new double[no*length];
		for (i=0;i<no;i++)
		{
			pEle = pureElementList.GetAt(i);
			eleName = pEle->Name;
			specFileName = "PureIntensity\\" + eleName + ".spc";
			specInfo.LoadSpectrumFromBinary(specFileName);
			
			win[i*2]=specInfo.m_baseline;
			win[i*2+1]=specInfo.m_topline;
			for(j=0;j<length;j++)
			{
				datas[length*i+j] = specInfo.GetCountClone(j);
			}
		}
		SetElementWindows(no,win);
		SetPureElementsData(length,datas);

		delete[] win;
		delete[] datas;
		FreeLibrary(hDllInst);
}

void CPureIntensityDlg::CleanPureElementList()
{
	if(pureElementList.IsEmpty() != TRUE) 
	{
		int no=pureElementList.GetSize();
		for(int i=0;i<no;i++) 
			delete pureElementList.GetAt(i);
		pureElementList.RemoveAll();
	}
}


