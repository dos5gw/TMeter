// MdiSpectrumView.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "MdiSpectrumView.h"
#include "AdjustZoneDlg.h"
#include "PeriodicTableDlg.h"
#include "TimeTickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;
/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumView

IMPLEMENT_DYNCREATE(CMdiSpectrumView, CFormView)

CMdiSpectrumView::CMdiSpectrumView()
	: CFormView(CMdiSpectrumView::IDD)
{
	//{{AFX_DATA_INIT(CMdiSpectrumView)
	m_CursorChannel = 0;
	//}}AFX_DATA_INIT
	m_bInitialized=FALSE;
	m_bAutoScale = TRUE;
	m_pPeriodicTableDlg=NULL;
	m_SpectrumDisplay.AttachSpectrumInfo(&g_SpectrumInfo);
	//m_pApp = NULL;
	//m_pImageList=NULL;
}

CMdiSpectrumView::~CMdiSpectrumView()
{
	//if(m_pPeriodicTableDlg)	delete m_pPeriodicTableDlg
}

void CMdiSpectrumView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdiSpectrumView)
	DDX_Control(pDX, IDC_COLOUR_LIST, m_colorListEx);
	DDX_Control(pDX, IDC_CHS_STATIC, m_CurrentChannelInfo);
	DDX_Control(pDX, IDC_ALL_STATIC, m_AllChannelsInfo);
	DDX_Control(pDX, IDC_CHANNEL_SPIN, m_CursorSpin);
	DDX_Control(pDX, IDC_MOVERIGHT, m_BtnMoveRight);
	DDX_Control(pDX, IDC_MOVELEFT, m_BtnMoveLeft);
	DDX_Control(pDX, IDC_SPECTRUMEX, m_SpectrumDisplay);
	DDX_Text(pDX, IDC_CHANNELS, m_CursorChannel);
	DDV_MinMaxInt(pDX, m_CursorChannel, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdiSpectrumView, CFormView)
	//{{AFX_MSG_MAP(CMdiSpectrumView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_MEASURE, OnMeasure)
	ON_BN_CLICKED(IDC_BTN_STB, OnStbSample)
	ON_COMMAND(ID_MENU_YCONTRACT, OnYContract)
	ON_COMMAND(ID_MENU_YEXTEND, OnYExtend)
	ON_COMMAND(ID_MENU_XEXTEND, OnXExtend)
	ON_COMMAND(ID_MENU_XCONTRACT, OnXContract)
	ON_BN_CLICKED(IDC_MOVERIGHT, OnMoveRight)
	ON_BN_CLICKED(IDC_MOVELEFT, OnMoveLeft)
	ON_COMMAND(ID_MENU_SETLINE, OnSetZone)
	ON_COMMAND(ID_MENU_AUTOSCALE, OnAutoScale)
	ON_COMMAND(ID_MENU_FILL, OnFillSpectrum)
	ON_COMMAND(ID_MENU_PERIODICTABLE, OnOpenPeriodicTable)
	ON_EN_CHANGE(IDC_CHANNELS, OnChangeChannels)
	ON_COMMAND(ID_MENU_USERSCALE, OnClickUserScale)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SPECTRUMEX, OnClickSpectrumEx)
	ON_COMMAND(ID_MENU_CLOSEALLSPEC, CleanAllContrastSpec)
	ON_COMMAND(ID_MENU_CLOSESPEC, CleanLastContrastSpec)
	ON_NOTIFY(NM_CLICK, IDC_COLOUR_LIST, OnClickColorList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLOUR_LIST, OnColorListChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumView diagnostics

#ifdef _DEBUG
void CMdiSpectrumView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMdiSpectrumView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumView message handlers

void CMdiSpectrumView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_BtnMoveRight.SetIcon(IDI_MOVER);
	m_BtnMoveLeft.SetIcon(IDI_MOVEL);

	//m_pApp = (CTMeterApp*)AfxGetApp(); assert(m_pApp);
	//m_pInstrument = &(m_pApp->m_Instrument);
	m_pInstrument = &(((CTMeterApp*)AfxGetApp())->m_Instrument);
	
	m_CursorSpin.SetRange(0,255);
    m_CursorSpin.SetBuddy(GetDlgItem(IDC_CHANNELS));

	DWORD exstyle = m_colorListEx.GetExtendedStyle();
	m_colorListEx.SetExtendedStyle(exstyle | LVS_NOCOLUMNHEADER | LVS_EX_CHECKBOXES);
	CRect rect;
	m_colorListEx.GetClientRect(&rect);
    int nColInterval = rect.Width();
	m_colorListEx.InsertColumn(0, "Name", LVCFMT_LEFT,nColInterval/2);
	m_colorListEx.InsertColumn(1, "Color", LVCFMT_LEFT,nColInterval/2);

	m_bInitialized=TRUE;
	//UpdateStaticControls();
	GetParentFrame()->ShowWindow(SW_MAXIMIZE);
	GetDocument()->SetTitle("X-Ray Thickness Meter: Spectrum View");
	
	//set mdi menu
	
	//SetTimer(0,100,NULL);
}

