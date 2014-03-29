// StdSample.cpp: implementation of the CStdSample class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tmeter.h"
#include "StdSample.h"
#include "TMeterString.h"
#include "tinyxml.h"
#include "tinystr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CStandardSample::XMLSerialize(BOOL IsStoring,TiXmlElement* fatherNode)
{
	char stringBuff[64];
	OneLayerDefine* pOneLayer;
	TiXmlElement *stdNode,*layerNode;
	int NumOfLayers;
	if(IsStoring){ // SAVE
		TiXmlElement *t_OneStandard =new TiXmlElement("OneStandard");
        fatherNode->LinkEndChild(t_OneStandard);

		// newly added @2012-08-29
		TiXmlElement *t_name =new TiXmlElement("StdName");
		t_OneStandard->LinkEndChild(t_name);
		sprintf(stringBuff,"%s",m_tag);
        TiXmlText* v_name = new TiXmlText(stringBuff);            
        t_name->LinkEndChild(v_name);

		TiXmlElement *t_num =new TiXmlElement("Num_OneLayer");
		t_OneStandard->LinkEndChild(t_num);
		NumOfLayers = m_layerArray.GetSize();
		sprintf(stringBuff,"%d",NumOfLayers);
        TiXmlText* v_num = new TiXmlText(stringBuff);            
        t_num->LinkEndChild(v_num);

		TiXmlElement *t_layers =new TiXmlElement("Layers");
		t_OneStandard->LinkEndChild(t_layers);
		for(int i=0;i<NumOfLayers;i++) {
			pOneLayer = m_layerArray.GetAt(i);
			pOneLayer->XMLSerialize(TRUE,t_layers);
		}
	}
	else{ // LOAD
		//RemoveAll();

		// newly added @2012-08-29
		stdNode = fatherNode->FirstChildElement("StdName"); 
		if(stdNode)
		{
			strcpy(stringBuff,stdNode->GetText());
			m_tag = stringBuff;
		}
		
		stdNode = fatherNode->FirstChildElement("Num_OneLayer"); 
		if(stdNode)
		{
			strcpy(stringBuff,stdNode->GetText());
			NumOfLayers = atoi(stringBuff);
		}

		stdNode = fatherNode->FirstChildElement("Layers"); 
		layerNode = stdNode->FirstChildElement("COneLayer");
		for(int i=0;i<NumOfLayers;i++)
		{
			pOneLayer = new OneLayerDefine();
			pOneLayer->XMLSerialize(FALSE,layerNode);
			m_layerArray.Add(pOneLayer);
			layerNode = layerNode->NextSiblingElement();

		}
	}
}

/*******************************************************/
/*                                                     */
/*******************************************************/

CStandardSamples::CStandardSamples()
{

}

CStandardSamples::~CStandardSamples()
{
	CleanAll();
}

void CStandardSamples::SetIntensity(int index)
{
	int nStds = GetSize();
	if(index >= nStds) return;

	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	CStandardSample* pOneStd = GetAt(index);
	int nLayer = pOneStd->m_layerArray.GetSize();
	for(int i=0;i<nLayer;i++)
	{
		OneLayerDefine* pOneLayer = pOneStd->m_layerArray.GetAt(i);
		int nElem = pOneLayer->m_elementList.GetSize();
		for(int j=0;j<nElem;j++)
		{
			CClassicalElement* pOneElem = pOneLayer->m_elementList.GetAt(j);
			pOneElem->Intensity = pApp->m_IntensityCalculator.GetIntensity(pOneElem->Name);
		}
	}
}

BOOL CStandardSamples::LoadSourceData(CString file)
{
	TiXmlDocument doc(file);
	if(doc.LoadFile())
	{
		XMLSerialize(FALSE,&doc);
		return TRUE;
	}
	
	CString text;
	text.Format("标准片载入失败, 请检查文件%s",file);
	AfxMessageBox(text);
	return FALSE;
	
}

