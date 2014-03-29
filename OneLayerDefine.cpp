// OneLayerDefine.cpp: implementation of the COneLayerDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMeter.h"
#include "OneLayerDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OneLayerDefine::OneLayerDefine()
{
	isBackground = FALSE;
	m_thickMode=0;   //will calculate thickness
	m_contentMode=2; //content is known
	NumOfElement=0;
	m_thickness=0.0;
	m_theoryness=0.0;
	m_correctness=0.0;
	m_density=0.0;
	m_isSaturated=FALSE;
	m_isExist=TRUE;
}

OneLayerDefine::~OneLayerDefine()
{
	CClassicalElement* pElem;
	int num = m_elementList.GetSize();
	for(int i=0;i<num;i++)
	{
		pElem = m_elementList.GetAt(i);
		delete pElem;
	}
	m_elementList.RemoveAll();
	
}

void OneLayerDefine::CopyFrom(OneLayerDefine* pSrc)
{
	isBackground = pSrc->isBackground;
	m_thickMode  = pSrc->m_thickMode;  
	m_contentMode= pSrc->m_contentMode; 
	NumOfElement = pSrc->NumOfElement;
	m_thickness  = pSrc->m_thickness;
	m_theoryness = pSrc->m_theoryness;  /* theory thickness */
	m_correctness= pSrc->m_correctness; /* correct thickness */
	m_density    = pSrc->m_density;
	m_isSaturated= pSrc->m_isSaturated;
	m_isExist = pSrc->m_isExist;  // newly added @ 2012-08-30

	CClassicalElement *pSrcElem,*pNewElem;
	NumOfElement = pSrc->m_elementList.GetSize();
	for(int i=0;i<NumOfElement;i++)
	{
		pSrcElem = pSrc->m_elementList.GetAt(i);
		pNewElem = new CClassicalElement(pSrcElem);
		m_elementList.Add(pNewElem);
	}
}

CString OneLayerDefine::GetLayerName()
{
	CString name;
	
	CClassicalElement* pElem;
	int num = m_elementList.GetSize();
	for(int i=0;i<num;i++)
	{
		pElem = m_elementList.GetAt(i);
		name += pElem->Name + "-";
	}
	name = name.Left(name.GetLength()-1);
	return name;
}

float OneLayerDefine::CalcDensity()
{
	//float densityTemp = 0.0;
	m_density = 0.0;
	CClassicalElement* pElem;
	int num = m_elementList.GetSize();
	for(int i=0;i<num;i++)
	{
		pElem = m_elementList.GetAt(i);
		m_density += ((pElem->Density)*(pElem->Content/100));
	}
	if(m_density < 0.00001) m_density = 0.0;
	return m_density;
}

float OneLayerDefine::CalcAverageDensity()
{
	m_density = 0.0;
	CClassicalElement* pElem;
	int num = m_elementList.GetSize();
	float tempVol = 0.0;
	
	for(int i=0;i<num;i++)
	{
		pElem = m_elementList.GetAt(i);
		tempVol += ((pElem->Content)/100)/(pElem->Density);
	}
	if(tempVol < 0.00001) {
		m_density = 0.0;
	} 
	else{
		m_density = 1/tempVol;
		if(m_density < 0.00001) m_density = 0.0;
	}
	return m_density;
}

void OneLayerDefine::RemoveAll()
{
	int no = m_elementList.GetSize();
	for(int i=0;i<no;i++)
	{
		delete m_elementList.GetAt(i);
	}
	m_elementList.RemoveAll();
}

