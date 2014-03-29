#if !defined(AFX_WORKZONEDATASET_H__926C9C72_24F7_4DBB_BC93_2B37398C275C__INCLUDED_)
#define AFX_WORKZONEDATASET_H__926C9C72_24F7_4DBB_BC93_2B37398C275C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkZoneDataSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WorkZoneDataSet recordset

class WorkZoneDataSet : public CRecordset
{
public:
	WorkZoneDataSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(WorkZoneDataSet)

// Field/Param Data
	//{{AFX_FIELD(WorkZoneDataSet, CRecordset)
	long	m_ID;
	CString	m_Tag;
	CTime	m_ModifyTime;
	CTime	m_MeasureTime;
	CTime	m_CorrectTime;
	long	m_SampleTimes;
	long	m_SampleRepeat;
	long	m_ContentUnit;
	long	m_ThickUnit;
	long	m_Precision;
	long	m_AdjustTimes;
	long	m_AdjustRepeat;
	long	m_MqMax;
	long	m_Collimator;
	long	m_Current;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WorkZoneDataSet)
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

#endif // !defined(AFX_WORKZONEDATASET_H__926C9C72_24F7_4DBB_BC93_2B37398C275C__INCLUDED_)
