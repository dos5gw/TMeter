# include "MP4010.h"
# include <bios.h>
# include <dos.h>
# include <stdio.h>
# include <process.h>
# include <malloc.h>
# include <graphics.h>
//int MP4010::CardStart=0;
//int CardNumber=2;
OneChannel::OneChannel() {
	CardNo=0;SubBoardNo=0;SharedCh=0;CardType=0;
	SetRepeatTimes=60;CurrTimes=0;
	Finished=FALSE;
    data=(DWORD huge*)farcalloc(1024,sizeof(DWORD));
	//data=new DWORD[1024];
	for(int i=0;i<1024;i++) data[i]=0;
	Modified=FALSE;
	}
	
OneChannel::~OneChannel() {
    delete[] data;
}

void OneChannel::ReadAndInc(MP4010* pCards) {
	if(Finished)return;
	if(pCards[CardNo].CardType==0) pCards[CardNo].ReadRAM(SubBoardNo,data);
	else { 
		if(pCards[CardNo].nChannelSelected!=SubBoardNo) return;
	       	pCards[CardNo].ReadAndAdd(SharedCh,data);
	}
	CurrTimes++;
	Modified=TRUE;
	if(CurrTimes>=SetRepeatTimes) Finished=TRUE;
	}


void OneChannel::Reset() {
		CurrTimes=0;
		Finished = (SetRepeatTimes == 0?TRUE:FALSE);
		for(int i=0;i<1024;i++) data[i]=0;
	}

ChnConfig::ChnConfig() {
	nCards=0;pMp4010Cards=NULL;
	nChCount=0; pChannels=NULL;
	InitSuccess=FALSE;
	Finished=FALSE;
	ReadyStart=FALSE;
	DataReady=FALSE;
	chSequence=0;
	chCurrSeq=0;
	if(LoadMPCardConfig()) {
   //	LoadMPCardConfig();
	InitSuccess=LoadChannelConfig();
	}
	else
	InitSuccess=FALSE;
	}

void ChnConfig::StartAllCard() {
	if(pMp4010Cards!=NULL) for(int i=0;i<nCards;i++)
	{
		if(pMp4010Cards[i].CardType==0)pMp4010Cards[i].Run();
		if(pMp4010Cards[i].CardType==1)pMp4010Cards[i].DAP912_ADSSam(0,100*1000000,3,5);
	}
}

void ChnConfig::StopAllCard() {
	if(pMp4010Cards!=NULL) for(int i=0;i<nCards;i++)
	{
		if(pMp4010Cards[i].CardType==0)pMp4010Cards[i].Stop();
		if(pMp4010Cards[i].CardType==1)pMp4010Cards[i].DAP912_Rew();
	}
}

BOOL ChnConfig::CollectFreeChs() {
	BOOL bOldCardChannelFinished;
	BOOL bNewCardChannelFinished;
	Finished=TRUE;
	static BOOL Forward=TRUE;
	FreeChns=0;
//910 channels
	if(Forward){
		for(int i=0;i<nCh910Count;i++) {
			if(pChannels[i].Finished) continue;
			else {
				bOldCardChannelFinished=FALSE;
				if(IsCardFree(pChannels[i].CardNo,pChannels[i].SubBoardNo,pChannels[i].SharedCh,pChannels[i].CardType)) {
					FreeChns|=0x01<<i;
				}
			}
		}
		Forward=FALSE;
	}
	else {
		for(int i=nCh910Count-1;i>=0;i--) {
			if(pChannels[i].Finished) continue;
			else {
				bOldCardChannelFinished=FALSE;
				if(IsCardFree(pChannels[i].CardNo,pChannels[i].SubBoardNo,pChannels[i].SharedCh,pChannels[i].CardType)) FreeChns|=0x01<<i;
			}
		}
		Forward=TRUE;
	}
//912 channels
	int nChannelSelected;
	BOOL bChannelFinished;
	for(int i=nCards910;i<nCards;i++)
	{
		if(pMp4010Cards[i].Finished)	continue;

		nChannelSelected = !(pMp4010Cards[i].nChannelSelected);

		bChannelFinished = AreChannelFinished(i,nChannelSelected);

		if(bChannelFinished)
		{
			nChannelSelected = pMp4010Cards[i].nChannelSelected;
			bChannelFinished = AreChannelFinished(i,nChannelSelected);
		}

		if(bChannelFinished)	pMp4010Cards[i].Finished = TRUE;
		else
		{
			pMp4010Cards[i].nChannelSelected = nChannelSelected;
			pMp4010Cards[i].DAP912_ADCH(nChannelSelected);

			bNewCardChannelFinished=FALSE;
		}
	}

	Finished=(BOOL)(bNewCardChannelFinished&&bOldCardChannelFinished);

	return Finished;
}

