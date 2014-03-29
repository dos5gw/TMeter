// WorkZoneDataSet.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "WorkZoneDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WorkZoneDataSet

IMPLEMENT_DYNAMIC(WorkZoneDataSet, CRecordset)

WorkZoneDataSet::WorkZoneDataSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(WorkZoneDataSet)
	m_ID = 0;
	m_Tag = _T("");
	m_SampleTimes = 0;
	m_SampleRepeat = 0;
	m_ContentUnit = 0;
	m_ThickUnit = 0;
	m_Precision = 0;
	m_AdjustTimes = 0;
	m_AdjustRepeat = 0;
	m_MqMax = 0;
	m_Collimator = 0;
	m_nFields = 15;
	m_Current = 0;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString WorkZoneDataSet::GetDefaultConnect()
{
	//return _T("ODBC;DSN=TMRecord");
	//return _T("ODBC;FILEDSN=./BxDataLink.DSN;PWD=Bx3DataBase");
	return _T("ODBC;FILEDSN=./TMeterDataLink.DSN");
}

CString WorkZoneDataSet::GetDefaultSQL()
{
	return _T("[WorkZone]");
}

void WorkZoneDataSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(WorkZoneDataSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Tag]"), m_Tag);
	RFX_Date(pFX, _T("[ModifyTime]"), m_ModifyTime);
	RFX_Date(pFX, _T("[MeasureTime]"), m_MeasureTime);
	RFX_Date(pFX, _T("[CorrectTime]"), m_CorrectTime);
	RFX_Long(pFX, _T("[SampleTimes]"), m_SampleTimes);
	RFX_Long(pFX, _T("[SampleRepeat]"), m_SampleRepeat);
	RFX_Long(pFX, _T("[ContentUnit]"), m_ContentUnit);
	RFX_Long(pFX, _T("[ThickUnit]"), m_ThickUnit);
	RFX_Long(pFX, _T("[Precision]"), m_Precision);
	RFX_Long(pFX, _T("[AdjustTimes]"), m_AdjustTimes);
	RFX_Long(pFX, _T("[AdjustRepeat]"), m_AdjustRepeat);
	RFX_Long(pFX, _T("[MqMax]"), m_MqMax);
	RFX_Long(pFX, _T("[Collimator]"), m_Collimator);
	RFX_Long(pFX, _T("[Current]"), m_Current);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// WorkZoneDataSet diagnostics

#ifdef _DEBUG
void WorkZoneDataSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void WorkZoneDataSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
