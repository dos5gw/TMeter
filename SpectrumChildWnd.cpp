// SpectrumChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "SpectrumChildWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpectrumChildWnd

IMPLEMENT_DYNCREATE(CSpectrumChildWnd, CMDIChildWnd)

CSpectrumChildWnd::CSpectrumChildWnd()
{
}

CSpectrumChildWnd::~CSpectrumChildWnd()
{
}


BEGIN_MESSAGE_MAP(CSpectrumChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSpectrumChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectrumChildWnd message handlers


int CSpectrumChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	HMENU hMenu; 
	hMenu = ::GetSystemMenu(this-> m_hWnd,FALSE); 
	EnableMenuItem(hMenu,SC_CLOSE|MF_BYCOMMAND,MF_GRAYED);
	return 0;
}

void CSpectrumChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if(pActivateWnd != NULL &&  pActivateWnd->m_hWnd == this->m_hWnd)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
		if(pMainFrame != NULL)
		{
			pMainFrame->m_wndToolBar.LoadToolBar(IDR_MDISPECTRUM_TYPE);
			//pMainFrame->m_wndToolBar.AdjustLayout(); 
		}
	}
	if(pDeactivateWnd != NULL && pDeactivateWnd->m_hWnd == this->m_hWnd)
	{
		if(pActivateWnd == NULL)
		{
			CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
			if(pMainFrame != NULL)
			{
				pMainFrame->m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
				//pMainFrame->m_wndToolBar.AdjustLayout();
			}
		}
	}
	
}
