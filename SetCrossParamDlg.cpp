// SetCrossParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "SetCrossParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCrossParamDlg dialog


CSetCrossParamDlg::CSetCrossParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCrossParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCrossParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetCrossParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCrossParamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCrossParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCrossParamDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCrossParamDlg message handlers
