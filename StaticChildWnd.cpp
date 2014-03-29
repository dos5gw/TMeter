// StaticChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "StaticChildWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticChildWnd

IMPLEMENT_DYNCREATE(CStaticChildWnd, CMDIChildWnd)

CStaticChildWnd::CStaticChildWnd()
{
}

CStaticChildWnd::~CStaticChildWnd()
{
}


BEGIN_MESSAGE_MAP(CStaticChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStaticChildWnd)
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticChildWnd message handlers

int CStaticChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	HMENU hMenu; 
	hMenu = ::GetSystemMenu(this-> m_hWnd,FALSE); 
	EnableMenuItem(hMenu,SC_CLOSE|MF_BYCOMMAND,MF_GRAYED);
	return 0;
}

void CStaticChildWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	if(pActivateWnd != NULL &&  pActivateWnd->m_hWnd == this->m_hWnd) // first param
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
		if(pMainFrame != NULL)
		{
			pMainFrame->m_wndToolBar.LoadToolBar(IDR_MDIFRAME_TYPE);
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
