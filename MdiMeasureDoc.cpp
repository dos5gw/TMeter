// MdiMeasureDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "MdiMeasureDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureDoc

IMPLEMENT_DYNCREATE(CMdiMeasureDoc, CDocument)

CMdiMeasureDoc::CMdiMeasureDoc()
{
}

BOOL CMdiMeasureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMdiMeasureDoc::~CMdiMeasureDoc()
{
}


BEGIN_MESSAGE_MAP(CMdiMeasureDoc, CDocument)
	//{{AFX_MSG_MAP(CMdiMeasureDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureDoc diagnostics

#ifdef _DEBUG
void CMdiMeasureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMdiMeasureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureDoc serialization

void CMdiMeasureDoc::Serialize(CArchive& ar)
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
// CMdiMeasureDoc commands