BOOL ChnConfig::AreChannelFinished(int cardId, int chn)
{
	for(int j=nCh910Count;j<nChCount;j++)
	{
		if((pChannels[j].CardNo == cardId) && (pChannels[j].SubBoardNo == chn)
			&& (!pChannels[j].Finished))		return FALSE;
	}

	return TRUE;
}

BOOL ChnConfig::IsCardFree(int card,int sub,int share,int CardType) {
	if((card>=nCards)||(pMp4010Cards==NULL)||(!pMp4010Cards[card].Success)) return FALSE;
	if(sub==0) {
		if(pMp4010Cards[card].SubBoard1Busy) return FALSE;
		else {
			pMp4010Cards[card].SubBoard1Busy=TRUE;
			pMp4010Cards[card].Select(sub,share);
			pMp4010Cards[card].ClearRAM(0,CardType);
			return TRUE;
		}
	}
	else {
		if(pMp4010Cards[card].SubBoard2Busy) return FALSE;
		else {
			pMp4010Cards[card].SubBoard2Busy=TRUE;
			pMp4010Cards[card].Select(sub,share);
			pMp4010Cards[card].ClearRAM(1,CardType);
			return TRUE;
		}
	}
}


BOOL ChnConfig::LoadMPCardConfig() {
	int cardi,cardtype,baseaddr;
	BOOL loadSuccess=TRUE;
	FILE *fp=fopen("CFG\\MPCARDCF.INI","rt");
	if(fp) {
		fscanf(fp,"%d %d %d",&nCards,&nCards910,&nCards912);
		pMp4010Cards=new MP4010[nCards];
		if(pMp4010Cards==NULL) loadSuccess=FALSE;
		else {
			for(int i=0;i<nCards;i++) {
	    	    fscanf(fp,"%d%d%x",&cardi,&cardtype,&baseaddr);
			    pMp4010Cards[i].ConfigCard(cardi,cardtype,baseaddr);
			    pMp4010Cards[i].BaseAddr912=baseaddr;
			}
			if(nCards910>0){
				for(int j=0;j<nCards910;j++){
			   //	pMp4010Cards[j].ConfigCard(cardi,cardtype,baseaddr);
					if(!pMp4010Cards[j].TestAP910()) {
						printf("Test AP910 Card %d Failed!\n",j);
						getch();
				}
			}
		    }//(nCards910>0
			if(nCards912>0){
				for(int k=nCards910;k<nCards;k++){
				    //pMp4010Cards[k].CardType=1;
					pMp4010Cards[k].DAP912_OpenDevice(pMp4010Cards[k].BaseAddr912,0);
					if(!pMp4010Cards[k].TestAP912()) {
						printf("Test AP912 Card %d Failed! Press any key to continue...\n",k); 
						getch();
			        }
			    }
		    }//nCards912>0
	    }
	fclose(fp);
	return loadSuccess;
	}
	else return FALSE;
}

BOOL MP4010::TestAP912()
{
	Success=TRUE;

	int i,j, k=0;
	for(i=0;i<255;i++)
	{
		DAP912_outb(0xe,i);
		j=DAP912_inb(0xe);
		if(j!=i)	k++;
	}

	if(k != 0)
	{
		printf("\n%lx reg[e] Error!\n",hDevice);
		//Success = FALSE;
	}
	if(!TestRAM(0,1))
	{
		printf("%lx Channel 0 Test Error!\n",hDevice);
		//Success = FALSE;
	}
	if(!TestRAM(1,1))
	{
		printf("%lx Channel 1 Test Error!\n",hDevice);
		//Success = FALSE;
	}

	return Success;
}

