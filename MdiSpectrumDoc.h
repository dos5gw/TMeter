#if !defined(AFX_MDISPECTRUMDOC_H__3C0CC186_0341_4F8B_8DC5_F856A5324C7D__INCLUDED_)
#define AFX_MDISPECTRUMDOC_H__3C0CC186_0341_4F8B_8DC5_F856A5324C7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiSpectrumDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMdiSpectrumDoc document

class CMdiSpectrumDoc : public CDocument
{
protected:
	CMdiSpectrumDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiSpectrumDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiSpectrumDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMdiSpectrumDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMdiSpectrumDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDISPECTRUMDOC_H__3C0CC186_0341_4F8B_8DC5_F856A5324C7D__INCLUDED_)
