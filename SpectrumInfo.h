
#ifndef SPEC_INFO_H
#define SPEC_INFO_H
#include "XrfInstrument.h"
# include "Vector.h"
# include "Matrix.h"
# include <math.h>

template<class _TSPEC,int MAX>
//class CSpectrumInfo
class CSpectrumInfo : public CObject
{
	friend class CXrfInstrument;
	friend class CSpectrumDisplay;
	friend class CConditionMonitorDlg;
	friend class CAdjustZoneDlg;
	//friend class CMdiMeasureView;
public:
	CSpectrumInfo():spectrum(NULL),maxsize(MAX)
	{
		spectrum = new _TSPEC[maxsize];
		cloneSpectrum = new _TSPEC[maxsize];
		for(int i=0;i<maxsize;i++)
		{
			spectrum[i]=cloneSpectrum[i]=0;
		}
		m_totalTimes=5;
		m_currentTimes=0;
		m_isFinished=FALSE;
		m_bModified=TRUE;
		m_cps=0.0;
		m_precision=0.0;
		m_peakPos=0;
		m_halfWidth=0;
		m_baseline=10;
		m_topline=245;
		m_SmoothWidth=10;
		m_peakPosDword = 0.0;
		//added @2012-07-09 :
		m_fileName="";
		m_color=RGB(70,130,180); //SteelBlue
		m_bNeedDisplay = TRUE;
	}

	int GetPercent();
	int GetCurrentSampleTimes(){ return m_currentTimes; }
	int GetTotalSampleTimes(){ return m_totalTimes; }
	void SetCurrentSampleTimes(int t){ m_currentTimes=t; }
	void SetTotalSampleTimes(int t){ m_totalTimes=t; }
	_TSPEC GetDataAt(int index)
	{
		if(index>=maxsize) return (_TSPEC)0;
		else return spectrum[index];
	}
	void SetDataAt(int index,_TSPEC data)
	{
		if(index>=maxsize) return;
		else spectrum[index]=data;
	}
	BOOL m_isFinished; 
	BOOL m_bModified;

private:
	_TSPEC *spectrum;   // spectrum[]
	_TSPEC *cloneSpectrum; // cloneSpectrum[], smooth curve

	int m_totalTimes; // m_totalTimes = 10*TotalSecond
	int m_currentTimes;

	int m_SmoothWidth;
	
	double m_cps;
	double m_precision;
	double m_maxValue; 
	int m_peakPos;
	double m_peakPosDword;
	int m_halfWidth;

public:
	int m_baseline; // alias Windows Left
	int m_topline;  // alias Windows Right
	int maxsize;

	CString m_fileName;
	COLORREF m_color;
	BOOL m_bNeedDisplay;

public:
	//void MergeSpectrum();
	void CalculatePeak();
	void Calculate(BOOL PeekAdj,BOOL BxCorrect);

	BOOL LoadSpectrumFromFiles(CString fileName);
	void SaveSpectrumToFiles(CString fileName);

	BOOL LoadSpectrumFromBinary(CString fileName);
	void SaveSpectrumToBinary(CString fileName);
	
	void AutoSaveSpectrum();

	void ResetSpectrum();
	const double GetCountClone(int i);
	void Smooth(int SmoothWidth);
	void Smoothing();
	void GetPeekPos();
	void DetermineSmoothWidth(double peekHigh);
		
	double GetCps(){ return m_cps;}
	double GetPrecision() {return m_precision;}
	int GetPeak() { return m_peakPos;}
	double GetPeakDword() { return m_peakPosDword;}
	int GetHalfWidth() {return m_halfWidth;}

	~CSpectrumInfo()
	{
		free(spectrum);
	}
};