void OneLayerDefine::XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode)
{
	char stringBuff[64];
	TiXmlElement *layerNode,*elemNode;
	//TiXmlAttribute *layerAttribute;
	CClassicalElement* pEle;
    if(IsStoring){
		TiXmlElement *t_layer =new TiXmlElement("COneLayer");
        fatherNode->LinkEndChild(t_layer);
		
		TiXmlElement *t_isBackground =new TiXmlElement("isBackground"); //(1)
		t_layer->LinkEndChild(t_isBackground);
		sprintf(stringBuff,"%d",isBackground);
        TiXmlText* v_isBackground = new TiXmlText(stringBuff);            
        t_isBackground->LinkEndChild(v_isBackground);

		TiXmlElement *t_thickMode =new TiXmlElement("ThickMode"); //(2)
		t_layer->LinkEndChild(t_thickMode);
		sprintf(stringBuff,"%d",m_thickMode);
        TiXmlText* v_thickMode = new TiXmlText(stringBuff);            
        t_thickMode->LinkEndChild(v_thickMode);

		TiXmlElement *t_contentMode =new TiXmlElement("ContentMode"); //(3)
		t_layer->LinkEndChild(t_contentMode);
		sprintf(stringBuff,"%d",m_contentMode);
        TiXmlText* v_contentMode = new TiXmlText(stringBuff);            
        t_contentMode->LinkEndChild(v_contentMode);

		TiXmlElement *t_thickness =new TiXmlElement("Thickness"); //(4)
		t_layer->LinkEndChild(t_thickness);
		sprintf(stringBuff,"%lf",m_thickness);
        TiXmlText* v_thickness = new TiXmlText(stringBuff);            
        t_thickness->LinkEndChild(v_thickness);

		TiXmlElement *t_theoryness =new TiXmlElement("Theoryness"); //(4.1) theory thickness
		t_layer->LinkEndChild(t_theoryness);
		sprintf(stringBuff,"%lf",m_theoryness);
        TiXmlText* v_theoryness = new TiXmlText(stringBuff);            
        t_theoryness->LinkEndChild(v_theoryness);

		TiXmlElement *t_correctness =new TiXmlElement("Correctness"); //(4.2) correct thickness
		t_layer->LinkEndChild(t_correctness);
		sprintf(stringBuff,"%lf",m_correctness);
        TiXmlText* v_correctness = new TiXmlText(stringBuff);            
        t_correctness->LinkEndChild(v_correctness);

		TiXmlElement *t_density =new TiXmlElement("Density"); //(5)
		t_layer->LinkEndChild(t_density);
		sprintf(stringBuff,"%lf",m_density);
        TiXmlText* v_density = new TiXmlText(stringBuff);            
        t_density->LinkEndChild(v_density);

		//newly added @ 2012-07-03 
		TiXmlElement *t_saturated =new TiXmlElement("Saturated"); //(6)
		t_layer->LinkEndChild(t_saturated);
		sprintf(stringBuff,"%d",m_isSaturated);
        TiXmlText* v_saturated = new TiXmlText(stringBuff);            
        t_saturated->LinkEndChild(v_saturated);

		//newly added @ 2012-08-30
		TiXmlElement *t_exist =new TiXmlElement("Exist"); //(6++)
		t_layer->LinkEndChild(t_exist);
		sprintf(stringBuff,"%d",m_isExist);
        TiXmlText* v_exist = new TiXmlText(stringBuff);            
        t_exist->LinkEndChild(v_exist);

		NumOfElement = m_elementList.GetSize();
		TiXmlElement *t_num =new TiXmlElement("NumOfElement"); //(7)
		t_layer->LinkEndChild(t_num);
		sprintf(stringBuff,"%d",NumOfElement);
        TiXmlText* v_num = new TiXmlText(stringBuff);            
        t_num->LinkEndChild(v_num);

		TiXmlElement *t_elementList =new TiXmlElement("ElementList"); //(8)
		t_layer->LinkEndChild(t_elementList);
		for(int i=0;i<NumOfElement;i++)
		{
			pEle = m_elementList.GetAt(i);
			pEle->XMLSerialize(TRUE,t_elementList);
		}

	}
	else{ // Loading, fatherNode = <COneLayer>
		layerNode = fatherNode->FirstChildElement("isBackground"); //(1)
		strcpy(stringBuff,layerNode->GetText());
		isBackground = (BOOL)atoi(stringBuff);

		layerNode = fatherNode->FirstChildElement("ThickMode"); //(2)
		strcpy(stringBuff,layerNode->GetText());
		m_thickMode = atoi(stringBuff);

		layerNode = fatherNode->FirstChildElement("ContentMode"); //(3)
		strcpy(stringBuff,layerNode->GetText());
		m_contentMode = atoi(stringBuff);

		layerNode = fatherNode->FirstChildElement("Thickness"); //(4)
		strcpy(stringBuff,layerNode->GetText());
		m_thickness = (float)atof(stringBuff);

		layerNode = fatherNode->FirstChildElement("Theoryness"); //(4.1) new format
		if(layerNode)
		{
			strcpy(stringBuff,layerNode->GetText());
			m_theoryness = (float)atof(stringBuff);
		}

		layerNode = fatherNode->FirstChildElement("Correctness"); //(4.2) new format
		if(layerNode)
		{
			strcpy(stringBuff,layerNode->GetText());
			m_correctness = (float)atof(stringBuff);
		}

		layerNode = fatherNode->FirstChildElement("Density"); //(5)
		if(layerNode)
		{
			strcpy(stringBuff,layerNode->GetText());
			m_density = (float)atof(stringBuff);
		}
		

		layerNode = fatherNode->FirstChildElement("Saturated"); //(6)
		if(layerNode)
		{
			strcpy(stringBuff,layerNode->GetText());
			m_isSaturated = (BOOL)atoi(stringBuff);
		}
		

		layerNode = fatherNode->FirstChildElement("Exist"); //(6++)
		if(layerNode)
		{
			strcpy(stringBuff,layerNode->GetText());
			m_isExist = (BOOL)atoi(stringBuff);
		}
		

		layerNode = fatherNode->FirstChildElement("NumOfElement"); //(7)
		strcpy(stringBuff,layerNode->GetText());
		int num = atoi(stringBuff);

		layerNode = fatherNode->FirstChildElement("ElementList");
		elemNode = layerNode->FirstChildElement("OneElement");
		for(int i=0;i<num;i++)
		{
			pEle = new CClassicalElement();
			pEle->XMLSerialize(FALSE,elemNode);
			m_elementList.Add(pEle);
			elemNode = elemNode->NextSiblingElement();
		}
	}
}