void CMdiSpectrumView::OnMeasure() 
{
	if(m_pInstrument->CheckOccupied())
	{
		return;
	}
	g_pMeasureView->OnMeasure();
	CTimeTickDlg *pDlg = new CTimeTickDlg();
	pDlg->Create(IDD_TIMETICKDLG,this);
	pDlg->ShowWindow(SW_SHOW);
}

void CMdiSpectrumView::OnStbSample() 
{
	if(m_pInstrument->CheckOccupied())
	{
		return;
	}
	g_pMeasureView->OnStopSample();
}

void CMdiSpectrumView::OnYContract() 
{
	m_SpectrumDisplay.ContractYAxis();
}

void CMdiSpectrumView::OnYExtend() 
{
	m_SpectrumDisplay.ExtendYAxis();
}

void CMdiSpectrumView::OnXExtend() 
{
	m_SpectrumDisplay.ExtendXAxis();
}

void CMdiSpectrumView::OnXContract() 
{
	m_SpectrumDisplay.ContractXAxis();
}

void CMdiSpectrumView::OnMoveRight() 
{
	m_SpectrumDisplay.XAxisMoveRight();
}

void CMdiSpectrumView::OnMoveLeft() 
{
	m_SpectrumDisplay.XAxisMoveLeft();
}

void CMdiSpectrumView::OnSetZone() 
{
	CAdjustZoneDlg dlg;
	dlg.DoModal();
}

void CMdiSpectrumView::OnOpenPeriodicTable() 
{
	if(m_pPeriodicTableDlg==NULL)
	{
		//create a new dialog
		m_pPeriodicTableDlg = new CPeriodicTableDlg();
		m_pPeriodicTableDlg->Create(IDD_PERIODICTABLEDLG,this);
		m_pPeriodicTableDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pPeriodicTableDlg->ShowWindow(SW_SHOW);
	}
}

void CMdiSpectrumView::OnAutoScale() 
{
	m_SpectrumDisplay.SetDafaultScale();	
}

void CMdiSpectrumView::OnClickUserScale() 
{
	//CSpectrumScaleCfgDlg dlg;
	//dlg.DoModal();
}

void CMdiSpectrumView::OnFillSpectrum() 
{
	m_SpectrumDisplay.FillCurve();
}

void CMdiSpectrumView::OnChangeChannels() 
{
	if(!m_bInitialized) return;
	m_CursorChannel = m_CursorSpin.GetPos();
	if(m_CursorChannel<0) m_CursorChannel=0;
	else if(m_CursorChannel>255) m_CursorChannel=255;
	UpdateData(FALSE); //Member Variables  ->  Interface
	m_SpectrumDisplay.ChangeCurrentCursor(m_CursorChannel);
	UpdateStaticControls();
}

void CMdiSpectrumView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CMdiSpectrumView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	/*
	int x=point.x;
	int y=point.y;
	CRect rect;
	m_SpectrumDisplay.GetWindowRect(&rect);
	
	if(rect.PtInRect(point))
	{
		m_SpectrumDisplay.PreChangeCursor();
	}*/
	CFormView::OnLButtonDown(nFlags, point);
}

void CMdiSpectrumView::OnClickSpectrumEx() 
{
	if(m_SpectrumDisplay.PreChangeCursor(m_CursorChannel))
	{
		UpdateStaticControls();
		UpdateData(FALSE); //m_CursorChannel, Member Variables -> Interface
	}
}

void CMdiSpectrumView::OnTimer(UINT nIDEvent) 
{	
	CFormView::OnTimer(nIDEvent);
}

void CMdiSpectrumView::UpdateStaticControls()
{
	//m_CurrentChannelInfo
	//m_AllChannelsInfo
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	CString text;
	double cps = g_SpectrumInfo.GetCountClone(m_CursorChannel);
	double kev = pApp->GetEnergyByChannel(m_CursorChannel); 
	text.Format("%4.2lf keV,    %4.2lf cps",kev,cps);
	m_CurrentChannelInfo.SetWindowText(text);
}

