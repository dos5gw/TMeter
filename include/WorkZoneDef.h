// WorkZoneDef.h: interface for the WorkZoneDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKZONEDEF_H__0161591B_1E86_4933_8EDA_E6107891D9A9__INCLUDED_)
#define AFX_WORKZONEDEF_H__0161591B_1E86_4933_8EDA_E6107891D9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ClassicalElement.h"
#include "DefMa.h"
#include "StdSample.h"
#include "tinyxml.h"
#include "tinystr.h"


class WorkZoneElemList : public CTypedPtrArray<CObArray,CClassicalElement*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
};

class WorkZoneDef : public CObject  
{
	friend class CDefineProductTab;
	friend class CDefineAppTab;
	friend class CDefineUnitTab;
	friend class CCreateWorkZoneDlg;
	friend class CTMeterApp;
	friend class CSelCollimatorDlg;
	friend class CMdiMeasureView;

public:
	CDefMa defMa;
	CStandardSamples stdSample;

public:
	WorkZoneDef();
	virtual ~WorkZoneDef();
	virtual void Serialize(CArchive& ar);
	void XMLSerialize(BOOL IsStoring,TiXmlDocument* pDoc);
	void INISerialize(BOOL IsStoring,const CString &file);
	
	CString GetTag(){return Tag;}
	void SetTag(CString tag){Tag=tag;}
	int GetDirIndex(){return DirNameIndex;}
	void SetDirIndex(int index){DirNameIndex=index;}

	void SetWorkZonePath();
	CString GetFullSpectrumPath()
	{
		return m_SpectrumPath;
	};
	CString GetFullFpPath()
	{
		return m_FpPath;
	};
	CString GetFullLogPath()
	{
		return m_LogPath;
	}

	void LoadDefMa(CString file);   // called when opening exist work-zone 
	void LoadStdSamples(CString file); // called when opening exist work-zone 
	void FpCalibrate();  // called when 
	
	void SaveDefMa();
	void SaveStdSample();

//private:
	int DirNameIndex; // work-zone directory, database main key, i.e. "0002" mean directory named "TMeter\\DefineFP\\0002"

	/***产品程式***/
	//int Index;   //编号
	CString Tag; // "CrV/Ni/Cu/Fe"
	CTime ModifyTime; //修改时间
	CTime MeasureTime;//测量时间
	CTime CorrectTime;//调校,归一化时间
	int SampleTimes;  // = 测量秒数/10
	int Repeat;       // 测量次数
	int ContentUnit;  //0:%, 1:o/oo, 2:ppm, 3:Karat
	int ThickUnit;    //0:um, 1:nm, 2:mils, 3:u"
	int Precision;    //1:0.0, 2:0.00, 3:0.000, 4:0.0000

	/***应用程式***/
	int AdjustTimes; // = 调校秒数/10
	int AdjustRepeat;// 调校次数
	int MqMax;
	int Collimator; //准直器, 1,2,3,4
	int Current;    //User Define Current, it will make effect if Current!=0
	
	/***表达方式***/
	BOOL ArrayVariable; 
	int  ArraySize;
	int ReportStyle; //0:大数字, 1:数表, 2:SPC


	/** full path name **/
	CString m_SpectrumPath;  // i.e D:\project\TMeter\SpecData\HV50\FLT1
	CString m_FpPath;        // i.e D:\project\TMeter\DefineFP\0037
	CString m_LogPath;       // i.e D:\project\TMeter\Log
};

#endif // !defined(AFX_WORKZONEDEF_H__0161591B_1E86_4933_8EDA_E6107891D9A9__INCLUDED_)
