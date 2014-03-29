// DefMADataSet.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "DefMADataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DefMADataSet

IMPLEMENT_DYNAMIC(DefMADataSet, CRecordset)

DefMADataSet::DefMADataSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(DefMADataSet)
	m_ID = 0;
	m_Mode = _T("");
	m_Tag = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString DefMADataSet::GetDefaultConnect()
{
	//return _T("ODBC;DSN=TMRecord");
	return _T("ODBC;FILEDSN=./TMeterDataLink.DSN");
}

CString DefMADataSet::GetDefaultSQL()
{
	return _T("[DefMA]");
}

void DefMADataSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(DefMADataSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Mode]"), m_Mode);
	RFX_Text(pFX, _T("[Tag]"), m_Tag);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// DefMADataSet diagnostics

#ifdef _DEBUG
void DefMADataSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void DefMADataSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
