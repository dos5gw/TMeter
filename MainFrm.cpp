// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TMeter.h"

#include "MainFrm.h"
#include "MdiMeasureView.h"
#include "ConditionMonitorDlg.h"
#include "StbTestDlg.h"

#include "MdiCalibDoc.h"
#include "MdiCalibView.h"

#include "CreateWorkZoneDlg.h"
#include "StdSampleDlg.h"
#include "WorkZoneListDlg.h"
#include "DefMAListDlg.h"
#include "SetMeasHeadParamDlg.h"
#include "EnergyCalibDlg.h"
//#include "PureIntensityDlg.h"
//#include "dvrsdk.h"

#define MEASURE_MENU_INDEX 3  //m_pMeasureTemplate
#define SPECTRUM_MENU_INDEX 4 //m_pMeasureTemplate
#define SCALE_MENU_INDEX 2    //m_pSpectrumTemplate

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_DAEMON, OnOpenDaemonDlg)
	ON_COMMAND(ID_MENU_OPENTUBE, OnClickOpenTube)
	ON_COMMAND(ID_MENU_CLOSETUBE, OnClickCloseTube)
	ON_COMMAND(ID_MENU_OPENSPEC, OnClickOpenSpec)
	ON_COMMAND(ID_MENU_SAVESPEC, OnClickSaveSpec)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_AUTOSAVE, OnClickAutoSave)
	ON_COMMAND(ID_MENU_AUTOSORT, OnClickAutoSort)
	ON_COMMAND(ID_MENU_OPENSPECVIEW, OnSwitchSpectrumView)
	ON_COMMAND(ID_MENU_EXITSPEC, OnSwitchMeasureView)
	ON_COMMAND(ID_MENU_STB, OnClickStbTest)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTOSAVE, OnUpdateAutoSave)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTOSORT, OnUpdateAutoSort)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_MENU_PUREELEMENT, OnClickTestFunction)
	ON_COMMAND(ID_MENU_CREATEWZ, OnCreateWorkZone)
	ON_COMMAND(ID_MENU_OPENWZ, OnModifyWorkZone)
	ON_COMMAND(ID_MENU_SWITCHWZ, OnSwitchWorkZone)
	ON_UPDATE_COMMAND_UI(ID_MENU_FILL, OnUpdateFillSpec)
	ON_COMMAND(ID_MENU_FILL, OnClickFillSpec)
	ON_COMMAND(ID_MENU_NORMALIZE, OnMenuNormalize)
	ON_COMMAND(ID_MENU_CORRECT, OnMenuCorrect)
	ON_COMMAND(ID_MENU_CORRECTBASE, OnMenuCorrectBase)
	ON_COMMAND(ID_MENU_CORRECTSTD, OnMenuCorrectStd)
	ON_COMMAND(ID_MENU_PARAM, OnSetMeasHeadParam)
	ON_COMMAND(ID_MENU_XYZ, OnSetXYZParam)
	ON_COMMAND(ID_MENU_CROSS, OnSetCrossParam)
	ON_COMMAND(ID_MENU_ENERGY_CALIB, OnMenuEnergyCalib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	pMDIFrameMenu=NULL;
	pSpectrumSubMenu=NULL;
	pMeasureSubMenu =NULL;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	} 

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable (toolbar can't be moved) @2012-06-15
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY); 
	//DockControlBar(&m_wndToolBar);

	// set mdi menu
	//pMDIFrameMenu = CMenu::FromHandle(((CTMeterApp*)AfxGetApp())->m_pMeasureTemplate->m_hMenuShared);
	//pMDIFrameMenu->GetSubMenu(MEASURE_MENU_INDEX)->CheckMenuItem(ID_MENU_AUTOSORT,MF_BYCOMMAND|MF_UNCHECKED);
	//pMDIFrameMenu->GetSubMenu(SPECTRUM_MENU_INDEX)->CheckMenuItem(ID_MENU_AUTOSAVE,MF_BYCOMMAND|MF_UNCHECKED);
	
	//pMDIFrameMenu = CMenu::FromHandle(((CTMeterApp*)AfxGetApp())->m_pSpectrumTemplate->m_hMenuShared);
	//pMDIFrameMenu->GetSubMenu(SCALE_MENU_INDEX)->CheckMenuItem(ID_MENU_AUTOSCALE,MF_BYCOMMAND|MF_CHECKED);
	//pMDIFrameMenu->GetSubMenu(SCALE_MENU_INDEX)->CheckMenuItem(ID_MENU_USERSCALE,MF_BYCOMMAND|MF_CHECKED);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style&=~WS_MAXIMIZEBOX; //禁用最大化按钮
	//cs.style&=~WS_MINIMIZEBOX;
	//cs.style&=~WS_THICKFRAME;  //禁止调整窗口大小
	//cs.cx=800; //固定窗口宽度
	//cs.cy=600; //固定窗口高度（包括菜单栏等）

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnOpenDaemonDlg() 
{
	CConditionMonitorDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnClickOpenTube() 
{
	g_pMeasureView->WarmUpXTube();
}

void CMainFrame::OnClickCloseTube() 
{
	g_pMeasureView->CoolDownXTube();
}

void CMainFrame::OnClickOpenSpec() 
{
	g_pMeasureView->OpenSpectrumFiles();
}

void CMainFrame::OnClickSaveSpec() 
{
	g_pMeasureView->SaveSpectrumFiles();
}

void CMainFrame::OnClickAutoSort() 
{
	g_pMeasureView->m_bAutoSortResult = !g_pMeasureView->m_bAutoSortResult;
	/*
	pMDIFrameMenu = CMenu::FromHandle(((CTMeterApp*)AfxGetApp())->m_pMeasureTemplate->m_hMenuShared);
	if(g_pMeasureView->m_bAutoSortResult)
		pMDIFrameMenu->GetSubMenu(MEASURE_MENU_INDEX+1)->CheckMenuItem(ID_MENU_AUTOSORT,MF_BYCOMMAND|MF_CHECKED);
	else
		pMDIFrameMenu->GetSubMenu(MEASURE_MENU_INDEX+1)->CheckMenuItem(ID_MENU_AUTOSORT,MF_BYCOMMAND|MF_UNCHECKED);
	*/
}

void CMainFrame::OnClickAutoSave() 
{
	g_pMeasureView->m_bAutoSaveSpec = !g_pMeasureView->m_bAutoSaveSpec;
	/*
	pMDIFrameMenu = CMenu::FromHandle(((CTMeterApp*)AfxGetApp())->m_pMeasureTemplate->m_hMenuShared);
	if(g_pMeasureView->m_bAutoSaveSpec)
		pMDIFrameMenu->GetSubMenu(SPECTRUM_MENU_INDEX+1)->CheckMenuItem(ID_MENU_AUTOSAVE,MF_BYCOMMAND|MF_CHECKED);
	else
		pMDIFrameMenu->GetSubMenu(SPECTRUM_MENU_INDEX+1)->CheckMenuItem(ID_MENU_AUTOSAVE,MF_BYCOMMAND|MF_UNCHECKED);
	*/
}

void CMainFrame::OnClickFillSpec() 
{
	// TODO: Add your command handler code here
	g_pSpectrumView->OnFillSpectrum();
	
}


void CMainFrame::OnUpdateAutoSort(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_pMeasureView->m_bAutoSortResult)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateAutoSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_pMeasureView->m_bAutoSaveSpec)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateFillSpec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_pSpectrumView->m_SpectrumDisplay.m_beNeedFilled)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnSwitchSpectrumView() 
{
	if(g_pSpectrumView != NULL)
	{
		g_pSpectrumView->GetParentFrame()->ActivateFrame();
	}
	//MDIActivate(g_pSpectrumView->GetParentFrame());
}