void ChnConfig::SetChannelTimer(int curCh,BYTE ti) {
	if(curCh<nChCount) pChannels[curCh].SetRepeatTimes=ti;
	}


BOOL ChnConfig::LoadChannelConfig()
{
	BOOL LoadSuccess=TRUE;
	FILE *fp=fopen("CFG\\MPCHCONF.INI","rt");
	if(fp!=NULL) {
		fscanf(fp,"%d %d %d",&nChCount,&nCh910Count,&nCh912Count);
		if(nChCount>0) {
			pChannels=new OneChannel[nChCount];
			if(pChannels==NULL) LoadSuccess=FALSE;
			else {
				for(int i=0;i<nChCount;i++) {
					fscanf(fp,"%d%d%d%d%d%d",&pChannels[i].nIdInBX,&pChannels[i].CardNo,&pChannels[i].SubBoardNo,&pChannels[i].SharedCh,&pChannels[i].CardType,&pChannels[i].SetRepeatTimes);
					if(!pMp4010Cards[pChannels[i].CardNo].Success) pChannels[i].Finished=TRUE;
					}
				}
			}
		fclose(fp);
		return LoadSuccess;
		}
	else return FALSE;
}

BOOL ChnConfig::Write2File()
{
	FILE *fp=fopen("CFG\\MPCHCONF.INI","wt");
	if(fp!=NULL)
	{
		fprintf(fp,"%d %d %d\n",nChCount,nCh910Count,nCh912Count);
		for(int i=0;i<nChCount;i++)
		{
			fprintf(fp,"%d %d %d %d %d %d\n",pChannels[i].nIdInBX,pChannels[i].CardNo,pChannels[i].SubBoardNo,pChannels[i].SharedCh,pChannels[i].CardType,pChannels[i].SetRepeatTimes);
		}
		fclose(fp);
		return TRUE;
	}
	else return FALSE;
}

ChnConfig::~ChnConfig() {
	if(pMp4010Cards) delete[] pMp4010Cards;
	if(pChannels) delete[] pChannels;
	}

MP4010::MP4010() {
    //data=(DWORD huge*)farcalloc(1024,sizeof(DWORD));
    xdata=(long huge*)farcalloc(5000,sizeof(long));
	sdata=(DWORD huge*)farcalloc(1024,sizeof(long));
    nChannelSelected=0;
    SubBoard1Busy=FALSE;
    SubBoard2Busy=FALSE;
}

MP4010::~MP4010() {
    delete xdata;
    delete sdata;
}

void MP4010::Select(int sub,long ch) {
	if(!Success) return;
  BYTE mode=GetMode();
	if(sub==0) {
		if(ch==0) mode&=0xfe;
		else mode|=0x01;
		mode&=0xfb;
		}
	else {
		if(ch==0) mode&=0xfd;
		else mode|=0x02;
		mode&=0xf7;
		}
	SetMode(mode);
	RamRewind();
	}

BOOL MP4010::TestAP910() {
	Success=TRUE;
	unsigned char TestByte=0;
	RamRewind();SetMode(TestByte);if(GetMode()!=TestByte) Success=FALSE;
	TestByte=0x81;SetMode(TestByte);if(GetMode()!=TestByte) Success=FALSE;
	if(!TestRAM(0,0)) Success=FALSE;
	if(!TestRAM(1,0)) Success=FALSE;
	return Success;
	}

void MP4010::ClearRAM(long ramch,int CardType) {
	if(CardType==0){
	if(!Success) return;
	unsigned char mode=GetMode();
	if(ramch==0) mode&=0x7f;
	else mode|=0x80;
	SetMode(mode);
	RamRewind();
	for(int i=0;i<4096;i++) outpw(BaseAddr+RAM_ACCS,0x0000);
	RamRewind();
	}
	else if(CardType==1){
	int i, k=4096;
	for(i=0;i<k;i++)
	{
		xdata[i] = 0;
	}
	DAP912_RamWrite(ramch,k,xdata);
	}
	}

