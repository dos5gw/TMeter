// TMeterView.h : interface of the CTMeterView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMETERVIEW_H__DDDBB7D4_701D_4615_84AA_3F8FD1E6CD12__INCLUDED_)
#define AFX_TMETERVIEW_H__DDDBB7D4_701D_4615_84AA_3F8FD1E6CD12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTMeterView : public CView
{
protected: // create from serialization only
	CTMeterView();
	DECLARE_DYNCREATE(CTMeterView)

// Attributes
public:
	CTMeterDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMeterView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTMeterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTMeterView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TMeterView.cpp
inline CTMeterDoc* CTMeterView::GetDocument()
   { return (CTMeterDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMETERVIEW_H__DDDBB7D4_701D_4615_84AA_3F8FD1E6CD12__INCLUDED_)
