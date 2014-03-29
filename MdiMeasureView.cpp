// MdiMeasureView.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "MdiMeasureView.h"
#include "SpectrumInfo.h"
//#include "SpectrumAnalyzeDlg.h"
#include "CreateDefmaDlg.h"
#include "CreateWorkZoneDlg.h"
#include "TimeTickDlg.h"
#include "WorkZoneListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;

void ConvertCmdBytes(COneCommand* pCommand)
{
	if(pCommand->m_Type != ConfigBX) return;

	//to-do : convert COneCommand::m_CommandBuffer
}

void ConvertRecvBytes(COneCommand* pCommand)
{
	if(pCommand->m_Type != LoadBX) return;
	
	//to-do : convert COneCommand::m_ReceiveBuffer
}

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureView

IMPLEMENT_DYNCREATE(CMdiMeasureView, CFormView)

CMdiMeasureView::CMdiMeasureView()
	: CFormView(CMdiMeasureView::IDD)
{
	//{{AFX_DATA_INIT(CMdiMeasureView)
	//}}AFX_DATA_INIT
	m_bAutoSaveSpec=FALSE;
	m_bAutoSortResult=FALSE;
	m_bInitialized=FALSE;
	m_bSpectrumReady=FALSE;
	m_bResultAnalysis=FALSE;
	m_finishedPercent=0;
	m_pCurCommand=NULL;
	m_pWorkZone=NULL;
	//m_SpectrumDisplay.AttachSpectrumInfo(&g_SpectrumInfo);
	bBackSpectrumExist=FALSE;
	bSystemParamSended=FALSE;
	m_beCalibMode=FALSE;
	//m_pResult=NULL;
	m_ResultIndex = 1;
}

CMdiMeasureView::~CMdiMeasureView()
{
	/* also see OnDestory */

	//delete m_pWorkZone; // This is a pointer copy of CTMeter::m_pWorkZone, it's been destoryed in ~CTMeterApp()
	//delete m_pResult;
	
}

void CMdiMeasureView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMdiMeasureView)
	DDX_Control(pDX, IDC_SWITCHWZ, m_BtnSwitchWorkZone);
	DDX_Control(pDX, IDC_OPENWZ, m_BtnOpenWorkZone);
	DDX_Control(pDX, IDC_STATISTIC_LIST, m_StatisList);
	DDX_Control(pDX, IDC_ARRAY_LIST, m_ArrayList);
	DDX_Control(pDX, IDC_WZTITLE, m_BtnWorkZoneTitle);
	DDX_Control(pDX, IDC_BTN_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BTN_START, m_BtnStart);
	DDX_Control(pDX, IDC_VIDEO, m_VideoDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMdiMeasureView, CFormView)
	//{{AFX_MSG_MAP(CMdiMeasureView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_START, OnStartSample)
	ON_BN_CLICKED(IDC_BTN_STOP, OnStopSample)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MENU_MATERIAL, CreateDefMa)
	ON_COMMAND(ID_MENU_THICK, CreateDefThick)
	ON_COMMAND(ID_MENU_TESTDEFMA, TestDefMa)
	ON_BN_CLICKED(IDC_SWITCHWZ, OnSwitchWz)
	ON_COMMAND(ID_MENU_OPENVIDEO, OnOpenVideo)
	ON_COMMAND(ID_MENU_CLOSEVIDEO, OnCloseVideo)
	ON_BN_CLICKED(IDC_OPENWZ, OnEditWz)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureView diagnostics

#ifdef _DEBUG
void CMdiMeasureView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMdiMeasureView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureView message handlers

void CMdiMeasureView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	m_BtnStart.SetIcon(IDI_START);
	m_BtnStart.SetFlat(FALSE);
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetFlat(FALSE);
	m_BtnSwitchWorkZone.SetIcon(IDI_SWITCHWZ);
	m_BtnSwitchWorkZone.SetFlat(FALSE);
	m_BtnOpenWorkZone.SetIcon(IDI_OPENWZ);
	m_BtnOpenWorkZone.SetFlat(FALSE);
	m_BtnWorkZoneTitle.Use3D(FALSE); // @2012-07-06

	// Initialize serial port
	m_pInstrument = &(((CTMeterApp*)AfxGetApp())->m_Instrument);
	m_SerialPortInstance.ReconfigComport(m_pInstrument->m_SerialPortNum,m_pInstrument->m_SerialPortBaud);

	// Initialize list control
	DWORD exstyle = m_StatisList.GetExtendedStyle();
	m_StatisList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	UpdateStatisticList();
	
	// Set Style for List Control
	exstyle = m_ArrayList.GetExtendedStyle();
	m_ArrayList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	UpdateArrayList();

	// Show windows
	m_bInitialized=TRUE;
	GetParentFrame()->ShowWindow(SW_MAXIMIZE);
	GetDocument()->SetTitle("X-Ray Thickness Meter: Measure View");

	// Create timer
	SetTimer(0,100,NULL);
}