void CMainFrame::OnSwitchMeasureView() 
{
	if(g_pMeasureView != NULL)
	{
		g_pMeasureView->GetParentFrame()->ActivateFrame();
		g_pSpectrumView->CleanContrastSpec();
	}
}

void CMainFrame::OnClickStbTest() 
{
	// TODO: Add your command handler code here
	//CStbTestDlg dlg;
	//dlg.DoModal();
	CStbTestDlg *pDlg = new CStbTestDlg();
	pDlg->Create(IDD_STBTESTDLG,this);
	pDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnClickTestFunction() // function test
{
	g_pMeasureView->CalcPureIntensity();
	//g_pMeasureView->CalibrateWorkZone();
	g_pMeasureView->CalculateQuantify();
	g_pMeasureView->AddNewResultToArray();
}

void CMainFrame::OnCreateWorkZone() 
{
	CString defMaName;
	WorkZoneDef *pNewWz = new WorkZoneDef();

	// (1) Select one def.ma
	CDefMAListDlg maListDlg; 
	if(maListDlg.DoModal()==IDOK) 
		defMaName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineMA\\" + maListDlg.defMaName;
	else {
		delete pNewWz;
		return;	
	}

	// (2) Loading def.ma from file
	TiXmlDocument doc(defMaName);
	if(!doc.LoadFile())
	{
		CString text;
		text.Format("Def.MA载入失败, 请检查文件%s",defMaName);
		AfxMessageBox(text);
		delete pNewWz;
		return;
	}
	else pNewWz->defMa.XMLSerialize(FALSE,&doc); 

	// (3) Input work-zone tag
	CWorkZoneListDlg wzListDlg;
	wzListDlg.m_wzTag = maListDlg.defMaTag; // Def.MA name as default work-zone tag
	if(wzListDlg.DoModal()==IDOK){
		pNewWz->SetTag(wzListDlg.m_wzTag);
	}
	else {
		delete pNewWz;
		return;
	}
	
	// (4) Define work-zone
	CCreateWorkZoneDlg wzDlg;
	wzDlg.LinkValue(TRUE,pNewWz);
	if(wzDlg.DoModal()==IDCANCEL) {// CCreateWorkZoneDlg::OnOK(), save def.ma to sub-folder, and save work-zone basic infomation to database
		delete pNewWz;
		return; 
	}

	// (4) Select Collimator

	// (5) Define standard sample
	CStdSampleDlg stdDlg;
	stdDlg.LinkValue(TRUE,pNewWz);
	if(stdDlg.DoModal()==IDCANCEL) {// CStdSampleDlg::OnOK(), save standard to sub-folder
		delete pNewWz;
		return;
	}

	// (6) Standard Samples Calibrate View 
	SwitchCalibrateView(TRUE,TRUE,pNewWz);
}

void CMainFrame::SwitchCalibrateView(BOOL NeedStd,BOOL beNewWz,WorkZoneDef *pWz)
{
	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	//pApp->SwitchCalibrateView(beNeedStd,beNewWz,pWz);

	pApp->m_pCalibTemplate->OpenDocumentFile(NULL);
	// then CMdiCalibView::CMdiCalibView(), CMdiCalibView::OnInitialUpdate() will run 
	
	POSITION pos;
	pos=pApp->m_pCalibTemplate->GetFirstDocPosition();
	CMdiCalibDoc *pDoc=(CMdiCalibDoc*)(pApp->m_pCalibTemplate)->GetNextDoc(pos);
	pos=pDoc->GetFirstViewPosition();
	CMdiCalibView *pCalibView=(CMdiCalibView*)pDoc->GetNextView(pos); 
	
	pCalibView->beNeedStd = NeedStd;
	pCalibView->m_beCalibNewWz = beNewWz;
	pCalibView->m_pCalibWz = pWz;
	pCalibView->CalibrateView(NeedStd,beNewWz,pWz);	
}

// 当前曲线-归一化
void CMainFrame::OnMenuNormalize() 
{
	if(((CTMeterApp*)AfxGetApp())->m_pWorkZone == NULL)
	{
		AfxMessageBox("没有打开任何工作曲线");
		return;
	}
	// Measure Standard is not necessary for Normalizing
	SwitchCalibrateView(FALSE,FALSE,((CTMeterApp*)AfxGetApp())->m_pWorkZone);
	
}

// 当前曲线-调校
void CMainFrame::OnMenuCorrect() 
{
	if(((CTMeterApp*)AfxGetApp())->m_pWorkZone == NULL)
	{
		AfxMessageBox("没有打开任何工作曲线");
		return;
	}
	// Measure Standard is necessary for Calibrating
	SwitchCalibrateView(TRUE,FALSE,((CTMeterApp*)AfxGetApp())->m_pWorkZone);
	
}

// 当前曲线-基材校正
void CMainFrame::OnMenuCorrectBase() 
{
	// TODO: Add your command handler code here
	
}

// 修改当前曲线
void CMainFrame::OnModifyWorkZone() 
{
	g_pMeasureView->OnEditWz();
}

// 切换当前曲线
void CMainFrame::OnSwitchWorkZone() 
{
	g_pMeasureView->OnSwitchWz();
}


// 当前曲线-调校标样片
void CMainFrame::OnMenuCorrectStd() 
{
	WorkZoneDef *ptr = ((CTMeterApp*)AfxGetApp())->m_pWorkZone;
	if(!ptr)
	{
		AfxMessageBox("没有打开任何工作曲线");
		return;
	}

	CStdSampleDlg stdDlg;
	stdDlg.LinkValue(FALSE,ptr);
	stdDlg.DoModal();
}



void CMainFrame::OnMenuEnergyCalib() 
{
	g_SpectrumInfo.SetTotalSampleTimes(2); // 20 sec
	g_pMeasureView->SendSystemParameter();

	EnergyCalibDlg dlg;
	dlg.DoModal();
}



/*********************************************************/
/*       Menu - Instrument Setting - Measure Param       */
/*********************************************************/

void CMainFrame::OnSetMeasHeadParam() 
{
	CSetMeasHeadParamDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnSetXYZParam() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnSetCrossParam() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	//lpMMI-> ptMinTrackSize.x = 800; 
	//lpMMI-> ptMinTrackSize.y = 600; 
	CFrameWnd::OnGetMinMaxInfo(lpMMI); 
}

void CMainFrame::OnClose() 
{
	if(AfxMessageBox("This will exit the application. Are you sure?",MB_YESNO)==IDYES) 
	{
		((CTMeterApp*)AfxGetApp())->TryCloseAllDoc();
		if(!((CTMeterApp*)AfxGetApp())->HasDocUnclosed()) 
		{
			CMDIFrameWnd::OnClose();
		}
	}
}