BOOL CStandardSamples::SaveSourceData(CString file)
{
	TiXmlDocument doc;
	XMLSerialize(TRUE,&doc);
	if(doc.SaveFile((LPTSTR)(LPCTSTR)file))
	{
		return TRUE;	
	}
	
	CString text;
	text.Format("%s 保存失败",file);
	AfxMessageBox(text);
	return FALSE;
	
}

void CStandardSamples::CleanAll()
{
	int n = GetSize();
	for(int i=0;i<n;i++) delete GetAt(i);
	RemoveAll();
}

void CStandardSamples::XMLSerialize(BOOL IsStoring,TiXmlDocument* fatherNode)
{
	CStandardSample* pOneStd;
	int no;
	char stringBuff[128];
	TiXmlElement *root;
	TiXmlElement *SamplesNode,*OneSampleNode;
	//TiXmlAttribute *defMaAttribute;
	CString test;
	
	if(IsStoring){ // SAVE STANDARD SAMPLES
		TiXmlElement *StandardSamples =new TiXmlElement("StandardSamples");
		fatherNode->LinkEndChild(StandardSamples);

		no = GetSize();
		TiXmlElement *t_NumOfSamples =new TiXmlElement("NumOfSamples");
		StandardSamples->LinkEndChild(t_NumOfSamples);
		sprintf(stringBuff,"%d",no);
        TiXmlText* v_NumOfSamples = new TiXmlText(stringBuff);            
        t_NumOfSamples->LinkEndChild(v_NumOfSamples);

		TiXmlElement *t_Samples =new TiXmlElement("Samples");
		StandardSamples->LinkEndChild(t_Samples);
		for(int i=0;i<no;i++) {
			pOneStd = GetAt(i);
			pOneStd->XMLSerialize(TRUE,t_Samples);
		}

	}
	else{  // LOAD STANDARD SAMPLES

		RemoveAll();
		root = fatherNode->RootElement(); 

		SamplesNode = root->FirstChildElement("NumOfSamples");
		//test = defMaNode->Value();
		strcpy(stringBuff,SamplesNode->GetText());
		no = atoi(stringBuff);

		SamplesNode = root->FirstChildElement("Samples");
		OneSampleNode = SamplesNode->FirstChildElement("OneStandard");
		for(int i=0;i<no;i++){
			pOneStd = new CStandardSample();
			pOneStd->XMLSerialize(FALSE,OneSampleNode);
			Add(pOneStd);
			OneSampleNode = OneSampleNode->NextSiblingElement();
		}
	}

}





/************************************************************************/
/*               the following class are not used                       */
/************************************************************************/
OneStdLayer::OneStdLayer()
{
	m_Index = 1;
	m_Thickness = 0.0;
	m_TheoryThickness = 0.0;
	m_CorrectThickness = 0.0;
}

OneStdLayer::OneStdLayer(int index,float thick,float theory, float correct)
{
	m_Index = index;
	m_Thickness = thick;
	m_TheoryThickness = theory;
	m_CorrectThickness = correct;
}

void OneStdLayer::Serialize(CArchive& ar)
{
	if(ar.IsLoading())
	{
		ar>>m_Index;
		ar>>m_Thickness;
		ar>>m_TheoryThickness;
		ar>>m_CorrectThickness;
	}
	else
	{
		ar<<m_Index;
		ar<<m_Thickness;
		ar<<m_TheoryThickness;
		ar<<m_CorrectThickness;
	}
}

OneStdLayer::~OneStdLayer()
{

}


/************************************************************************/
/*   class CStdSample : public CTypedPtrArray<CObArray,OneStdLayer*>    */
/************************************************************************/
void CStdSample::ClearAll()
{
	int n = GetSize();
	for(int i=0;i<n;i++)
	{
		delete GetAt(i);
	}
	RemoveAll();
}

