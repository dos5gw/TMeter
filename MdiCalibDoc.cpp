// MdiCalibDoc.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "MdiCalibDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibDoc

IMPLEMENT_DYNCREATE(CMdiCalibDoc, CDocument)

CMdiCalibDoc::CMdiCalibDoc()
{
}

BOOL CMdiCalibDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMdiCalibDoc::~CMdiCalibDoc()
{
}


BEGIN_MESSAGE_MAP(CMdiCalibDoc, CDocument)
	//{{AFX_MSG_MAP(CMdiCalibDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibDoc diagnostics

#ifdef _DEBUG
void CMdiCalibDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMdiCalibDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibDoc serialization

void CMdiCalibDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibDoc commands
