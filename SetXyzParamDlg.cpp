// SetXyzParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "SetXyzParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetXyzParamDlg dialog


CSetXyzParamDlg::CSetXyzParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetXyzParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetXyzParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetXyzParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetXyzParamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetXyzParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSetXyzParamDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetXyzParamDlg message handlers
