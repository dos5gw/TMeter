// TMeterDoc.cpp : implementation of the CTMeterDoc class
//

#include "stdafx.h"
#include "TMeter.h"

#include "TMeterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMeterDoc

IMPLEMENT_DYNCREATE(CTMeterDoc, CDocument)

BEGIN_MESSAGE_MAP(CTMeterDoc, CDocument)
	//{{AFX_MSG_MAP(CTMeterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMeterDoc construction/destruction

CTMeterDoc::CTMeterDoc()
{
	// TODO: add one-time construction code here

}

CTMeterDoc::~CTMeterDoc()
{
}

BOOL CTMeterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTMeterDoc serialization

void CTMeterDoc::Serialize(CArchive& ar)
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
// CTMeterDoc diagnostics

#ifdef _DEBUG
void CTMeterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTMeterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTMeterDoc commands
