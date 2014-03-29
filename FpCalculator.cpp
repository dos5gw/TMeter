// FpCalculator.cpp: implementation of the FpCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tmeter.h"
#include "FpCalculator.h"
#include "TMeterString.h"
#include "StdSample.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL wrtCalHeader( char *FileName )
{
	char Date[12], str1[128];
	char crlf[]={13,10,0};
	HFILE hFile; OFSTRUCT OfStruct;
	
	///Assume FileName is "temp.cal" ...
    hFile = OpenFile( FileName, &OfStruct, OF_CREATE );
    if( hFile < 0 ) return FALSE;
	
    strcpy( str1, "CALIBRATION FILE: " );
    strcat( str1, crlf);
    strcat( str1, "Date of Creation: " );
    _strdate( Date );
    strcat( str1, Date );
    strcat( str1, crlf);
    int len = strlen( str1 );
    _lwrite( hFile, str1, len );
	
    strcpy( str1, "Condition File: None" );
    strcat( str1, crlf);
    strcat( str1, "Sample Library: None" );
    strcat( str1, crlf);
    len = strlen( str1 );
    _lwrite( hFile, str1, len );
	
	strcpy( str1, "No. Standards: N/A" ); len = strlen( str1 );
    strcat( str1, crlf);
    strcat( str1, "Calibration Status: " );
    strcat( str1, "Not calibrated" );
    strcat( str1, crlf);
    len = strlen( str1 );
    _lwrite( hFile, str1, len );
	
	strcpy( str1, "Standard List: N/A" );
    strcat( str1, crlf);
    strcat( str1, crlf);
    strcat( str1, "Calibration Coefficients:");
	
    len = strlen( str1 );
    _lwrite( hFile, str1, len );
    _lclose( hFile );
	
	return TRUE;
}

BOOL FpCalculator::LoadModuleLibrary()
{
	hDllInst = LoadLibrary("fppro.dll");
	if(!hDllInst) 
		return FALSE;
	fpSetSourceData = (FP_SetSourceData)GetProcAddress(hDllInst,"fpSetSourceData");
	fpCalibrate = (FP_Calibrate)GetProcAddress(hDllInst,"fpCalibrate");
	fpQuantify = (FP_Quantify)GetProcAddress(hDllInst,"fpQuantify");
	fpSetRootPath = (FP_SetRootPath)GetProcAddress(hDllInst,"SetRootPath");
	if(!fpSetSourceData || !fpCalibrate || !fpQuantify || !fpSetRootPath)
	{
		return FALSE;
	}

	return TRUE;
}

void FpCalculator::SetFpCondition(FP_SOURCE_DATA* pSrc,WorkZoneDef* pWz)
{
	memset((void *)pSrc,0,sizeof(FP_SOURCE_DATA));

	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	pSrc->NumberOfConditionCode = 1;
	pSrc->TargetAtomicNumber[0] = pApp->GetTargetAtomicNumber();
	pSrc->XrayTargetTakeOffAngle[0] = pApp->GetTargetTakeOffAngle();
	pSrc->TubeVoltage[0]            = pApp->GetTubeVoltage(pWz->defMa.m_hv);
	pSrc->TubeWindowThickness[0]    = pApp->GetWindowThickness();
	pSrc->XrayIncidentAngle[0]      = pApp->GetIncidentAngle();
	pSrc->XrayEmergentAngle[0]      = pApp->GetEmergentAngle();
	pSrc->PrimaryFilterAtomicNumber[0] = pApp->GetFilterAtomicNumber(pWz->defMa.m_filter);
    pSrc->PrimaryFilterThickness[0]    = pApp->GetFilterThickness(pWz->defMa.m_filter);

}

float FpCalculator::GetIntensity(CString elem)
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	return pApp->m_IntensityCalculator.GetIntensity(elem);
}