void MP4010::ReadRAM(int bank,DWORD huge* pData) {
	if(!Success) return;
	int i;DWORD data;
	unsigned char mode=GetMode();
	if(bank==0) mode&=0x7f;
	else mode|=0x80;
	SetMode(mode);
	data=0;//int j=0;
	RamRewind();
		for(i=0;i<1024;i++) {
			pData[i]+=(inpw(BaseAddr+RAM_ACCS)+data);
			}
	  //	}
	}

BOOL MP4010::TestRAM(long ramch,int CardType) { //printf("Channel 0 Test Error!\n");
	BOOL testRAM;
	//--test ap910
	if(CardType==0){
		unsigned int n,data;
		unsigned char mode=GetMode();
		if(ramch==0) mode&=0x7f;
		else mode|=0x80;
		SetMode(mode);
		RamRewind();
		for(n=0;n<4096;n++)	outpw(BaseAddr+RAM_ACCS,n);
		RamRewind();
		for(n=0;n<4096;n++) {
			data=inpw(BaseAddr+RAM_ACCS);
			if(data!=n) testRAM=FALSE;
			else testRAM=TRUE;
		}
		RamRewind();
	}
	//--end test ap910
	else if(CardType==1){
		long i, j;
		long k=4096;
		long tmp1;

		//set data
		for(i=4095;i>=0;i--)
		{
			tmp1=(i&0xff);
			tmp1|= ((i<<8)&0xff00);
			tmp1|= ((i<<16)&0xff0000);
			tmp1&= 0xffffff;
			xdata[i]=tmp1;
		}

		//write and read
		DAP912_RamWrite(ramch,k,xdata);
		DAP912_RamRead(ramch);

		j=0;
		for(i=4095;i>=0;i--)
		{
			tmp1=(i&0xff);
			tmp1|= ((i<<8)&0xff00);
			tmp1|= ((i<<16)&0xff0000);
			tmp1&= 0xffffff;  //24bit data

			if(xdata[i]!=tmp1)
			{
				j++;
				/*printf diff data
				if(hDevice==0x300) {
					printf("data[%ld]=%ld\n",i,xdata[i]);
				} */
			}
		}

		if(j > 0) {
			testRAM=FALSE;
			printf("different=%d : ",j);
		}
		else testRAM=TRUE;

	} //end test ap912
	return testRAM;
}

void ChnConfig::ClearCardBusyFlag() {
	for(int i=0;i<nCards;i++) {
		pMp4010Cards[i].SubBoard1Busy=FALSE;
		pMp4010Cards[i].SubBoard2Busy=FALSE;
		}
	}

void ChnConfig::ResetAllChn() {
	for(int i=0;i<nCards;i++) {
		pMp4010Cards[i].ClearRAM(0,pMp4010Cards[i].CardType);
		pMp4010Cards[i].SubBoard1Busy=FALSE;
		pMp4010Cards[i].ClearRAM(1,pMp4010Cards[i].CardType);
		pMp4010Cards[i].SubBoard2Busy=FALSE;
		}
	for(i=0;i<nChCount;i++) {
		pChannels[i].Reset();
		}
	for(i=0;i<nCards;i++)		pMp4010Cards[i].Finished = FALSE;

	}

BOOL ChnConfig::Update() {
	if(DataReady) {
		for(int i=0;i<nCh910Count;i++) {
			if((FreeChns&(0x0001<<i))!=0x0000)
				pChannels[i].ReadAndInc(pMp4010Cards);
			}
		for(int j=nCh910Count;j<nChCount;j++)
			pChannels[j].ReadAndInc(pMp4010Cards);
		DataReady=FALSE;
		ClearCardBusyFlag();
		ReadyStart=(BOOL)!CollectFreeChs();
		return TRUE;
		}
	else return FALSE;
	}

