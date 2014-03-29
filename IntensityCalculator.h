#ifndef INTENSITYCALCULTOR_H
#define INTENSITYCALCULTOR_H

#include <afxtempl.h>
#include "SpectrumInfo.h"
#include "TMeterTypeDefine.h"

typedef void (*fp_SetElementWindows)(int count, int* win);
typedef void (*fp_SetElementData)(int length, double* datas);
typedef void (*fp_PreFactorFit)(double* area, double* data);
typedef void (*fp_ElementFit)(double* area, double* data);

class OnePureElement : public CObject
{
public:
	CString Name;
	int left;
	int right;
	float intensity;
	BOOL bActive;
public:
	OnePureElement(CString name,int l,int r,BOOL active=FALSE):Name(name),left(l),right(r)
	{ 
		intensity = 0.0;
		bActive = active;
	}
	~OnePureElement()
	{ 
		// do something
	}
};

class PureElementPtrList : public CTypedPtrArray<CObArray,OnePureElement*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
	BOOL ExistElement(CString elem);
	
};

class IntensityCalculator
{
public:
    IntensityCalculator();
	
	BOOL LoadPureElementList();
	BOOL LoadPureElementListXML(); // *.xml
	BOOL LoadModuleLibrary();
	BOOL LoadPureSpectrum(WorkZoneDef *pWz);
	void ReloadPureElemList(WorkZoneDef *pWz);
	int m_pureCalcMethod;
	//calculate intensity of pure elements
	void CalcPureIntensity(CSpectrumInfo<DWORD,SPEC_SIZE> &spec);

	void CleanPureElementList();
	float GetIntensity(CString ele);

    ~IntensityCalculator();

public: //pointer to Functions
	fp_SetElementWindows SetElementWindows;
	fp_SetElementData SetPureElementsData;
	fp_PreFactorFit PreFactorFit;
	fp_ElementFit ElementFit;

	CSpectrumInfo<DWORD,SPEC_SIZE> specInfo;
	PureElementPtrList pureElementList;

private:
	HINSTANCE hDllInst;
    
};
#endif