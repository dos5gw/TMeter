// WorkZoneDef.cpp: implementation of the WorkZoneDef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMeter.h"
#include "WorkZoneDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WorkZoneDef::WorkZoneDef()
{
	Tag="";
	ModifyTime =CTime::GetCurrentTime();
	MeasureTime=CTime::GetCurrentTime();
	CorrectTime=CTime::GetCurrentTime();
	SampleTimes=10;  //10 sec
	Repeat=1; 
	ContentUnit=0;  //0:%, 1:o/oo, 2:ppm, 3:Karat, 4,user
	ThickUnit=0;    //0:um, 1:nm, 2:mils, 3:u", 4,user
	Precision=1;    //1:0.0, 2:0.00, 3:0.000, 4:0.0000
	
	AdjustTimes=10;
	AdjustRepeat=1;
	MqMax=0;
	Collimator=1; //1,2,3,4
	Current=0;

	ArrayVariable=TRUE; 
	ArraySize=5;
	ReportStyle=0;

}

WorkZoneDef::~WorkZoneDef()
{
	//defMa.~CDefMa();
	//stdSample.~CStandardSamples();
	//AfxMessageBox("~WorkZoneDef()");
}

void WorkZoneDef::SetWorkZonePath()
{
	CString folderName;
	folderName.Format("%04d",DirNameIndex);
	m_FpPath = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName;
	m_LogPath= ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\Log";

	m_SpectrumPath = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\SpecData";
	switch(defMa.m_hv)
	{
	case 30:
		m_SpectrumPath += "\\HV30";
		break;
	case 40:
		m_SpectrumPath += "\\HV40";
		break;
	case 50:
		m_SpectrumPath += "\\HV50";
		break;
	default:
		m_SpectrumPath += "\\HV30";
	}

	switch(defMa.m_filter)
	{
	case 0:  //Ni
		m_SpectrumPath += "\\FLT1";
		break;
	case 1:  //--
		m_SpectrumPath += "\\FLT2";
		break;
	case 2:  //Al
		m_SpectrumPath += "\\FLT3";
		break;
	default:
		m_SpectrumPath += "\\FLT1";
	}
	
	
}

void WorkZoneDef::LoadDefMa(CString file)
{
	defMa.LoadSettings(file);
}

void WorkZoneDef::LoadStdSamples(CString file)
{
	stdSample.LoadSourceData(file);
}

void WorkZoneDef::SaveDefMa()
{
	CString folderName;
	folderName.Format("%04d",GetDirIndex());
	CString fullPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName + "\\analysis.xma"; 
	
	/***   start saving  ****/
	TiXmlDocument doc;
	defMa.XMLSerialize(TRUE,&doc);
	if(!doc.SaveFile((LPTSTR)(LPCTSTR)fullPathName))
	{
		CString text;
		text.Format("%s ±£´æÊ§°Ü",fullPathName);
		AfxMessageBox(text);
		return;
	}
}

void WorkZoneDef::SaveStdSample()
{
	CString folderName;
	folderName.Format("%04d",GetDirIndex());
	CString fullPathName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\DefineFP\\" + folderName + "\\sample.std";

	/***   start saving  ****/
	stdSample.SaveSourceData(fullPathName);
}

void WorkZoneDef::FpCalibrate()
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	pApp->m_FpCalculator.CalibrateWorkZone(this);
}

void WorkZoneDef::Serialize(CArchive& ar) 
{
	if(ar.IsLoading()){
		//ar>>DirName; //1
		//ar>>Index;   //2
		ar>>Tag;     //3
		ar>>ModifyTime; //4
		ar>>MeasureTime;//5
		ar>>CorrectTime;//6
		ar>>SampleTimes;//7
		ar>>Repeat; //8
		ar>>ContentUnit; //9
		ar>>ThickUnit;   //10
		ar>>Precision;   //11
		ar>>AdjustTimes; //12
		ar>>AdjustRepeat;//13
		ar>>MqMax; //14
		ar>>Collimator; //15
		//ar>>DefMaName;  //16
		ar>>ArrayVariable;//17
		ar>>ArraySize;  //18
	}
	else{
		//ar<<DirName;
		//ar<<Index;
		ar<<Tag;
		ar<<ModifyTime;
		ar<<MeasureTime;
		ar<<CorrectTime;
		ar<<SampleTimes;
		ar<<Repeat;
		ar<<ContentUnit;
		ar<<ThickUnit;
		ar<<Precision;
		ar<<AdjustTimes;
		ar<<AdjustRepeat;
		ar<<MqMax;
		ar<<Collimator;
		//ar<<DefMaName;
		ar<<ArrayVariable;
		ar<<ArraySize;
	}
}