void FpCalculator::SetFpSample(FP_SAMPLE_DATA* pSrc,WorkZoneDef* pWz)
{
	memset((void *)pSrc,0,sizeof(FP_SAMPLE_DATA)*2);
	
	char tempBuff[16];
	int j,m;
	int eleNum=0;
	OneLayerDefine *pDefMaLayer;
	CClassicalElement *pDefMaElem;
	CDefMa *pDefMa = &(pWz->defMa);
	FP_SAMPLE_DATA* pFpSample = &pSrc[0];
	
	/***  start copy data to pSrc ***/
	sprintf(tempBuff,"Result");
	strcpy(pFpSample->SampleID,tempBuff);

	pFpSample->NumberOfLayers = pDefMa->m_layerArray.GetSize();
	for(j=0; j<pFpSample->NumberOfLayers; j++)
	{
		pDefMaLayer=pDefMa->m_layerArray.GetAt(j);
		
		pFpSample->LayerDensity[j]  = pDefMaLayer->m_density;
		pFpSample->LayerThickness[j]= pDefMaLayer->m_thickness; 
		if(pDefMaLayer->m_thickMode == 0) pFpSample->LayerCalculationFlag[j] = 1; // calculate
		else pFpSample->LayerCalculationFlag[j] = 2; // fixed
		if(pDefMaLayer->m_isSaturated) pFpSample->LayerCalculationFlag[j] = 2;    // this layer is saturated, will not be calculated
		
		int nConstituent = pDefMaLayer->m_elementList.GetSize();
		for(m=0; m<nConstituent; m++)
		{
			pDefMaElem= pDefMaLayer->m_elementList.GetAt(m);
			if(pDefMaElem->beActive)
			{
				strcpy(pFpSample->ConstituentFormula[eleNum],(LPTSTR)(LPCTSTR)(pDefMaElem->Name));
				pFpSample->ConstituentLayerNumber[eleNum] = j+1;
				pFpSample->ConstituentConcentrationUnit[eleNum] = 1; // 1-Weight percent, 2-ppm
				//@2012-08-25
				if(pDefMaLayer->m_contentMode==0) pFpSample->ConstituentCalculationFlag[eleNum] = 1;// 1-Calculated, 2-Fixed;
				else pFpSample->ConstituentCalculationFlag[eleNum] = 2;
				pFpSample->AnalyteLine[eleNum] = pDefMaElem->DefaultLine;
				pFpSample->AnalyteConditionCode[eleNum] = 1;
				pFpSample->ConstituentConcentration[eleNum] = pDefMaElem->Content; 
				pFpSample->AnalyteAtomicNumber[eleNum] = pDefMaElem->Id;
				pFpSample->AnalyteIntensity[eleNum] = GetIntensity(pDefMaElem->Name);  /** Get From IntensityCalculator **/
				
				eleNum++;
			}
		} // foreach Constituent(element)
	}
	pFpSample->NumberOfConstituents = eleNum;
}

