// DefMa.h: interface for the CDefMa class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFMA_H__2FE5A9E1_77F9_427A_B6AB_3EF6B52DF415__INCLUDED_)
#define AFX_DEFMA_H__2FE5A9E1_77F9_427A_B6AB_3EF6B52DF415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tinystr.h"
#include "tinyxml.h"
#include "OneLayerDefine.h"
//#include "CreateDefmaDlg.h"

class CDefMa : public CObject
{
	friend class CCreateDefmaDlg;
public:
	CDefMa();
	virtual ~CDefMa();
	void CopyFrom(CDefMa*);

public:
	//void AddOneLayer();
	//void DelOneLayer();
	//void RepOneLayer();

//private:
	LayerArray m_layerArray;
	CString m_tag;  // also alias CStandardSample's Name
	CString m_thickMode;
	CString m_note;

	int m_hv; // 30, 40, 50
	int m_filter; //0:Ni, 1:--, 2:Al, filter change also change MA

	BOOL bAllElemArray;
	BOOL bMqShown;
	BOOL bScaleCompensated;
	BOOL bOffsetCompensated;
	BOOL bFilterUsed;
	BOOL bNeedMeasureElem;

public:
	CString GetStdSampleName();
	
	BOOL LoadSettings(CString file);
	BOOL SaveSettings(CString file);

	void RemoveAll();
	void XMLSerialize(BOOL bStoring,TiXmlDocument* pDoc);

};

#endif // !defined(AFX_DEFMA_H__2FE5A9E1_77F9_427A_B6AB_3EF6B52DF415__INCLUDED_)
