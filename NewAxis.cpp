#include "StdAfx.h"
#include "NewAxis.h"
#include <math.h>

CNewAxis::CNewAxis(void)
{
	base=10.0;
	minRange=0.0; 
	maxRange=1.0; 
	extendFactor=1.0; 
	mainStep=0.1; 
}

CNewAxis::CNewAxis(double extend)
{
	base=10.0;
	minRange=0.0; 
	maxRange=1.0; 
	extendFactor=extend; 
	mainStep=0.1; 
}

CNewAxis::~CNewAxis(void)
{

}

void CNewAxis::SetTitle(CString title) 
{
	axisTitle=title;
}

void CNewAxis::SetScale(double m,double M)
{
	maxRange=M;
	minRange=m;
	nSteps=0;
	ScaleAxis();
}

BOOL CNewAxis::ScaleAxis()
{
	double minScale,maxScale;
	int jep,i;
	double scaleLength,stepLength;

	if(maxRange<minRange) return FALSE;
	scaleLength=(maxRange-minRange)*extendFactor; 

	if(scaleLength<1.0)
	{
		maxRange=0.1;
		minRange=0.0;
		mainStep=0.02;
		nSteps=5;
		nJep=3;
		scaleFormat="%3.2f";
		return TRUE;
		/*
		for(i=0;;i++)
		{
			stepLength=pow(base,-i);
			if(scaleLength>stepLength)
			{
				jep=i;
				maxScale=pow(base,-i);
				minScale=pow(base,-i-1);
				break;
			}
		}*/
	}
	else
	{
		for(i=0;;i++)
		{
			stepLength=pow(base,i);
			if(scaleLength<=stepLength)
			{
				jep=i; //format of Y-Axis
				maxScale=pow(base,i-1); 
				minScale=pow(base,i-2); 
				break;
			}
		}
	}
	nJep=jep+1; // newly added @2012-06-07
	scaleFormat.Format("%c%d.1lf",'%',jep);

	if(scaleLength/maxScale>4) 
	{
		mainStep=maxScale;
		nSteps=(int)(scaleLength/mainStep);
		return TRUE;
	}
	else
	{
		for(i=1;;i++) 
		{
			if(scaleLength/(minScale*i)<8)
			{
				mainStep=minScale*i;
				nSteps=(int)(scaleLength/mainStep);
				break;
			}
			else if(minScale*i>scaleLength)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}