void FpCalculator::SetFpStandard(FP_SAMPLE_DATA* fpStd,int nStd,int nPure,WorkZoneDef* pWz)
{
	memset((void *)fpStd,0,sizeof(FP_SAMPLE_DATA)*(nStd+nPure));

	CDefMa *pDefMa = &(pWz->defMa);
	FP_SAMPLE_DATA* pFpSample;
	CStandardSample *pOneStd;
	OneLayerDefine *pOneLayer,*pDefMaLayer;
	CClassicalElement *pOneElem,*pDefMaElem;
	char tempBuff[16];
	int i,j,m;
	int eleNum,layerNum;

	// standard sample 
	for(i=0;i<nStd;i++)
	{
		pOneStd = pWz->stdSample.GetAt(i);
		pFpSample = &fpStd[i];
		eleNum = 0;
		layerNum = 0;

		sprintf(tempBuff,"std-%2d",i+1);
		strcpy(pFpSample->SampleID,tempBuff);

		int nLayers = pDefMa->m_layerArray.GetSize(); 
		for(j=0; j<nLayers; j++)
		{
			pOneLayer = pOneStd->m_layerArray.GetAt(j);
			pDefMaLayer=pDefMa->m_layerArray.GetAt(j);

			if(pOneLayer->m_isExist == FALSE) continue; // @2012-08-30 

			pFpSample->LayerDensity[layerNum]  = pOneLayer->m_density;   /** Get From Standard **/
			pFpSample->LayerThickness[layerNum]= pOneLayer->m_thickness; /** Get From Standard **/
			if(pDefMaLayer->m_thickMode == 0) 
				pFpSample->LayerCalculationFlag[layerNum] = 1; // calculate thick
			else // m_thickMode = 1correct, 2known
				pFpSample->LayerCalculationFlag[layerNum] = 2; // fix thick

			if(pDefMaLayer->m_isSaturated) 
				pFpSample->LayerCalculationFlag[layerNum] = 2; // this layer is saturated, thickness will not be calculated

			int nConstituent = pDefMaLayer->m_elementList.GetSize();
			for(m=0; m<nConstituent; m++)
			{
				pOneElem  = pOneLayer->m_elementList.GetAt(m);
				pDefMaElem= pDefMaLayer->m_elementList.GetAt(m);
				if(pDefMaElem->beActive)
				{
					strcpy(pFpSample->ConstituentFormula[eleNum],(LPTSTR)(LPCTSTR)(pDefMaElem->Name));
					pFpSample->ConstituentLayerNumber[eleNum] = layerNum+1;
					pFpSample->ConstituentConcentrationUnit[eleNum] = 1; // 1-Weight percent, 2-ppm

					if(pDefMaLayer->m_contentMode==0) 
						pFpSample->ConstituentCalculationFlag[eleNum] = 1;// 1-Calculated, 2-Fixed;
					else 
						pFpSample->ConstituentCalculationFlag[eleNum] = 2;

					pFpSample->AnalyteLine[eleNum] = pDefMaElem->DefaultLine;
					pFpSample->AnalyteConditionCode[eleNum] = 1;
					pFpSample->ConstituentConcentration[eleNum] = pOneElem->Content;  /** Get From Standard **/
					pFpSample->AnalyteAtomicNumber[eleNum] = pDefMaElem->Id;
					pFpSample->AnalyteIntensity[eleNum] = pOneElem->Intensity;        /** Get From Standard **/

					eleNum++;
				}
			} // foreach element

			layerNum++;
			// if this layer is saturated, the other layer(under current layer) will not exist
			if(pDefMaLayer->m_isSaturated) break;
			

		} // foreach layer
		pFpSample->NumberOfLayers = layerNum;
		pFpSample->NumberOfConstituents = eleNum;

	} //foreach standard


	// pure elements
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	IntensityCalculator* pCalculator = &(pApp->m_IntensityCalculator);

	for(i=nStd;i<(nStd+nPure);i++)
	{
		pFpSample = &fpStd[i];
		OnePureElement* pPure = pCalculator->pureElementList.GetAt(i-nStd);

		strcpy(pFpSample->SampleID,pPure->Name);
		pFpSample->NumberOfLayers = 1;
		pFpSample->LayerDensity[0] = pApp->m_PeriodicTable.GetElemDensity(pPure->Name);            // get from CClassicalElement
		pFpSample->LayerThickness[0] = 0;              
		pFpSample->LayerCalculationFlag[0] = 2;      
		pFpSample->NumberOfConstituents = 1;
		strcpy(pFpSample->ConstituentFormula[0],(LPTSTR)(LPCTSTR)(pPure->Name));
		pFpSample->ConstituentLayerNumber[0] = 1;
		pFpSample->ConstituentConcentration[0] = 100;        
		pFpSample->ConstituentConcentrationUnit[0] = 1;
		pFpSample->ConstituentCalculationFlag[0] = 2;  
		pFpSample->AnalyteAtomicNumber[0] = pApp->m_PeriodicTable.GetElemAtomicNum(pPure->Name);  // get from CClassicalElement
		pFpSample->AnalyteLine[0] = pApp->m_PeriodicTable.GetElemLine(pPure->Name);               // get from CClassicalElement
		pFpSample->AnalyteConditionCode[0] = 1;       
		pFpSample->AnalyteIntensity[0] = 1;    
	} //foreach pure 
}

