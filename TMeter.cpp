// TMeter.cpp : Defines the class behaviors for the application.
//

//#include <direct.h>
#include "stdafx.h"
#include "TMeter.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TMeterDoc.h"
#include "TMeterView.h"

#include "CRC32.H"

#include "MdiMeasureDoc.h"   //mdi 1
#include "MdiMeasureView.h"
#include "StaticChildWnd.h"

#include "MdiSpectrumDoc.h" // mdi 2
#include "MdiSpectrumView.h"
#include "SpectrumChildWnd.h"

#include "MdiCalibDoc.h"    // mdi 3
#include "MdiCalibView.h"
#include "CalibChildWnd.h"


#include "SpectrumInfo.h"
#include "WorkZoneDataSet.h"
#include "MdiCalibView.h"

#define MAX_APP_DIR_PATH 128
#define UNIQUE_NAME "TMETER.APP"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define LOG_MODE

CTMeterApp theApp;
CSpectrumInfo<DWORD,SPEC_SIZE> g_SpectrumInfo;
CMdiMeasureView*  g_pMeasureView=NULL;
CMdiSpectrumView* g_pSpectrumView=NULL;

char rootPath[128], calFile[12];

void GetAppDirectory(CString& dir);

BOOL IsSamplingFinished(int& percent)
{
	percent=g_SpectrumInfo.GetPercent();
	return g_SpectrumInfo.m_isFinished;
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterApp

BEGIN_MESSAGE_MAP(CTMeterApp, CWinApp)
	//{{AFX_MSG_MAP(CTMeterApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMeterApp construction
CTMeterApp::CTMeterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMeasureTemplate=NULL;
	m_pSpectrumTemplate=NULL;
	m_pWorkZone = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterApp initialization
BOOL CTMeterApp::InitInstance()
{

	m_hOneInstance=::CreateMutex(NULL,FALSE,UNIQUE_NAME);
	if (GetLastError()==ERROR_ALREADY_EXISTS) 
	{
		AfxMessageBox("TMeter is already running!");
		return FALSE;
	}

	char *path=new char[MAX_APP_DIR_PATH];
	GetCurrentDirectory(MAX_APP_DIR_PATH,path);
	m_AppWorkDirectory=path; // "F:\project\TMeter"
	delete[] path;

	if(m_Instrument.LoadHardwareSettings()==FALSE)
	{
		AfxMessageBox("Load Hardware Setting Failed");
		return FALSE;
	}
	
	LoadCrc32Table("Crc32.tab");
	LoadPeriodicTable(&m_PeriodicTable); // ElementTable.xml

	if(m_energyCalib.LoadEnergyCalib() == TRUE)
		m_energyCalib.CalcEnergyCalib(CalibK,CalibB); // EnergyCalib.ini

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pMeasureTemplate = new CMultiDocTemplate(
		IDR_MDIFRAME_TYPE,
		RUNTIME_CLASS(CMdiMeasureDoc),
		RUNTIME_CLASS(CStaticChildWnd), 
		RUNTIME_CLASS(CMdiMeasureView));
	if (!m_pMeasureTemplate)
		return FALSE;
	AddDocTemplate(m_pMeasureTemplate); 

	m_pSpectrumTemplate = new CMultiDocTemplate(
		IDR_MDISPECTRUM_TYPE,
		RUNTIME_CLASS(CMdiSpectrumDoc),
		RUNTIME_CLASS(CSpectrumChildWnd),
		RUNTIME_CLASS(CMdiSpectrumView));
	if (!m_pSpectrumTemplate)
		return FALSE;
	AddDocTemplate(m_pSpectrumTemplate); 

	m_pCalibTemplate = new CMultiDocTemplate(
		IDR_MDICALIB_TYPE,
		RUNTIME_CLASS(CMdiCalibDoc),
		RUNTIME_CLASS(CCalibChildWnd),
		RUNTIME_CLASS(CMdiCalibView));
	if(!m_pCalibTemplate)
		return FALSE;
	AddDocTemplate(m_pCalibTemplate);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo)) // then run CMdiMeasureView::OnInitialUpdate()
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();

	m_pSpectrumTemplate->OpenDocumentFile(NULL);
	m_pMeasureTemplate->OpenDocumentFile(NULL);

	POSITION pos;
	pos=m_pMeasureTemplate->GetFirstDocPosition();
	CMdiMeasureDoc *pDoc=(CMdiMeasureDoc*)m_pMeasureTemplate->GetNextDoc(pos);
	pos=pDoc->GetFirstViewPosition();
	g_pMeasureView=(CMdiMeasureView*)pDoc->GetNextView(pos); 

	pos=m_pSpectrumTemplate->GetFirstDocPosition();
	CMdiSpectrumDoc *pDoc2=(CMdiSpectrumDoc*)m_pSpectrumTemplate->GetNextDoc(pos);
	pos=pDoc2->GetFirstViewPosition();
	g_pSpectrumView=(CMdiSpectrumView*)pDoc2->GetNextView(pos); 


	/* load functions from Fack.dll */
	if(m_IntensityCalculator.LoadModuleLibrary()==FALSE)
	{
		AfxMessageBox("Load Fake.dll Failed!");
	}


	/* load functions from fppro.dll */
	if(m_FpCalculator.LoadModuleLibrary()==FALSE)
	{
		AfxMessageBox("Load fppro.dll Failed!");
	}

	// set short path
	GetCurrentDirectory(128, rootPath);
	strcat(rootPath, "\\FpFiles");
	GetShortPathName(rootPath, rootPath, 64);
	m_FpCalculator.fpSetRootPath(rootPath);
	strcpy(calFile, "temp.cal");

	return TRUE;
}

