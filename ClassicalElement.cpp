// ClassicalElement.cpp: implementation of the CClassicalElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMeter.h"
#include "ClassicalElement.h"
#include "tinyxml.h"
#include "tinystr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


float PeriodicTable::GetElemDensity(CString ele)
{
	CClassicalElement* pOneElem;
	int nElem = GetSize();
	for(int i=0;i<nElem;i++)
	{
		pOneElem = GetAt(i);
		if(pOneElem->Name == ele)
			return pOneElem->Density;
	}
	return 0.f;
}

int PeriodicTable::GetElemLine(CString ele)
{
	CClassicalElement* pOneElem;
	int nElem = GetSize();
	for(int i=0;i<nElem;i++)
	{
		pOneElem = GetAt(i);
		if(pOneElem->Name == ele)
			return pOneElem->DefaultLine;
	}
	return 1; // k-alpha 
}

int PeriodicTable::GetElemAtomicNum(CString ele)
{
	CClassicalElement* pOneElem;
	int nElem = GetSize();
	for(int i=0;i<nElem;i++)
	{
		pOneElem = GetAt(i);
		if(pOneElem->Name == ele)
			return pOneElem->Id;
	}
	return 0; 
}

BOOL LoadPeriodicTable(PeriodicTable* ptable)
{
	assert(ptable);
	/////////utf-16 !!!!!!!!!!!
	//TiXmlDocument doc("Element.xml");
	TiXmlDocument doc("PeriodicTable.xml");
	if(!doc.LoadFile()) return FALSE;

	TiXmlElement* root = doc.RootElement(); // <ElemTable>
	TiXmlElement* OneElement;
	TiXmlAttribute* ElementAttribute;
	int ElementID;
	CString ElementName;
	float ElementWeight;
	float ElementDensity;
	float ElementKa;
	float ElementKb;
	float ElementLa,ElementLb,ElementLr,ElementLe;
	int line; //1-Ka , 2-Kb , 3-La , 4-Lb , 5-Lr, 6-Ma   
	CString lineStr;
	CString test;

	OneElement = root->FirstChildElement(); //Elem
	while(OneElement)
	{
		ElementAttribute = OneElement->FirstAttribute();
		test = ElementAttribute->Name();   //ID
		ElementID = ElementAttribute->IntValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Name
		ElementName = ElementAttribute->Value();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Weight
		ElementWeight = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Density
		ElementDensity = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Ka
		ElementKa = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Kb
		ElementKb = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //LA
		ElementLa = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Lb
		ElementLb = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Lr
		ElementLr = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //Le
		ElementLe = (float)ElementAttribute->DoubleValue();
		ElementAttribute = ElementAttribute->Next();

		test = ElementAttribute->Name();   //DefaultLine
		lineStr = ElementAttribute->Value();  //1-Ka , 2-Kb , 3-La , 4-Lb , 5-Lr, 6-Ma
		if(lineStr == "Ka") line=1;
		else if(lineStr == "Kb") line=2;
		else if(lineStr == "La") line=3;
		else if(lineStr == "Lb") line=4;
		else if(lineStr == "Lr") line=5;
		else if(lineStr == "Ma") line=6;
		else line=1; //default 

		CClassicalElement* pEle = new CClassicalElement(ElementID,ElementName,ElementWeight,ElementDensity,ElementKa,ElementKb,line);
		ptable->Add(pEle);
	
		OneElement = OneElement->NextSiblingElement();
	}
	return TRUE;
}

void FreePeriodicTable(PeriodicTable* ptable)
{
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();

	if(ptable->IsEmpty() != TRUE) 
	{
		int cno=ptable->GetSize();
		for(int i=0;i<cno;i++) 
			delete ptable->GetAt(i);
		ptable->RemoveAll();
	}
}