// this function is for release version
void FpCalculator::CalibrateWorkZone(WorkZoneDef* pWz) 
{
	CString folderName,fpPath;
	folderName.Format("%04d",pWz->GetDirIndex());
	fpPath = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName;

	// (1) set measurement condition
	FP_SOURCE_DATA *pConditions = new FP_SOURCE_DATA();
	SetFpCondition(pConditions,pWz);
	
	char cmpFlt[MAXCC][18]; memset((void *)cmpFlt,0,sizeof(cmpFlt)); 
	fpSetSourceData(1,pConditions,&cmpFlt[0][0]); 

	// (2) set standard samples and calibrate
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	IntensityCalculator* pCalculator = &(pApp->m_IntensityCalculator);

	int nStd = pWz->stdSample.GetSize();
	int nPure= pCalculator->pureElementList.GetSize();
	int nSamples = nStd + nPure;

	if(nSamples==0) return;
	FP_SAMPLE_DATA *pSamples = new FP_SAMPLE_DATA[nSamples];
	SetFpStandard(pSamples,nStd,nPure,pWz);

	SetCurrentDirectory(fpPath);
	if( !wrtCalHeader("temp.cal") )
	{
		AfxMessageBox("Error writing cal file.");
		return;
	}
	if( !wrtCalHeader("temp2.cal") )
	{
		AfxMessageBox("Error writing cal2 file.");
		return;
	}

	// Calibrate Work-Zone 
	BOOL rgrs32 = FALSE;
	if(!rgrs32)
	{
		//char nMode[] = {3,3,3,3}; 
		char* nMode = new char[nPure];
		for(int m=0;m<nPure;m++) nMode[m] = 1;
		fpCalibrate(nSamples,nMode,pSamples,"temp.cal");    // standard sample
		fpCalibrate(nPure,nMode,pSamples+nStd,"temp2.cal"); // pure elements
	}
	else
	{
		fpCalibrate(nSamples,NULL,pSamples,"temp.cal");
		fpCalibrate(nPure,NULL,pSamples+nStd,"temp2.cal"); // pure elements
	}


	/** START: save log in absolute path **/
	CString logFile;
	CString logFolder = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\Log\\";
	
	for(int i=0;i<nSamples;i++)
	{
		logFile.Format("CalibStd%d.txt",i);
		logFile = logFolder + logFile;
		SaveSampleData(logFile,pSamples+i);
	}

	logFile = logFolder + "CalibCondition.txt";
	SaveSourceData(logFile,pConditions);
	/** DONE: save log in absolute path **/

	// (3) restore current directory
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);
}

// this function is for release version
void FpCalculator::CalculateQuantify(WorkZoneDef* pWz)
{
	CString folderName,fpPath;
	folderName.Format("%04d",pWz->GetDirIndex());
	fpPath = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName;

	// (1) set condition
	FP_SOURCE_DATA *pConditions = new FP_SOURCE_DATA();
	SetFpCondition(pConditions,pWz);
	
	char cmpFlt[MAXCC][18]; memset((void *)cmpFlt,0,sizeof(cmpFlt)); 
	fpSetSourceData(1,pConditions,&cmpFlt[0][0]); 

	// (2) set quantify
	SetFpSample(m_pResultData,pWz);
	SetFpSample(m_pResultDataTheory,pWz);
	
	// (3) calculate thickness
	SetCurrentDirectory(fpPath);
	fpQuantify(m_pResultData,"temp.cal"); 
	fpQuantify(m_pResultDataTheory,"temp2.cal");
	
	/******** start : save log in absolute path ********/
	CString logFile;
	CString logFolder = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\Log\\";

	logFile = logFolder + "QuantifyResult.txt";
	SaveSampleData(logFile,&m_pResultData[0]);

	logFile = logFolder + "QuantifyResultTheory.txt";
	SaveSampleData(logFile,&m_pResultDataTheory[0]);

	logFile = logFolder + "QuantifyCondition.txt";
	SaveSourceData(logFile,pConditions);
	/******** done : save log in absolute path ********/


	// (3) restore current directory
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);
}


float FpCalculator::GetLayerThicknessFromResult(int nLayer,BOOL beTheory)
{
	float ret = 0.f;

	if(beTheory){
		if(nLayer < MAXCC)
			ret =  m_pResultData[0].LayerThickness[nLayer];
		else
			ret = 0.f;

	}
	else{
		if(nLayer < MAXCC)
			ret = m_pResultDataTheory[0].LayerThickness[nLayer];
		else
			ret = 0.f;
	}
	
	return ret;
}

float FpCalculator::GetElemContentFromResult(CString ele,BOOL beTheory)
{
	float ret = 0.f;

	if(beTheory){
		for(int i=0;i<MAXELT;i++)
		{
			if(ele.Compare(m_pResultDataTheory[0].ConstituentFormula[i]) == 0)
			{
				ret = m_pResultDataTheory[0].ConstituentConcentration[i];
			}
		}

	}
	else{
		for(int i=0;i<MAXELT;i++)
		{
			if(ele.Compare(m_pResultData[0].ConstituentFormula[i]) == 0)
			{
				ret = m_pResultData[0].ConstituentConcentration[i];
			}
		}
	}
	
	return ret;
}