void CMdiMeasureView::UpdateStatisticList()
{
	m_StatisList.DeleteAllItems();
	while(m_StatisList.DeleteColumn(0)){}

	m_StatisList.InsertColumn(0,"数组",LVCFMT_LEFT,80);
	CRect rect;
	m_StatisList.GetClientRect(&rect);
    int nColInterval = (rect.Width()-45)/5;
	
	CTMeterApp* pApp = ((CTMeterApp*)AfxGetApp());
	if(pApp->m_pWorkZone == NULL) return;
	
	CDefMa *pDefMa = &(pApp->m_pWorkZone->defMa);
	int i,j;
	int col = 1; 
	OneLayerDefine *pOneLayer;
	CClassicalElement *pOneElem;
	int nLayers = pDefMa->m_layerArray.GetSize();
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_thickMode == 0) // 0:calculate thickness, 1:correct thickness, 2:thickness is known
		{
			m_StatisList.InsertColumn(col++, pOneLayer->GetLayerName()+" [um]",LVCFMT_LEFT,nColInterval);
		}
	}
	
	int nElem;
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_contentMode == 0) // 0:calculate content ,  1:correct content,   2:content is known
		{
			nElem = pOneLayer->m_elementList.GetSize();
			for(j=0;j<nElem;j++)
			{
				pOneElem = pOneLayer->m_elementList.GetAt(j);
				m_StatisList.InsertColumn(col++, pOneElem->Name +" [%]",LVCFMT_LEFT,nColInterval);
			}
		}
	}
	
	
	m_StatisList.InsertItem(1,"平均值");
	m_StatisList.InsertItem(2,"标准偏差");
	m_StatisList.InsertItem(3,"变化率");
	m_StatisList.InsertItem(4,"读数数量");
	m_StatisList.InsertItem(5,"订货号");
	m_StatisList.InsertItem(6,"标签号");
	m_StatisList.InsertItem(7,"使用者");
	
}

void CMdiMeasureView::UpdateArrayList()
{
	//clean all row and column
	m_ArrayList.DeleteAllItems();
	while(m_ArrayList.DeleteColumn(0)){}
	
	//re-insert row and column
	m_ArrayList.InsertColumn(0,"编号",LVCFMT_LEFT,45);
	CRect rect;
	m_ArrayList.GetClientRect(&rect);
    int nColInterval = (rect.Width()-45)/6;

	CTMeterApp* pApp = ((CTMeterApp*)AfxGetApp());
	if(pApp->m_pWorkZone == NULL) return;

	CDefMa *pDefMa = &(pApp->m_pWorkZone->defMa);
	int i,j;
	int col = 1; 
	OneLayerDefine *pOneLayer;
	CClassicalElement *pOneElem;
	int nLayers = pDefMa->m_layerArray.GetSize();
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_thickMode == 0) // 0:calculate thickness, 1:correct thickness, 2:thickness is known
		{
			m_ArrayList.InsertColumn(col++, pOneLayer->GetLayerName()+" [um]",LVCFMT_LEFT,nColInterval);
		}
	}

	int nElem;
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_contentMode == 0) // 0:calculate content ,  1:correct content,   2:content is known
		{
			nElem = pOneLayer->m_elementList.GetSize();
			for(j=0;j<nElem;j++)
			{
				pOneElem = pOneLayer->m_elementList.GetAt(j);
				m_ArrayList.InsertColumn(col++, pOneElem->Name +" [%]",LVCFMT_LEFT,nColInterval);
			}
		}
	}
}

