#include "StdAfx.h"
#include "TMeter.h"
#include "IntensityCalculator.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "TMeterTypeDefine.h"
#include <windows.h> 


BOOL PureElementPtrList::ExistElement(CString elem)
{
	OnePureElement* pOneElem = NULL;
	int nElem = GetSize();
	for(int i=0;i<nElem;i++)
	{
		pOneElem = GetAt(i);
		//if(elem.Compare(pOneElem->Name))
		if(elem == pOneElem->Name)
			return TRUE;
	}
	return FALSE;
}

IntensityCalculator::IntensityCalculator()
{
	SetElementWindows=NULL;
	SetPureElementsData=NULL;
	PreFactorFit=NULL;
	ElementFit=NULL;
	m_pureCalcMethod = 1;
}

BOOL IntensityCalculator::LoadPureElementListXML()
{
	TiXmlDocument doc("PureElenemtList.xml");
	if(!doc.LoadFile()) return FALSE;
	
	TiXmlElement* root = doc.RootElement(); // <Element>
	
	TiXmlElement* OneElement;
	TiXmlElement* MethodElement;
	TiXmlAttribute* ElementAttribute;
	CString ElementName;
	int Left, Right;
	CString test;
	
	//OneElement = root->FirstChildElement(); //Elem
	MethodElement = root->FirstChildElement(); 
	ElementAttribute = MethodElement->FirstAttribute();
	test = ElementAttribute->Name();  
	m_pureCalcMethod = ElementAttribute->IntValue();
	
	OneElement = MethodElement->NextSiblingElement();
	while(OneElement)
	{
		ElementAttribute = OneElement->FirstAttribute();
		test = ElementAttribute->Name();   //Name
		ElementName = ElementAttribute->Value();
		ElementAttribute = ElementAttribute->Next();
		
		test = ElementAttribute->Name();   //Left
		Left = ElementAttribute->IntValue();
		ElementAttribute = ElementAttribute->Next();
		
		test = ElementAttribute->Name();   //Right
		Right = ElementAttribute->IntValue();
		ElementAttribute = ElementAttribute->Next();
		
		OnePureElement* pEle=new OnePureElement(ElementName,Left,Right,TRUE);
		pureElementList.Add(pEle);
		
		OneElement = OneElement->NextSiblingElement();
	}
	return TRUE;
}

