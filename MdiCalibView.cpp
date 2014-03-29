// MdiCalibView.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "MdiCalibView.h"
#include "TimeTickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibView

IMPLEMENT_DYNCREATE(CMdiCalibView, CFormView)

CMdiCalibView::CMdiCalibView()
	: CFormView(CMdiCalibView::IDD)
{
	//{{AFX_DATA_INIT(CMdiCalibView)
	//}}AFX_DATA_INIT
	m_pCalibWz = NULL;
	m_pCurrTask = NULL;
	m_beCalibNewWz = FALSE;
	m_bInitialized = FALSE;
	m_beCalibFinished = FALSE;
	m_beCalibing = FALSE;
	m_pInstrument = NULL;
}

CMdiCalibView::~CMdiCalibView()
{
}

void CMdiCalibView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdiCalibView)
	DDX_Control(pDX, IDC_CALIBTITLE, m_BtnWorkZoneTitle);
	DDX_Control(pDX, IDC_CALIBVIDEO, m_VideoDisplay);
	DDX_Control(pDX, IDC_CALIBSTATISTIC_LIST, m_StatisList);
	DDX_Control(pDX, IDC_CALIB_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_CALIB_START, m_BtnStart);
	DDX_Control(pDX, IDC_CALIBARRAY_LIST, m_ArrayList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdiCalibView, CFormView)
	//{{AFX_MSG_MAP(CMdiCalibView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_QUITCALIB, OnQuitCalib)
	ON_COMMAND(ID_MENU_CALIB_REDO, OnCalibRedo)
	ON_COMMAND(ID_MENU_CALIB_LOAD, OnCalibLoadFromFile)
	ON_COMMAND(ID_MENU_CALIB_SKIP, OnCalibSkip)
	ON_BN_CLICKED(IDC_CALIB_START, OnBtnStart)
	ON_BN_CLICKED(IDC_CALIB_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibView diagnostics

#ifdef _DEBUG
void CMdiCalibView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMdiCalibView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibView message handlers

void CMdiCalibView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_BtnStart.SetIcon(IDI_START);
	m_BtnStart.SetFlat(FALSE);
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetFlat(FALSE);
	m_BtnWorkZoneTitle.Use3D(FALSE); 

	//m_pApp = (CTMeterApp*)AfxGetApp(); assert(m_pApp);
	//m_pInstrument = &(m_pApp->m_Instrument);
	m_pInstrument = &(((CTMeterApp*)AfxGetApp())->m_Instrument);

	// Initialize list control

	// Set Style for List Control
	DWORD exstyle = m_ArrayList.GetExtendedStyle();
	m_ArrayList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	UpdateCpsArrayList();

	// Show windows
	m_bInitialized=TRUE;
	GetParentFrame()->ShowWindow(SW_MAXIMIZE);
	GetDocument()->SetTitle("X-Ray Thickness Meter: Calibrate View");

	SetTimer(0,200,NULL);
}

int CMdiCalibView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/* TODO: Add your specialized creation code here
	HMENU hMenu; 
	hMenu = ::GetSystemMenu(this-> m_hWnd,FALSE); 
	EnableMenuItem(hMenu,SC_CLOSE|MF_BYCOMMAND,MF_GRAYED);
	*/
	return 0;
}

void CMdiCalibView::UpdateCpsArrayList()
{
	m_ArrayList.InsertColumn(0,"标样",LVCFMT_LEFT,100);
	m_ArrayList.InsertColumn(1,"Cps",LVCFMT_LEFT,180);
}

void CMdiCalibView::AddNewCpsToArray()
{
	CString text;
	int itemCount = m_ArrayList.GetItemCount();

	text.Format("%s",m_pCurrTask->m_Tag);
	m_ArrayList.InsertItem(itemCount,text);

	text.Format("%5.2lf",g_SpectrumInfo.GetCps());
	m_ArrayList.SetItemText(itemCount,1,text);

}

