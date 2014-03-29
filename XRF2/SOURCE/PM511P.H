# include "TypeDef.h"
# ifndef __PM511P_H__
# define __PM511P_H__
struct AdStruct {
	int AdCh;
	int Times;
	int CurTimes;
	float Sum;
	float Data;
	BOOL Ready;
	AdStruct();
	};
void InitAnalog();
void AnalogUpdate(void);
BOOL ReadAd(int ch,float& data);
void PrepairAdConv();
void AboutQuitAnalog();
void DaOut(int ch,float& v);
void SetDioBit(int port,int bit,BOOL high);
BOOL GetDioBitHigh(int port,int bit);
# endif