void CMdiMeasureView::AddNewResultToArray()
{
	CString text;
	int itemCount = m_ArrayList.GetItemCount();
	
	text.Format("%d",m_ResultIndex++);
	m_ArrayList.InsertItem(itemCount,text);

	CTMeterApp* pApp = ((CTMeterApp*)AfxGetApp());
	FpCalculator* pFpCalc = &(pApp->m_FpCalculator);
	CDefMa *pDefMa = &(pApp->m_pWorkZone->defMa);

	BOOL beTheory = FALSE;
	int stdNum = pApp->m_pWorkZone->stdSample.GetSize();
	if(stdNum == 0) beTheory = TRUE; 

	int i,j;
	int col = 1; 
	OneLayerDefine *pOneLayer;
	CClassicalElement *pOneElem;
	int nLayers = pDefMa->m_layerArray.GetSize();
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_thickMode == 0) // 0:calculate thickness, 1:correct thickness, 2:thickness is known
		{
			text.Format("%7.4lf",pFpCalc->GetLayerThicknessFromResult(i,beTheory));
			m_ArrayList.SetItemText(itemCount,col++,text);
		}
	}
	
	int nElem;
	for(i=0;i<nLayers;i++)
	{
		pOneLayer = pDefMa->m_layerArray.GetAt(i);
		if(pOneLayer->m_contentMode == 0) // 0:calculate content ,  1:correct content,   2:content is known
		{
			nElem = pOneLayer->m_elementList.GetSize();
			for(j=0;j<nElem;j++)
			{
				pOneElem = pOneLayer->m_elementList.GetAt(j);
				text.Format("%7.4lf",pFpCalc->GetElemContentFromResult(pOneElem->Name,beTheory));
				m_ArrayList.SetItemText(itemCount,col++,text);
			}
		}
	}
}

void CMdiMeasureView::ChangeMeasureCondition(WorkZoneDef* pWz)
{
	if(!pWz) return;

	CTMeterApp *pApp = (CTMeterApp*)AfxGetApp();
	//CDefMa::m_hv, WorkZoneDef::Collimator, CDefMa::m_filter/ma, (WorkZoneDef::AdjustTimes)
	g_SpectrumInfo.SetTotalSampleTimes(pWz->AdjustTimes);
	m_pInstrument->m_WorkingHv = pApp->GetTubeVoltage(pWz->defMa.m_hv);
	if(pWz->Current == 0)
		m_pInstrument->m_WorkingMa = pApp->GetTubeCurrent(pWz->Collimator);
	else
		m_pInstrument->m_WorkingMa = (float)(pWz->Current);
	
	SendSystemParameter();
}

void CMdiMeasureView::OnSwitchWz() 
{
	// make sure ODBC can find ./TMeterDataLink.DSN
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);

	CWorkZoneListDlg wzListDlg;
	wzListDlg.beNewWz = FALSE;
	if(wzListDlg.DoModal()==IDOK)
	{
		((CTMeterApp*)AfxGetApp())->OpenWorkZone(wzListDlg.Index);
		((CTMeterApp*)AfxGetApp())->LoadWorkZonePureElem();

		/* for debug, remove this when release */
		((CTMeterApp*)AfxGetApp())->WriteSwitchWz2Log();

	}
	else return;
	
	//update CMdiMeasureView::m_pWorkZone, and work-zone title
	m_pWorkZone = ((CTMeterApp*)AfxGetApp())->m_pWorkZone;

	//delete m_pResult; // delete old
	//m_pResult->CopyFrom(&(m_pWorkZone->defMa));
	UpdateArrayList();
	UpdateStatisticList();

	CString wzIndex; 
	wzIndex.Format("ID%d: ",m_pWorkZone->GetDirIndex());
	m_BtnWorkZoneTitle.SetWindowText(wzIndex + m_pWorkZone->GetTag()); 


	//send parameter to dct
	ChangeMeasureCondition(m_pWorkZone);
}

void CMdiMeasureView::OnEditWz() 
{
	WorkZoneDef *ptr = ((CTMeterApp*)AfxGetApp())->m_pWorkZone;
	if(!ptr)
	{
		AfxMessageBox("没有打开任何工作曲线");
		return;
	}
	CCreateWorkZoneDlg wzDlg;
	wzDlg.LinkValue(FALSE,ptr);
	wzDlg.DoModal();
}

void CMdiMeasureView::UpdateWzTile()
{
	if(m_pWorkZone == NULL) return;
	CString wzIndex; 
	wzIndex.Format("ID%d: ",m_pWorkZone->GetDirIndex());
	m_BtnWorkZoneTitle.SetWindowText(wzIndex + m_pWorkZone->GetTag());
}

void CMdiMeasureView::OpenLastWorkZone()
{
	CString file;
	
	file =  ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\LastOpen.DAT";
	// load last work-zone.......

}

