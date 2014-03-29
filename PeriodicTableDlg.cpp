#include "stdafx.h"
#include "TMeter.h"
#include "PeriodicTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeriodicTableDlg dialog

#define BORDER 2000

CPeriodicTableDlg::CPeriodicTableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeriodicTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeriodicTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CPeriodicTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeriodicTableDlg)
	/*
	DDX_Control(pDX, IDC_ELE01, m_btn01);
	DDX_Control(pDX, IDC_ELE02, m_btn02);
	DDX_Control(pDX, IDC_ELE03, m_btn03);
	DDX_Control(pDX, IDC_ELE04, m_btn04);
	DDX_Control(pDX, IDC_ELE05, m_btn05);
	DDX_Control(pDX, IDC_ELE06, m_btn06);
	DDX_Control(pDX, IDC_ELE07, m_btn07);
	DDX_Control(pDX, IDC_ELE08, m_btn08);
	DDX_Control(pDX, IDC_ELE09, m_btn09);
	DDX_Control(pDX, IDC_ELE10, m_btn10);
	DDX_Control(pDX, IDC_ELE11, m_btn11);
	DDX_Control(pDX, IDC_ELE12, m_btn12);
	DDX_Control(pDX, IDC_ELE13, m_btn13);
	DDX_Control(pDX, IDC_ELE14, m_btn14);
	DDX_Control(pDX, IDC_ELE15, m_btn15);
	DDX_Control(pDX, IDC_ELE16, m_btn16);
	DDX_Control(pDX, IDC_ELE17, m_btn17);
	DDX_Control(pDX, IDC_ELE18, m_btn18);
	DDX_Control(pDX, IDC_ELE19, m_btn19);
	DDX_Control(pDX, IDC_ELE20, m_btn20);
	DDX_Control(pDX, IDC_ELE21, m_btn21);
	DDX_Control(pDX, IDC_ELE22, m_btn22);
	DDX_Control(pDX, IDC_ELE23, m_btn23);
	DDX_Control(pDX, IDC_ELE24, m_btn24);
	DDX_Control(pDX, IDC_ELE25, m_btn25);
	DDX_Control(pDX, IDC_ELE26, m_btn26);
	DDX_Control(pDX, IDC_ELE27, m_btn27);
	DDX_Control(pDX, IDC_ELE28, m_btn28);
	DDX_Control(pDX, IDC_ELE29, m_btn29);
	DDX_Control(pDX, IDC_ELE30, m_btn30);
	DDX_Control(pDX, IDC_ELE31, m_btn31);
	DDX_Control(pDX, IDC_ELE32, m_btn32);
	DDX_Control(pDX, IDC_ELE33, m_btn33);
	DDX_Control(pDX, IDC_ELE34, m_btn34);
	DDX_Control(pDX, IDC_ELE35, m_btn35);
	DDX_Control(pDX, IDC_ELE36, m_btn36);
	DDX_Control(pDX, IDC_ELE37, m_btn37);
	DDX_Control(pDX, IDC_ELE38, m_btn38);
	DDX_Control(pDX, IDC_ELE39, m_btn39);
	DDX_Control(pDX, IDC_ELE40, m_btn40);
	DDX_Control(pDX, IDC_ELE41, m_btn41);
	DDX_Control(pDX, IDC_ELE42, m_btn42);
	DDX_Control(pDX, IDC_ELE43, m_btn43);
	DDX_Control(pDX, IDC_ELE44, m_btn44);
	DDX_Control(pDX, IDC_ELE45, m_btn45);
	DDX_Control(pDX, IDC_ELE46, m_btn46);
	DDX_Control(pDX, IDC_ELE47, m_btn47);
	DDX_Control(pDX, IDC_ELE48, m_btn48);
	DDX_Control(pDX, IDC_ELE49, m_btn49);
	DDX_Control(pDX, IDC_ELE50, m_btn50);
	DDX_Control(pDX, IDC_ELE51, m_btn51);
	DDX_Control(pDX, IDC_ELE52, m_btn52);
	DDX_Control(pDX, IDC_ELE53, m_btn53);
	DDX_Control(pDX, IDC_ELE54, m_btn54);
	DDX_Control(pDX, IDC_ELE55, m_btn55);
	DDX_Control(pDX, IDC_ELE56, m_btn56);
	DDX_Control(pDX, IDC_ELE57, m_btn57);
	DDX_Control(pDX, IDC_ELE58, m_btn58);
	DDX_Control(pDX, IDC_ELE59, m_btn59);
	DDX_Control(pDX, IDC_ELE60, m_btn60);
	DDX_Control(pDX, IDC_ELE61, m_btn61);
	DDX_Control(pDX, IDC_ELE62, m_btn62);
	DDX_Control(pDX, IDC_ELE63, m_btn63);
	DDX_Control(pDX, IDC_ELE64, m_btn64);
	DDX_Control(pDX, IDC_ELE65, m_btn65);
	DDX_Control(pDX, IDC_ELE66, m_btn66);
	DDX_Control(pDX, IDC_ELE67, m_btn67);
	DDX_Control(pDX, IDC_ELE68, m_btn68);
	DDX_Control(pDX, IDC_ELE69, m_btn69);
	DDX_Control(pDX, IDC_ELE70, m_btn70);
	DDX_Control(pDX, IDC_ELE71, m_btn71);
	DDX_Control(pDX, IDC_ELE72, m_btn72);
	DDX_Control(pDX, IDC_ELE73, m_btn73);
	DDX_Control(pDX, IDC_ELE74, m_btn74);
	DDX_Control(pDX, IDC_ELE75, m_btn75);
	DDX_Control(pDX, IDC_ELE76, m_btn76);
	DDX_Control(pDX, IDC_ELE77, m_btn77);
	DDX_Control(pDX, IDC_ELE78, m_btn78);
	DDX_Control(pDX, IDC_ELE79, m_btn79);
	DDX_Control(pDX, IDC_ELE80, m_btn80);
	DDX_Control(pDX, IDC_ELE81, m_btn81);
	DDX_Control(pDX, IDC_ELE82, m_btn82);
	DDX_Control(pDX, IDC_ELE83, m_btn83);
	DDX_Control(pDX, IDC_ELE84, m_btn84);
	DDX_Control(pDX, IDC_ELE85, m_btn85);
	DDX_Control(pDX, IDC_ELE86, m_btn86);
	DDX_Control(pDX, IDC_ELE87, m_btn87);
	DDX_Control(pDX, IDC_ELE88, m_btn88);
	DDX_Control(pDX, IDC_ELE89, m_btn89);
	DDX_Control(pDX, IDC_ELE90, m_btn90);
	DDX_Control(pDX, IDC_ELE91, m_btn91);
	DDX_Control(pDX, IDC_ELE92, m_btn92);
	*/
	//}}AFX_DATA_MAP
}

