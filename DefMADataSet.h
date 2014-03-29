#if !defined(AFX_DEFMADATASET_H__6C9FF6DB_E5A6_4BFD_BE3A_CFCAECD77A0B__INCLUDED_)
#define AFX_DEFMADATASET_H__6C9FF6DB_E5A6_4BFD_BE3A_CFCAECD77A0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefMADataSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DefMADataSet recordset

class DefMADataSet : public CRecordset
{
public:
	DefMADataSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(DefMADataSet)

// Field/Param Data
	//{{AFX_FIELD(DefMADataSet, CRecordset)
	long	m_ID;
	CString	m_Mode;
	CString	m_Tag;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DefMADataSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFMADATASET_H__6C9FF6DB_E5A6_4BFD_BE3A_CFCAECD77A0B__INCLUDED_)
