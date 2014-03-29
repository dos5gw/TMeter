// OneResult.cpp: implementation of the COneResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tmeter.h"
#include "OneResult.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COneResult::COneResult()
{
	m_pResultData = (FP_SAMPLE_DATA*)malloc(sizeof(FP_SAMPLE_DATA));
	memset((void*)m_pResultData,0,sizeof(FP_SAMPLE_DATA));
}

COneResult::~COneResult()
{
	free(m_pResultData);
}


float COneResult::GetLayerThickness(int nLayer)
{
	if(nLayer < MAXCC)
		return m_pResultData->LayerThickness[nLayer];
	else
		return 0.f;
}

float COneResult::GetElemContent(CString ele)
{
	for(int i=0;i<MAXELT;i++)
	{
		if(ele.Compare(m_pResultData->ConstituentFormula[i]) == 0)
		{
			return m_pResultData->ConstituentConcentration[i];
		}
	}
	return 0.f;
}

void COneResult::ResetData()
{
	memset((void*)m_pResultData,0,sizeof(FP_SAMPLE_DATA));
	m_AnalysisTime = GetCurrentTime();
}

void COneResult::SaveToDatabase()
{

}