void CMdiCalibView::CalibrateView(BOOL beNeedStd,BOOL beNew,WorkZoneDef* pWz)
{
	CString title;
	if(beNeedStd) // Calibrate mode, Need Measure Standard Samples
	{
		title.Format("调校 ID:%d %s",pWz->GetDirIndex(),pWz->GetTag());
		m_calibMode = CALIBRATE;
	} 
	else         // Normalizing mode
	{
		title.Format("归一化 ID:%d %s",pWz->GetDirIndex(),pWz->GetTag());
		m_calibMode = NORMALIZE;
	}
	m_BtnWorkZoneTitle.SetWindowText(title);
	
	// if m_beCalibNewWz==TURE, calibrate new created work-zone, must delete this new work-zone after finishing calibrating
	m_pCalibWz = pWz;
	m_beCalibNewWz = beNew; 
	
	// send parameter to dct
	ChangeMeasureCondition(pWz);
	
	// reload pure elements list for work-zone to be calibrated (measurement task queue)
	((CTMeterApp*)AfxGetApp())->ReloadPureElemList(pWz);
	
	CreateCalibTaskTrack(beNeedStd);
	
	//SetTimer(2,200,NULL);
}

void CMdiCalibView::CreateCalibTaskTrack(BOOL beNeedStd)
{
	COneTask* pOneTask;
	OnePureElement* pOnePure = NULL;
	CStandardSample* pOneStd = NULL;
	CString tag;
	
	// add measure [pure element] task
	int nPureElems = ((CTMeterApp*)AfxGetApp())->m_IntensityCalculator.pureElementList.GetSize();
	for(int i=0;i<nPureElems;i++)
	{
		pOnePure = ((CTMeterApp*)AfxGetApp())->m_IntensityCalculator.pureElementList.GetAt(i);
		pOneTask = new COneTask(1,0,pOnePure->Name,i);
		m_TaskTrack.Add(pOneTask);
	}
	
	// add measure [standard] and [calib] task
	if(beNeedStd==TRUE)
	{
		int nStds = m_pCalibWz->stdSample.GetSize();
		for(int j=0;j<nStds;j++)
		{
			pOneStd = m_pCalibWz->stdSample.GetAt(j);
			
			tag.Format("%s %d",pOneStd->m_tag,j+1);
			pOneTask = new COneTask(2,0,tag,j);
			m_TaskTrack.Add(pOneTask);
			
			//pOneTask = new COneTask(3,0,"Calculate Intensity"); //  Calculate Intensity, re-save Standard
			//m_TaskTrack.Add(pOneTask);
		}
		pOneTask = new COneTask(4,0,"Calibrate",0); // Calibrate work-zone, re-build temp.cal
		m_TaskTrack.Add(pOneTask);
	}
	
}

void CMdiCalibView::OnCalibLoadFromFile() 
{
	// TODO: Add your command handler code here
	BOOL loadSuccess = FALSE;
	CString calibSpecFile;
	CString specFolder = m_pCalibWz->GetFullSpectrumPath();
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"Spectrum Binary(.spc)|*.spc||");
	dlg.m_ofn.lpstrInitialDir = specFolder;
	if(dlg.DoModal() == IDOK)
	{
		calibSpecFile = dlg.GetPathName();
		loadSuccess = g_SpectrumInfo.LoadSpectrumFromBinary(calibSpecFile);
	}

	assert(m_pCurrTask);

	m_beCalibing = TRUE;

	if(m_pCurrTask->m_Type == 1){
		m_pCurrTask->m_Executed = TRUE;
		AddNewCpsToArray();
	}
	else if(m_pCurrTask->m_Type == 2){
		m_pCurrTask->m_SampleFinished = TRUE;
	}
	
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory); // @2012-09-03
}

void CMdiCalibView::OnCalibRedo() 
{
	m_pCurrTask = m_TaskTrack.GetPreTask();
	m_pCurrTask->m_Executed = FALSE;
	m_pCurrTask->m_SampleFinished = FALSE;

	g_pMeasureView->ResetDataReady();
}

void CMdiCalibView::OnCalibSkip() 
{
	m_pCurrTask = m_TaskTrack.GetNextTask();
	m_pCurrTask->m_Executed = FALSE;
	m_pCurrTask->m_SampleFinished = FALSE;

	g_pMeasureView->ResetDataReady();
}

