// ClassicalElement.h: interface for the CClassicalElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSICALELEMENT_H__E66A0202_94EE_452D_A994_091B58869C90__INCLUDED_)
#define AFX_CLASSICALELEMENT_H__E66A0202_94EE_452D_A994_091B58869C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "tinystr.h"
#include "tinyxml.h"

//typedef enum Lines {Ka,Kb,La,Lb} Lines;
class CClassicalElement : public CObject  
{
public:
	CClassicalElement():Name("NC")
	{
		Id=0;
		Weight=0;
		Density=0;
		Ka=0;
		Kb=0;
		La=0;
		Lb=0;
		Lr=0;
		Le=0;
		DefaultLine=1; //Ka
		Content=100.00;
		beActive=TRUE;
		WinLeft = 10;
		WinRight = 245;
		Intensity = 0.0;
	}
	CClassicalElement(int id,CString name,float weight,float density,float ka,float kb,int line):Id(id),Name(name),Weight(weight),Density(density),Ka(ka),Kb(kb),DefaultLine(line)
	{
		La=0;
		Lb=0;
		Lr=0;
		Le=0;
		DefaultLine=1; //Ka

		Intensity = 0.0;
		Content=100.00;
		beActive=TRUE;
		WinLeft = 10;
		WinRight = 245;
	}
	CClassicalElement(CClassicalElement* p)
	{
		//assert(p);
		Id=p->Id;
		Name=p->Name;
		Weight=p->Weight;
		Density=p->Density;
		Ka=p->Ka;
		Kb=p->Kb;
		La=p->La;
		Lb=p->Lb;
		Lr=p->Lr;
		Le=p->Le;
		DefaultLine=p->DefaultLine; 

		Intensity = p->Intensity;
		Content=p->Content;
		beActive=p->beActive;
		WinLeft = p->WinLeft;
		WinRight = p->WinRight;
	}
	virtual ~CClassicalElement()
	{
		//do nothing
	}

public:
	// the following member are loaded from PeriodicTable.xml
	int Id;
	CString Name;
	float Weight;
	float Density;
	float Ka; // Energy at Ka
	float Kb; // Energy at Kb
	float La; // Energy at La
	float Lb; // Energy at Lb
	float Lr; // Energy at Lr
	float Le;
	int DefaultLine; //1-Ka , 2-Kb , 3-La , 4-Lb , 5-Lr, 6-Ma 
	
	// the following member are defined in standard or FP_SAMPLE_DATA
	float Intensity;      //added @2012-08-21, used in standard samples
	float Content;  //added @2012-07-02, used in defMA, Range 0.f~100.f
	BOOL beActive;  //added @2012-07-03, used in defMA, TRUE:Calculate, FALSE:Not Calculate
	int WinLeft;
	int WinRight;

	void XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode);
};



class PeriodicTable : public CTypedPtrArray<CObArray,CClassicalElement*>
{
public:
	float GetElemDensity(CString ele);
	int GetElemLine(CString ele);
	int GetElemAtomicNum(CString ele);

	BOOL IsEmpty() {return (GetSize()==0);}
};

BOOL LoadPeriodicTable(PeriodicTable* ptable);
void FreePeriodicTable(PeriodicTable* ptable);


#endif // !defined(AFX_CLASSICALELEMENT_H__E66A0202_94EE_452D_A994_091B58869C90__INCLUDED_)