template<class _TSPEC,int MAX>
int CSpectrumInfo<_TSPEC,MAX>::GetPercent()
{
	if(m_totalTimes==0) 
		return 0;
	else 
		return 100*m_currentTimes/m_totalTimes;
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::ResetSpectrum()
{
	for(int i=0;i<maxsize;i++)
	{
		spectrum[i]=0;
		cloneSpectrum[i]=0;
	}
	m_currentTimes=0;
	m_isFinished=FALSE;
	m_cps=0.0;
	m_precision=0.0;
	m_peakPos=0;
	m_halfWidth=0;
	m_peakPosDword = 0.0;
	m_bNeedDisplay = TRUE;
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::AutoSaveSpectrum()
{
	
	CTime tm = CTime::GetCurrentTime();
	CString tmString = tm.Format("%Y-%m-%d %H-%M-%S");
	CString fileName= "SPECTRUM " + tmString + ".spc";
	SaveSpectrumToBinary(fileName);
}

template<class _TSPEC,int MAX>
BOOL CSpectrumInfo<_TSPEC,MAX>::LoadSpectrumFromBinary(CString fileName)
{	
	CFile file;
	
	if(file.Open(fileName,CFile::modeRead))
	{
		CArchive ar(&file,CArchive::load);
		ar>>m_currentTimes;
		ar>>m_totalTimes;
		ar>>m_baseline;
		ar>>m_topline;
		ar>>m_SmoothWidth;
		ar>>maxsize;
		for(int i=0;i<maxsize;i++)
		{
			ar>>spectrum[i];
		}
		ar.Close();
		file.Close();
		CalculatePeak();
		return TRUE;
	}
	else
	{
		//AfxMessageBox("Load Binary Spec Failed!");
		return FALSE;
	}
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::SaveSpectrumToBinary(CString fileName)
{
	//CString specFileName;
	//if(fileName.IsEmpty())
	
	CFile file;
	if(file.Open(fileName,CFile::modeCreate|CFile::modeWrite))
	{
		CArchive ar(&file,CArchive::store);
		ar<<m_currentTimes;
		ar<<m_totalTimes;
		ar<<m_baseline;
		ar<<m_topline;
		ar<<m_SmoothWidth;
		ar<<maxsize;
		for(int i=0;i<maxsize;i++)
		{
			ar<<spectrum[i];
		}
		ar.Close();
		file.Close();
	}
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::SaveSpectrumToFiles(CString fileName)
{
	// SPEC*.txt
	/****************************************************************
	TestTime=100
	TubCurrent=150
	TubVoltage=20
	Filter=1
	Collimator=1
	Vacuumed=false
	AdjustRate=false
	Channel=256
	Data=                                                               
	*****************************************************************/
	CStdioFile iniFile;
	CString specFileName,writeString;
	
	if(iniFile.Open(fileName,CFile::modeCreate|CFile::modeWrite))
	{
		writeString.Format("TestTime=%d\n",m_totalTimes);
		iniFile.WriteString(writeString);
		
		iniFile.WriteString("TubCurrent=150\n");
		iniFile.WriteString("TubVoltage=20\n");
		iniFile.WriteString("Filter=1\n");
		iniFile.WriteString("Collimator=1\n");
		iniFile.WriteString("Vacuumed=false\n");
		iniFile.WriteString("AdjustRate=false\n");
		writeString.Format("Channel=%d\n",maxsize);
		iniFile.WriteString(writeString);
		iniFile.WriteString("Data=\n");
		
		for(int i=0;i<maxsize;i++)
		{
			writeString.Format("%d\n",spectrum[i]);
			iniFile.WriteString(writeString);
		}
		iniFile.Close();
	}
}

template<class _TSPEC,int MAX>
BOOL CSpectrumInfo<_TSPEC,MAX>::LoadSpectrumFromFiles(CString fileName)
{
	CStdioFile iniFile;
	CString inBuff,value;
	int index=0,findPos;
	BOOL bFoundDataRow=FALSE;
	if(iniFile.Open(fileName,CFile::modeRead))
	{
		while(iniFile.ReadString(inBuff)) 
		{
			if(inBuff.Find("TestTime")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				value=inBuff.Mid(findPos);
				m_totalTimes=m_currentTimes=atoi(value); 
			}
			else if(inBuff.Find("Channel")!=-1) 
			{
				findPos=inBuff.Find('=')+1;
				value=inBuff.Mid(findPos);
				if(maxsize!=atoi(value)) // length error
				{
					AfxMessageBox("Value of [Channel] not match!");
					iniFile.Close();
					return FALSE;
				}
			}
			else if(inBuff.Find("Data")!=-1) 
			{
				bFoundDataRow=TRUE;
				continue;
			}
			if(bFoundDataRow)
			{
				spectrum[index++]=atoi(inBuff);

				//Exception
				if(index > maxsize-1) index=maxsize-1;
				//AfxMessageBox("Unknown File Type!")
			}
		}
		iniFile.Close();
		CalculatePeak();
		return TRUE;
	}
	else 
		return FALSE;
}

template<class _TSPEC,int MAX>
const double CSpectrumInfo<_TSPEC,MAX>::GetCountClone(int i)
{
	double count;
	if((i>=maxsize)||(m_currentTimes==0)) count=0.0;
	else count=cloneSpectrum[i]/(m_currentTimes*10);
	return count;
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::Smooth(int SmoothWidth) 
{
	int i,j;double no,x,y,a,b,c;CMatrix A(3,3),*invA;CVector B(3),*abc;
	double xbar,x2bar,x3bar,x4bar,ybar,xybar,x2ybar,Sx,Sx2,Sx3,Sx4,Sy,Sxy,Sx2y;
	for(i=0;i<maxsize;i++) {
		if(i-SmoothWidth<0||i+SmoothWidth>=maxsize) cloneSpectrum[i]=0;
		else {
			no=0;xbar=x2bar=x3bar=x4bar=Sx=Sx2=Sx3=Sx4=Sy=Sxy=Sx2y=0;
			ybar=xybar=x2ybar=0;
			for(j=i-SmoothWidth;j<i+SmoothWidth;j++) {
				x=j;y=spectrum[j];
				Sx+=x;Sx2+=x*x;Sx3+=x*x*x;Sx4+=x*x*x*x;
				Sy+=y;Sxy+=(x*y);Sx2y+=(x*x*y);
				no+=1.0;
			}
			xbar=Sx/no,x2bar=Sx2/no,x3bar=Sx3/no,x4bar=Sx4/no;
			ybar=Sy/no,xybar=Sxy/no,x2ybar=Sx2y/no;
			A.put(1.0,0,0);A.put(xbar,0,1);A.put(x2bar,0,2);
			A.put(xbar,1,0);A.put(x2bar,1,1);A.put(x3bar,1,2);
			A.put(x2bar,2,0);A.put(x3bar,2,1);A.put(x4bar,2,2);
			B.put(ybar,0);
			B.put(xybar,1);
			B.put(x2ybar,2);
			invA=A.inverse();
			abc=(*invA)*B;
			a=abc->get(0),b=abc->get(1),c=abc->get(2);
			cloneSpectrum[i]=(_TSPEC)(a+b*i+c*i*i);
			delete invA;delete abc;
		}
	}
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::Smoothing() 
{	
	/*smooth 5 points*/
	//int halfWidth=2;
	for(int i=0;i<maxsize;i++)
	{
		if(i<2 || i>253)
			cloneSpectrum[i]=0;
		else
			//cloneSpectrum[i]=(spectrum[i-2] + 2*spectrum[i-1] + 3*spectrum[i] + 2*spectrum[i+1] + spectrum[i+2])/9;
			cloneSpectrum[i]=(spectrum[i-2] + 4*spectrum[i-1] + 6*spectrum[i] + 4*spectrum[i+1] + spectrum[i+2])/16;
	}
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::CalculatePeak() //....
{
	int win=3;
	m_maxValue=0.0; int i;
	m_cps = 0;
	Smoothing();
	//Smoothing(); // smoothing twice
    
	//for(i=m_baseline; i<m_topline; i++)
	for(i=0; i<maxsize; i++)
	{
		if(cloneSpectrum[i] > m_maxValue)
		{
			m_peakPos=i;
			m_maxValue=cloneSpectrum[i];
		}   
		m_cps += cloneSpectrum[i];
	}
	int winLeft=m_peakPos-win;
	int winRight=m_peakPos+win;
	if(winLeft<0) winLeft=0;
	if(winRight>255) winRight=255;
	double  xy=0, sx=0; 
	for(i=winLeft;i<=winRight;i++)
	{
		xy+=cloneSpectrum[i]*i;
		sx+=cloneSpectrum[i];
	}
	if(fabs(sx)<0.001) m_peakPosDword=0;
	else m_peakPosDword= xy/sx;

	if((m_maxValue==0)||(m_currentTimes==0)) return;
	m_cps /= m_currentTimes*10;
	
	int leftHalf = 0;
	int rightHalf = 0;
	BOOL leftHalfCross=FALSE;
	BOOL rightHalfCross=FALSE;

	for(i=0; i<maxsize; i++)
	{
		if(i<m_peakPos)
		{
			if(!leftHalfCross && (cloneSpectrum[i] >= m_maxValue*0.5))
			{
				leftHalf=i;
				leftHalfCross=true;
			}
		}
		else if(i>m_peakPos)
		{
			if(!rightHalfCross && (cloneSpectrum[i] < m_maxValue*0.5))
			{
				rightHalf=i;
				rightHalfCross=true;
			}
		}
	}
	m_halfWidth=rightHalf-leftHalf;

	m_baseline= int(m_peakPosDword - m_halfWidth); // set win-left for intensity calculate
	m_topline = int(m_peakPosDword + m_halfWidth); // set win-right for intensity calculate

	if(m_peakPos==0) return;
	m_precision=m_halfWidth/m_peakPosDword*100;
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::Calculate(BOOL PeekAdj,BOOL BxCorrect)
{
	GetPeekPos();
	//GetherInfo(PeekAdj,BxCorrect);
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::GetPeekPos(void)
{
	int i=0;
	Smooth(10);
	double maxCps=0,peekPos=0;
	for(i=m_baseline;i<=m_topline;i++)
	{
		if(cloneSpectrum[i]>maxCps){
			peekPos=i; maxCps=cloneSpectrum[i];
		}
	}
	if(maxCps==0||m_totalTimes<1) return; 
	DetermineSmoothWidth(maxCps/m_totalTimes);
	Smooth(m_SmoothWidth);

	maxCps=0;
	for(i=m_baseline;i<=m_topline;i++) {
		if(cloneSpectrum[i]>maxCps) {
			peekPos=i;maxCps=cloneSpectrum[i];
		}
	}
	if(peekPos<=0)return;
	/*
	pChn->m_peek=GetCloseToPos(peekPos);
	double leftHalf,rightHalf=0;
	BOOL leftHalfCross=FALSE,rightHalfCross=FALSE;
	for(i=pChn->m_baseline;i<=pChn->m_topline;i++) 
	{
		if(i<pChn->m_peek) 
		{
			if(!leftHalfCross&&(cloneSpectrum[i]>=cloneSpectrum[int(pChn->m_peek)]*0.5)) 
			{
				leftHalf=double(i);
				leftHalfCross=TRUE;
			}
		}
		else if(i>pChn->m_peek) 
		{
			if(!rightHalfCross&&cloneSpectrum[i]<cloneSpectrum[int(pChn->m_peek)]*0.5) 
			{
				rightHalf=double(i);			
				rightHalfCross=TRUE;
			}
		}
	}
	int left=0;int right=0;
	if(leftHalfCross&&rightHalfCross)
	{
		left=LowInt(leftHalf);
		right=UpInt(rightHalf);
	}
	else if(!leftHalfCross&&rightHalf>0)
	{
		left=LowInt(2*pChn->m_peek-rightHalf);
		right=UpInt(rightHalf);
	}
	else if(!rightHalf&&leftHalfCross>0)
	{
		left=LowInt(leftHalf);
		right=UpInt(2*pChn->m_peek-leftHalf);
	}
	
	CMatrix A(right-left+1,3),*At,*AtA,*invAtA,*invAtADotAt;CVector B(right-left+1),*abc;
	for (i=0;i<right-left+1;i++) {
		A.put(1,i,0);
		A.put(i,i,1);
		A.put(i*i,i,2);
		B.put(log(cloneSpectrum[i+left]),i);
	}
	At=A.transmit();
	AtA=(*At)*A;
	invAtA=(*AtA).inverse();
	invAtADotAt=(*invAtA)*(*At);
	abc=(*invAtADotAt)*B;
	double a1,b1,c1;
	a1=abc->get(0),b1=abc->get(1),c1=abc->get(2);
	delete At;delete AtA;delete invAtA;delete invAtADotAt;delete abc;
	pChn->m_halfw=2*sqrt(-log(2)/c1);
	double xmax=-b1/(2*c1);
	double ymax=exp(a1 - xmax * xmax * c1);
	double cps=pChn->m_halfw*b1/(-2*c1)*sqrt(3.14159/log(2))/2;
	pChn->m_peek=left+xmax;
	*/
}

template<class _TSPEC,int MAX>
void CSpectrumInfo<_TSPEC,MAX>::DetermineSmoothWidth(double peekHigh) 
{
	if(peekHigh<1) m_SmoothWidth=100;
	else if(peekHigh<3) m_SmoothWidth=75;
	else if(peekHigh<5) m_SmoothWidth=50;
	else if(peekHigh<10) m_SmoothWidth=40;
	else if(peekHigh<50) m_SmoothWidth=30;
	else if(peekHigh<100) m_SmoothWidth=20;
	else if(peekHigh<300) m_SmoothWidth=15;
	else m_SmoothWidth=10;
}

#endif 