void OneChannel::ShowSpectrum(int offset,double div) {
	char title[16];
	setfillstyle(SOLID_FILL,1);bar(64,10,640-64,440);
	int i,xpos,ypos;long int maxCount=0;double countSum=0;
	for(i=100;i<800;i++) {
		if(data[i]>maxCount) maxCount=data[i];
		countSum+=data[i];
		}
	if(maxCount<1) maxCount=1;
	setfillstyle(SOLID_FILL,2);
	rectangle(64,10,640-64,110);
	xpos=64+(offset/2);bar(xpos,10,xpos+256,110);
	setcolor(14);
	for(i=0;i<1024;i++) {
		xpos=64+(i/2);
		ypos=110-100.0*double(data[i])/double(maxCount);
		if(i>100&&i<800) setcolor(13);
		else setcolor(15);
		if(ypos<10) ypos=10;
		else if(ypos>109) ypos=109;
		if(i==0) moveto(xpos,ypos);
		else lineto(xpos,ypos);
		}
	maxCount*=div;
	setcolor(12);
	setcolor(14);rectangle(64,110,640-64,440);
	for(i=offset;(i<offset+512)&&(i<1024);i++) {
		if(i>100&&i<800) setcolor(13);
		else setcolor(15);
		xpos=64+(i-offset);
		ypos=440-330.0*double(data[i])/double(maxCount);
		if(ypos<111) ypos=111;
		else if(ypos>439) ypos=439;
		if(i==offset) moveto(xpos,ypos);
		else lineto(xpos,ypos);
		}
	sprintf(title,"%d:%03d/%03d",nIdInBX,CurrTimes,SetRepeatTimes);
	setcolor(11);outtextxy(80,120,title);
	if(CurrTimes>0) {
		sprintf(title,"CPS=%7.4lf",countSum/CurrTimes);
		setcolor(11);outtextxy(80,140,title);
		}
	}

void ChnConfig::ShowCurve() {
	int ch=0,chSave;char title[30];
	double div=1.0;int offset=0;
	int drv=DETECT,mode,key;
	initgraph(&drv,&mode,"");
	BOOL exit=FALSE;
	setcolor(15);moveto(0,456);lineto(639,456);
	setcolor(14);outtextxy(0,462,"<PgUp> - Prev Curve <PgDn> - Next Curve  <+> Zoom In <-> Zoom Out <F10> - return");
	pChannels[ch].ShowSpectrum(offset,1.00);
	while(!exit) {
		while(!bioskey(1));
		key=bioskey(0);
		switch(key) {
			case 0x4900://Page Up
				chSave=ch;
				ch--;if(ch<0) ch=0;
				if(chSave!=ch) pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x5100://Page Dn
				chSave=ch;
				ch++;if(ch>nChCount-1) ch=nChCount-1;
				if(chSave!=ch) pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x4d00: //right
				offset+=100;
				if(offset>1024-512) offset=1024-512;
				pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x4b00: //left
				offset-=100;
				if(offset<0) offset=0;
				pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x4e2b: // +
			case 0x0d2b:
				div*=0.618;
				pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x4a2d: // -
			case 0x0c2d:
				div*=1.618;
				pChannels[ch].ShowSpectrum(offset,div);
				break;
			case 0x4400: //F10
			case 0x011b: //esc
				exit=TRUE;
				break;
			}
		}
	closegraph();
	}

void ChnConfig::ShowSpectrum(int offset) {
	for(int i=0;i<nChCount;i++) {
		for(int j=0;j<20;j++) {
			gotoxy(12,4+j);textattr(0x0b);
			if(offset+j<1024) cprintf("%04d",offset+j+1);
			else cprintf("    ");
			gotoxy(20+i*12,4+j);textattr(0x07);
			if(offset+j<1024) cprintf("%6u",pChannels[i].data[offset+j]);
			else cprintf("      ");
			}
		}
	}

void ChnConfig::SaveSpectrum() {
	FILE *fp=fopen("spectrum.dat","wt");
	for(int j=0;j<1024;j++) {
		fprintf(fp,"%d\t",j);
		for(int i=0;i<nChCount;i++) {
			fprintf(fp,"%5u\t",pChannels[i].data[j]);
			}
		fprintf(fp,"\n");
		}
	fclose(fp);
	}

