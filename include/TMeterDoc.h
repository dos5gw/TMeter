// TMeterDoc.h : interface of the CTMeterDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMETERDOC_H__375CFC39_3C55_456A_9CE4_8596372A8421__INCLUDED_)
#define AFX_TMETERDOC_H__375CFC39_3C55_456A_9CE4_8596372A8421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTMeterDoc : public CDocument
{
protected: // create from serialization only
	CTMeterDoc();
	DECLARE_DYNCREATE(CTMeterDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMeterDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTMeterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTMeterDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMETERDOC_H__375CFC39_3C55_456A_9CE4_8596372A8421__INCLUDED_)
