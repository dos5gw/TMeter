# include "TypeDef.h"
# include "ComPort.h"
# include "Crc32.h"
WORD PortAddr;
WORD PortFlag;
WORD BaudTable[]={0xc0,0x60,0x30,0x18,0xc,6,4,3,2,1};
BYTE ParityTable[]={0x00,0x08,0x18,0x28,0x38};
BYTE ReceiveBuff[BUFFLEN],SendBuff[BUFFLEN];
BYTE lsr=0x00,msr=0x00;
int ReceiveBuffInPtr=0,ReceiveBuffOutPtr=0;
int SendBuffInPtr=0,SendBuffOutPtr=0;
#ifdef __cplusplus
	#define __CPPARGS ...
#else
	#define __CPPARGS
#endif
void interrupt (*oldComVect)(__CPPARGS);
void PutToBuffer(BYTE ch) {
	int temp=ReceiveBuffInPtr;
	if(++ReceiveBuffInPtr==BUFFLEN) ReceiveBuffInPtr=0;
	if(ReceiveBuffInPtr!=ReceiveBuffOutPtr) ReceiveBuff[ReceiveBuffInPtr]=ch;
	else ReceiveBuffInPtr=temp;
	}
void ClearReceiveBuff() {
	ReceiveBuffInPtr=0,ReceiveBuffOutPtr=0;
	}
int GetBufferLen() {
	if(ReceiveBuffInPtr>=ReceiveBuffOutPtr) {
		return ReceiveBuffInPtr-ReceiveBuffOutPtr;
		}
	else {
		return BUFFLEN-(ReceiveBuffInPtr-ReceiveBuffOutPtr);
		}
	}
void AddToSendBuffer(BYTE ch) {
	int temp=SendBuffInPtr;
	if(++SendBuffInPtr==BUFFLEN) SendBuffInPtr=0;
	if(SendBuffInPtr!=SendBuffOutPtr) SendBuff[SendBuffInPtr]=ch;
	else SendBuffInPtr=temp;
	}


void ResetSendBuffer() {
	SendBuffInPtr=0;
	SendBuffOutPtr=0;
	}

void ResetCommandBuffer() {
	ReceiveBuffInPtr=0;
	ReceiveBuffOutPtr=0;
	}

void Add2ToSendBuffer(WORD word,DWORD& crc) {
	BYTE hbyte=word>>8,lbyte=word&0xff;
	AddToSendBuffer(hbyte);UpdateCRC32(hbyte,crc);
	AddToSendBuffer(lbyte);UpdateCRC32(lbyte,crc);
	}

void Add4ToSendBuffer(DWORD dword,DWORD& crc) {
	WORD h16=dword>>16,l16=dword&0xffff;
	Add2ToSendBuffer(h16,crc);Add2ToSendBuffer(l16,crc);
	}

void LAdd4ToSendBuffer(DWORD dword,DWORD& crc){
	WORD h16=dword>>16,l16=dword&0xffff;
	LAdd2ToSendBuffer(l16,crc);LAdd2ToSendBuffer(h16,crc);
}
void LAdd2ToSendBuffer(WORD word,DWORD& crc) {
	BYTE hbyte=word>>8,lbyte=word&0xff;
	AddToSendBuffer(lbyte);UpdateCRC32(lbyte,crc);
	AddToSendBuffer(hbyte);UpdateCRC32(hbyte,crc);
	}

BYTE ReadComBuffer() {
	if(ReceiveBuffInPtr!=ReceiveBuffOutPtr) {
		if(++ReceiveBuffOutPtr==BUFFLEN) ReceiveBuffOutPtr=0;
		return ReceiveBuff[ReceiveBuffOutPtr];
		}
	return 0x00;
	}

BOOL CheckComBuffer() {
	if(ReceiveBuffInPtr!=ReceiveBuffOutPtr) return TRUE;
	return FALSE;
	}

BOOL ReadSendBuffer(BYTE& ch) {
	if(SendBuffInPtr!=SendBuffOutPtr) {
		if(++SendBuffOutPtr==BUFFLEN) SendBuffOutPtr=0;
		ch=SendBuff[SendBuffOutPtr];
		return TRUE;
		}
	return FALSE;
	}

void interrupt comVect(__CPPARGS) {
	BYTE data=inportb(PortAddr+RXD);
	PutToBuffer(data);
	outportb(ICREG,EOI);
	}


void ComSender(BYTE ch) {
	while(!(inportb(PortAddr+LSR)&EMPTY));
	outportb(PortAddr+TXD,ch);
	}

BOOL Initialize(BaudIdx bIdx,int datab,int stopb,ParityIdx pIdx) {
	if(bIdx>9||pIdx>4||datab<5||datab>8||stopb<1||stopb>2) return FALSE;
	disable();outportb(PortAddr+LCR,0x80);
	outportb(PortAddr+LSB,BaudTable[bIdx]&0xff);
	outportb(PortAddr+MSB,BaudTable[bIdx]>>8);
	outportb(PortAddr+LCR,ParityTable[pIdx]|(datab-5)|((stopb-1)<<1));
	enable(); return TRUE;
	}

void OpenComPort() {
	oldComVect=getvect(PortFlag+8);
	disable();
	inportb(PortAddr+RXD);inportb(PortAddr+MSR);
	inportb(PortAddr+LSR);inportb(PortAddr+IIR);
	outportb(PortAddr+IER,IERV);
	outportb(PortAddr+MCR,OUT2|ERTS|EDTR);
	outportb(IMASKREG,inportb(IMASKREG)&(~(1<<PortFlag)));
	setvect(PortFlag+8,comVect);
	enable();
	}

void CloseComPort() {
	disable();
	outportb(PortAddr+IER,0);
	outportb(PortAddr+MCR,0);
	outportb(IMASKREG,inportb(IMASKREG)|(1<<PortFlag));
	enable();
	setvect(PortFlag+8,oldComVect);
	}

BOOL GetComportAddr(int port) {
	if(port>3) return FALSE;
	PortAddr=peek(0x40,port*2);
	if(PortAddr==0x00) return FALSE;
	PortFlag=(port==0)?4:3;
	return TRUE;
	}


BOOL StartComService(int port,BaudIdx bIdx,int datab,int stopb,ParityIdx pIdx) {
	if(!GetComportAddr(port)) return FALSE;
	if(!Initialize(bIdx,datab,stopb,pIdx)) return FALSE;
	OpenComPort();
	return TRUE;
	}

void StopComService() {
	CloseComPort();
	}

void UpdateCommport() {
	BYTE ch;
	lsr=inportb(PortAddr+LSR);
	msr=inportb(PortAddr+MSR);
	if(((lsr&EMPTY)==EMPTY)&&((msr&READY)==READY)) {
	//if((lsr&EMPTY)==EMPTY) {
		while(ReadSendBuffer(ch)) {
			ComSender(ch);
			}
		}
	}