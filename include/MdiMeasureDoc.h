#if !defined(AFX_MDIMEASUREDOC_H__D49D8073_9168_4838_9D63_7563EB017EE1__INCLUDED_)
#define AFX_MDIMEASUREDOC_H__D49D8073_9168_4838_9D63_7563EB017EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiMeasureDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMdiMeasureDoc document

class CMdiMeasureDoc : public CDocument
{
protected:
	CMdiMeasureDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiMeasureDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiMeasureDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMdiMeasureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMdiMeasureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIMEASUREDOC_H__D49D8073_9168_4838_9D63_7563EB017EE1__INCLUDED_)
