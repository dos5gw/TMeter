// OneLayerDefine.h: interface for the COneLayerDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONELAYERDEFINE_H__A3C42DF9_D669_4D34_82BF_DFEEC6B1CA6C__INCLUDED_)
#define AFX_ONELAYERDEFINE_H__A3C42DF9_D669_4D34_82BF_DFEEC6B1CA6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxtempl.h>
//#include "IntensityCalculator.h"
#include "ClassicalElement.h"
#include "tinystr.h"
#include "tinyxml.h"

class LayerElementList : public CTypedPtrArray<CObArray,CClassicalElement*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
};

class OneLayerDefine  :public CObject
{
public:
	OneLayerDefine();
	virtual ~OneLayerDefine();
	void CopyFrom(OneLayerDefine*);
public:
	LayerElementList m_elementList; //get Element from PeriodicTable CTMeterApp::m_PeriodicTable
	
	int m_thickMode;   // 0:calculate thickness, 1:correct thickness, 2:thickness is known
	int m_contentMode; // 0:calculate content ,  1:correct content,   2:content is known
	int NumOfElement; 
	float m_thickness; //um, microns
	float m_theoryness;  // @2012-08-22 theory thickness
	float m_correctness; // @2012-08-22 correct thickness
	float m_density;   //g/cm^3
	BOOL m_isSaturated; //(ÎÞÇîºñ)±¥ºÍ
	BOOL m_isExist;  // used in standard sample

	//¸Ã¶Æ²ãÊÇ"±³¾°¹âÆ×":
	BOOL isBackground;   //if true, this layer is background spectrum
	BOOL isCompSpectrum; //²¹³¥¹âÆ×
	CString CompSpectrumString;
	BOOL isSubsSpectrum; //µ×²Ä¹âÆ×
	CString SubsSpectrumString;



	CString GetLayerName();

	float CalcDensity();
	float CalcAverageDensity();
	void RemoveAll();
	void XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode);

};

class LayerArray : public CTypedPtrArray<CObArray,OneLayerDefine*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
	
};

#endif // !defined(AFX_ONELAYERDEFINE_H__A3C42DF9_D669_4D34_82BF_DFEEC6B1CA6C__INCLUDED_)
