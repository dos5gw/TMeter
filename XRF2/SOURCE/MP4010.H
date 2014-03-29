# include <conio.h>
# include "Typedef.h"
# define MODE_REG 0x00
# define STOP_REW 0x01
# define STARTREC 0x01
# define RAM_ACCS 0x02
# define ABLE_R 0x0c
# define ABLE_W 0x0d

# define LENGTH 1024
class MP4010;
struct OneChannel {
	int nIdInBX;
	int CardNo;
	int SubBoardNo;
	int SharedCh;
	int CardType;
	unsigned int CurrTimes,SetRepeatTimes;
	BOOL Modified;
	BOOL Finished;
	DWORD huge* data;
	OneChannel();
	~OneChannel();
	void Reset();
	void ReadAndInc(MP4010* pCards);
	void ShowSpectrum(int offset,double div);
	};
class ChnConfig {
	int nCards910;int nCards912;
	int nCards;int nChCount,nCh910Count,nCh912Count;
	int chSequence,chCurrSeq;
    WORD FreeChns;
	BOOL InitSuccess;
	MP4010 *pMp4010Cards;
	OneChannel *pChannels;
	BOOL LoadMPCardConfig();
	BOOL LoadChannelConfig();
	void ClearCardBusyFlag();
	BOOL Finished;
public:
	OneChannel*  GetChnAndMoveToNext();
	void SetChannelTimer(int curCh,BYTE ret);
	void ConfirmCurChn();
	BOOL CollectFreeChs();
	BOOL AreChannelFinished(int cardId, int chn);
    BOOL IsCardFree(int card,int sub,int share,int cardtype);
	BOOL Success() {return InitSuccess;}
	int GetChNo() {return nChCount;}
	ChnConfig();
	~ChnConfig();
	void ResetAllChn();
	void StartAllCard();
	void StopAllCard();
	BOOL Write2File();
	BOOL SampleFinished() {return Finished;}
	OneChannel* GetChannel(int i) {
		return  &pChannels[i];
		}
	BOOL Update();
	void ShowSpectrum(int offset);
	void ShowCurve();
	void SaveSpectrum();
	void ClrSampleFinished() {
		Finished=FALSE;
		}
	void Show();
	BOOL DataReady;
	BOOL ReadyStart;
	};

class MP4010 {
	long BaseAddr;
	# define SetMode(mode) outp(BaseAddr+MODE_REG,(mode))
	# define GetMode() inp(BaseAddr+MODE_REG)
	# define RamRewind() inp(BaseAddr+STOP_REW)
	//long xdata[5000];
public:
    DWORD huge* sdata; //smooth data , added @2012-06-14
    long huge* xdata;
	long BaseAddr912;
	int CardID,CardType;
	BOOL Success,Finished;
	int nChannelSelected;
	BOOL SubBoard1Busy,SubBoard2Busy;

	long hDevice;
	long DAP912_OpenDevice(long iobase, long DeviceNum);//Open Device
	long DAP912_CloseDevice();				// CloseIfOpen
	long DAP912_ADCH(int ch);				//sel AD input

// start a Specture sam program
// tdata: sam timer to end, 1uS base timer
// endnum0, endnum1: ch0, ch1 end cnt number
	long DAP912_ADSSam(long adsp,long tdata, long endnum0, long endnum1);

//**************************************************************
//start a wave sam program
// sm 4k point
//chbank=0,1 sam 2ch Bank0 or bank1
	long DAP912_ADWSam(long adsp);

//****************************************************************
//* set Freq sam end counter number
//* number=1-15
//* return read back, low 8 is: number0 / data bit15-bi8 : number1
	long DAP912_SetEndNum(long endnum0, long endnum1);

//****************************************************************
//* read back state of AD
//* return =0 : ad sam end / =1 busya
//* *sammode: woring mode of user set
//* *adsp: 0-3 via ad speed 40/20/10/5 Mhz
	long DAP912_State(long *sammode, long *adsp,long *ramch);

//****************************************************************
//* poll if end
//* return =0 : ad sam end / =1 busya
//* rtadat=temp timer data
	long DAP912_ADPoll(long *rtdata);

//---------------------------------------------------------------------------
//RAM Function
//***************************************************************
//Filling data to RAM bankN
//this program will rewrite mode reg.
	long DAP912_RamWrite(long ramch, long wlen,long huge* wdata);

//****************************************************************
//read Ram data of 4K
//*rdata must > 4K long
	long DAP912_RamRead(long ramch);

//****************************************************************
//set timer data
//tdata:24bit data , time base=1uS
	long DAP912_SetTimer(long tdata);


//****************************************************************
//read back timer
	long DAP912_ReadTimer();

//****************************************************************
//* set working mode
//* adsp: 0-3 via speed 40/20/10/5Mhz
//* adch: set read/write ram channel =0/1
//* sammode: =0 sampling data / =1 Freq. sam
//****************************************************************
	long DAP912_Mode(long sammode,long adsp, long adch);


//**************************************************************
//start  AD operation
	long DAP912_Start();

//**************************************************************
//reset or stop AD operation
	long DAP912_Rew();

/////////////////////////////////////////////////////////////////////////////////
//DAP912 Self Function

//inb
//adr is offset of IO
	long DAP912_inb(long adr);

//sequetial read   , adr , adr+1 , adr+2
	long DAP912_insb(long adr, long huge* rdata,long readl);

//outb
	long  DAP912_outb(long adr,long data1);

//-----------------------------------------------------------------------------------
//outsb: write seq. data of number=writel  from buffer=*wdata
//length <1024
//ou 24bit data by byte
	long DAP912_outsb(long adr, long huge* wdata,long writel);
public:
	MP4010();
	~MP4010();
	void ConfigCard(int id,int type,long baseaddr) {
		CardID=id;CardType=type;
		BaseAddr=baseaddr;
		}
	BOOL TestAP910();
	BOOL TestAP912();
	int GetCardID() {return CardID;}
	void Select(int sub,long ch);
	void ClearRAM(long bank,int CardType);
	void Run() {
		if(Success) outp(BaseAddr+STARTREC,0x00);
		}
	void Stop() {
		if(Success) RamRewind();
		}
	void ReadRAM(int ch,DWORD huge* pData);
	void ReadAndAdd(long ramch, DWORD huge* data);
	BOOL TestRAM(long ramch,int CardType);
	//void TimeDelay(unsigned   long   microsec);
	void Smooth(DWORD huge* data); // added @2012-06-14
	};