void ChnConfig::Show() {
	for(int i=0;i<nChCount;i++) {
		gotoxy(3+i*8,20);textattr(0x03);
		cprintf("%d:%1d%c%1d%c",pChannels[i].nIdInBX,pChannels[i].CardNo,(pChannels[i].SubBoardNo==0)?'A':'B',pChannels[i].SharedCh,(pChannels[i].CardType==0)?'L':'H');
		gotoxy(3+i*8,21);textattr(pChannels[i].Finished?0x0e:0x0f);
		cprintf("%3d",pChannels[i].CurrTimes);
		gotoxy(7+i*8,21);textattr(pChannels[i].Finished?0x0e:0x0f);
		cprintf("/%3d",pChannels[i].SetRepeatTimes);
		gotoxy(3+i*8,22);textattr(pChannels[i].Modified?0x0d:0x0b);
		cprintf(pChannels[i].Modified?"Ready":"Trans");
		}
	}

void ChnConfig::ConfirmCurChn() {
	pChannels[chCurrSeq].Modified=FALSE;
	}

OneChannel*  ChnConfig::GetChnAndMoveToNext() {
	OneChannel* p=&pChannels[chCurrSeq++];
	if(chCurrSeq==nChCount) chCurrSeq=0;
	return p;
	}
//Open Device
long MP4010::DAP912_OpenDevice(long iobase, long DeviceNum)
{
	hDevice = iobase | (DeviceNum<<16);
	BaseAddr = iobase;
	CardID = DeviceNum;
	return 0;
}

// CloseIfOpen
long MP4010::DAP912_CloseDevice()
{
	return 0;
}

//sel AD input
long MP4010::DAP912_ADCH(int ch)
{
   //	BYTE mode=GetMode();;
	long i=0;
   //	if(sub==0){
	//DAP912_outb(0xf,ch);
   //	if(ch==0)mode&=0xfe;
  /*	else mode|=0x01;
	mode&=0xfb;
	}else{
	if(ch==0)mode&=0xfd;
	else mode|=0x02;
	mode&=0xf7;
	}*/
	DAP912_outb(0xf,ch);
//delay 5mS 5000 IO
	for(i=0;i<5000;i++)	DAP912_inb(0);
	return 0;
}

//**************************************************************
// start a Specture sam program
// tdata: sam timer to end, 1uS base timer
// endnum0, endnum1: ch0, ch1 end cnt number
long MP4010::DAP912_ADSSam(long adsp,long tdata, long endnum0, long endnum1)
{
	long i=0, ch=0;
	for(i=0;i<5000;i++)		xdata[i]=0;
	//clr ram
	for(ch=0;ch<2;ch++)
	{
		DAP912_Rew();
	//set ch
		DAP912_Mode(0,0,ch);
		DAP912_outsb(0x2,xdata,4096);
	}
//stop and set mode
	DAP912_Rew();
//set mode = spec.
	DAP912_Mode(1,adsp,0);
//set end number
	DAP912_SetEndNum(endnum0,endnum1);
//set timer
	DAP912_SetTimer(tdata);
//strat ad
	DAP912_Start();
	return 0;
}

//**************************************************************
//start a wave sam program 
// sm 4k point
//chbank=0,1 sam 2ch Bank0 or bank1
long MP4010::DAP912_ADWSam(long adsp)
{
	long i=0;
//stop and set mode
	DAP912_Rew();
//set mode
	DAP912_Mode(0,adsp,0);
//strat ad
	DAP912_Start();
	do
	{
		i=DAP912_inb(0) & 1;
	}while(i!=0);

	return 0;
}

//****************************************************************
//* set Freq sam end counter number
//* number=1-15
//* return read back, low 8 is: number0 / data bit15-bi8 : number1
long MP4010::DAP912_SetEndNum(long endnum0, long endnum1)
{
	long i0,i1;

//set end number0
	DAP912_outb(0xa,endnum0);

//set end number1
	DAP912_outb(0xb,endnum1);

//read back
	i0=DAP912_inb(0xa) & 0xf;   //num0
	i1=DAP912_inb(0xb) & 0xf;   //num1

	i0=i0 | (i1<<8);

	return i0;  //return number
}