void CMdiMeasureView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int timerCount=0;
	BOOL curCommandExcuted=FALSE;
	COneCommand *pNewCommand=NULL;
	CmdStatus status;
	
	/*********************************/
	/*    command from serial port   */
	/*********************************/
	if(m_CommandTrack.IsEmpty())
	{
		timerCount=(timerCount+1)%30;
		if(timerCount%15 == 0) //every 1.5 sec request real-time parameter
		{
			if((m_pInstrument->bSystemParamLoaded) && !bSystemParamSended) 
			{
				SendSystemParameter();
			}
			//else if(!m_pInstrument->bSystemParamLoaded)  //read parameter from dct
			else
			{
				pNewCommand=new COneCommand;
				pNewCommand->m_Type=LoadBX;
				pNewCommand->AddByteToComamnd('X',TRUE,FALSE);
				pNewCommand->PrepareCommand('*',0xff,52,200,TRUE);
				m_CommandTrack.Add(pNewCommand);
			}
		}
		else if(timerCount%20 == 0) //every 2 sec request spectrum data
		{
			if(m_pInstrument->GetInstrumentStatus() == Sampling)
			{
				if(m_bSpectrumReady=::IsSamplingFinished(m_finishedPercent))  //return g_SpectrumInfo.m_isFinished
				{	
					pNewCommand=new COneCommand;
					pNewCommand->m_Type=ChgStatus;
					pNewCommand->AddByteToComamnd('E',TRUE,FALSE); // change status to SwitchingToTubeReady
					pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
					m_CommandTrack.Add(pNewCommand);

					//GetDlgItem(IDC_SAMPLE)->EnableWindow(TRUE);
					//GetDlgItem(IDC_STOPSAMPLE)->EnableWindow(FALSE);
					if(m_bResultAnalysis)
					{
						m_bResultAnalysis=FALSE;
						m_pInstrument->SetOccupied(FALSE,"NONE"); 
						
						// calculate intensity, and FpCalculate
						CalcPureIntensity();
						CalculateQuantify();
						AddNewResultToArray();
					}
					if(m_bAutoSaveSpec && !m_beCalibMode)
					{
						g_SpectrumInfo.AutoSaveSpectrum();
					}
				} // END IsSamplingFinished()
				else
				{
					pNewCommand=new COneCommand;
					pNewCommand->m_Type=ReadSpectrum;
					pNewCommand->AddByteToComamnd('P',TRUE,FALSE);
					//pNewCommand->PrepareCommand('*',0xff,4104,200,TRUE); // 1024 Channels
					pNewCommand->PrepareCommand('*',0xff,1034,200,TRUE);   // 256 Channels
					m_CommandTrack.Add(pNewCommand);
				}
			} 
		} 
	}
	if((m_pCurCommand==NULL) && !(m_CommandTrack.IsEmpty()))
	{
		m_pCurCommand = m_CommandTrack.GetAt(0);
		ConvertCmdBytes(m_pCurCommand);//convert float to dword(0~4095), for u-fpu dct
		curCommandExcuted=m_SerialPortInstance.ExcuteCommand(m_pCurCommand); // write bytes to com, and read
		if(m_SerialPortInstance.m_bComError || !curCommandExcuted)
		{
			delete m_pCurCommand;m_pCurCommand=NULL; 
			m_CommandTrack.RemoveAt(0);
			m_pInstrument->m_status=Unknown; // change instrument status
			bSystemParamSended=FALSE;
		}
	}
	if(m_pCurCommand != NULL)
	{
		if(m_pCurCommand->CommandExcuted(status))
		{
			ConvertRecvBytes(m_pCurCommand);//convert dword(0~4095) to float, for u-fpu dct
			switch(status)
			{
			case Success:
				switch(m_pCurCommand->m_Type)
				{
				case ConfigBX:
					//set system parameter to dct 
					bSystemParamSended=TRUE;
					break;
				case LoadBX:
					//read system parameter from dct
					ReadSystemParameter(&m_pCurCommand->m_ReceiveBuffer);
					break;
				case ReadSpectrum:
					//read spectrum data from dct
					PutToSpectrumInfo(&m_pCurCommand->m_ReceiveBuffer);
					break;
				case ChgStatus:
					//command such as M/e, L/l
					//m_cRetStatus.SetWindowText("Change Status");
					break;
				}
				break;
			case Failed:
				break;
			case OverTime:
				break;
			case CrcError:
				break;
			}
			delete m_pCurCommand;m_pCurCommand=NULL;  //delete OneCommand from Track
			m_CommandTrack.RemoveAt(0);
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CMdiMeasureView::PutToSpectrumInfo(CByteArray *buff) 
{
	DWORD data;
	int pos;

	pos=1;
	g_SpectrumInfo.m_isFinished=buff->GetAt(pos); 

	pos=2;
	g_SpectrumInfo.SetCurrentSampleTimes(buff->GetAt(pos)*256+buff->GetAt(pos+1));

	pos=4;
	g_SpectrumInfo.SetTotalSampleTimes(buff->GetAt(pos)*256+buff->GetAt(pos+1));

	//for(int i=0;i<1024;i++)
	for(int i=0;i<SPEC_SIZE;i++)
	{
		data=(buff->GetAt(i*4+6)<<24) + (buff->GetAt(i*4+7)<<16) + (buff->GetAt(i*4+8)<<8) + buff->GetAt(i*4+9);
		g_SpectrumInfo.SetDataAt(i,data);
	}
	g_SpectrumInfo.CalculatePeak();
	g_SpectrumInfo.m_bModified=TRUE;
	g_pSpectrumView->m_SpectrumDisplay.Update();
}

void CMdiMeasureView::ReadSystemParameter(CByteArray* buff)
{
	BYTE ch; FloatBytes floatBytes;
	int pos,i;

	pos=1;
	ch=buff->GetAt(pos);
	m_pInstrument->SetInstrumentStatus(ch);

	pos=2; //(1)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_CurrentHv=floatBytes.real;

	pos=6; //(2)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_CurrentMa=floatBytes.real;

	pos=10; //(3)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_CurrentHtStdVol=floatBytes.real;

	pos=14; //(4)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_H2500CurrentHv=floatBytes.real;

	pos=18; //(5)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_Current24Vol=floatBytes.real;

	pos=22; //(6)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_Current12Vol=floatBytes.real;

	pos=26; //(7)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_Current5Vol=floatBytes.real;

	pos=30; //(8)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_CurrentAngle=floatBytes.real;

	pos=34; //(9)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_Temperature1=floatBytes.real;

	pos=38; //(10)
	for(i=0;i<4;i++) floatBytes.byte[i]=buff->GetAt(pos++);
	m_pInstrument->m_Temperature2=floatBytes.real;

	pos=42;
	//key-1

	pos=45;
	//key-2

	pos=44;
	ch=buff->GetAt(pos);
	m_pInstrument->m_BeamStopOpenPos=ch;

	pos=45;
	ch=buff->GetAt(pos);
	m_pInstrument->m_BeamStopClosePos=ch;

	pos=46; //sample lid 1
	ch=buff->GetAt(pos);
	m_pInstrument->m_SampleLidOpenPos=ch;

	pos=47; //sample lid 2
	ch=buff->GetAt(pos);
	m_pInstrument->m_SampleLidClosePos=ch;
}

void CMdiMeasureView::SendSystemParameter()
{
	COneCommand *pNewCommand=NULL;
	CByteArray cmd; BYTE byte;  int i;
	FloatBytes floatByte; ShortBytes shortByte; 

	byte=0x78; cmd.Add(byte);
	floatByte.real=m_pInstrument->m_WorkingHv;  // (1)
	for(i=0;i<4;i++)
	{
		byte=floatByte.byte[i];	
		cmd.Add(byte);
	}

	floatByte.real=m_pInstrument->m_WorkingMa; // (2)
	for(i=0;i<4;i++)
	{
		byte=floatByte.byte[i];	
		cmd.Add(byte);
	}

	floatByte.real=m_pInstrument->m_H2500WorkingHv; // (3)
	for(i=0;i<4;i++)
	{
		byte=floatByte.byte[i];	
		cmd.Add(byte);
	}

	for(i=0;i<5;i++) cmd.Add(m_pInstrument->PanelLight[i]);
	for(i=0;i<4;i++) cmd.Add(m_pInstrument->LampLight[i]);

	cmd.Add(m_pInstrument->m_FanStatus);
	cmd.Add(m_pInstrument->m_XRayTubeStatus);
	cmd.Add(m_pInstrument->m_BeamStopStatus);

	shortByte.real=g_SpectrumInfo.GetTotalSampleTimes();
	for(i=0;i<2;i++)
	{
		byte=shortByte.byte[i];	
		cmd.Add(byte);
	}
	
	floatByte.real=m_pInstrument->m_UThresholdTemp; 
	for(i=0;i<4;i++)
	{
		byte=floatByte.byte[i];	
		cmd.Add(byte);
	}
	floatByte.real=m_pInstrument->m_LThresholdTemp; 
	for(i=0;i<4;i++)
	{
		byte=floatByte.byte[i];	
		cmd.Add(byte);
	}

	pNewCommand=new COneCommand;
	pNewCommand->m_Type=ConfigBX;
	pNewCommand->ConstructCommand(cmd,TRUE,'*',0xff,1,400,FALSE);
	m_CommandTrack.Add(pNewCommand);

}

void CMdiMeasureView::OnStartSample()
{
	if(m_pInstrument->GetInstrumentStatus() == Unknown)
	{
		// loading Spectrum File...
		OpenSpectrumFiles();
	}
	else
	{
		if(m_pInstrument->CheckOccupied())
		{
			return;
		}
		/*
		if(pDoc->curWZone==NULL) OpenWorkZone();
		if(m_sampleid.IsEmpty())
		msg.Format("您能确认样品 %s 已在测量室里吗？",trueid);
		switch(AfxMessageBox(msg,MB_YESNOCANCEL)) 
		*/
		OnMeasure();
		m_bResultAnalysis = TRUE;
		CString ocp;
		ocp.LoadString(IDS_INFO_OCUUPY_MEASURE);
		m_pInstrument->SetOccupied(TRUE,ocp);
		// SetTimer(1,100,NULL);
		
		//create time tick
		CTimeTickDlg *pDlg = new CTimeTickDlg();
		pDlg->Create(IDD_TIMETICKDLG,this);
		//pDlg->MoveWindow(....);
		pDlg->ShowWindow(SW_SHOW);
	}
}

void CMdiMeasureView::OnMeasure()
{
	COneCommand *pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('S',TRUE,FALSE);
	pNewCommand->PrepareCommand('*',0xff,1,100,FALSE);
	m_CommandTrack.Add(pNewCommand);

	m_bSpectrumReady = FALSE;
	g_SpectrumInfo.ResetSpectrum();
	g_pSpectrumView->m_SpectrumDisplay.SetDafaultScale();
	g_pSpectrumView->m_SpectrumDisplay.Update();
}

void CMdiMeasureView::OnStopSample() // switch to [Tube Ready]
{
	COneCommand *pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('E',TRUE,FALSE);
	pNewCommand->PrepareCommand('*',0xff,1,100,FALSE);
	m_CommandTrack.Add(pNewCommand);

	m_bResultAnalysis=FALSE;
	m_pInstrument->SetOccupied(FALSE,"NONE");
}

void CMdiMeasureView::WarmUpXTube()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('E',TRUE,FALSE); // change status to SwitchingToTubeReady
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::CoolDownXTube()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('e',TRUE,FALSE); // change status to SwitchingToIdle
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::OpenBeamShut()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('L',TRUE,FALSE); 
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::CloseBeamShut()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('l',TRUE,FALSE);
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::OpenLampLight()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('B',TRUE,FALSE); 
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::CloseLampLight()
{
	COneCommand* pNewCommand=new COneCommand;
	pNewCommand->m_Type=ChgStatus;
	pNewCommand->AddByteToComamnd('b',TRUE,FALSE);
	pNewCommand->PrepareCommand('*',0xff,1,20,FALSE);
	m_CommandTrack.Add(pNewCommand);
}

void CMdiMeasureView::OpenSpectrumFiles()
{
	
	BOOL loadSuccess=FALSE;
	CString strFilename,shortFileName,format; 
	CSpectrumInfo<DWORD,SPEC_SIZE> *pNewConSpec;
	
	//CFileDialog fileDlg(TRUE,"TXT", NULL, OFN_HIDEREADONLY,"Spectrum Files(.txt)|*.txt||");
	//CFileDialog fileDlg(TRUE,"SPC", NULL, OFN_HIDEREADONLY,"Spectrum Files(.spc)|*.spc||");
	
	// @2012-07-03, can open *.spc and *.txt files
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"Spectrum Binary(.spc)|*.spc|Spectrum Text (*.txt)|*.txt||");
	fileDlg.m_ofn.lpstrInitialDir = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory;
	if(fileDlg.DoModal() == IDOK)
	{
		strFilename = fileDlg.GetPathName();
		shortFileName = fileDlg.GetFileName();
		format = strFilename.Right(3);
		if(!bBackSpectrumExist)
		{
			if(format == "spc"){
				loadSuccess = g_SpectrumInfo.LoadSpectrumFromBinary(strFilename);
				g_SpectrumInfo.m_fileName = shortFileName;
			}
			else if(format == "txt"){
				loadSuccess = g_SpectrumInfo.LoadSpectrumFromFiles(strFilename);
				g_SpectrumInfo.m_fileName = shortFileName;
			}
			bBackSpectrumExist=TRUE;
		}
		else // add CSpectrumInfo to CSpectrumDisplay::m_ContrastSpectrum, 叠加的谱图
		{
			pNewConSpec = new CSpectrumInfo<DWORD,SPEC_SIZE>();
			if(format == "spc"){
				loadSuccess = pNewConSpec->LoadSpectrumFromBinary(strFilename);
			}
			else if(format == "txt"){
				loadSuccess = pNewConSpec->LoadSpectrumFromFiles(strFilename);
			}
			loadSuccess = g_pSpectrumView->m_SpectrumDisplay.AddToContrastSpec(pNewConSpec,shortFileName); //CSpectrumDisplay::AddToContrastSpec(CSpectrumInfo<DWORD,SPEC_SIZE> *p)
		}
	}
	
	if(loadSuccess) 
	{
		g_pSpectrumView->m_SpectrumDisplay.SetDafaultScale();
		g_pSpectrumView->m_SpectrumDisplay.Update();
		g_pSpectrumView->UpdateColorControls();
		//PostMessage(WM_SIZE);
	}

	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory); // @2012-09-03
}

