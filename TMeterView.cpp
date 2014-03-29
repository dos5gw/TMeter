// TMeterView.cpp : implementation of the CTMeterView class
//

#include "stdafx.h"
#include "TMeter.h"

#include "TMeterDoc.h"
#include "TMeterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMeterView

IMPLEMENT_DYNCREATE(CTMeterView, CView)

BEGIN_MESSAGE_MAP(CTMeterView, CView)
	//{{AFX_MSG_MAP(CTMeterView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMeterView construction/destruction

CTMeterView::CTMeterView()
{
	// TODO: add construction code here

}

CTMeterView::~CTMeterView()
{
}

BOOL CTMeterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterView drawing

void CTMeterView::OnDraw(CDC* pDC)
{
	CTMeterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterView printing

BOOL CTMeterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTMeterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTMeterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTMeterView diagnostics

#ifdef _DEBUG
void CTMeterView::AssertValid() const
{
	CView::AssertValid();
}

void CTMeterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTMeterDoc* CTMeterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTMeterDoc)));
	return (CTMeterDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTMeterView message handlers
