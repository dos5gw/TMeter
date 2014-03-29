#include "StdAfx.h"
#include "TMeterString.h"

// if(!CheckDelimiterNum(inBuff,nStd,","))
BOOL CheckDelimiterNum(CString buff,int num,CString dlt)
{
	int n=0;
	for(int i=0;i<buff.GetLength();i++)
	{
		if(dlt==buff[i]) n++;
	}
	return (n==(num-1))?TRUE:FALSE;
}

//	dataBuff=GetStringAt(inBuff,0,",");
CString GetStringAt(CString buff,int num, CString dlt)
{
    CString dataBuff;
	int n=0;
	int start,len;

	for(int i=0; i<buff.GetLength(); i++)
	{
		if(dlt==buff[i]) n++;
		if(num==n) // check 
		{
			if(i==0) start=i;
			else start=i+1;
			//int subLength = buff.GetLength()-start;
			for(int j=start; j<buff.GetLength(); j++)
			{
				if(dlt==buff[j]) {
					len=j-start;
					break;
				}
				else if(j==(buff.GetLength()-1)){
					len=j-start+1;
					break;
				}
			}
			break;
		}
	}
	dataBuff = buff.Mid(start,len);
    return dataBuff;
}