void CMdiMeasureView::SaveSpectrumFiles()
{
	CString specFileName;
	//CString specFileExt;
	//CFile file;
	
	CTime tm = CTime::GetCurrentTime();
	CString tmString = tm.Format("%Y-%m-%d %H-%M-%S");
	specFileName= "SPECTRUM " + tmString;
	
	CFileDialog openFile(FALSE,NULL,specFileName,OFN_OVERWRITEPROMPT,"Text Files (.txt)|*.txt|Binary Files (.spc)|*.spc||");
	openFile.m_ofn.lpstrInitialDir = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory;
	if(IDOK==openFile.DoModal())
	{
		specFileName = openFile.GetFileName();
		switch (openFile.m_ofn.nFilterIndex) {
		case 1:
			specFileName = specFileName + ".txt";
			g_SpectrumInfo.SaveSpectrumToFiles(specFileName);  // save to *.txt
			break;
		case 2:
			specFileName = specFileName + ".spc";
			g_SpectrumInfo.SaveSpectrumToBinary(specFileName); // save to *.spc 
			break;
		}
	}
}

BOOL CMdiMeasureView::SpectrumDataReady(int &percent) //m_finishedPercent & m_bSpectrumReady
{
	percent = m_finishedPercent;
	return m_bSpectrumReady;
}

