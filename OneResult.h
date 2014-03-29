// OneResult.h: interface for the COneResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONERESULT_H__15849FCE_36B3_4ACC_B060_D2B2284BC505__INCLUDED_)
#define AFX_ONERESULT_H__15849FCE_36B3_4ACC_B060_D2B2284BC505__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DefMa.h"
#include "FpCalculator.h"

class COneResult  :public CObject
{
	friend class CMdiMeasureView;
	friend class FpCalculator;
public:
	COneResult();
	virtual ~COneResult();

private: 
	FP_SAMPLE_DATA *m_pResultData;
	CTime m_AnalysisTime;

public:  // method

	float GetLayerThickness(int nLayer);
	float GetElemContent(CString ele);
	void ResetData();
	void SaveToDatabase();

};

#endif // !defined(AFX_ONERESULT_H__15849FCE_36B3_4ACC_B060_D2B2284BC505__INCLUDED_)
