# include <dos.h>
# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
# include "pm511p.H"
# include "CtrlCnst.h"
WORD baseAddr=0x0100; // Base Address of PM511P
WORD startAdConv=0x00; // Read
WORD muxOption=0x00;   //Write
WORD adStatus=0x01;    //Read
WORD adModeInt=0x01;   //Write
WORD readAdData=0x02;  //Read
WORD ClearISB=0x02;  //Read
WORD setAdTimerFactor=0x02; //Write
WORD selectDaCh=0x04;  //Write
WORD writeDaData=0x06; //Write

WORD dio8255PA=0x08; //Read & Write
WORD dio8255PB=0x09; //Read & Write
WORD dio8255PC=0x0A; //Read & Write

WORD set8255Cw=0x0B; //Read & Write
WORD ctc8254C1=0x0C; //Read & Write
WORD ctc8254C2=0x0D; //Read & Write
WORD ctc8254C3=0x0E; //Read & Write
WORD set8254Cw=0x0F; //Read & Write

BYTE CurrDioPort[3]={0x00,0x00,0x00};
int LoopSeq=0;
int ConnectAdChs=0;
struct AdStruct* pAd=NULL;

void SetDefaultAnalog();
void Set8255Mode(BOOL paIn,BOOL pbIn,BOOL pcIn);
void Set8253Mode(BYTE chn,BYTE mode);

AdStruct::AdStruct() {
	static int s_Adch=0;
	AdCh=s_Adch++;
	Times=1000;
	CurTimes=-1;
	Sum=0.0;Data=0.0;
	Ready=FALSE;
	}

void SetDefaultAnalog() {
	ConnectAdChs=10; // 
	pAd=new AdStruct[ConnectAdChs];
	FILE *fp=fopen("CFG\\ADCONFIG.INI","wt");
	fprintf(fp,"%d\n",ConnectAdChs);
	for(int i=0;i<ConnectAdChs;i++) {
		//fprintf(fp,"%d\n",pAd[i].AdCh);
		//fprintf(fp,"%d\n",pAd[i].Times);
		fprintf(fp,"%d %d\n",pAd[i].AdCh,pAd[i].Times);
		}
	fclose(fp);
	}

void InitAnalog() {
	FILE *fp=fopen("CFG\\ADCONFIG.INI","rt");
	if(!fp) {
		SetDefaultAnalog();
		printf("511p error1\n");
		getch();
	}
	else {
		if(fscanf(fp,"%d",&ConnectAdChs)==1) {
			pAd=new AdStruct[ConnectAdChs];
			for(int i=0;i<ConnectAdChs;i++) {
				if(fscanf(fp,"%d%d",&(pAd[i].AdCh),&(pAd[i].Times))!=2) break;
				//printf("pAd[%d] ch=%d,times=%d\n",i,pAd[i].AdCh,pAd[i].Times);
			}
			fclose(fp);
		}
		else {
			fclose(fp);
			printf("511p error2\n");
			getch();
			SetDefaultAnalog();
		}
	}
	Set8255Mode(FALSE,TRUE,FALSE);
}

void AnalogUpdate(void) 
{
	if(inportb(baseAddr+adStatus)&0x01) 
	{
	    return; //adStatus =0x01, if inportb return 0x00, A/D analog not complete
	}
	
	WORD adData=inport(baseAddr+readAdData)&0xfff; //readAdData=0x02
	if(++pAd[LoopSeq].CurTimes>0) 
	{
		pAd[LoopSeq].Sum+=adData;
		if(pAd[LoopSeq].CurTimes==pAd[LoopSeq].Times) 
		{
			pAd[LoopSeq].Data=(pAd[LoopSeq].Sum/pAd[LoopSeq].Times)/4095.0*10.0;
			pAd[LoopSeq].Ready=TRUE;
			pAd[LoopSeq].Sum=0.0;
			pAd[LoopSeq].CurTimes=-1;
			LoopSeq++;
			if(LoopSeq==ConnectAdChs) LoopSeq=0;
			outportb(baseAddr+muxOption,pAd[LoopSeq].AdCh); //muxOption=0x00, change A/D channels
		}
	}
	inport(baseAddr+startAdConv); //startAdConv = 0x00, start A/D
}