void CStdSample::Serialize(CArchive& ar)
{
	int i;
	OneStdLayer* pOneLayer;
	if(ar.IsLoading())
	{
		ClearAll();
		ar>>nLayers;
		for(i=0;i<nLayers;i++)
		{
			pOneLayer = new OneStdLayer();
			pOneLayer->Serialize(ar);
			Add(pOneLayer);
		}
	}
	else
	{
		ar<<nLayers;
		for(i=0;i<nLayers;i++)
		{
			pOneLayer = GetAt(i);
			pOneLayer->Serialize(ar);
		}
	}
}

/************************************************************************/
/*   class CStdSamples : public CTypedPtrArray<CObArray,CStdSample*>    */
/************************************************************************/

void CStdSamples::ClearAll()
{
	//CStdSample* pOneStd;
	int n = GetSize();
	for(int i=0;i<n;i++)
	{
		delete GetAt(i);
	}
	RemoveAll();
}

void CStdSamples::Serialize(CArchive& ar)
{

	int i;
	CStdSample* pOneStd;
	if(ar.IsLoading())
	{
		ClearAll();
		ar>>nSamples;
		for(i=0;i<nSamples;i++)
		{
			pOneStd = new CStdSample();
			pOneStd->Serialize(ar);
			Add(pOneStd);
		}
	}
	else
	{
		ar<<nSamples;
		for(i=0;i<nSamples;i++)
		{
			pOneStd = GetAt(i);
			pOneStd->Serialize(ar);
		}
	}
}



/***********************************************************************
float Thickness[][];
float TheoryThickness[][];
float CorrectThickness[][];
***********************************************************************/
void CStdSamples::ZeroAll()
{
	int n = MAX_STD_NUM * MAX_ELE_NUM;
	memset((void*)Thickness,0,sizeof(float)*n);
	memset((void*)TheoryThickness,0,sizeof(float)*n);
	memset((void*)CorrectThickness,0,sizeof(float)*n);

}

BOOL CStdSamples::LoadSourceData(CString file)
{
	CStdioFile iniFile;
	int findPos,i,j;
	CString inBuff,dataBuff;
	
	ZeroAll();
	if(iniFile.Open(file,CFile::modeRead)) 
	{
		while(iniFile.ReadString(inBuff))
		{
			if(inBuff.Find("nLayers=")!=-1)
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				nLayers = atoi(inBuff); 
				
			}
			else if(inBuff.Find("nSamples=")!=-1)
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				nSamples = atoi(inBuff); 
			}
			else if(inBuff.Find("#SampleStart")!=-1)
			{
				for(i=0;i<nSamples;i++)
				{
					iniFile.ReadString(inBuff);
					//CString StandardName = inBuff;
					
					iniFile.ReadString(inBuff); //(1) 厚度
					findPos=inBuff.Find('=')+1;
					inBuff =inBuff.Mid(findPos);
					for(j=0;j<nLayers;j++)
					{
						dataBuff=GetStringAt(inBuff,j,",");
						Thickness[i][j]=(float)atof(dataBuff);
					}
					
					iniFile.ReadString(inBuff); //(2) 理论厚度
					findPos=inBuff.Find('=')+1;
					inBuff =inBuff.Mid(findPos);
					for(j=0;j<nLayers;j++)
					{
						dataBuff=GetStringAt(inBuff,j,",");
						TheoryThickness[i][j]=(float)atof(dataBuff);
					}
					
					iniFile.ReadString(inBuff); //(3) 校正厚度
					findPos=inBuff.Find('=')+1;
					inBuff =inBuff.Mid(findPos);
					for(j=0;j<nLayers;j++)
					{
						dataBuff=GetStringAt(inBuff,j,",");
						CorrectThickness[i][j]=(float)atof(dataBuff);
					}
				} // foreach sample
			}
			else if(inBuff.Find("#SampleEnd")!=-1)
			{
				//break;
			}
		} // foreach row

		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CStdSamples::SaveSourceData(CString file)
{
	CStdioFile iniFile;
	CString outBuff,tempBuff;
	//int i;
	if(iniFile.Open(file,CFile::modeCreate|CFile::modeWrite))
	{
		outBuff.Format("nLayers=%d\n",nLayers); 

	
		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}