void FpCalculator::ResetResultData()
{
	memset((void*)m_pResultData,0,2*sizeof(FP_SAMPLE_DATA));
	m_AnalysisTime = GetCurrentTime();
}

void FpCalculator::SaveResultToDatabase()
{
	
}



// this function is for debug version
void FpCalculator::CalibrateWorkZone(CString fpPath,WorkZoneDef* pWz) 
{
	// fpPath, for example E:\TMeter\DefineFP\Ni-Cu-Zn-Ag
	// Load FP_SOURCE_DATA,FP_SAMPLE_DATA from files
	// Generate temp.cal, and copy it to \DefineFP\sub
	int i;
	CString conditionPath = fpPath + "\\" + "condition.ini";
	int nConditions = 1;//pWz->NumOfConditions;
	FP_SOURCE_DATA *pConditions = new FP_SOURCE_DATA();
	memset((void *)pConditions,0,sizeof(FP_SOURCE_DATA));
	LoadSourceData(conditionPath,pConditions);
	
	CString samplePath,fileName;;
	int nSamples = 1;//pWz->NumOfSamples;
	FP_SAMPLE_DATA *pSamples = new FP_SAMPLE_DATA[nSamples];
	memset((void *)pSamples,0,sizeof(FP_SAMPLE_DATA)*nSamples);
	for(i=0;i<nSamples;i++)
	{
		fileName.Format("%s%02d%s","sample", i+1, ".std");
		samplePath = fpPath + "\\" + fileName;
		LoadSampleData(samplePath,&pSamples[i]);
	}

	//(1) set measurement condition
	char cmpFlt[MAXCC][18];
	memset((void *)cmpFlt,0,sizeof(cmpFlt)); 
	fpSetSourceData(1,pConditions,&cmpFlt[0][0]); 

	//(2) calibrate stand samples, generate TEMP.CAL in Workzone-Sub folder
	SetCurrentDirectory(fpPath);
	if( !wrtCalHeader("temp.cal") ){
		AfxMessageBox("Error writing cal file.");
		return;
	}
	BOOL rgrs32 = FALSE;
	if( !rgrs32 ){
		char nMode[] = {3,3,3,3};
		fpCalibrate(nSamples,nMode,pSamples,"temp.cal");
	}
	else{
		fpCalibrate(nSamples,NULL,pSamples,"temp.cal"); 
	}

	// (3) restore current directory
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);

	//CString text;
	//text.Format("校正文件保存在%s",fpPath + "\\" + "temp.cal");
	//AfxMessageBox(text);
}

