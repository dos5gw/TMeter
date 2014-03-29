// DefMa.cpp: implementation of the CDefMa class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMeter.h"
#include "DefMa.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefMa::CDefMa()
{
	m_tag = "default";
	m_thickMode = "default";
	m_note = "default";

	m_hv = 50;
	m_filter = 0;

	bAllElemArray=FALSE;
	bMqShown=FALSE;
	bScaleCompensated=FALSE;
	bOffsetCompensated=TRUE;
	bFilterUsed=FALSE;
	bNeedMeasureElem=FALSE;
}

void CDefMa::CopyFrom(CDefMa* pSrc)
{
	//m_tag = pSrc->m_tag;
	bScaleCompensated =pSrc->bScaleCompensated;
	bOffsetCompensated=pSrc->bOffsetCompensated;
	bNeedMeasureElem  =pSrc->bNeedMeasureElem;

	OneLayerDefine *pSrcLayer,*pNewLayer;
	int nLayer = pSrc->m_layerArray.GetSize();
	for(int i=0;i<nLayer;i++)
	{
		pSrcLayer = pSrc->m_layerArray.GetAt(i);
		pNewLayer = new OneLayerDefine();
		pNewLayer->CopyFrom(pSrcLayer);
		m_layerArray.Add(pNewLayer);
	}
}

CString CDefMa::GetStdSampleName() /* return name of standard(CDefMa's derive) , for example "Cr/Ni/Cu-Zn" */
{
	CString name;
	OneLayerDefine* pOneLayer;
	CString tempName;
	int nLayer = m_layerArray.GetSize();
	for(int i=0;i<nLayer;i++)
	{
		pOneLayer = m_layerArray.GetAt(i);
		tempName = pOneLayer->GetLayerName() + "/";
		name += tempName;
	}
	name.TrimRight("/");

	return name;
}

void CDefMa::RemoveAll()
{
	int no = m_layerArray.GetSize();
	for(int i=0;i<no;i++)
	{
		delete m_layerArray.GetAt(i);
	}
	m_layerArray.RemoveAll();

}

BOOL CDefMa::LoadSettings(CString file)
{
	TiXmlDocument doc(file);
	if(doc.LoadFile())
	{
		XMLSerialize(FALSE,&doc);
		return TRUE;
	}
	
	CString text;
	text.Format("Def.MAÔØÈëÊ§°Ü, Çë¼ì²éÎÄ¼þ%s",file);
	AfxMessageBox(text);
	return FALSE;
}

BOOL CDefMa::SaveSettings(CString file)
{
	TiXmlDocument doc;
	XMLSerialize(TRUE,&doc);
	if(doc.SaveFile((LPTSTR)(LPCTSTR)file))
	{
		return TRUE;	
	}
	
	CString text;
	text.Format("%s ±£´æÊ§°Ü",file);
	AfxMessageBox(text);
	return FALSE;
}

