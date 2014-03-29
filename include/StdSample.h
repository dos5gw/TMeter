// StdSample.h: interface for the CStdSample class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDSAMPLE_H__84607D32_D64F_4A1F_AFD5_F52F996A164E__INCLUDED_)
#define AFX_STDSAMPLE_H__84607D32_D64F_4A1F_AFD5_F52F996A164E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DefMa.h"
#include "tinyxml.h"
#include "tinystr.h"

#define MAX_ELE_NUM 35  /* define in fp-pro head file*/
#define MAX_STD_NUM 10

class CStandardSample : public CDefMa
{
public:
	void XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode);

};

class CStandardSamples : public CTypedPtrArray<CObArray,CStandardSample*>
{
	friend class CStdSampleDlg;
private:
	void CleanAll();
public:

	CStandardSamples();
	~CStandardSamples();

	void SetIntensity(int index); // Set Intensity for GetAt(index)

	BOOL IsEmpty() {return (GetSize()==0);}
	BOOL LoadSourceData(CString file); 
	BOOL SaveSourceData(CString file);
	void XMLSerialize(BOOL IsStoring,TiXmlDocument* fatherNode);

};



/************************************************************************/
/*               the following class are not used                       */
/************************************************************************/
class OneStdLayer : public CObject
{
public:
	OneStdLayer();
	OneStdLayer(int index,float thick,float theory, float correct);
	int m_Index;
	float m_Thickness;
	float m_TheoryThickness;
	float m_CorrectThickness;
	virtual void Serialize(CArchive& ar);
	~OneStdLayer();

};

class CStdSample : public CTypedPtrArray<CObArray,OneStdLayer*>
{
public:
	int nLayers;
	void ClearAll();
	BOOL IsEmpty() {return (GetSize()==0);}
	virtual void Serialize(CArchive& ar);

};


class CStdSamples : public CTypedPtrArray<CObArray,CStdSample*>
{
	friend class CStdSampleDlg;
public:
	int nSamples;
	int nLayers;
	void ClearAll();
	void ZeroAll();
	BOOL IsEmpty() {return (GetSize()==0);}
	virtual void Serialize(CArchive& ar);
	void XMLSerialize(BOOL IsStoring,TiXmlDocument* pDoc);

	float Thickness[MAX_STD_NUM][MAX_ELE_NUM]; // not used
	float TheoryThickness[MAX_STD_NUM][MAX_ELE_NUM];  // not used
	float CorrectThickness[MAX_STD_NUM][MAX_ELE_NUM]; // not used

	BOOL LoadSourceData(CString file); // not used
	BOOL SaveSourceData(CString file); // not used
	
};

#endif // !defined(AFX_STDSAMPLE_H__84607D32_D64F_4A1F_AFD5_F52F996A164E__INCLUDED_)