void CPeriodicTableDlg::OnOK()
{
	//press Enter
	DestroyWindow(); 
}

void CPeriodicTableDlg::OnCancel()
{
	//press ESC or 'Close' , if u want to destroy dialog resource
	DestroyWindow(); 
}

BEGIN_MESSAGE_MAP(CPeriodicTableDlg, CDialog)
	//{{AFX_MSG_MAP(CPeriodicTableDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED,IDC_ELE01,IDC_ELE92,OnClickElement)
	ON_BN_CLICKED(IDC_CLEANALL, OnCleanAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeriodicTableDlg message handlers

BOOL CPeriodicTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// draw button background
	/*
	for(int i=0;i<92;i++)
	{
		((CButtonST*)GetDlgItem(BORDER+1+i))->SetFlat(FALSE);
		//((CButtonST*)GetDlgItem(BORDER+1+i))->SetActiveBgColor(RGB(152,245,255)); // change colour when click
		((CButtonST*)GetDlgItem(BORDER+1+i))->SetInactiveBgColor(RGB(152,245,255));
		//((CButtonST*)GetDlgItem(BORDER+1+i))->SetShowText(FALSE);
		//((CButtonST*)GetDlgItem(BORDER+1+i))->SetAlign(ST_ALIGN_VERT);
		//((CButtonST*)GetDlgItem(BORDER+1+i))->SetWindowText("12\r\nMg");
	}*/
	return TRUE; 
}

void CPeriodicTableDlg::OnClickElement(UINT uID)
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	int atomicNum = uID-BORDER;
	CString element;
	double ka,kb;
	double kaChannel,kbChannel;
	pApp->GetEnergyByAtomicNum(atomicNum,element,ka,kb,kaChannel,kbChannel);
	g_pSpectrumView->m_SpectrumDisplay.AddAnalysisLines(element,kaChannel,kbChannel);
	g_pSpectrumView->m_SpectrumDisplay.Update();

}
void CPeriodicTableDlg::OnCleanAll()
{
	g_pSpectrumView->m_SpectrumDisplay.FreeAnalysisLineArray();
	g_pSpectrumView->m_SpectrumDisplay.Update();
}

void CPeriodicTableDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CPeriodicTableDlg::OnClose() 
{
	CDialog::OnClose();
}

void CPeriodicTableDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(0);
}

void CPeriodicTableDlg::PostNcDestroy() 
{
	delete this;
	g_pSpectrumView->m_pPeriodicTableDlg=NULL;
	CDialog::PostNcDestroy();
}
