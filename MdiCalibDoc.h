#if !defined(AFX_MDICALIBDOC_H__FA4D8BB2_0380_4AD4_86CE_7B5D98636663__INCLUDED_)
#define AFX_MDICALIBDOC_H__FA4D8BB2_0380_4AD4_86CE_7B5D98636663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiCalibDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMdiCalibDoc document

class CMdiCalibDoc : public CDocument
{
protected:
	CMdiCalibDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMdiCalibDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiCalibDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMdiCalibDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMdiCalibDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICALIBDOC_H__FA4D8BB2_0380_4AD4_86CE_7B5D98636663__INCLUDED_)