void CMdiSpectrumView::OnColorListChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// do something
	*pResult = 0;
}

/*******************************************************************
  NM_CLICK 消息, 单击时CListCtrl时产生, 此时checkbox的状态还未改变.
 *******************************************************************/
void CMdiSpectrumView::OnClickColorList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(pNMHDR==NULL) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;
	int num = m_SpectrumDisplay.m_ContrastSpectrum.GetSize();
	if((num==0) || (row<0)) 
		return;
	
	//判断点击是否在checkbox上
	DWORD dwPos = GetMessagePos(); 
	CPoint point(LOWORD(dwPos),HIWORD(dwPos)); 
	m_colorListEx.ScreenToClient(&point);   
	UINT nFlag;
	int nItem = m_colorListEx.HitTest(point,&nFlag);
	if(nFlag != LVHT_ONITEMSTATEICON)  //点击的位置不是checkbox
		return;

	CSpectrumInfo<DWORD,SPEC_SIZE> *p;
	if(row==0)
	{
		p=&g_SpectrumInfo;
	} 
	else
	{
		p = m_SpectrumDisplay.m_ContrastSpectrum.GetAt(row-1);
	}
	
	//at this time,GetCheck() is true, and it will be changed to false after running this function
	if(m_colorListEx.GetCheck(row)){ 
		p->m_bNeedDisplay=FALSE;
	}
	else{
		p->m_bNeedDisplay=TRUE;
	}
	/*p->m_bNeedDisplay = !m_colorListEx.GetCheck(row);*/
	m_SpectrumDisplay.Update(); 
	*pResult = 0;
}

// 打开多个谱图文件时, 更新谱颜色列表
void CMdiSpectrumView::UpdateColorControls()
{
	m_colorListEx.DeleteAllItems();
	
	int num = m_SpectrumDisplay.m_ContrastSpectrum.GetSize();
	if(num==0) return;
	
	CSpectrumInfo<DWORD,SPEC_SIZE> *p;
	CString specName = "原光谱" + g_SpectrumInfo.m_fileName;
	m_colorListEx.InsertItem(0,specName);
	//m_colorListEx.SetItemText(0,1,"color");
	m_colorListEx.SetItemColor(0,1,RGB(0,0,0),g_SpectrumInfo.m_color);

	if(g_SpectrumInfo.m_bNeedDisplay) 
		m_colorListEx.SetCheck(0,TRUE);
	else 
		m_colorListEx.SetCheck(0,FALSE);
	
	//for(int i=0;i<num;i++)
	for(int i=1;i<=num;i++)
	{
		p = m_SpectrumDisplay.m_ContrastSpectrum.GetAt(i-1);
		m_colorListEx.InsertItem(i,p->m_fileName);
		//m_colorListEx.SetItemText(i,1,"color");
		//m_colorListEx.SetItemColor(i,RGB(0,0,0),p->m_color);
		m_colorListEx.SetItemColor(i,1,RGB(0,0,0),p->m_color);
		
		if(p->m_bNeedDisplay) 
			m_colorListEx.SetCheck(i,TRUE);
		else 
			m_colorListEx.SetCheck(i,FALSE);
	}
}

//从"光谱界面"切换到"测量界面"时, 不需要清除原光谱
void CMdiSpectrumView::CleanContrastSpec() 
{
	g_pMeasureView->bBackSpectrumExist=FALSE;
	m_SpectrumDisplay.RemoveAllContrastSpec();
	m_SpectrumDisplay.Update();
	UpdateColorControls();
}

//光谱界面, 菜单->光谱->关闭所有光谱
void CMdiSpectrumView::CleanAllContrastSpec() 
{
	g_pMeasureView->bBackSpectrumExist=FALSE;
	m_SpectrumDisplay.ZeroSpectrum();
	m_SpectrumDisplay.RemoveAllContrastSpec();
	m_SpectrumDisplay.Update();
	UpdateColorControls();
}

//光谱界面, 菜单->光谱->关闭(最后一个打开的)光谱
void CMdiSpectrumView::CleanLastContrastSpec() 
{
	m_SpectrumDisplay.RemoveLastContrastSpec();
	m_SpectrumDisplay.Update();
	UpdateColorControls();
}