void CMdiCalibView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//if(!(m_TaskTrack.ReachTail()) && (m_pCurrTask==NULL))
	
	if(m_beCalibFinished) return;
	int percent;
	m_pCurrTask = m_TaskTrack.GetCurrentTask(); 
	UpdateCalibTip(m_pCurrTask);
	
	if(m_beCalibing){
		if(m_pCurrTask->m_SampleFinished == FALSE) // avoid changing m_SampleFinished which has been changed in OnCalibLoadFromFile()
			m_pCurrTask->m_SampleFinished = g_pMeasureView->SpectrumDataReady(percent);
		
		if(m_pCurrTask->m_Executed == FALSE)
		{
			ExecuteTask(m_pCurrTask);
		}
		else
		{
			if(m_TaskTrack.ReachTail())
			{
				//PreExitCalibView(); return; // automatic exit calibrate view
				
				m_beCalibFinished = TRUE;
				CString text;
				if(m_calibMode == NORMALIZE) text.Format("归一化已完成");
				else if(m_calibMode == CALIBRATE) text.Format("调校已完成");
				AfxMessageBox(text);
			}
			else
			{
				m_pCurrTask = m_TaskTrack.GetNextTask();
				if(m_pCurrTask->m_Type != 4)
					m_beCalibing = FALSE;
			}
		}
		
	}
	CFormView::OnTimer(nIDEvent);
}

void CMdiCalibView::ExecuteTask(COneTask* pTask)
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	//assert(pTask);

	switch(pTask->m_Type){

	case 1:   /** Pure Element **/
		if(pTask->m_SampleFinished)
		{
			CString specPath;
			specPath.Format("_%s.spc",m_pCurrTask->m_Tag);
			specPath = m_pCalibWz->GetFullSpectrumPath() + "\\" + specPath;
			g_SpectrumInfo.SaveSpectrumToBinary(specPath);
			pTask->m_Executed = TRUE;
			AddNewCpsToArray();
		}
		
		break;

	case 2:  /** Standard Sample **/
		if(pTask->m_SampleFinished)
		{
			pApp->m_IntensityCalculator.LoadPureSpectrum(m_pCalibWz);
			pApp->m_IntensityCalculator.CalcPureIntensity(g_SpectrumInfo); 
			m_pCalibWz->stdSample.SetIntensity(pTask->m_Index);
			pTask->m_Executed = TRUE;
			AddNewCpsToArray();
		}
		break;

	case 4:  /** Calibrate Work-Zone **/
		m_pCalibWz->SaveStdSample();
		m_pCalibWz->FpCalibrate();
		pTask->m_Executed = TRUE;
		break;
	}
	
}

void CMdiCalibView::UpdateCalibTip(COneTask* pTask)
{
	CString text;

	switch(pTask->m_Type){
	case 1:
		text.Format("纯元素 %s",pTask->m_Tag);
		break;
	case 2:
		text.Format("标样片 %s",pTask->m_Tag);
		break;
	case 3:
		break;
	case 4:
		break;
	}

	GetDlgItem(IDC_CALIBLASTRESULT)->SetWindowText(text);
}

void CMdiCalibView::OnBtnStart() 
{
	if(m_pInstrument->CheckOccupied())
	{
		return;
	}
	m_beCalibing = TRUE;
	g_pMeasureView->OnMeasure();
	CTimeTickDlg *pDlg = new CTimeTickDlg();
	pDlg->Create(IDD_TIMETICKDLG,this);
	pDlg->ShowWindow(SW_SHOW);	
}

void CMdiCalibView::OnBtnStop() 
{
	m_beCalibing = FALSE;
	g_pMeasureView->OnStopSample();
}

void CMdiCalibView::OnQuitCalib() 
{
	// TODO: Add your command handler code here
	if(!m_beCalibFinished) 
		if(AfxMessageBox("Exit Calibrating Process?",MB_OKCANCEL)==IDCANCEL) return;
		
	PreExitCalibView();
}

void CMdiCalibView::PreExitCalibView()
{
	m_beCalibFinished = TRUE; 

	g_pMeasureView->UpdateWzTile();
	
	//return to measurement view, delete new work-zone, 
	if(m_beCalibNewWz) delete m_pCalibWz;
	
	// send parameter to dct
	ChangeMeasureCondition(((CTMeterApp*)AfxGetApp())->m_pWorkZone);
	
	// reload pure elements list from current opened work-zone
	((CTMeterApp*)AfxGetApp())->LoadWorkZonePureElem();
	
	// Clean m_TaskTrack and m_pCurrTask
	CleanTaskTrack();
	
	// FILEDSN=./TMeterDataLink.DSN
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);
	
	// Close MDI Document
	(GetDocument())->OnCloseDocument();
}

void CMdiCalibView::CleanTaskTrack()
{
	/*
	if(m_pCurrTask) 
	{
		delete m_pCurrTask;
		m_pCurrTask=NULL;
	}*/
	
	//if(!m_TaskTrack.IsEmpty()) 
	int tno=m_TaskTrack.GetSize();
	for(int i=1;i<tno;i++) 
		delete m_TaskTrack.GetAt(i);
	m_TaskTrack.RemoveAll();
	
}