BOOL IntensityCalculator::LoadPureElementList()
{
	CStdioFile iniFile;
	int findPos;
	CString inBuff;
	
	if(iniFile.Open("PureElenemtList.ini",CFile::modeRead)) 
	{
		while(iniFile.ReadString(inBuff))
		{
			if(inBuff.Find("# ")!=-1) continue;
			else if(inBuff.Find("Method=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				m_pureCalcMethod = atoi(inBuff);
			}
			else if(inBuff.Find("Element=")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				inBuff =inBuff.Mid(findPos);
				OnePureElement* pEle=new OnePureElement(inBuff,0,0,TRUE);  // nePureElement(CString name,int l,int r,BOOL active=FALSE)
				pureElementList.Add(pEle);
			}
		}
		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL IntensityCalculator::LoadModuleLibrary()
{
	hDllInst = LoadLibrary("Fack.dll");
	if(!hDllInst) 
		return FALSE;
	SetElementWindows = (fp_SetElementWindows)GetProcAddress(hDllInst,"SetElementsWindow");
	SetPureElementsData  = (fp_SetElementData)GetProcAddress(hDllInst,"SetPureElementsData");
	PreFactorFit = (fp_PreFactorFit)GetProcAddress(hDllInst,"PreFactorFit");
	ElementFit = (fp_ElementFit)GetProcAddress(hDllInst,"ElementFit");
	if(!SetElementWindows || !SetPureElementsData || !PreFactorFit || !ElementFit) 
		return FALSE;
	// load module done...
	
	return TRUE;
}

void IntensityCalculator::ReloadPureElemList(WorkZoneDef *pWz)
{
	CleanPureElementList();

	int i,j;
	OneLayerDefine* pOneLayer; 
	CClassicalElement* pOneEle;

	/* Reload Pure-Element-List from Work-Zone */
	CDefMa* pDefMa = &(pWz->defMa);
	int nLayer = pDefMa->m_layerArray.GetSize();
	for(i=0;i<nLayer;i++)
	{
		pOneLayer=pDefMa->m_layerArray.GetAt(i);
		
		int nElem=pOneLayer->m_elementList.GetSize();
		for(j=0;j<nElem;j++)
		{
			pOneEle=pOneLayer->m_elementList.GetAt(j);
			if(pureElementList.ExistElement(pOneEle->Name)) continue;
			else {
				OnePureElement* pPure = new OnePureElement(pOneEle->Name,pOneEle->WinLeft,pOneEle->WinRight,pOneEle->beActive);
				pureElementList.Add(pPure);
			}
			
		} // foreach elements
		
	} // foreach layers
}

BOOL IntensityCalculator::LoadPureSpectrum(WorkZoneDef *pWz)
{
	CleanPureElementList();

	int i,j;
	OneLayerDefine* pOneLayer; 
	CClassicalElement* pOneEle;

	/* Reload Pure-Element-List from Work-Zone */
	CDefMa* pDefMa = &(pWz->defMa);
	int nLayer = pDefMa->m_layerArray.GetSize();
	for(i=0;i<nLayer;i++)
	{
		pOneLayer=pDefMa->m_layerArray.GetAt(i);

		int nElem=pOneLayer->m_elementList.GetSize();
		for(j=0;j<nElem;j++)
		{
			pOneEle=pOneLayer->m_elementList.GetAt(j);
			if(pureElementList.ExistElement(pOneEle->Name)) continue;
			else {
				OnePureElement* pPure = new OnePureElement(pOneEle->Name,pOneEle->WinLeft,pOneEle->WinRight,pOneEle->beActive);
				pureElementList.Add(pPure);
			}

		} // foreach elements
		
	} // foreach layers

	/* Reload Spectrum in Pure-Element-List  */
	OnePureElement* pEle;
	CString specFileName;
	int count = pureElementList.GetSize();
	int* win = new int[count*2];
	double* datas= new double[count*SPEC_SIZE];
	double* peaks= new double[count]; // for debug log output
	
	for(i=0;i<count;i++)
	{
		pEle = pureElementList.GetAt(i);
		specFileName = pWz->GetFullSpectrumPath() + "\\_" + pEle->Name + ".spc";
		if(!specInfo.LoadSpectrumFromBinary(specFileName))
		{
			CString text;
			text.Format("ÔØÈëÊ§°Ü %s",specFileName);
			AfxMessageBox(text);
		}
		//win[i*2]=pEle->left;
		//win[i*2+1]=pEle->right;
		win[i*2]=specInfo.m_baseline;
		win[i*2+1]=specInfo.m_topline;
		peaks[i] = specInfo.GetPeakDword();
		
		for(j=0;j<SPEC_SIZE;j++)
		{
			datas[SPEC_SIZE*i+j] = specInfo.GetCountClone(j);
		}
	}
	SetElementWindows(count,win);
	SetPureElementsData(SPEC_SIZE,datas);


	/** start: write to log **/
	/**   PureWindows.txt   **/
	CStdioFile iniFile;
	CString outBuff,tempBuff;
	CString logName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\Log\\PureWindows.txt";
	if(iniFile.Open(logName,CFile::modeCreate|CFile::modeWrite))
	{
		for(i=0;i<count;i++)
		{
			pEle = pureElementList.GetAt(i);
			outBuff.Format("%s :  peak=%5.2lf,  win=%d,%d\n",pEle->Name,peaks[i],win[i*2],win[i*2+1]);
			iniFile.WriteString(outBuff);
		}
		iniFile.Close();
	}
	/** done : write to log **/
	
	delete[] win;
	delete[] datas;
	delete[] peaks;
	
	return TRUE;
}

void IntensityCalculator::CalcPureIntensity(CSpectrumInfo<DWORD,SPEC_SIZE> &spec)
{

	int count=pureElementList.GetSize();
	if(count == 0) return; //assert(count);
	double* intensityArray = new double[count];
	double* data = new double[SPEC_SIZE]; 
	OnePureElement* pEle;
	for(int i=0;i<SPEC_SIZE;i++)
	{
		data[i] = spec.GetCountClone(i);
	}
	
	switch(m_pureCalcMethod)
	{
	case 1:
		PreFactorFit(intensityArray,data);
		break;
	case 2:
		ElementFit(intensityArray,data);
		break;
	}
	
	//int count = pureElementList.GetSize();
	for(int n=0;n<count;n++)
	{
		pEle = pureElementList.GetAt(n);
		if(intensityArray[n]<0) pEle->intensity =0.0;
		else pEle->intensity = (float)intensityArray[n];
	}	

	delete[] intensityArray;
	delete[] data;

	//-------------DEBUG, write to file, delete following code when release -------------
	CStdioFile iniFile;
	CString outBuff,tempBuff;
	CString logName = ((CTMeterApp*)AfxGetApp())->m_AppWorkDirectory + "\\Log\\PureIntensity.txt";
	if(iniFile.Open(logName,CFile::modeCreate|CFile::modeWrite))
	{
		outBuff.Format("# Pure Elements Intensity, Selected Method=%d \n",m_pureCalcMethod);
		iniFile.WriteString(outBuff);
		for(n=0;n<count;n++)
		{
			pEle = pureElementList.GetAt(n);
			outBuff.Format("%s = %f \n",pEle->Name,pEle->intensity);
			
			iniFile.WriteString(outBuff);
		}	
		iniFile.Close();
	}


	/* what the fuck! AfxMessageBox() interrupt CMdiMeasureView::ExecuteTask() and jump into CMdiMeasureView::ontime
	CXrfInstrument* pInstrument = &(((CTMeterApp*)AfxGetApp())->m_Instrument);
	CString ocp;
	ocp.LoadString(IDS_INFO_OCUUPY_STBTEST);
	if(pInstrument->CheckOccupied(ocp) == FALSE)
		AfxMessageBox(msgBuff);
	*/
}


float IntensityCalculator::GetIntensity(CString ele)
{
	//double intensity = 0.0;
	OnePureElement* pEle;
	int count = pureElementList.GetSize();
	for(int i=0;i<count;i++)
	{
		pEle = pureElementList.GetAt(i);
		if(pEle->Name.CompareNoCase(ele) ==0 )
		{
			return pEle->intensity;
		}
	}
	return 0.0;
}

void IntensityCalculator::CleanPureElementList()
{
	if(pureElementList.IsEmpty() != TRUE) 
	{
		int count=pureElementList.GetSize();
		for(int i=0;i<count;i++) 
			delete pureElementList.GetAt(i);
		pureElementList.RemoveAll();
	}
}

IntensityCalculator::~IntensityCalculator()
{
	CleanPureElementList();
	FreeLibrary(hDllInst);
}