BOOL ReadAd(int ch,float& data) 
{
	for(int i=0;i<ConnectAdChs;i++) 
	{
		if(pAd[i].AdCh==ch) 
		{
			if(pAd[i].Ready) 
			{
				data=pAd[i].Data;
				pAd[i].Ready=FALSE;
				return TRUE;
			}
			else return FALSE;
		}
	}
	return FALSE;
}

void PrepairAdConv() {
	outportb(baseAddr+adModeInt,inportb(baseAddr+adModeInt)&0xf1); //adModeInt=0x01
	outportb(baseAddr+muxOption,pAd[LoopSeq].AdCh); //muxOption=0x00, change A/D channels
	inport(baseAddr+startAdConv);
	}

void AboutQuitAnalog() {
	if(pAd) delete[] pAd;
	}

void SetDioBit(int port,int bit,BOOL high) {
	//if((port>2)||(bit>7)) return;
	BYTE mask=0x01<<bit;
	BYTE outByte=CurrDioPort[port];
	if(high) outByte|=mask;else outByte&=~mask;
	outportb(baseAddr+dio8255PA+port,outByte);
	CurrDioPort[port]=outByte;
	}


BOOL GetDioBitHigh(int port,int bit) {
	BYTE mask=0x01<<bit;
	BYTE inByte=CurrDioPort[port]=inportb(baseAddr+dio8255PA+port);
	if((inByte&mask)==0x00) return FALSE;
	else return TRUE;
	}

void DaOut(int ch,float& v) {
	//if(ch>3) return;
	if(v<0) v=0;
	else  if(v>5.0) v=5.0;
	WORD code=4095.0*v/5.0;  //DA: 0~5V
	if(code>4095) code=4095;
	outportb(baseAddr+selectDaCh,ch);  //0x04
	outport(baseAddr+writeDaData,code);//0x06
	}

void StartPAdjMoter(BOOL forward,int steps) {
	SetDioBit(PressAdjMoterDir_Port,PressAdjMoterDir_Bit,forward);
	if(steps<1||steps>600) return;
	DWORD counter=100*steps;
	SetDioBit(PressAdjMoterRun_Port,PressAdjMoterRun_Bit,TRUE);
	outportb(baseAddr+ctc8254C3,counter&0xff);
	outportb(baseAddr+ctc8254C3,counter>>8);
	}

void StopPAdjMoter() {
	SetDioBit(PressAdjMoterRun_Port,PressAdjMoterRun_Bit,FALSE);
	}
//Set8255Mode(FALSE,TRUE,FALSE);
void Set8255Mode(BOOL paIn,BOOL pbIn,BOOL pcIn) {
	BYTE mode=0x80; //1000 0000
	if(paIn) mode|=0x10;
	if(pbIn) mode|=0x02; // mode = 1000 0000 | 0000 0010 = 1000 0010
	if(pcIn) mode|=0x09;
	outp(baseAddr+set8255Cw,mode); //0x0B
	if(paIn) CurrDioPort[0]=inportb(baseAddr+dio8255PA);
	else {
		CurrDioPort[0]=0x00;
		outport(baseAddr+dio8255PA,CurrDioPort[0]);
		}
	if(pbIn) CurrDioPort[1]=inportb(baseAddr+dio8255PB);
	else {
		CurrDioPort[1]=0x00;
		outport(baseAddr+dio8255PB,CurrDioPort[1]);
		}
	if(pcIn) CurrDioPort[2]=inportb(baseAddr+dio8255PC);
	else {
		CurrDioPort[2]=0x00;
		outport(baseAddr+dio8255PC,CurrDioPort[2]);
		}
	}


void Set8253Mode(BYTE chn,BYTE mode) {
	BYTE cw=0x30;cw|=mode<<1;cw|=chn<<6;
	outp(baseAddr+set8254Cw,cw);
	}