//****************************************************************
//* read back state of AD
//* return =0 : ad sam end / =1 busya
//* *sammode: woring mode of user set
//* *adsp: 0-3 via ad speed 40/20/10/5 Mhz
long MP4010::DAP912_State(long *sammode, long *adsp,long *ramch)
{
	long i = DAP912_inb(0);

	*sammode=(i>>1 ) & 1;
	*adsp=(i>>2) & 3;
	*ramch=(i>>7) & 1;

	return i & 1;
}

//****************************************************************
//* poll if end
//* return =0 : ad sam end / =1 busya
//* rtadat=temp timer data
long MP4010::DAP912_ADPoll( long *rtdata)
{
	long i=0;
	*rtdata=DAP912_ReadTimer();

	i=DAP912_inb(0);

	return i & 1;
}

//---------------------------------------------------------------------------
//RAM Function
//***************************************************************
//Filling data to RAM bankN
//this program will rewrite mode reg.
long MP4010::DAP912_RamWrite(long ramch, long wlen,long huge* wdata)
{
	long i=0;
	//   long tmp1;

	//amx 4K
	if(wlen>4096)	   wlen=4096;

	DAP912_Rew();

	//set ch
	DAP912_Mode(0,0,ramch);

	i=DAP912_outsb(0x2,wdata,wlen);

	//return write length
	return i;
}

//****************************************************************
//read Ram data of 4K
//*rdata must > 4K long
long MP4010::DAP912_RamRead(long ramch)
{
	DAP912_Rew();

	//set ch
	DAP912_Mode(0,0,ramch);

	DAP912_insb(0x2,xdata,4096);

	return 0; //read data length
}

//****************************************************************
//set timer data
//tdata:24bit data , time base=1uS
long MP4010::DAP912_SetTimer( long tdata)
{
	long i=tdata;

	//set low
	DAP912_outb(0x6,tdata & 0xff);

	//set m8
	i=tdata>>8;
	i=i & 0xff;
	DAP912_outb(0x7, i);

	//set h8
	i=tdata>>16;
	i=i & 0xff;
	DAP912_outb(0x8, i);

	//return timer data
	return 0;
}


//****************************************************************
//read back timer
long MP4010::DAP912_ReadTimer()
{
	long i=0;
	//read back
	i=DAP912_inb(0x6);  //low 8 and load data
	i=i | (DAP912_inb(0x7)<<8);   //m8
	i=i | (DAP912_inb(0x8)<<16);  //h8

	//return timer data 
	return i;
}

//****************************************************************
//* set working mode
//* adsp: 0-3 via speed 40/20/10/5Mhz
//* adch: set read/write ram channel =0/1
//* sammode: =0 sampling data / =1 Freq. sam
//****************************************************************

long MP4010::DAP912_Mode(long sammode,long adsp, long adch)
{
	long i=0;

	adsp=adsp & 3;
	adch=adch & 1;

	i=sammode & 1;
	i=i | (adsp<<4);
	i=i | (adch<<7);

	DAP912_outb(0,i);

	return 0;
}


//**************************************************************
//start  AD operation
long MP4010::DAP912_Start()
{
    long temp;
    temp=DAP912_outb(1,1);
    return temp;
	//return DAP912_outb(1,0);
}

//**************************************************************
//reset or stop AD operation
long MP4010::DAP912_Rew()
{
    long temp;
    temp=DAP912_outb(1,0);
    return temp;
	//return DAP912_inb(1);
}

/////////////////////////////////////////////////////////////////////////////////
//DAP912 Self Function

//inb
//adr is offset of IO
long MP4010::DAP912_inb(long adr)
{
	int ioadr;
	ioadr=(hDevice & 0xffff) + adr;

	return (long)inp(ioadr);
}

