#include <math.h>
#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"
#include "Axis.h"

void CAxis::ForcePathZero(BOOL pz) {
	path0=pz;
	}

BOOL CAxis::ScaleAxis(double base) {
	double tempMin;
	if(path0&&min>0) tempMin=0;
	else tempMin=min;
	double stepLen,length;
	int jep,n;
	if(fabs(max-tempMin)<1.0e-6)
	{
		min -= 1.0;
		tempMin = min;
		max += 1.0;
	}
	else if(max<=tempMin) return FALSE;
	length=max-tempMin;
	if(length<=1.0) {
		for(jep=0;;jep++) {
			stepLen=pow(base,-jep);
			if(length>stepLen) {
				scaleFormat.Format("%c%d.%dlf",'%',jep+2,jep);
				break;
				}
			}
		}
	else {
		for(jep=0;;jep++) {
			stepLen=pow(base,jep);
			if(length<=stepLen) {
				jep--;
				stepLen=pow(base,jep);
				if(length<10) scaleFormat.Format("%c%d.1lf",'%',jep+2);
				else scaleFormat.Format("%c%d.0lf",'%',jep);
				break;
				}
			}
		}
	BOOL minReady=FALSE;
	BOOL maxReady=FALSE;
	for(n=-(int)base;;n++) {
		if((!minReady)&&(n*stepLen>tempMin)) {
			minScale=(n-1)*stepLen;
			minReady=TRUE;
			}
		if((!maxReady)&&(n*stepLen>=max)) {
			maxScale=n*stepLen;
			maxReady=TRUE;
			}
		if(minReady&&maxReady) break;
		}
	mainStep=stepLen;
	mainSteps=(int)((maxScale-minScale+1.0e-6)/mainStep);
	switch(int(base)) {
		case 10:
			switch(mainSteps) {
				case 1:
					subSteps=10;
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					subSteps=5;
					break;
				default:
					subSteps=2;
					break;
				}
			break;
		case 4:
			subSteps=4;
			break;
		case 8:
			subSteps=2;
			break;
		default:
			subSteps=1;
			break;
		}
	return TRUE;
	}

double CAxis::GetWidth() 
{
	return maxScale-minScale;
}