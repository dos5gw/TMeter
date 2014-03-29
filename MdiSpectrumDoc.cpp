// MdiSpectrumDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "MdiSpectrumDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumDoc

IMPLEMENT_DYNCREATE(CMdiSpectrumDoc, CDocument)

CMdiSpectrumDoc::CMdiSpectrumDoc()
{
}

BOOL CMdiSpectrumDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMdiSpectrumDoc::~CMdiSpectrumDoc()
{
}


BEGIN_MESSAGE_MAP(CMdiSpectrumDoc, CDocument)
	//{{AFX_MSG_MAP(CMdiSpectrumDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumDoc diagnostics

#ifdef _DEBUG
void CMdiSpectrumDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMdiSpectrumDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumDoc serialization

void CMdiSpectrumDoc::Serialize(CArchive& ar)
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
// CMdiSpectrumDoc commands