void CClassicalElement::XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode)
{
	char stringBuff[64];
	TiXmlElement *elemNode;
    if(IsStoring){
		TiXmlElement *t_elem =new TiXmlElement("OneElement");
        fatherNode->LinkEndChild(t_elem);
		
		//Id; Name; Density; Content
		TiXmlElement *t_id =new TiXmlElement("ID"); //(1)
		t_elem->LinkEndChild(t_id);
		sprintf(stringBuff,"%d",Id);
        TiXmlText* v_id = new TiXmlText(stringBuff);            
        t_id->LinkEndChild(v_id);

		TiXmlElement *t_name =new TiXmlElement("Name"); //(2)
		t_elem->LinkEndChild(t_name);
		sprintf(stringBuff,"%s",Name);
        TiXmlText* v_name = new TiXmlText(stringBuff);            
        t_name->LinkEndChild(v_name);

		TiXmlElement *t_density =new TiXmlElement("Density"); //(3)
		t_elem->LinkEndChild(t_density);
		sprintf(stringBuff,"%lf",Density);
        TiXmlText* v_density = new TiXmlText(stringBuff);            
        t_density->LinkEndChild(v_density);

		TiXmlElement *t_content =new TiXmlElement("Content"); //(4)
		t_elem->LinkEndChild(t_content);
		sprintf(stringBuff,"%lf",Content);
        TiXmlText* v_content = new TiXmlText(stringBuff);            
        t_content->LinkEndChild(v_content);

		TiXmlElement *t_line =new TiXmlElement("Line"); //(5)
		t_elem->LinkEndChild(t_line);
		sprintf(stringBuff,"%d",DefaultLine);
        TiXmlText* v_line = new TiXmlText(stringBuff);            
        t_line->LinkEndChild(v_line);

		TiXmlElement *t_calculated =new TiXmlElement("Calculated"); //(6)
		t_elem->LinkEndChild(t_calculated);
		sprintf(stringBuff,"%d",beActive);
        TiXmlText* v_calculated = new TiXmlText(stringBuff);            
        t_calculated->LinkEndChild(v_calculated);

		TiXmlElement *t_left =new TiXmlElement("WinLeft"); //(7) newly add
		t_elem->LinkEndChild(t_left);
		sprintf(stringBuff,"%d",WinLeft);
        TiXmlText* v_left = new TiXmlText(stringBuff);            
        t_left->LinkEndChild(v_left);

		TiXmlElement *t_right =new TiXmlElement("WinRight"); //(8) newly add
		t_elem->LinkEndChild(t_right);
		sprintf(stringBuff,"%d",WinRight);
        TiXmlText* v_right = new TiXmlText(stringBuff);            
        t_right->LinkEndChild(v_right);

		TiXmlElement *t_intensity =new TiXmlElement("Intensity"); //(9) new Intensity 
		t_elem->LinkEndChild(t_intensity);
		sprintf(stringBuff,"%lf",Intensity);
        TiXmlText* v_intensity = new TiXmlText(stringBuff);            
        t_intensity->LinkEndChild(v_intensity);

	}
	else{ // Loading, fatherNode = <OneElement>
		elemNode = fatherNode->FirstChildElement("ID"); //(1)
		strcpy(stringBuff,elemNode->GetText());
		Id = atoi(stringBuff);
		
		elemNode = fatherNode->FirstChildElement("Name"); //(2)
		strcpy(stringBuff,elemNode->GetText());
		Name = stringBuff;

		elemNode = fatherNode->FirstChildElement("Density"); //(3)
		strcpy(stringBuff,elemNode->GetText());
		Density = (float)atof(stringBuff);

		elemNode = fatherNode->FirstChildElement("Content"); //(4)
		strcpy(stringBuff,elemNode->GetText());
		Content = (float)atof(stringBuff);

		elemNode = fatherNode->FirstChildElement("Line"); //(5)
		strcpy(stringBuff,elemNode->GetText());
		DefaultLine = atoi(stringBuff);

		elemNode = fatherNode->FirstChildElement("Calculated"); //(6)
		strcpy(stringBuff,elemNode->GetText());
		beActive = atoi(stringBuff);

		elemNode = fatherNode->FirstChildElement("WinLeft"); //(7)
		if(elemNode){
			strcpy(stringBuff,elemNode->GetText());
			WinLeft = atoi(stringBuff);
		}
		
		elemNode = fatherNode->FirstChildElement("WinRight"); //(8)
		if(elemNode){
			strcpy(stringBuff,elemNode->GetText());
			WinRight = atoi(stringBuff);
		}

		elemNode = fatherNode->FirstChildElement("Intensity"); //(9)
		if(elemNode){
			strcpy(stringBuff,elemNode->GetText());
			Intensity = (float)atof(stringBuff);
		}
		
	}
}