void CTMeterApp::OpenWorkZone(int id)
{
	CString fileName,fullFpPathName;
	WorkZoneDataSet *pSet = new WorkZoneDataSet();
	if (!pSet->IsOpen()) pSet->Open();
	pSet->m_strFilter.Format("ID = %d",id);
	if(!pSet->Requery()){
		AfxMessageBox(_T("CWorkZoneListDlg::OnInitDialog(), Requery failed!"));
	}
	else{
		if(pSet->IsEOF()) 
		{
			AfxMessageBox("Exception: Query Result IsEOF");
		}
		else
		{
			if(m_pWorkZone) delete m_pWorkZone; /* delete old work-zone */
			m_pWorkZone = new WorkZoneDef();  /* create one work-zone base */
			m_pWorkZone->SetDirIndex(pSet->m_ID);
			m_pWorkZone->Tag = pSet->m_Tag;
			m_pWorkZone->ModifyTime = pSet->m_ModifyTime;
			m_pWorkZone->MeasureTime= pSet->m_MeasureTime;
			m_pWorkZone->CorrectTime= pSet->m_CorrectTime;
			m_pWorkZone->SampleTimes= pSet->m_SampleTimes;
			m_pWorkZone->Repeat = pSet->m_SampleRepeat;
			m_pWorkZone->ContentUnit = pSet->m_ContentUnit;
			m_pWorkZone->ThickUnit   = pSet->m_ThickUnit;
			m_pWorkZone->Precision   = pSet->m_Precision;
			m_pWorkZone->AdjustTimes = pSet->m_AdjustTimes;
			m_pWorkZone->AdjustRepeat= pSet->m_AdjustRepeat;
			m_pWorkZone->MqMax = pSet->m_MqMax;
			m_pWorkZone->Collimator = pSet->m_Collimator;
			m_pWorkZone->Current = pSet->m_Current ;
			
			//get sub-folder name
			fileName.Format("%04d",id);
			fullFpPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + fileName;
			
			//load def.ma
			CString defMaName = fullFpPathName + "\\analysis.xma";
			m_pWorkZone->LoadDefMa(defMaName);
			
			//load samples
			CString stdName = fullFpPathName + "\\sample.std";
			m_pWorkZone->LoadStdSamples(stdName);

			//reset folder
			m_pWorkZone->SetWorkZonePath();
		}
	}
	delete pSet;
}

void CTMeterApp::LoadWorkZonePureElem()
{
	m_IntensityCalculator.LoadPureSpectrum(m_pWorkZone);
}

void CTMeterApp::ReloadPureElemList(WorkZoneDef *pWz)
{
	m_IntensityCalculator.ReloadPureElemList(pWz);
}