//sequetial read   , adr , adr+1 , adr+2
long MP4010::DAP912_insb(long adr, long huge* rdata,long readl)
{
	int i;
	long dd,dd1;
	int ioadr=(hDevice & 0xfff) +adr;
	int card_mode_adr   =(hDevice & 0xfff) + MODE_REG;
	int stop_action_adr =(hDevice & 0xfff) + STOP_REW;
	int able_read_adr   =(hDevice & 0xfff) + ABLE_R;
	int able_write_adr  =(hDevice & 0xfff) + ABLE_W;
    
    //--pre reading--
	//BYTE curr_mode=inp(card_mode_adr) & 0xff;
	//curr_mode&=0Xfe; //1111,1110
	//outp(card_mode_adr,curr_mode);

	outp(stop_action_adr,0);
	outp(able_write_adr,0);
	outp(able_read_adr,1);
	
    //--begin reading--
	for(i=0;i<readl;i++)
	{
		dd=inp(ioadr);

		dd1=inp(ioadr+1);
		dd=dd | (dd1<<8);

		dd1=inp(ioadr+2);
		dd=dd | (dd1<<16);

		rdata[i]=dd;
	}
	
	//--stop reading--
	DAP912_outb(able_read_adr,0);
	return 0;
}


//outb
long MP4010::DAP912_outb(long adr,long data1)
{
	int ioadr;
	int dd;
	dd=data1 & 0xff;
	ioadr=(hDevice & 0xfff) +adr;

	return outp(ioadr,dd);
}

//-----------------------------------------------------------------------------------
//outsb: write seq. data of number=writel  from buffer=*wdata
//length <1024
//ou 24bit data by byte

long MP4010::DAP912_outsb(long adr, long huge* wdata,long writel)
{
	int i, dd;
	int ioadr=(hDevice & 0xfff) + adr;
	int card_mode_adr   =(hDevice & 0xfff) + MODE_REG;
	int stop_action_adr =(hDevice & 0xfff) + STOP_REW;
	int able_read_adr   =(hDevice & 0xfff) + ABLE_R;
	int able_write_adr  =(hDevice & 0xfff) + ABLE_W;
    
    //--pre reading--
	//BYTE curr_mode=inp(card_mode_adr) & 0xff;
	//curr_mode&=0Xfe; //1111,1110
	//outp(card_mode_adr,curr_mode);
    
    outp(stop_action_adr,0);
    outp(able_read_adr,0);
    outp(able_write_adr,1);
    
    //outp(0x302,low byte);outp(0x303,mid byte);outp(0x304,high byte);
	for(i=0;i<writel;i++)
	{
		dd=wdata[i] & 0xff;
		outp(ioadr,dd);

		dd=(wdata[i]>>8) & 0xff;
		outp(ioadr+1,dd);

		dd=(wdata[i]>>16) & 0xff;
		outp(ioadr+2,dd);
	}
	
	//--stop writing--
	DAP912_outb(able_write_adr,0);
	return 0;
}

//ReadAndAdd(SharedCh,data);
void MP4010::ReadAndAdd(long ramch, DWORD huge* data)
{
	DAP912_RamRead(ramch);
	for(int i=0;i<1024;i++)	
	    data[i] += xdata[2*LENGTH+2*i]+xdata[2*LENGTH+2*i+1];
	//Smooth(data);
}

void MP4010::Smooth(DWORD huge* data){ //DWORD huge* OneChannel::data

	//3-point smooth
	//S[i]=(d[i-1] + d[i] + d[i+1])/3

	for(int i=0;i<1024;i++)
	{
		if(i<1 || i>1022)
			sdata[i]=0; //sdata[0]=sdata[1023]=0;
		else
			sdata[i]=(data[i-1] + data[i] + data[i+1])/3;
	}
	for(int j=0;j<1024;j++)
	{
		data[j]=sdata[j];
	}
}
/*
void MP4010::Smooth(DWORD huge* data){ //DWORD huge* OneChannel::data

	//5-point smooth
	//S[i]=(d[i-2] + 2*d[i-1] + 3*d[i] + 2*d[i+1] + d[i+2])/9

	for(int i=0;i<1024;i++)
	{
		if(i<2 || i>1021)
			sdata[i]=0; //sdata[0]=sdata[1]=sdata[1022]=sdata[1023]=0;
		else
			sdata[i]=(data[i-2] + 2*data[i-1] + 3*data[i] + 2*data[i+1] + data[i+2])/9;
	}
	for(int j=0;j<1024;j++)
	{
		data[j]=sdata[j];
	}
} */