void CMdiCalibView::ChangeMeasureCondition(WorkZoneDef* pWz)
{
	g_pMeasureView->ChangeMeasureCondition(pWz);
}

void CMdiCalibView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_bInitialized) return;
	CRect clientRect,btnRect;
	CRect rectWzName,rectVideo;
	CRect rectArrayList,rectStatistic,rectResultText;
	int btnHeight,btnWidth;
	int clientHeight,clientWidth;
	int yRectSpace = 10, xRectSpace=10;
	GetClientRect(&clientRect);
	clientHeight=clientRect.Height();
	clientWidth =clientRect.Width();
	
	/*GetDlgItem(IDC_SWITCHWZ)->GetWindowRect(&btnRect);
	btnHeight=btnRect.Height();
	btnWidth=clientRect.Width()*7/60;
	btnRect.left = clientRect.left + xRectSpace;
	btnRect.right= btnRect.left + btnWidth;
	btnRect.top  = clientRect.top + yRectSpace;
	btnRect.bottom=btnRect.top + btnHeight;
	GetDlgItem(IDC_SWITCHWZ)->MoveWindow(&btnRect); */
	
	GetDlgItem(IDC_CALIBTITLE)->GetWindowRect(&btnRect);
	btnHeight=btnRect.Height();
	rectWzName.left = clientRect.left + xRectSpace;
	rectWzName.right = clientRect.right - xRectSpace;
	rectWzName.top  = clientRect.top + yRectSpace;
	rectWzName.bottom=rectWzName.top + btnHeight;
	GetDlgItem(IDC_CALIBTITLE)->MoveWindow(&rectWzName);
	
	btnHeight = btnWidth = (clientHeight-xRectSpace*4-rectWzName.Height())*1/2;
	rectVideo.top   =rectWzName.bottom+yRectSpace*2;
	rectVideo.bottom=rectVideo.top + btnHeight;
	rectVideo.right =clientRect.right-xRectSpace;
	rectVideo.left  =rectVideo.right-btnWidth;
	GetDlgItem(IDC_CALIBVIDEO)->MoveWindow(&rectVideo);
	
	btnHeight = (clientHeight-xRectSpace*4-rectWzName.Height())*1/8;
	rectResultText.top  =rectVideo.bottom+yRectSpace;
	rectResultText.left =rectVideo.left;
	rectResultText.right=rectVideo.right;
	rectResultText.bottom=rectResultText.top+btnHeight;
	GetDlgItem(IDC_CALIBLASTRESULT)->MoveWindow(&rectResultText);
	
	btnWidth = (rectResultText.Width()-xRectSpace*4)/2;
	btnHeight = (clientHeight-yRectSpace*4-rectWzName.Height())*1/8;
	if(btnWidth>165) btnWidth=165;
	else if(btnWidth<70) btnWidth=70;
	if(btnHeight>50) btnHeight=50;
	else if(btnHeight<25) btnHeight=25;
	btnRect.right = clientRect.right-xRectSpace;
	btnRect.left = btnRect.right -btnWidth;
	btnRect.bottom = clientRect.bottom-yRectSpace;
	btnRect.top =  btnRect.bottom-btnHeight;
	GetDlgItem(IDC_CALIB_STOP)->MoveWindow(&btnRect);
	
	btnRect.right =btnRect.left-xRectSpace*2;
	btnRect.left  =btnRect.right-btnWidth;
	GetDlgItem(IDC_CALIB_START)->MoveWindow(&btnRect);
	
	btnHeight = (clientHeight-yRectSpace*4-rectWzName.Height())*1/2;
	rectStatistic.top=rectVideo.top;
	rectStatistic.bottom=rectStatistic.top+btnHeight;
	rectStatistic.left = clientRect.left+xRectSpace;
	rectStatistic.right=rectVideo.left-xRectSpace*2;
	GetDlgItem(IDC_CALIBSTATISTIC_LIST)->MoveWindow(&rectStatistic);
	
	rectArrayList.top = rectStatistic.bottom+yRectSpace;
	rectArrayList.bottom=clientRect.bottom-yRectSpace;
	rectArrayList.left  =rectStatistic.left;
	rectArrayList.right =rectStatistic.right;
	GetDlgItem(IDC_CALIBARRAY_LIST)->MoveWindow(&rectArrayList);
}


void CMdiCalibView::OnDestroy() 
{
	CFormView::OnDestroy();
	KillTimer(0);
}