void CDefMa::XMLSerialize(BOOL IsStoring,TiXmlDocument* fatherNode)
{
	OneLayerDefine* pOneLayer; int no;
	char stringBuff[128];
	TiXmlElement *root;
	TiXmlElement *defMaNode,*layerNode;
	//TiXmlAttribute *defMaAttribute;
	CString test;

	if(IsStoring){
		TiXmlElement *classDefMa =new TiXmlElement("CDefMa");
		fatherNode->LinkEndChild(classDefMa);

		TiXmlElement *t_tag =new TiXmlElement("Tag");
		classDefMa->LinkEndChild(t_tag);
		sprintf(stringBuff,"%s",m_tag);
        TiXmlText* v_tag = new TiXmlText(stringBuff);            
        t_tag->LinkEndChild(v_tag);

		TiXmlElement *t_thickMode =new TiXmlElement("Mode");
		classDefMa->LinkEndChild(t_thickMode);
		sprintf(stringBuff,"%s",m_thickMode);
        TiXmlText* v_thickMode = new TiXmlText(stringBuff);            
        t_thickMode->LinkEndChild(v_thickMode);

		TiXmlElement *t_hv =new TiXmlElement("HV");
		classDefMa->LinkEndChild(t_hv);
		sprintf(stringBuff,"%d",m_hv);
        TiXmlText* v_hv = new TiXmlText(stringBuff);            
        t_hv->LinkEndChild(v_hv);

		TiXmlElement *t_filter =new TiXmlElement("Filter");
		classDefMa->LinkEndChild(t_filter);
		sprintf(stringBuff,"%d",m_filter);
        TiXmlText* v_filter = new TiXmlText(stringBuff);            
        t_filter->LinkEndChild(v_filter);

		TiXmlElement *t_bAllElemArray =new TiXmlElement("All_Element_Array");
		classDefMa->LinkEndChild(t_bAllElemArray);
		sprintf(stringBuff,"%d",bAllElemArray);
        TiXmlText* v_bAllElemArray = new TiXmlText(stringBuff);            
        t_bAllElemArray->LinkEndChild(v_bAllElemArray);

		TiXmlElement *t_bMqShown =new TiXmlElement("MQ_Shown");
		classDefMa->LinkEndChild(t_bMqShown);
		sprintf(stringBuff,"%d",bMqShown);
        TiXmlText* v_bMqShown = new TiXmlText(stringBuff);            
        t_bMqShown->LinkEndChild(v_bMqShown);

		TiXmlElement *t_bScaleCompensated =new TiXmlElement("Scale_Compensated");
		classDefMa->LinkEndChild(t_bScaleCompensated);
		sprintf(stringBuff,"%d",bScaleCompensated);
        TiXmlText* v_bScaleCompensated = new TiXmlText(stringBuff);            
        t_bScaleCompensated->LinkEndChild(v_bScaleCompensated);

		TiXmlElement *t_bOffsetCompensated =new TiXmlElement("Offset_Compensated");
		classDefMa->LinkEndChild(t_bOffsetCompensated);
		sprintf(stringBuff,"%d",bOffsetCompensated);
        TiXmlText* v_bOffsetCompensated = new TiXmlText(stringBuff);            
        t_bOffsetCompensated->LinkEndChild(v_bOffsetCompensated);

		TiXmlElement *t_bFilterUsed =new TiXmlElement("Filter_Used");
		classDefMa->LinkEndChild(t_bFilterUsed);
		sprintf(stringBuff,"%d",bFilterUsed);
        TiXmlText* v_bFilterUsed = new TiXmlText(stringBuff);            
        t_bFilterUsed->LinkEndChild(v_bFilterUsed);

		TiXmlElement *t_bNeedMeasureElem =new TiXmlElement("Need_Measurement");
		classDefMa->LinkEndChild(t_bNeedMeasureElem);
		sprintf(stringBuff,"%d",bNeedMeasureElem);
        TiXmlText* v_bNeedMeasureElem = new TiXmlText(stringBuff);            
        t_bNeedMeasureElem->LinkEndChild(v_bNeedMeasureElem);

		TiXmlElement *t_num =new TiXmlElement("Num_OneLayer");
		classDefMa->LinkEndChild(t_num);
		no = m_layerArray.GetSize();
		sprintf(stringBuff,"%d",no);
        TiXmlText* v_num = new TiXmlText(stringBuff);            
        t_num->LinkEndChild(v_num);

		TiXmlElement *t_layers =new TiXmlElement("Layers");
		classDefMa->LinkEndChild(t_layers);
		for(int i=0;i<no;i++) {
			pOneLayer = m_layerArray.GetAt(i);
			pOneLayer->XMLSerialize(TRUE,t_layers);
		}
	}
	else{ //IsLoading, Read XML
		RemoveAll();
		root = fatherNode->RootElement(); 
		defMaNode = root->FirstChildElement("Tag");
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText()); //strcpy(stringBuff,defMaNode->FirstChild()->Value());
		m_tag = stringBuff;
		
		defMaNode = root->FirstChildElement("Mode");
		//test = defMaNode->Value();
		if(defMaNode){
			strcpy(stringBuff,defMaNode->GetText());
			m_thickMode = stringBuff;
		}
		
		defMaNode = root->FirstChildElement("HV");
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		m_hv = atoi(stringBuff);

		defMaNode = root->FirstChildElement("Filter");
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		m_filter = atoi(stringBuff);

		defMaNode = root->FirstChildElement("All_Element_Array"); // BOOL(1)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bAllElemArray = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("MQ_Shown"); // BOOL(2)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bMqShown = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("Scale_Compensated"); // BOOL(3)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bScaleCompensated = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("Offset_Compensated"); // BOOL(4)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bOffsetCompensated = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("Filter_Used"); // BOOL(5)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bFilterUsed = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("Need_Measurement"); // BOOL(6)
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		bNeedMeasureElem = (BOOL)atoi(stringBuff);

		defMaNode = root->FirstChildElement("Num_OneLayer");
		//test = defMaNode->Value();
		strcpy(stringBuff,defMaNode->GetText());
		int num = atoi(stringBuff);

		defMaNode = root->FirstChildElement("Layers");
		layerNode = defMaNode->FirstChildElement("COneLayer");
		OneLayerDefine* pLayer;
		for(int i=0;i<num;i++)
		{
			pLayer = new OneLayerDefine();
			pLayer->XMLSerialize(FALSE,layerNode);
			m_layerArray.Add(pLayer);
			layerNode = layerNode->NextSiblingElement();
		}
	}
}

CDefMa::~CDefMa()
{
	OneLayerDefine* pOneLayer;
	int num = m_layerArray.GetSize();
	for(int i=0;i<num;i++)
	{
		pOneLayer = m_layerArray.GetAt(i);
		delete pOneLayer;
	}
	m_layerArray.RemoveAll();

}