// this function is for debug version
void FpCalculator::CalculateQuantify(CString fpPath,WorkZoneDef* pWz)
{
	//load quantify files
	FP_SAMPLE_DATA test[2];
	memset((void *)test,0,sizeof(FP_SAMPLE_DATA)*2);
	CString quantifyPath = fpPath + "\\" + "unknown.std" ;
	LoadSampleData(quantifyPath,&test[0]);

	SetCurrentDirectory(fpPath);
	fpQuantify(test,"temp.cal");
	//save result to file
	SaveSampleData(quantifyPath,&test[0]);
	SetCurrentDirectory(((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory);

	//CString text;
	//text.Format("计算结果保存在%s",fpPath + "\\" + "unknown.std");
	//AfxMessageBox(text);
}

BOOL LoadSourceData(CString file,FP_SOURCE_DATA* pData)
{
	//condition.ini
	CStdioFile iniFile;
	int findPos;
	CString inBuff,dataBuff;
	//CString floatFormat,intFormat;
	int nStd;
	int i; 
	if(iniFile.Open(file,CFile::modeRead)) 
	{
		memset((void *)pData, 0, sizeof(FP_SOURCE_DATA));
		while(iniFile.ReadString(inBuff))
		{
			if(inBuff.Find("NumberOfCode=")!=-1)
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				nStd = atoi(inBuff); 
				pData->NumberOfConditionCode = nStd;
				/*
				for(i=0;i<nStd;i++)
				{
					floatFormat+="%lf,";
					intFormat+="%d,";
				}
				floatFormat.Mid(0,floatFormat.GetLength()-1);
				intFormat.Mid(0,intFormat.GetLength()-1); */
			}
			else if(inBuff.Find("TargetAtomicNumber=")!=-1)
			{
				 
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("TargetAtomicNumber, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->TargetAtomicNumber[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("TakeOffAngle=")!=-1) 
			{	
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("TakeOffAngle, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->XrayTargetTakeOffAngle[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("TubeVoltage=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("TubeVoltage, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->TubeVoltage[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("TubeWindowThickness=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("TubeWindowThickness, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->TubeWindowThickness[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("XrayIncidentAngle=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("XrayIncidentAngle, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,","); //
					pData->XrayIncidentAngle[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("XrayEmergentAngle=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("XrayEmergentAngle, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,","); //check
					pData->XrayEmergentAngle[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("PrimaryFilterAtomicNumber=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("PrimaryFilterAtomicNumber, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->PrimaryFilterAtomicNumber[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("PrimaryFilterThickness=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("XrayEmergentAngle, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->PrimaryFilterThickness[i] = (float)atof(dataBuff);
				}
			}
		}

		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL LoadSampleData(CString file,FP_SAMPLE_DATA* pData)
{
	//sample01.std
	CStdioFile iniFile;
	int findPos,i;
	CString inBuff,dataBuff;
	int nStd,nEle;
	if(iniFile.Open(file,CFile::modeRead)) 
	{
		memset((void *)pData, 0, sizeof(FP_SAMPLE_DATA));
		while(iniFile.ReadString(inBuff)){
			if(inBuff.Find("SampleID=")!=-1)
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				memset(pData->SampleID,'\0',MAXSIDCHRS+1);
				memcpy(pData->SampleID,(LPTSTR)(LPCTSTR)inBuff,inBuff.GetLength());
			}
			else if(inBuff.Find("NumberOfLayers=")!=-1) //2
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				nStd = atoi(inBuff); 
				pData->NumberOfLayers=nStd;
			}
			else if(inBuff.Find("LayerDensity=")!=-1)   //3
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				if(!CheckDelimiterNum(inBuff,nStd,",")){
					AfxMessageBox("LayerDensity, read string failed");
					iniFile.Close();
					return FALSE;
				}
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->LayerDensity[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("LayerThickness=")!=-1)   //4
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->LayerThickness[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("LayerCalculationFlag=")!=-1)   //5
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nStd;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->LayerCalculationFlag[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("NumberOfConstituents=")!=-1)   //6
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				nEle = atoi(inBuff);
				pData->NumberOfConstituents=nEle;
			}
			else if(inBuff.Find("ConstituentFormula=")!=-1)   //7
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					memset(pData->ConstituentFormula[i],'\0',MAXCMPCHRS+1);
					memcpy(pData->ConstituentFormula[i],dataBuff,dataBuff.GetLength());
				}
			}
			else if(inBuff.Find("ConstituentLayerNumber=")!=-1)   //8
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->ConstituentLayerNumber[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("ConstituentConcentration=")!=-1)   //9
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->ConstituentConcentration[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("ConstituentConcentrationUnit=")!=-1)   //10
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->ConstituentConcentrationUnit[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("ConstituentCalculationFlag=")!=-1)   //11
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->ConstituentCalculationFlag[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("AnalyteAtomicNumber=")!=-1)   //12
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->AnalyteAtomicNumber[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("AnalyteLine=")!=-1)   //13
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->AnalyteLine[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("AnalyteConditionCode=")!=-1)   //14
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->AnalyteConditionCode[i] = atoi(dataBuff);
				}
			}
			else if(inBuff.Find("AnalyteIntensity=")!=-1)   //15
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->AnalyteIntensity[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("AnalyteBlank=")!=-1)   //16
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->AnalyteBlank[i] = (float)atof(dataBuff);
				}
			}
			else if(inBuff.Find("ElementConcentration=")!=-1)   //17
			{
				
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				//check
				for(i=0;i<nEle;i++)
				{
					dataBuff=GetStringAt(inBuff,i,",");
					pData->ElementConcentration[i] = (float)atof(dataBuff);
				}
			}
		}
		//pData->Asrat = pData->Msthk = pData->Loi = 0.0;
		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

/* initialize measurement condition */
void InitSourceData(FP_SOURCE_DATA* pData,WorkZoneDef* pWorkzone)
{
	memset((void*)pData,0,sizeof(FP_SOURCE_DATA));
	if(!pWorkzone)
	{
		return;
		
	} 
	else
	{
		pData->NumberOfConditionCode = 1;
		pData->TargetAtomicNumber[0] = 74;        // "W" Target 
		pData->XrayTargetTakeOffAngle[0] = 20;
		switch(pWorkzone->defMa.m_hv)
		{
		case 30:
			pData->TubeVoltage[0] = 30.0;
			break;
		case 40:
			pData->TubeVoltage[0] = 40.0;
			break;
		case 50:
			pData->TubeVoltage[0] = 50.0;
			break;
		}
		pData->TubeWindowThickness[0] = 0.12f;     // in millimeters
		pData->XrayIncidentAngle[0] = 45.0f;         // in Degrees
		pData->XrayEmergentAngle[0] = 45.0f;         // in Degrees
		switch(pWorkzone->defMa.m_filter)
		{
		case 0: //FILTER_NI
			pData->PrimaryFilterAtomicNumber[0] = 28;
			pData->PrimaryFilterThickness[0] = 0.3f;
			break;
		case 1: //FILTER_NONE
			pData->PrimaryFilterAtomicNumber[0] = 0;
			pData->PrimaryFilterThickness[0] = 0.0f;
			break;
		case 2: //FILTER_AL
			pData->PrimaryFilterAtomicNumber[0] = 13;
			pData->PrimaryFilterThickness[0] = 0.5f; 
			break;
		}
	}
}

/* initialize stand sample */
void InitSampleData(FP_SAMPLE_DATA* pData,WorkZoneDef* pWorkzone)
{
	memset((void*)pData,0,sizeof(FP_SAMPLE_DATA));

	if(!pWorkzone)
	{
		return;
	} 
	else
	{
		int i;
		OneLayerDefine* pOneLayer;

		CDefMa& defMa = pWorkzone->defMa;
		strcpy(pData->SampleID,"Sample01");
		int nLayers = defMa.m_layerArray.GetSize();
		pData->NumberOfLayers = nLayers;
		for(i=0;i<nLayers;i++)
		{
			pOneLayer = defMa.m_layerArray.GetAt(i);
			pData->LayerDensity[i]  = pOneLayer->m_density;    // in g/cm3
			pData->LayerThickness[i] = pOneLayer->m_thickness; // in microns
			if(pOneLayer->m_thickMode ==0)
				pData->LayerCalculationFlag[i] = 1;         // 1-Calculated, 2-Fixed;
			if(pOneLayer->m_contentMode ==0)
				pData->LayerCalculationFlag[i] = 2;
		}

		//int nElement = defMa.GetElementSum();
		/*
		pDate->NumberOfConstituents;
		pDate->ConstituentFormula[MAXELT][MAXCMPCHRS+1];
		pDate->ConstituentLayerNumber[MAXELT];
		pDate->ConstituentConcentration[MAXELT];        
		pDate->ConstituentConcentrationUnit[MAXELT];
		pDate->ConstituentCalculationFlag[MAXELT];     // 1-Calculated, 2-Fixed;
		pDate->AnalyteAtomicNumber[MAXELT];            // in Atomic Number
		pDate->AnalyteLine[MAXELT];                    // 1-Ka, 2-Kb, 3-La, 4-Lb, 5-Lg, .
		pDate->AnalyteConditionCode[MAXELT];        
		pDate->AnalyteIntensity[MAXELT];    
		pDate->AnalyteBlank[MAXELT];    
		pDate->ElementConcentration[MAXELT];           // used for report only
		pDate->Asrat, Msthk, Loi;  */
	}
}

BOOL SaveSourceData(CString file,FP_SOURCE_DATA* pData)
{
	if(!pData) return FALSE;
	CStdioFile iniFile;
	CString outBuff,tempBuff;
	if(iniFile.Open(file,CFile::modeCreate|CFile::modeWrite))
	{
		outBuff.Format("TargetAtomicNumber=%d\n",pData->TargetAtomicNumber[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("XrayTargetTakeOffAngle=%f\n",pData->XrayTargetTakeOffAngle[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("TubeVoltage=%f\n",pData->TubeVoltage[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("TubeWindowThickness=%f\n",pData->TubeWindowThickness[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("XrayIncidentAngle=%f\n",pData->XrayIncidentAngle[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("XrayEmergentAngle=%f\n",pData->XrayEmergentAngle[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("PrimaryFilterAtomicNumber=%d\n",pData->PrimaryFilterAtomicNumber[0]); 
		iniFile.WriteString(outBuff);

		outBuff.Format("PrimaryFilterThickness=%f\n",pData->PrimaryFilterThickness[0]); 
		iniFile.WriteString(outBuff);

		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL SaveSampleData(CString file,FP_SAMPLE_DATA* pData)
{
	if(!pData) return FALSE;
	CStdioFile iniFile;
	CString outBuff,tempBuff;
	int i;
	if(iniFile.Open(file,CFile::modeCreate|CFile::modeWrite))
	{
		outBuff.Format("SampleID=%s\n",pData->SampleID); //1
		iniFile.WriteString(outBuff);

		outBuff.Format("NumberOfLayers=%d\n",pData->NumberOfLayers); //2
		iniFile.WriteString(outBuff);

		int nLayers = pData->NumberOfLayers;
		outBuff = "LayerDensity=";
		for(i=0;i<nLayers;i++)
		{
			tempBuff.Format("%.2f,",pData->LayerDensity[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1); 
		outBuff += "\n";
		iniFile.WriteString(outBuff); //3

		outBuff = "LayerThickness=";
		for(i=0;i<nLayers;i++)
		{
			tempBuff.Format("%.2f,",pData->LayerThickness[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //4

		outBuff = "LayerCalculationFlag=";
		for(i=0;i<nLayers;i++)
		{
			tempBuff.Format("%d,",pData->LayerCalculationFlag[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //5

		outBuff.Format("NumberOfConstituents=%d\n",pData->NumberOfConstituents); //6
		iniFile.WriteString(outBuff);
		
		int nConstituents = pData->NumberOfConstituents;
		outBuff = "ConstituentFormula=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%s,",&(pData->ConstituentFormula[i][0]));
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //7

		outBuff = "ConstituentLayerNumber=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->ConstituentLayerNumber[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //8

		outBuff = "ConstituentConcentration=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%.2f,",pData->ConstituentConcentration[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //9

		outBuff = "ConstituentConcentrationUnit=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->ConstituentConcentrationUnit[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //10

		outBuff = "ConstituentCalculationFlag=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->ConstituentCalculationFlag[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //11

		outBuff = "AnalyteAtomicNumber=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->AnalyteAtomicNumber[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //12

		outBuff = "AnalyteLine=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->AnalyteLine[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //13

		outBuff = "AnalyteConditionCode=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%d,",pData->AnalyteConditionCode[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //14

		outBuff = "AnalyteIntensity=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%.2f,",pData->AnalyteIntensity[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //15

		outBuff = "AnalyteBlank=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%.2f,",pData->AnalyteBlank[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //16

		outBuff = "ElementConcentration=";
		for(i=0;i<nConstituents;i++)
		{
			tempBuff.Format("%.2f,",pData->ElementConcentration[i]);
			outBuff += tempBuff;
		}
		outBuff.TrimRight(',');//outBuff.Mid(0,outBuff.GetLength()-1);
		outBuff += "\n";
		iniFile.WriteString(outBuff); //17

		iniFile.Close();
		return TRUE;
	}
	else return FALSE;
	
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FpCalculator::FpCalculator()
{
	fpSetSourceData=NULL;
	fpCalibrate=NULL;
	fpQuantify=NULL;
	fpSetRootPath=NULL;

	m_pResultData = (FP_SAMPLE_DATA*)malloc(2*sizeof(FP_SAMPLE_DATA));
	memset((void*)m_pResultData,0,2*sizeof(FP_SAMPLE_DATA));

	m_pResultDataTheory = (FP_SAMPLE_DATA*)malloc(2*sizeof(FP_SAMPLE_DATA));
	memset((void*)m_pResultDataTheory,0,2*sizeof(FP_SAMPLE_DATA));

}

FpCalculator::~FpCalculator()
{
	free(m_pResultData);
	free(m_pResultDataTheory);
	FreeLibrary(hDllInst);
}