void CMdiSpectrumView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_bInitialized) return;
	
	CRect clientRect,btnRect;
	CRect rectColour,rectSpectrum,rectChsInfo,rectAllInfo;
	int btnHeight,btnWidth;
	int clientHeight,clientWidth;
	int yRectSpace = 10, xRectSpace=10;
	GetClientRect(&clientRect);
	clientHeight=clientRect.Height();
	clientWidth =clientRect.Width();
	
	btnHeight = (clientHeight-yRectSpace*4)*1/8;
	btnWidth=(clientWidth-xRectSpace*3)*3/8;
	rectColour.top = clientRect.top+yRectSpace;
	rectColour.bottom=rectColour.top+btnHeight;
	rectColour.left=clientRect.left+xRectSpace;
	rectColour.right=rectColour.left+btnWidth;
	GetDlgItem(IDC_COLOUR_LIST)->MoveWindow(&rectColour);
	
	btnHeight = (clientHeight-yRectSpace*4)*6/8;
	btnWidth=(clientWidth-xRectSpace*3)*2/3;
	rectSpectrum.top   = rectColour.bottom+yRectSpace;
	rectSpectrum.bottom= rectSpectrum.top+btnHeight;
	rectSpectrum.left =clientRect.left+xRectSpace;
	rectSpectrum.right=rectSpectrum.left+btnWidth;
	m_SpectrumDisplay.MoveWindow(&rectSpectrum);
	
	//rectChannel.top = rectSpectrum.bottom+yRectSpace*3;
	//rectChannel.bottom= clientRect.bottom-yRectSpace;
	//rectChannel.left =rectSpectrum.left;
	//rectChannel.right=rectSpectrum.right;
	//GetDlgItem(IDC_GROUP_STATIC)->MoveWindow(&rectChannel);

	//move channels edit/spin control
	btnHeight=yRectSpace*2; // 20 Pixel fixed
	btnWidth= xRectSpace*5; // 40 Pixel fixed
	btnRect.left = rectSpectrum.left + xRectSpace*2;
	btnRect.right= btnRect.left + btnWidth;
	btnRect.bottom = clientRect.bottom - yRectSpace;
	btnRect.top=btnRect.bottom - btnHeight;
	GetDlgItem(IDC_CHANNELS)->MoveWindow(&btnRect);

	btnRect.left=btnRect.right+1;
	btnRect.right=btnRect.left+xRectSpace*2;
	GetDlgItem(IDC_CHANNEL_SPIN)->MoveWindow(&btnRect);

	GetDlgItem(IDC_CHS_STATIC)->GetWindowRect(&rectChsInfo);
	btnHeight=rectChsInfo.Height();
	btnWidth= rectChsInfo.Width(); 
	rectChsInfo.left=btnRect.right + 2*xRectSpace;
	rectChsInfo.right=rectChsInfo.left + btnWidth;
	rectChsInfo.bottom=clientRect.bottom - yRectSpace;
	rectChsInfo.top=rectChsInfo.bottom - btnHeight;
	GetDlgItem(IDC_CHS_STATIC)->MoveWindow(&rectChsInfo);

	btnWidth = ((clientWidth-xRectSpace*3)*1/3-xRectSpace*8)/2;
	btnHeight = (clientHeight-yRectSpace*4-rectColour.Height())*1/8;
	if(btnWidth>165) btnWidth=165;
	if(btnHeight>50) btnHeight=50;
	//GetDlgItem(IDC_BTN_STB)->GetWindowRect(&btnRect);
	//btnHeight=btnRect.Height();
	//btnWidth=btnRect.Width();
	btnRect.right = clientRect.right-xRectSpace;
	btnRect.left = btnRect.right -btnWidth;
	btnRect.bottom = clientRect.bottom-yRectSpace;
	btnRect.top =  btnRect.bottom-btnHeight;
	GetDlgItem(IDC_BTN_STB)->MoveWindow(&btnRect);
	
	btnRect.right = btnRect.left - xRectSpace*4;
	btnRect.left= btnRect.right - btnWidth;
	GetDlgItem(IDC_BTN_MEASURE)->MoveWindow(&btnRect);
	
	//@ 2012-06-11
	GetDlgItem(IDC_MOVELEFT)->GetWindowRect(&btnRect);
	btnHeight=btnRect.Height();
	btnWidth=btnRect.Width();
	btnRect.left=rectSpectrum.left;
	btnRect.right=btnRect.left+btnWidth;
	btnRect.top = rectSpectrum.bottom+yRectSpace/2;
	btnRect.bottom=btnRect.top+btnHeight;
	GetDlgItem(IDC_MOVELEFT)->MoveWindow(&btnRect);
	
	btnRect.right=rectSpectrum.right;
	btnRect.left =btnRect.right-btnWidth;
	GetDlgItem(IDC_MOVERIGHT)->MoveWindow(&btnRect);
}

void CMdiSpectrumView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