void CMdiMeasureView::ResetDataReady()
{
	m_finishedPercent = 0;
	m_bSpectrumReady = FALSE;
}

void CMdiMeasureView::CalibrateWorkZone()
{
	((CTMeterApp*)AfxGetApp())->m_FpCalculator.CalibrateWorkZone(m_pWorkZone);
}

void CMdiMeasureView::CalcPureIntensity()
{
	((CTMeterApp*)AfxGetApp())->m_IntensityCalculator.CalcPureIntensity(g_SpectrumInfo);
}

void CMdiMeasureView::CalculateQuantify()
{
	((CTMeterApp*)AfxGetApp())->m_FpCalculator.CalculateQuantify(m_pWorkZone);
}

void CMdiMeasureView::CreateDefMa() 
{
	//CCreateDefmaDlg dlg;
	//dlg.DoModal();
}


void CMdiMeasureView::CreateDefThick() 
{
	CDefMa* ptr = new CDefMa();
	CCreateDefmaDlg dlg;  // modify and save ptr
	dlg.LinkValue(TRUE,ptr);
	dlg.DoModal();

	delete ptr;
}


void CMdiMeasureView::TestDefMa() 
{
	//load xml
	TiXmlDocument loadDoc("DefineMA\\24Ni-MoMn-Ceramic.xma");
	//TiXmlDocument loadDoc("DefineMA\\25CrV-Ni-Cu-Fe.xma");
	if(!loadDoc.LoadFile()) return;
	CDefMa defMa;
	defMa.XMLSerialize(FALSE,&loadDoc);
	
	//re-save xml
	TiXmlDocument saveDoc;
	defMa.XMLSerialize(TRUE,&saveDoc);
	saveDoc.SaveFile("24copy.xma");
	AfxMessageBox("success!");
}

