// CalibChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "CalibChildWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibChildWnd

IMPLEMENT_DYNCREATE(CCalibChildWnd, CMDIChildWnd)

CCalibChildWnd::CCalibChildWnd()
{
}

CCalibChildWnd::~CCalibChildWnd()
{
}


BEGIN_MESSAGE_MAP(CCalibChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCalibChildWnd)
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibChildWnd message handlers

int CCalibChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	HMENU hMenu; 
	hMenu = ::GetSystemMenu(this-> m_hWnd,FALSE); 
	EnableMenuItem(hMenu,SC_CLOSE|MF_BYCOMMAND,MF_GRAYED);
	return 0;
}

void CCalibChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	if(pActivateWnd != NULL &&  pActivateWnd->m_hWnd == this->m_hWnd) // first param
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
		if(pMainFrame != NULL)
		{
			pMainFrame->m_wndToolBar.LoadToolBar(IDR_MDICALIB_TYPE);
			//pMainFrame->m_wndToolBar.AdjustLayout(); 
		}
	}
	if(pDeactivateWnd != NULL && pDeactivateWnd->m_hWnd == this->m_hWnd) // second param
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


