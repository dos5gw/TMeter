// SpectrumAnalyzeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "SpectrumAnalyzeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzeDlg dialog


CSpectrumAnalyzeDlg::CSpectrumAnalyzeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpectrumAnalyzeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpectrumAnalyzeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSpectrumAnalyzeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpectrumAnalyzeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpectrumAnalyzeDlg, CDialog)
	//{{AFX_MSG_MAP(CSpectrumAnalyzeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzeDlg message handlers