void CMdiMeasureView::OnOpenVideo() 
{
	//UpdateData(TRUE);
	HxnDVR_UnInit();
	if(HxnDVR_Init(m_hWnd) == 0){
		AfxMessageBox("ERR:HxnDVR_Init()");
		return;
	}
	
	BOOL bConnected=FALSE;
	switch(m_pInstrument->m_InputChannel){
	case 1:
		bConnected=HxnDVR_ConnectDevice(MVCARD_INDEX,VInput_AV1);
		break;
	case 2:
		bConnected=HxnDVR_ConnectDevice(MVCARD_INDEX,VInput_AV2);
		break;
	case 3:
		bConnected=HxnDVR_ConnectDevice(MVCARD_INDEX,VInput_AV3); 
		break;
	default:
		bConnected=HxnDVR_ConnectDevice(MVCARD_INDEX,VInput_AV1);  //default: connect video channel 1
	}
	if(bConnected == FALSE){
		CString text;
		text.Format("ERR:HxnDVR_ConnectDevice(%d)",m_pInstrument->m_InputChannel);
		AfxMessageBox(text);
		return;
	}
	
	BOOL bInited=FALSE;
	bInited = HxnDVR_SetWindowPos(MVCARD_INDEX, m_VideoDisplay.m_hWnd, NULL); 
	bInited = HxnDVR_SetVPropertyValue(MVCARD_INDEX, VProperty_Brightness, m_pInstrument->m_Brightness); // range 0~510, default:255
	bInited = HxnDVR_SetVPropertyValue(MVCARD_INDEX, VProperty_Contrast, m_pInstrument->m_Contrast);     // range 0~510, default:255
	bInited = HxnDVR_SetVPropertyValue(MVCARD_INDEX, VProperty_Hue, m_pInstrument->m_Hue);              // range 0~510, default:255
	bInited = HxnDVR_SetVPropertyValue(MVCARD_INDEX, VProperty_Saturation, m_pInstrument->m_Saturation); // range 0~510, default:300
	
	if(bInited == FALSE){
		AfxMessageBox("ERR:HxnDVR_SetVPropertyValue()");
		return;
	}
	
	//m_bVideoRun = TRUE;
}	

