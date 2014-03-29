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

	/***��Ʒ��ʽ***/
	//int Index;   //���
	CString Tag; // "CrV/Ni/Cu/Fe"
	CTime ModifyTime; //�޸�ʱ��
	CTime MeasureTime;//����ʱ��
	CTime CorrectTime;//��У,��һ��ʱ��
	int SampleTimes;  // = ��������/10
	int Repeat;       // ��������
	int ContentUnit;  //0:%, 1:o/oo, 2:ppm, 3:Karat
	int ThickUnit;    //0:um, 1:nm, 2:mils, 3:u"
	int Precision;    //1:0.0, 2:0.00, 3:0.000, 4:0.0000

	/***Ӧ�ó�ʽ***/
	int AdjustTimes; // = ��У����/10
	int AdjustRepeat;// ��У����
	int MqMax;
	int Collimator; //׼ֱ��, 1,2,3,4
	int Current;    //User Define Current, it will make effect if Current!=0
	
	/***��﷽ʽ***/
	BOOL ArrayVariable; 
	int  ArraySize;
	int ReportStyle; //0:������, 1:����, 2:SPC


	/** full path name **/
	CString m_SpectrumPath;  // i.e D:\project\TMeter\SpecData\HV50\FLT1
	CString m_FpPath;        // i.e D:\project\TMeter\DefineFP\0037
	CString m_LogPath;       // i.e D:\project\TMeter\Log
};

#endif // !defined(AFX_WORKZONEDEF_H__0161591B_1E86_4933_8EDA_E6107891D9A9__INCLUDED_)