double CTMeterApp::GetEnergyByChannel(double ch)
{
	// channel = k*energy + b
	double energy=0.0;
	energy = (ch-CalibB)/CalibK;
	return energy;
}

double CTMeterApp::GetChannelByEnergy(double eng)
{
	// channel = k*energy + b
	double channel=0.0;
	channel = CalibK*eng + CalibB;
	return channel;
}

void CTMeterApp::GetEnergyByEleName(CString ele,double &ka,double &kb)
{
	CClassicalElement* pEle;
	int no = m_PeriodicTable.GetSize();
	for(int i=0;i<no;i++)
	{
		pEle=m_PeriodicTable.GetAt(i);
		if(ele.CompareNoCase(pEle->Name))
		{
			ka = pEle->Ka;
			kb = pEle->Kb;
			return;
		}
	}
}

void CTMeterApp::GetEnergyByAtomicNum(int num,CString &name,double &ka,double &kb,double &kaChannel,double &kbChannel)
{
	if(num<1 || num>92) return;
	CClassicalElement* pEle;
	//int no = m_PeriodicTable.GetSize();
	pEle = m_PeriodicTable.GetAt(num-1);
	//assert(pEle);
	name = pEle->Name;
	ka = pEle->Ka;
	kb = pEle->Kb;

	kaChannel = GetChannelByEnergy(ka); 
	if(kaChannel<0) kaChannel=0;
	else if(kaChannel>255) kaChannel=255;
	kbChannel = GetChannelByEnergy(kb);
	if(kbChannel<0) kbChannel=0;
	else if(kbChannel>255) kbChannel=255;
}


/************************************************/
/*  Get Hardware Parameter From  m_Instrument   */
/************************************************/
short CTMeterApp::GetTargetAtomicNumber()
{
	return m_Instrument.TargetAtomicNumber;
}

float CTMeterApp::GetTargetTakeOffAngle()
{
	return m_Instrument.TargetTakeOffAngle;
}

float CTMeterApp::GetTubeVoltage(int index) 
{
	switch(index)
	{
	case 30:
		return m_Instrument.Hv30HardCode;
		break;
	case 40:
		return m_Instrument.Hv40HardCode;
		break;
	case 50:
		return m_Instrument.Hv50HardCode;
		break;
	default:
		return m_Instrument.Hv30HardCode;
	}
}

float CTMeterApp::GetTubeCurrent(int coll)
{
	switch(coll) // Collimator
	{
	case 1: 
		return m_Instrument.Coll1Current;
		break;
	case 2: 
		return m_Instrument.Coll2Current;
		break;
	case 3: 
		return m_Instrument.Coll3Current;
		break;
	case 4: 
		return m_Instrument.Coll4Current;
		break;
	default:
		return m_Instrument.Coll1Current;
	}
}

float CTMeterApp::GetWindowThickness()
{
	return m_Instrument.WindowThickness;
}

float CTMeterApp::GetIncidentAngle()
{
	return m_Instrument.IncidentAngle;
}

float CTMeterApp::GetEmergentAngle()
{
	return m_Instrument.EmergentAngle;
}

short CTMeterApp::GetFilterAtomicNumber(int index)
{
	switch(index)
	{
	case 0: // FilterNi
		return 28;
		break;
	case 1: // Filter--
		return 0;
		break;
	case 2: // FilterAl
		return 13;
		break;
	default:
		return 28;
	}
}

float CTMeterApp::GetFilterThickness(int index)
{
	switch(index)
	{
	case 0: // FilterNi
		return m_Instrument.FilterNiThick;
		break;
	case 1: // Filter--
		return 0;
		break;
	case 2: // FilterAl
		return m_Instrument.FilterAlThick;
		break;
	default:
		return 0.0;
	}
}

void CTMeterApp::CalcEnergyCalib()
{
	m_energyCalib.CalcEnergyCalib(CalibK,CalibB);
}