void WorkZoneDef::XMLSerialize(BOOL bStoring,TiXmlDocument* fatherNode)
{
	char stringBuff[128];
	TiXmlElement *root,*wzNode;
	CString test;
	int nYear,nMonth,nDate,nHour,nMin,nSec;   

	if(bStoring){
		TiXmlElement *classRoot =new TiXmlElement("WorkZoneDef");
		fatherNode->LinkEndChild(classRoot);
		/*
		TiXmlElement *t_NumOfConditions =new TiXmlElement("NumOfConditions"); //@2012-08-09
		classRoot->LinkEndChild(t_NumOfConditions);
		sprintf(stringBuff,"%d",NumOfConditions);
        TiXmlText* v_NumOfConditions = new TiXmlText(stringBuff);            
        t_NumOfConditions->LinkEndChild(v_NumOfConditions);

		TiXmlElement *t_NumOfSamples =new TiXmlElement("NumOfSamples"); //@2012-08-09
		classRoot->LinkEndChild(t_NumOfSamples);
		sprintf(stringBuff,"%d",NumOfSamples);
        TiXmlText* v_NumOfSamples = new TiXmlText(stringBuff);            
        t_NumOfSamples->LinkEndChild(v_NumOfSamples); 

		TiXmlElement *t_DirName =new TiXmlElement("DirName"); //1
		classRoot->LinkEndChild(t_DirName);
		sprintf(stringBuff,"%s",DirName);
        TiXmlText* v_DirName = new TiXmlText(stringBuff);            
        t_DirName->LinkEndChild(v_DirName);

		TiXmlElement *t_Index =new TiXmlElement("Index"); //2
		classRoot->LinkEndChild(t_Index);
		sprintf(stringBuff,"%d",Index);
        TiXmlText* v_Index = new TiXmlText(stringBuff);            
        t_Index->LinkEndChild(v_Index); */

		TiXmlElement *t_Tag =new TiXmlElement("Tag"); //3
		classRoot->LinkEndChild(t_Tag);
		sprintf(stringBuff,"%s",Tag);
        TiXmlText* v_Tag = new TiXmlText(stringBuff);            
        t_Tag->LinkEndChild(v_Tag);

		TiXmlElement *t_ModifyTime =new TiXmlElement("ModifyTime"); //4
		classRoot->LinkEndChild(t_ModifyTime);
		sprintf(stringBuff,"%s",ModifyTime.Format("%Y-%m-%d %H:%M:%S"));
        TiXmlText* v_ModifyTime = new TiXmlText(stringBuff);            
        t_ModifyTime->LinkEndChild(v_ModifyTime);

		TiXmlElement *t_MeasureTime =new TiXmlElement("MeasureTime"); //5
		classRoot->LinkEndChild(t_MeasureTime);
		sprintf(stringBuff,"%s",MeasureTime.Format("%Y-%m-%d %H:%M:%S"));
        TiXmlText* v_MeasureTime = new TiXmlText(stringBuff);            
        t_MeasureTime->LinkEndChild(v_MeasureTime);

		TiXmlElement *t_CorrectTime =new TiXmlElement("CorrectTime"); //6
		classRoot->LinkEndChild(t_CorrectTime);
		sprintf(stringBuff,"%s",CorrectTime.Format("%Y-%m-%d %H:%M:%S"));
        TiXmlText* v_CorrectTime = new TiXmlText(stringBuff);            
        t_CorrectTime->LinkEndChild(v_CorrectTime);

		TiXmlElement *t_SampleTimes =new TiXmlElement("SampleTimes"); //7
		classRoot->LinkEndChild(t_SampleTimes);
		sprintf(stringBuff,"%d",SampleTimes);
        TiXmlText* v_SampleTimes = new TiXmlText(stringBuff);            
        t_SampleTimes->LinkEndChild(v_SampleTimes);

		TiXmlElement *t_Repeat =new TiXmlElement("Repeat"); //8
		classRoot->LinkEndChild(t_Repeat);
		sprintf(stringBuff,"%d",Repeat);
        TiXmlText* v_Repeat = new TiXmlText(stringBuff);            
        t_Repeat->LinkEndChild(v_Repeat);

		TiXmlElement *t_ContentUnit =new TiXmlElement("ContentUnit"); //9
		classRoot->LinkEndChild(t_ContentUnit);
		sprintf(stringBuff,"%d",ContentUnit);
        TiXmlText* v_ContentUnit = new TiXmlText(stringBuff);            
        t_ContentUnit->LinkEndChild(v_ContentUnit);

		TiXmlElement *t_ThickUnit =new TiXmlElement("ThickUnit"); //10
		classRoot->LinkEndChild(t_ThickUnit);
		sprintf(stringBuff,"%d",ThickUnit);
        TiXmlText* v_ThickUnit = new TiXmlText(stringBuff);            
        t_ThickUnit->LinkEndChild(v_ThickUnit);

		TiXmlElement *t_Precision =new TiXmlElement("Precision"); //11
		classRoot->LinkEndChild(t_Precision);
		sprintf(stringBuff,"%d",Precision);
        TiXmlText* v_Precision = new TiXmlText(stringBuff);            
        t_Precision->LinkEndChild(v_Precision);

		TiXmlElement *t_AdjustTimes =new TiXmlElement("AdjustTimes"); //12
		classRoot->LinkEndChild(t_AdjustTimes);
		sprintf(stringBuff,"%d",AdjustTimes);
        TiXmlText* v_AdjustTimes = new TiXmlText(stringBuff);            
        t_AdjustTimes->LinkEndChild(v_AdjustTimes);

		TiXmlElement *t_AdjustRepeat =new TiXmlElement("AdjustRepeat"); //13
		classRoot->LinkEndChild(t_AdjustRepeat);
		sprintf(stringBuff,"%d",AdjustRepeat);
        TiXmlText* v_AdjustRepeat = new TiXmlText(stringBuff);            
        t_AdjustRepeat->LinkEndChild(v_AdjustRepeat);

		TiXmlElement *t_MqMax =new TiXmlElement("MqMax"); //14
		classRoot->LinkEndChild(t_MqMax);
		sprintf(stringBuff,"%d",MqMax);
        TiXmlText* v_MqMax = new TiXmlText(stringBuff);            
        t_MqMax->LinkEndChild(v_MqMax);

		TiXmlElement *t_Collimator =new TiXmlElement("Collimator"); //15
		classRoot->LinkEndChild(t_Collimator);
		sprintf(stringBuff,"%d",Collimator);
        TiXmlText* v_Collimator = new TiXmlText(stringBuff);            
        t_Collimator->LinkEndChild(v_Collimator);

		//TiXmlElement *t_DefMaName =new TiXmlElement("DefMaName"); //16
		//classRoot->LinkEndChild(t_DefMaName);
		//sprintf(stringBuff,"%s",DefMaName);
        //TiXmlText* v_DefMaName = new TiXmlText(stringBuff);            
        //t_DefMaName->LinkEndChild(v_DefMaName);

		TiXmlElement *t_ArrayVariable =new TiXmlElement("ArrayVariable"); //17
		classRoot->LinkEndChild(t_ArrayVariable);
		sprintf(stringBuff,"%d",ArrayVariable);
        TiXmlText* v_ArrayVariable = new TiXmlText(stringBuff);            
        t_ArrayVariable->LinkEndChild(v_ArrayVariable);

		TiXmlElement *t_ArraySize =new TiXmlElement("ArraySize"); //18
		classRoot->LinkEndChild(t_ArraySize);
		sprintf(stringBuff,"%d",ArraySize);
        TiXmlText* v_ArraySize = new TiXmlText(stringBuff);            
        t_ArraySize->LinkEndChild(v_ArraySize);

	}
	else{
		root = fatherNode->RootElement(); 
		/*
		wzNode = root->FirstChildElement("NumOfConditions"); //@2012-08-09
		strcpy(stringBuff,wzNode->GetText());
		NumOfConditions = atoi(stringBuff);

		wzNode = root->FirstChildElement("NumOfSamples"); //@2012-08-09
		strcpy(stringBuff,wzNode->GetText());
		NumOfSamples = atoi(stringBuff);

		wzNode = root->FirstChildElement("DirName"); //1
		strcpy(stringBuff,wzNode->GetText()); 
		DirName = stringBuff;

		wzNode = root->FirstChildElement("Index"); //2
		strcpy(stringBuff,wzNode->GetText());
		Index = atoi(stringBuff); */
		
		wzNode = root->FirstChildElement("Tag"); //3
		strcpy(stringBuff,wzNode->GetText());
		Tag = stringBuff;

		wzNode = root->FirstChildElement("ModifyTime"); //4 
		strcpy(stringBuff,wzNode->GetText());
		sscanf(stringBuff,"%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDate,&nHour,&nMin,&nSec);   
		ModifyTime = CTime(nYear,nMonth,nDate,nHour,nMin,nSec);

		wzNode = root->FirstChildElement("MeasureTime"); //5
		strcpy(stringBuff,wzNode->GetText());
		sscanf(stringBuff,"%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDate,&nHour,&nMin,&nSec);  
		MeasureTime = CTime(nYear,nMonth,nDate,nHour,nMin,nSec);

		wzNode = root->FirstChildElement("CorrectTime"); //6
		strcpy(stringBuff,wzNode->GetText());
		sscanf(stringBuff,"%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDate,&nHour,&nMin,&nSec);  
		CorrectTime = CTime(nYear,nMonth,nDate,nHour,nMin,nSec);;

		wzNode = root->FirstChildElement("SampleTimes"); //7
		strcpy(stringBuff,wzNode->GetText());
		SampleTimes = atoi(stringBuff);

		wzNode = root->FirstChildElement("Repeat"); //8
		strcpy(stringBuff,wzNode->GetText());
		Repeat = atoi(stringBuff);

		wzNode = root->FirstChildElement("ContentUnit"); //9
		strcpy(stringBuff,wzNode->GetText());
		ContentUnit = atoi(stringBuff);

		wzNode = root->FirstChildElement("ThickUnit"); //10
		strcpy(stringBuff,wzNode->GetText());
		ThickUnit = atoi(stringBuff);

		wzNode = root->FirstChildElement("Precision"); //11
		strcpy(stringBuff,wzNode->GetText());
		Precision = atoi(stringBuff);

		wzNode = root->FirstChildElement("AdjustTimes"); //12
		strcpy(stringBuff,wzNode->GetText());
		AdjustTimes = atoi(stringBuff);

		wzNode = root->FirstChildElement("AdjustRepeat"); //13
		strcpy(stringBuff,wzNode->GetText());
		AdjustRepeat = atoi(stringBuff);

		wzNode = root->FirstChildElement("MqMax"); //14
		strcpy(stringBuff,wzNode->GetText());
		MqMax = atoi(stringBuff);

		wzNode = root->FirstChildElement("Collimator"); //15
		strcpy(stringBuff,wzNode->GetText());
		Collimator = atoi(stringBuff);

		//wzNode = root->FirstChildElement("DefMaName"); //16
		//strcpy(stringBuff,wzNode->GetText());
		//DefMaName = stringBuff;

		wzNode = root->FirstChildElement("ArrayVariable"); //17
		strcpy(stringBuff,wzNode->GetText());
		ArrayVariable = atoi(stringBuff);

		wzNode = root->FirstChildElement("ArraySize"); //18
		strcpy(stringBuff,wzNode->GetText());
		ArraySize = atoi(stringBuff);
	}
}

void WorkZoneDef::INISerialize(BOOL bStoring,const CString &file)
{

	if(bStoring){
	}
	else{
	}

}