void CMdiMeasureView::OnCloseVideo() 
{
	HxnDVR_UnInit();
	Invalidate(TRUE);
	//m_bVideoRun = FALSE;
}

/*
void CMdiMeasureView::OnClickSpectrumControl() 
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_SIZE);
}*/

void CMdiMeasureView::OnDestroy() 
{
	CFormView::OnDestroy();	
	// TODO: Add your message handler code here
	KillTimer(0);
	Sleep(500);
	if(!m_CommandTrack.IsEmpty()) 
	{
		int cno=m_CommandTrack.GetSize();
		for(int i=0;i<cno;i++) 
			delete m_CommandTrack.GetAt(i);
		m_CommandTrack.RemoveAll();
	}

	//if(m_bVideoRun)
	HxnDVR_UnInit(); //added @2012-07-17
}

void CMdiMeasureView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMdiMeasureView::OnSize(UINT nType, int cx, int cy) 
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
	
	GetDlgItem(IDC_SWITCHWZ)->GetWindowRect(&btnRect);
	btnHeight=btnRect.Height();
	btnWidth=clientRect.Width()*7/60;
	btnRect.left = clientRect.left + xRectSpace;
	btnRect.right= btnRect.left + btnWidth;
	btnRect.top  = clientRect.top + yRectSpace;
	btnRect.bottom=btnRect.top + btnHeight;
	GetDlgItem(IDC_SWITCHWZ)->MoveWindow(&btnRect);
	
	rectWzName.left = btnRect.right + xRectSpace;
	rectWzName.top  = btnRect.top;
	rectWzName.bottom=btnRect.bottom;
	
	btnRect.right = clientRect.right-xRectSpace;
	btnRect.left  = btnRect.right-btnWidth;
	GetDlgItem(IDC_OPENWZ)->MoveWindow(&btnRect);
	
	rectWzName.right = btnRect.left-xRectSpace;
	GetDlgItem(IDC_WZTITLE)->MoveWindow(&rectWzName);
	
	btnHeight = btnWidth = (clientHeight-xRectSpace*4-rectWzName.Height())*1/2;
	rectVideo.top   =rectWzName.bottom+yRectSpace*2;
	rectVideo.bottom=rectVideo.top + btnHeight;
	rectVideo.right =clientRect.right-xRectSpace;
	rectVideo.left  =rectVideo.right-btnWidth;
	GetDlgItem(IDC_VIDEO)->MoveWindow(&rectVideo);
	
	btnHeight = (clientHeight-xRectSpace*4-rectWzName.Height())*1/8;
	rectResultText.top  =rectVideo.bottom+yRectSpace;
	rectResultText.left =rectVideo.left;
	rectResultText.right=rectVideo.right;
	rectResultText.bottom=rectResultText.top+btnHeight;
	GetDlgItem(IDC_LASTRESULT)->MoveWindow(&rectResultText);
	
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
	GetDlgItem(IDC_BTN_STOP)->MoveWindow(&btnRect);
	
	btnRect.right =btnRect.left-xRectSpace*2;
	btnRect.left  =btnRect.right-btnWidth;
	GetDlgItem(IDC_BTN_START)->MoveWindow(&btnRect);
	
	btnHeight = (clientHeight-yRectSpace*4-rectWzName.Height())*1/2;
	rectStatistic.top=rectVideo.top;
	rectStatistic.bottom=rectStatistic.top+btnHeight;
	rectStatistic.left = clientRect.left+xRectSpace;
	rectStatistic.right=rectVideo.left-xRectSpace*2;
	GetDlgItem(IDC_STATISTIC_LIST)->MoveWindow(&rectStatistic);
	
	rectArrayList.top = rectStatistic.bottom+yRectSpace;
	rectArrayList.bottom=clientRect.bottom-yRectSpace;
	rectArrayList.left  =rectStatistic.left;
	rectArrayList.right =rectStatistic.right;
	GetDlgItem(IDC_ARRAY_LIST)->MoveWindow(&rectArrayList);
}