void CTMeterApp::TryCloseAllDoc()
{
	POSITION docpos;
	docpos=m_pMeasureTemplate->GetFirstDocPosition();
	while(docpos!=NULL) 
	{
		CMdiMeasureDoc* pDoc=(CMdiMeasureDoc*)m_pMeasureTemplate->GetNextDoc(docpos);
		pDoc->OnCloseDocument();
	}

	docpos=m_pSpectrumTemplate->GetFirstDocPosition();
	while(docpos!=NULL) 
	{
		CMdiSpectrumDoc* pDoc=(CMdiSpectrumDoc*)m_pSpectrumTemplate->GetNextDoc(docpos);
		pDoc->OnCloseDocument();
	}

	docpos=m_pCalibTemplate->GetFirstDocPosition();
	while(docpos!=NULL) 
	{
		CMdiCalibDoc* pDoc=(CMdiCalibDoc*)m_pCalibTemplate->GetNextDoc(docpos);
		pDoc->OnCloseDocument();
	}
}

BOOL CTMeterApp::HasDocUnclosed()
{
	POSITION docpos;
	docpos=m_pMeasureTemplate->GetFirstDocPosition();
	if(docpos!=NULL) return TRUE;

	docpos=m_pSpectrumTemplate->GetFirstDocPosition();
	if(docpos!=NULL) return TRUE;

	docpos=m_pCalibTemplate->GetFirstDocPosition();
	if(docpos!=NULL) return TRUE;

	return FALSE;
}


void CTMeterApp::WriteSwitchWz2Log()
{
#ifdef LOG_MODE
#endif
}

void CTMeterApp::WriteCalibWz2Log()
{
#ifdef LOG_MODE
	AppLog.WriteCalibWz2Log();
#endif
}

void CTMeterApp::WriteMeasureWz2Log()
{
#ifdef LOG_MODE
	AppLog.WriteMeasureWz2Log();
#endif
}

void CTMeterApp::WriteCalcIntensity2Log()
{
#ifdef LOG_MODE
	AppLog.WriteCalcIntensity2Log();
#endif
}

CTMeterApp::~CTMeterApp()
{
	if(!m_pWorkZone) delete m_pWorkZone;
}



//////EnergyCalib//////////////////////////////////////////////////////////////

void EnergyCalib::CalcEnergyCalib(float &k,float &b)
{
	//if(LoadEnergyCalib()==FALSE) return;

	//CALCULATE
	k = (AgChannel-CuChannel)/(AgEnergy-CuEnergy);
	b = AgChannel - k*AgEnergy;

	SaveEnergyCalib(k,b);
}
BOOL EnergyCalib::LoadEnergyCalib()
{
	CStdioFile iniFile;
	int findPos;
	CString inBuff,setting;
	
	if(iniFile.Open("Hardware\\EnergyCalib.INI",CFile::modeRead)) 
	{
		while(iniFile.ReadString(inBuff))
		{
			if(inBuff.Find("AgEnergy")!=-1) { 
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				AgEnergy=(float)atof(setting);	
			}
			else if(inBuff.Find("AgChannel")!=-1) { 
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				AgChannel=(float)atof(setting);	
			}
			else if(inBuff.Find("CuEnergy")!=-1) { 
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				CuEnergy=(float)atof(setting);	
			}
			else if(inBuff.Find("CuChannel")!=-1) {
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				CuChannel=(float)atof(setting);	
			}
		}
		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

void EnergyCalib::SaveEnergyCalib(float k,float b)
{
	CStdioFile iniFile;
	CString outBuff;
	if(iniFile.Open("Hardware\\EnergyCalib.INI",CFile::modeCreate|CFile::modeWrite))
	{
		iniFile.WriteString("# channel = k*energy + b\n");
	
		outBuff.Format("AgEnergy=%6.3f\n",AgEnergy);
		iniFile.WriteString(outBuff);

		outBuff.Format("AgChannel=%6.3f\n",AgChannel);
		iniFile.WriteString(outBuff);

		outBuff.Format("CuEnergy=%6.3f\n",CuEnergy);
		iniFile.WriteString(outBuff);

		outBuff.Format("CuChannel=%6.3f\n",CuChannel);
		iniFile.WriteString(outBuff);

		// save k
		outBuff.Format("K=%6.3f\n",k);
		iniFile.WriteString(outBuff);

		// save b
		outBuff.Format("B=%6.3f\n",b);
		iniFile.WriteString(outBuff);

		iniFile.Close();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTMeterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterApp message handlers


int CTMeterApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	Sleep(500);
	FreePeriodicTable(&m_PeriodicTable);
	return CWinApp::ExitInstance();
}
