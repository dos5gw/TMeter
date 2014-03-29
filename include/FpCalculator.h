// FpCalculator.h: interface for the FpCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FPCALCULATOR_H__E6822A57_87E5_4B9C_9061_7C0053916336__INCLUDED_)
#define AFX_FPCALCULATOR_H__E6822A57_87E5_4B9C_9061_7C0053916336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkZoneDef.h"
//#include "OneResult.h"

#define MAXCC                      5
#define MAXELT                    35
#define MAXLAYER                   6
#define MAXCMP			          35
#define MAXSTD			          30
#define MAXCMPELT		           7
#define NCMUPRMS		          24
#define MAXCMUELT	  MAXELT+4*MAXCC
#define MAXSIDCHRS		          24
#define MAXCMPCHRS		          18

#define MAX_ELTS        25
#define MAX_GAUSELTS    80
#define MAX_GAUSLINES  200
//#define SPEC_SIZE     4096  


//////////////////////////////////////////////////////////////////////////////////////////
// The data structures :
 
///////////////////////////////
// FP_SOURCE_DATA
//
// This data structure allows up to 5 different excitation conditions including tube 
// parameters and geometry.This is important since there may be cases where different 
// conditions have to be used for different analytes. 

typedef struct
{
     int   NumberOfConditionCode;
	 short TargetAtomicNumber[MAXCC];
     float XrayTargetTakeOffAngle[MAXCC];    // in Degrees
     float TubeVoltage[MAXCC];               // in kV
     float TubeWindowThickness[MAXCC];       // in millimeters
     float XrayIncidentAngle[MAXCC];         // in Degrees
     float XrayEmergentAngle[MAXCC];         // in Degrees
     short PrimaryFilterAtomicNumber[MAXCC]; 
     float PrimaryFilterThickness[MAXCC];    // in millimeters

} FP_SOURCE_DATA;

typedef struct
{
	char  SampleID[MAXSIDCHRS+1];
	int   NumberOfLayers;
	float LayerDensity[MAXLAYER];                 // in g/cm3
	float LayerThickness[MAXLAYER];               // in microns
	char  LayerCalculationFlag[MAXLAYER];         // 1-Calculated, 2-Fixed;
	int   NumberOfConstituents;
	char  ConstituentFormula[MAXELT][MAXCMPCHRS+1];
	char  ConstituentLayerNumber[MAXELT];
	float ConstituentConcentration[MAXELT];        
	char  ConstituentConcentrationUnit[MAXELT];
	char  ConstituentCalculationFlag[MAXELT];     // 1-Calculated, 2-Fixed;
	short AnalyteAtomicNumber[MAXELT];            // in Atomic Number
	char  AnalyteLine[MAXELT];                    // 1-Ka, 2-Kb, 3-La, 4-Lb, 5-Lg, .
	char  AnalyteConditionCode[MAXELT];        
	float AnalyteIntensity[MAXELT];    
	float AnalyteBlank[MAXELT];    
	float ElementConcentration[MAXELT];           // used for report only
	float Asrat, Msthk, Loi;                      // used for bulk samples
} FP_SAMPLE_DATA;

BOOL wrtCalHeader(char *FileName);

void InitSourceData(FP_SOURCE_DATA* pData,WorkZoneDef* pWz);
void InitSampleData(FP_SAMPLE_DATA* pData,WorkZoneDef* pWz);

BOOL LoadSourceData(CString file,FP_SOURCE_DATA* pData);
BOOL LoadSampleData(CString file,FP_SAMPLE_DATA* pData);

BOOL SaveSourceData(CString file,FP_SOURCE_DATA* pData);
BOOL SaveSampleData(CString file,FP_SAMPLE_DATA* pData);

typedef BOOL (*FP_SetSourceData)(int nCode, FP_SOURCE_DATA* src, char* cmpFlt);
typedef BOOL (*FP_Calibrate)(int nStds, char* nMode, FP_SAMPLE_DATA* stds, LPSTR CalFileName);
typedef BOOL (*FP_Quantify)(FP_SAMPLE_DATA* smp, LPSTR CalFileName);
typedef void (*FP_SetRootPath)(char* appPath); 


class FpCalculator  
{
public:
	BOOL LoadModuleLibrary();

	/*FP DLL API Definitions */
	FP_SetSourceData fpSetSourceData;
	FP_Calibrate fpCalibrate;
	FP_Quantify fpQuantify;
	FP_SetRootPath fpSetRootPath;

	/**/
	FP_SAMPLE_DATA *m_pResultData;
	FP_SAMPLE_DATA *m_pResultDataTheory;
	CTime m_AnalysisTime;

	void CalibrateWorkZone(CString fpPath,WorkZoneDef* pWz);  //for debug version
	void CalculateQuantify(CString fpPath,WorkZoneDef* pWz);

	void CalibrateWorkZone(WorkZoneDef* pWz); // for release version
	void CalculateQuantify(WorkZoneDef* pWz);

	void SetFpCondition(FP_SOURCE_DATA* pSrc,WorkZoneDef* pWz);
	void SetFpSample(FP_SAMPLE_DATA* pSrc,WorkZoneDef* pWz);
	void SetFpStandard(FP_SAMPLE_DATA* pStd,int nStd,int nPure,WorkZoneDef* pWz);

	float GetIntensity(CString elem); // get intensity from IntensityCalculator

	float GetLayerThicknessFromResult(int nLayer,BOOL beTheory);
	float GetElemContentFromResult(CString ele,BOOL beTheory);
	void ResetResultData();
	void SaveResultToDatabase();

	FpCalculator();
	virtual ~FpCalculator();

private:
	HINSTANCE hDllInst;
};

#endif // !defined(AFX_FPCALCULATOR_H__E6822A57_87E5_4B9C_9061_7C0053916336__INCLUDED_)
