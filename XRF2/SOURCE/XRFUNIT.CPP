//ext
# include <dos.h>
# include <bios.h>
# include <stdio.h>
# include <math.h>
# include "XRFUNIT.H"
# include "ComPort.h"
# include "MP4010.h"
# include "Crc32.h"

unsigned long timerCountHigh=0;
unsigned long timerCountLow=0;
extern BYTE CurrDioPort[];
extern BYTE lsr,msr;
BOOL Running=FALSE;
BOOL pause=FALSE;
BOOL SamplingStarted=FALSE;
unsigned long timerCount=0,startCount=0;

#ifdef __cplusplus
#define __CPPARGS ...
#else
#define __CPPARGS
#endif

void interrupt (*old1cvect)(__CPPARGS);

ChnConfig *pCh=NULL;

void interrupt new1cv(__CPPARGS) {
	if(Running) {
		timerCount++;
		if(timerCount==182) {
			pCh->StopAllCard();
			pCh->DataReady=TRUE;
			Running=FALSE;
		}
	}
	if(pCh->ReadyStart) {
		timerCount=0;
		pCh->StartAllCard();
		pCh->ReadyStart=FALSE;
		Running=TRUE;
		pCh->DataReady=FALSE;
	}
	timerCountLow++;if(timerCountLow==0) timerCountHigh++;
}

double getTimerTicks() {
	return 4294967296.0l*timerCountHigh+timerCountLow;
}

//ext
XInstrument::XInstrument() {
	InitAnalog();
	m_pXRayTube = new XRayTube(this);   //1
	m_pSampleLid = new XSampleLid(this);//2
	m_pShamber = new XShamber(this);    //3
	m_pBeamStop = new XBeamStop(this);  //4
	m_pLight = new XLight();
	m_pKey = new XKey();
	pCh=new ChnConfig();

	if(!m_pXRayTube || !m_pSampleLid || !m_pShamber || !m_pBeamStop || !m_pLight || !m_pKey){
		printf("init instrument error \n");
		getch();
	}
	m_status=SwitchToIdle;
	//m_status=SwitchToStandBy;
	LoadCrc32Table("Crc32.tab");
	old1cvect=getvect(0x1c);
	setvect(0x1c,new1cv);
	ReceivingBxConfig=FALSE;
	FaultEncountered=FALSE;
	//ReceivingTiConfig=FALSE;
	//ReceivingTubeConfig = FALSE;
}
void XInstrument::ScreenLayOut() {
	textbackground(8);clrscr();
	gotoxy(1,3);cprintf ("[Sample lid]");
	gotoxy(27,3);cprintf("[Key]");
	gotoxy(55,3);cprintf("[Beam Stop]");
	gotoxy(1,7);cprintf("[X-Ray Tube System]");
	gotoxy(55,7);cprintf("[Light System]");
	//gotoxy(55,7);cprintf("[Flow Gas System]");
	gotoxy(1,14);cprintf("[Temperature System]");
	gotoxy(27,14);cprintf("[BX System Monitor]");
	gotoxy(55,14);cprintf("[Communication Status]");
	gotoxy(1,19);cprintf("[Paluse Analysis System]");
	pCh->Show();
	gotoxy(1,25);textbackground(1);textcolor(6);
	cprintf("F1-Idle F2-Measure F3-Sampling F4-Graph F5-Upload");
	_setcursortype(_NOCURSOR);
}
void XInstrument::ShowComStatus() {
	gotoxy(55,15);textattr(0x05);cprintf("LSR=%2X",lsr);
	gotoxy(55,16);textattr(0x06);cprintf("MSR=%2X",msr);
	gotoxy(55,17);
	if((msr&DSR)==0) {
		textattr(0x84);
		ResetSendBuffer();
		ResetCommandBuffer();
		//RenewError();
		cprintf("Disconnected!");
	}
	else {
		textattr(0x02);
		cprintf("Connected!   ");
	}
}

void XInstrument::Show() {
	gotoxy(28,15);
	switch(m_status) {
	case Sampling:  //0
		textattr(0x02);cprintf("Sampling...         ");
		break;
	case TubeReady: //1
	    textattr(0x02);cprintf("XRayTube Ready      ");
		break;		
	case Idle:      //3
		textattr(0x01);cprintf("Idle                ");
		break;
	case SwitchToSampling: //4
		textattr(0x02);cprintf("Switching To Sample ");
		break;
	case SwitchToTubeReady://5
		textattr(0x82);cprintf("Switching To Ready  ");
		break;					
	case SwitchToIdle:     //7
		textattr(0x81);cprintf("Switching To Idle   ");
		break;	
}
	
	gotoxy(28,17);textattr(0x04);
	cprintf("PA=%2X PB=%2X PC=%2X",CurrDioPort[0],CurrDioPort[1],CurrDioPort[2]);
	textattr(0x07);
}

XInstrument::~XInstrument() {
	setvect(0x1c,old1cvect);
	AboutQuitAnalog();
	delete pCh;
}

void XInstrument::Run() {
	BOOL exit=FALSE;BYTE comCh;int i,key=0;
	ScreenLayOut();
	BOOL hasKey=FALSE,hasComCmd=FALSE;
	PrepairAdConv();
	StartComService(0,B28800,8,1,NONE);
	while(!exit)
	{
		while(!(hasKey=(BOOL)bioskey(1))&&!(hasComCmd=CheckComBuffer()))
		{
			Update();Show();
		}
		if(hasKey)
		{
			key=bioskey(0);
			switch(key)
			{
			case 0x11b:
				exit = TRUE;
				break;
			case 0x3b00: //F1
				m_status=SwitchToIdle;
				break;
			case 0x3c00: //F2
				//m_status=SwitchToStandBy;
				break;
			case 0x3d00: //F3
				m_status=SwitchToSampling;
				break;
			case 0x3e00: //F4
				if(!Running){
				    pCh->ShowCurve();
				    ScreenLayOut();
				}
				break;
			case 0x3f00:
				break;
			    /*
				DWORD crc;BYTE ret;
				OneChannel* p1Ch=NULL;
				p1Ch=pCh->GetChnAndMoveToNext();
				if(p1Ch==NULL)	{
					AddToSendBuffer(0xff);
					gotoxy(50,1);textattr(0x17);cprintf("Null      ",comCh);
				}
				crc=0xffffffff;
				ret=0x50;AddToSendBuffer(ret);UpdateCRC32(ret,crc);
				ret=p1Ch->Finished;AddToSendBuffer(ret);UpdateCRC32(ret,crc);
				Add2ToSendBuffer(p1Ch->CurrTimes,crc);
				DWORD chData;
				for(i=0;i<1024;i++)
				{
					chData=p1Ch->data[i];
					//chData=1024 - abs(i-512);//DEBUG
					Add4ToSendBuffer(chData,crc);
				}
				crc^=0xffffffff;
				AddToSendBuffer(crc>>24);
				AddToSendBuffer(crc>>16);
				AddToSendBuffer(crc>>8);
				AddToSendBuffer(crc);
				UpdateCommport();
				break;*/
			 /*
			 case 0x1910:
				Running=FALSE;
				pCh->ReadyStart=FALSE;
				pause=TRUE;
				gotoxy(70,1);textattr(0x16);cprintf(" Paused... ");
				break;
			case 0x1414:
				Running=FALSE;
				pCh->ReadyStart=FALSE;
				gotoxy(70,1);textattr(0x4e);cprintf("  Stoped!  ");
				break; */
			}
		}
		else if(hasComCmd)
		{
			DWORD crc,crcClip;BYTE ret;int len,curCh=0;
			if(ReceivingBxConfig)
			{
			    len=GetBufferLen();
			    if(len>=38)  // @2012-06-05
			    {
			        FloatByte floatByte;
			        ShortByte shortByte;
			        //set env param
					for(i=0;i<4;i++) {
						ret=floatByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					m_pXRayTube->m_WorkingHV=floatByte.real;
					//m_pXRayTube->SetHV(floatByte.real); //DEBUG


					for(i=0;i<4;i++) {
						ret=floatByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					m_pXRayTube->m_WorkingMA=floatByte.real;
					//m_pXRayTube->SetMA(floatByte.real); //DEBUG

					for(i=0;i<4;i++) {
						ret=floatByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					m_pXRayTube->m_H2500WorkingHV=floatByte.real;
					m_pXRayTube->bH2500VolDaout=TRUE;
					//m_pXRayTube->SetH2500HV(floatByte.real); //DEBUG

					ret=ReadComBuffer();
					//m_pLight->SetPanel1((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetPanel2((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetPanel3((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetPanel4((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetPanel5((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetLamp1((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetLamp2((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetLamp3((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer();
					//m_pLight->SetLamp4((BOOL)ret);
					UpdateCRC32(ret,crcBxConfig);

					ret=ReadComBuffer(); UpdateCRC32(ret,crcBxConfig);
					if(!ret){
						//if(m_pShamber->CanClose()) m_pShamber->Close(); //close fan
					}
					else{
						//if(m_pShamber->CanOpen()) m_pShamber->Open();   //open fan
					}
					
					ret=ReadComBuffer(); UpdateCRC32(ret,crcBxConfig);
					if(ret) {
					    //m_status=SwitchToTubeReady; //m_pXRayTube->WarmUpXTube();
					}
					else {
					    //m_status=SwitchToIdle; //m_pXRayTube->CoolDnXTube();
					}

					ret=ReadComBuffer(); UpdateCRC32(ret,crcBxConfig); 
					if(!ret){
						//if(m_pBeamStop->CanClose())
						//m_pBeamStop->Close();
					}
					else{
						//if(m_pBeamStop->CanOpen())
						//m_pBeamStop->Open();

					}

					for(i=0;i<2;i++) {
						ret=shortByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					pCh->SetChannelTimer(0,shortByte.real);
					
					for(i=0;i<4;i++) {
						ret=floatByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					m_pShamber->m_ULimitTemp=floatByte.real;
					
					for(i=0;i<4;i++) {
						ret=floatByte.byte[i]=ReadComBuffer();
						UpdateCRC32(ret,crcBxConfig);
					}
					m_pShamber->m_DLimitTemp=floatByte.real;

					crcBxConfig^=0xffffffff; 
					crcClip=ReadComBuffer();
					crc=crcClip<<24;
					crcClip=ReadComBuffer();
					crc+=crcClip<<16;
					crcClip=ReadComBuffer();
					crc+=crcClip<<8;
					crcClip=ReadComBuffer();
					crc+=crcClip;  

					if(crcBxConfig==crc)
					{
						AddToSendBuffer('*');
						//gotoxy(1,1);textattr(0x17);cprintf("0x%x,len=%2d,crc ok  ,",comCh,len+1); //debug
					}
					else
					{
						AddToSendBuffer(0xff);
						gotoxy(1,1);textattr(0x17);cprintf("0x78,len=%2d,crc err ,",len+1); //debug
					}
					UpdateCommport();
					ReceivingBxConfig=FALSE;
				}
			    else //receive time out
			    {
					if(getTimerTicks()-ReceivingBxConfigStart>4)
			        {
						gotoxy(1,1);textattr(0x17);cprintf("0x%x,len=%2d,Time out,",comCh,len+1); //debug
			            ClearReceiveBuff();
						AddToSendBuffer(0XFF);
						UpdateCommport();
						ReceivingBxConfig=FALSE;
			        }
			    }
			}
			else
			{	    
				DWORD crc,crcClip;BYTE ret;int len;
				FloatByte floatByte;	
				comCh=ReadComBuffer();
				
				switch(comCh) {
				case 'x': // 0x78, receive configuration
					ReceivingBxConfig=TRUE;
				    ReceivingBxConfigStart=getTimerTicks();
					crcBxConfig=0xffffffff;
					UpdateCRC32('x',crcBxConfig);
					break;
				case 'E': // 0x45, 
					AddToSendBuffer('*');
					UpdateCommport();
					m_status=SwitchToTubeReady;
				    break;
				case 'e': // 0x65, 
					AddToSendBuffer('*');
					UpdateCommport();
					m_status=SwitchToIdle;//SwitchToStandBy;
				    break;
				case 'L':
				    //open beam-shut
				    AddToSendBuffer('*');
					UpdateCommport();
				    m_pBeamStop->Open();
				    break;
				case 'l':
				    //close beam-shut
				    AddToSendBuffer('*');
					UpdateCommport();
				    m_pBeamStop->Close();
				    break;
				case 'B':
				    AddToSendBuffer('*');
					UpdateCommport();
				    m_pLight->SetLamp1(TRUE);
				    m_pLight->SetLamp2(TRUE);
				    m_pLight->SetLamp3(TRUE);
				    m_pLight->SetLamp4(TRUE);
				    break;
				case 'b':
				    AddToSendBuffer('*');
					UpdateCommport();
				    m_pLight->SetLamp1(FALSE);
				    m_pLight->SetLamp2(FALSE);
				    m_pLight->SetLamp3(FALSE);
				    m_pLight->SetLamp4(FALSE);
				    break;
				case 'S': // 0x53, start sampling
					AddToSendBuffer('*');
					UpdateCommport();
					m_status=SwitchToSampling;
					break;
				case 'P': // 0x50, upload sample data, 1034 Bytes, @2012-06-05
					OneChannel* p1Ch=NULL;
					p1Ch=pCh->GetChnAndMoveToNext();
					if(p1Ch==NULL)	{
						AddToSendBuffer(0xff);
						gotoxy(50,1);textattr(0x17);cprintf("Null      ",comCh);
					}
					else{
						crc=0xffffffff;
						ret=0x50;AddToSendBuffer(ret);UpdateCRC32(ret,crc);
						ret=p1Ch->Finished;AddToSendBuffer(ret);UpdateCRC32(ret,crc);
						Add2ToSendBuffer(p1Ch->CurrTimes,crc);
						Add2ToSendBuffer(p1Ch->SetRepeatTimes,crc); //added by Chen @2012-06-04
						DWORD chData;
						//for(i=0;i<1024;i++) //debug
						//{
							//chData=p1Ch->data[i];
							//chData^=0xaaaaaaaa;
							//p1Ch->data[i]=chData=512 - abs(i-512);//DEBUG
							//Add4ToSendBuffer(chData,crc);
						//}
						
						for(i=0;i<256;i++)
						{
						    chData=p1Ch->data[i*4]+
						        p1Ch->data[i*4+1]+
						        p1Ch->data[i*4+2]+
						        p1Ch->data[i*4+3];
						    //chData = chData/4;
						    Add4ToSendBuffer(chData,crc);
						}
						crc^=0xffffffff;
						AddToSendBuffer(crc>>24);
						AddToSendBuffer(crc>>16);
						AddToSendBuffer(crc>>8);
						AddToSendBuffer(crc);
					}
					UpdateCommport();    
					break;			
				case 'X': // 0x58,upload Environment, 52 Bytes, @2012-06-05
					crc=0xffffffff;
					ret=0x58; AddToSendBuffer(ret); UpdateCRC32(ret,crc);
					ret=m_status; AddToSendBuffer(ret); UpdateCRC32(ret,crc);
					
					//m_pXRayTube->ReadHV();    //(1)KV
					floatByte.real=m_pXRayTube->m_CurrHV; 
					//for(i=3;i>=0;i--){
					for(i=0;i<4;i++){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadMA();
					floatByte.real=m_pXRayTube->m_CurrMA; //(2)MA
					for(i=0;i<4;i++){
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadStdHV();
					floatByte.real=m_pXRayTube->m_StdCurrHV; //(3)Std KV
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadH2500HV();
					floatByte.real=m_pXRayTube->m_H2500CurrentHV; //(4)H2500 Vol
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadPower24Vol();
					floatByte.real=m_pXRayTube->Power24Vol; //(5)power 24v
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadPower12Vol();
					floatByte.real=m_pXRayTube->Power12Vol; //(6)power 12v
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pXRayTube->ReadPower5Vol();
					floatByte.real=m_pXRayTube->Power5Vol; //(7)power 5v
					//for(i=0;i<4;i++) {
					for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pShamber->ReadAngle();
					floatByte.real=m_pShamber->CurrAngle; //(8)angle
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					//m_pShamber->ReadTemp();
					floatByte.real=m_pShamber->m_CurrTemp1; //(9) temperature 1
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					floatByte.real=m_pShamber->m_CurrTemp2; //(10) temperature 2
					for(i=0;i<4;i++) {
					//for(i=3;i>=0;i--){
						ret=floatByte.byte[i];
						AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					}

					m_pKey->ReadAllKeyStatus();
					ret=m_pKey->Key1OnStatus;
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					ret=m_pKey->Key2OnStatus;
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);

					m_pBeamStop->ReadBeamStopPos();
					ret=m_pBeamStop->OpenPos; //shut open position 
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					
					ret=m_pBeamStop->ClosePos; //shut close position 
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);
						
                    m_pSampleLid->ReadSampleLidPos();
					ret=m_pSampleLid->Lid1OpenedStatus; //lid1 status
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);

					ret=m_pSampleLid->Lid2OpenedStatus; //lid2 status
					AddToSendBuffer(ret);UpdateCRC32(ret,crc);
					crc^=0xffffffff;
					AddToSendBuffer(crc>>24);
					AddToSendBuffer(crc>>16);
					AddToSendBuffer(crc>>8);
					AddToSendBuffer(crc);
					UpdateCommport();
					break;
				default:
					//len=GetBufferLen();
					//gotoxy(1,1);textattr(0x17);printf("unknow=%x,len=%d,",comCh,len);
					break;
				}//end switch
				//len = GetBufferLen();
				//gotoxy(1,1);textattr(0x17);printf("0x%x,len=%2d,         ",comCh,len+1);

		    }
		}
	}
	StopComService();
	_setcursortype(_NORMALCURSOR);
}

BOOL XInstrument::CanStartSample() {
	//if(m_pSampleLid->Lid1OpenedStatus || m_pSampleLid->Lid2OpenedStatus) return FALSE;
	//if(m_pBeamStop->ShutOpenedStatus) return FALSE;
    if(m_pXRayTube->m_status!=XRayTube::Ready) return FALSE;
	if(SamplingStarted) return FALSE;
	return TRUE;
	
}

void XInstrument::StartSample() {
	Running=FALSE;
	pCh->ResetAllChn();
	pCh->ReadyStart=(BOOL)!pCh->CollectFreeChs();
	SamplingStarted=TRUE;
	m_status=Sampling;
	gotoxy(70,1);textattr(0x17);cprintf("Sampling...");
}

BOOL XInstrument::CheckInstrumentStatus(Status target) {
	switch(target){
	case TubeReady:
	    //if(m_pSampleLid->Lid1OpenedStatus || m_pSampleLid->Lid2OpenedStatus) return FALSE;
	    //if(m_pBeamStop->ShutOpenedStatus) return FALSE;
		if(m_pXRayTube->m_status!=XRayTube::Ready) return FALSE;
		if(SamplingStarted) return FALSE;
	    break;
	case StandBy:
		//if(m_pSampleLid->Lid1OpenedStatus || m_pSampleLid->Lid2OpenedStatus) return FALSE;
		//if(m_pBeamStop->ShutOpenedStatus) return FALSE;
		if(m_pXRayTube->m_status!=XRayTube::StandBy) return FALSE;
		if(SamplingStarted) return FALSE;
		break;
	case Idle:
	    //if(m_pSampleLid->Lid1OpenedStatus || m_pSampleLid->Lid2OpenedStatus) return FALSE;
	    //if(m_pBeamStop->ShutOpenedStatus) return FALSE;
		if(m_pXRayTube->m_status!=XRayTube::Closed) return FALSE;
		if(SamplingStarted) return FALSE;
        break;
	}
	return TRUE;
}
void XInstrument::Update() {
    m_pSampleLid->Update();
    m_pSampleLid->Show();
    m_pBeamStop->Update();
    m_pBeamStop->Show();
    m_pXRayTube->Update();
    m_pXRayTube->Show();
    m_pShamber->Update();
    m_pShamber->Show();
    m_pLight->Show();
    m_pKey->Show();
    pCh->Show();
    UpdateCommport();
    ShowComStatus();
	switch(m_status) {
	case Idle:
	case StandBy:
	case TubeReady:
	case Sampling:
		pCh->Update();
		if(SamplingStarted&&pCh->SampleFinished()) {
			gotoxy(70,1);textattr(0x1f);cprintf("Finished!  ");
			Running=FALSE;
			pause=FALSE;
			SamplingStarted=FALSE;
			// m_status = SwitchToTubeReady; //added by chen @ 05-23
		}
		break;
	case SwitchToSampling: 
		if(m_pXRayTube->m_status!=XRayTube::Ready) m_pXRayTube->WarmUpXTube();
		if(m_pBeamStop->ShutOpenedStatus!=TRUE) m_pBeamStop->Open();
		//if(CanStartSample())
		if(TRUE)
			StartSample();
		break;
	case SwitchToTubeReady: // receive E command
		if(SamplingStarted) {
			Running=FALSE;
			pCh->ReadyStart=FALSE;
			gotoxy(70,1);textattr(0x4e);cprintf("  Stoped!  ");
			SamplingStarted=FALSE;
		}
		if(m_pBeamStop->ShutOpenedStatus!=FALSE) m_pBeamStop->Close(); //@ 2012-06-11
		
	    if(m_pXRayTube->m_status==XRayTube::Closed) {
	        m_pXRayTube->Open();
			m_pXRayTube->m_status=XRayTube::PreOpen;
			m_pXRayTube->actionStart=getTimerTicks();
	    }
		//else if(m_pXRayTube->m_status!=XRayTube::Ready) m_pXRayTube->WarmUpXTube();
		else m_pXRayTube->WarmUpXTube();
		m_pXRayTube->CheckReady(); //newly added @ 2012.4.15
		if(CheckInstrumentStatus(TubeReady)) m_status=TubeReady;
		break; 
	case SwitchToStandBy: //receive e command
		if(SamplingStarted) {
			Running=FALSE;
			pCh->ReadyStart=FALSE;
			gotoxy(70,1);textattr(0x4e);cprintf("  Stoped!  ");
			SamplingStarted=FALSE;
		}
		if(m_pBeamStop->ShutOpenedStatus!=FALSE) m_pBeamStop->Close();//@ 2012-06-11
		if(m_pXRayTube->m_status==XRayTube::Closed) {      
			if(m_pXRayTube->CanOpen()) {
			    m_pXRayTube->Open();
			    m_pXRayTube->actionStart=getTimerTicks();
			    m_pXRayTube->m_status=XRayTube::PreOpen;
			}
		}
		else if(m_pXRayTube->m_status!=XRayTube::StandBy) {
		    m_pXRayTube->CoolDnXTube();
		}
		if(CheckInstrumentStatus(StandBy)) m_status=StandBy;
		break;
	case SwitchToIdle:  //receive e command
		if(SamplingStarted) {
			Running=FALSE;
			pCh->ReadyStart=FALSE;
			gotoxy(70,1);textattr(0x4e);cprintf("  Stoped!  ");
			SamplingStarted=FALSE;
		}
		//if(m_pBeamStop->CanClose()) m_pBeamStop->Close();
		if(m_pBeamStop->ShutOpenedStatus!=FALSE) m_pBeamStop->Close(); //@ 2012-06-11
		//if(m_pXRayTube->m_status == XRayTube::Closed) do nothing		
		if((m_pXRayTube->m_status == XRayTube::PreOpen) || (m_pXRayTube->m_status == XRayTube::Opening) || (m_pXRayTube->m_status == XRayTube::StandBy)){
		    m_pXRayTube->Close();
		}
		else if(m_pXRayTube->m_status != XRayTube::Closed){ // status=Ready or WarmingUp or CoolingDn or Sleep
		    m_pXRayTube->CoolDnXTube();
		}
		if(CheckInstrumentStatus(Idle)) m_status=Idle;
		break;
	} 
}
BOOL XRayTube::CheckReady(){
	if((abs(m_WorkingHV-m_HvTarget)<0.1) && (abs(m_WorkingMA-m_MaTarget)<1.0)){
		m_status = Ready;
		return TRUE;
	}
	return FALSE;
}
BOOL XRayTube::CanSafetyClose() {
	switch(m_status) {
		case Closed:
			return FALSE;
		case StandBy:
		case PreOpen:
		case Opening:
			return TRUE; //m_pInstrument->CanCloseXRayTube();
		//case CoolingDn:
		case WarmingUp:
		case Sleep:
		case Ready:
		case SlowDown:
			return FALSE;
		default:
			return TRUE;
		}
	}

BOOL XInstrument::CanOpenXRayTube() {
	BOOL can=TRUE;
	//if(m_pXRayTube->m_status!=XRayTube::Closed) can=FALSE;
	//if(m_pSampleLid->Lid1OpenedStatus) can=FALSE;
	//if(m_pSampleLid->Lid2OpenedStatus) can=FALSE;
	//if(m_pBeamStop->ShutOpenedStatus) can=FALSE;
	return can;
}
BOOL XInstrument::CanOpenBeamStop() {
	BOOL can=TRUE;
	if(m_pSampleLid->Lid1OpenedStatus) can=FALSE;
    if(m_pSampleLid->Lid2OpenedStatus) can=FALSE;
	return can;
}

BOOL XRayTube::Initialize(){
    return TRUE;
}
BOOL XRayTube::CanOpen(){
    //if(m_status==Closed) return m_pInstrument->CanOpenXRayTube();
    return TRUE;
}
BOOL XRayTube::CanClose(){
    //if(m_status!=Closed) return TRUE;
	//else return FALSE;
	return TRUE;
}
void XRayTube::Open(){
    //m_HvTarget=0;m_MaTarget=0;
	//SetHV(-1.0);SetMA(-1.0);
	SetDioBit(DrvPort,DrvBit,TRUE);
	actionStart=getTimerTicks();
	//m_status=PreOpen;
}
void XRayTube::Close(){
	SetMA(0);SetHV(0);
	m_MaTarget = m_HvTarget = 0.0;
	SetDioBit(DrvPort,DrvBit,FALSE);
	m_status=Closed;    
}
void XRayTube::Update(){
	double actionLast;
    AnalogUpdate();   
    ReadHV(); ReadMA();
    ReadPower24Vol(); ReadPower5Vol(); ReadPower12Vol();
    ReadH2500HV();
    ReadStdHV();
    if(bH2500VolDaout)
    {
        SetH2500HV(m_H2500WorkingHV);
    }
    switch(m_status){
    case Closed:
        break;
    case PreOpen:
        actionLast=0.055*(getTimerTicks()-actionStart);
		if(actionLast>3.0) {
			m_HvTarget=m_SafeHV;m_MaTarget=m_SafeMA;
			SetHV(m_HvTarget); SetMA(m_MaTarget);
			actionStart=getTimerTicks();
			m_status=Opening;
		}
        break;
    case Opening:
        if((m_HvTarget-m_CurrHV<1.0) && (m_MaTarget-m_CurrMA<10)) 
            m_status=StandBy; 
        /*
        else {
		    actionLast=0.055*(getTimerTicks()-actionStart);
		    if(actionLast>5.0) {
			    gotoxy(70,1);textattr(0x1f);cprintf("TubeError#1");
			    Close();
			}
		}*/
        break;
    case StandBy:
        break;
    case Ready:
        break;
    case Sleep:
        actionLast=0.055*(getTimerTicks()-actionStart);
		if(actionLast>m_delaySeconds){ 
			//if((m_HvTarget==m_WorkingHV)&&(m_MaTarget==m_WorkingMA)) {
			if((fabs(m_WorkingHV-m_HvTarget)<0.1) &&
			    (fabs(m_WorkingMA-m_MaTarget)<1.0)){
				//m_HvAdjustStart=getTimerTicks();
				//m_MaAdjustStart=getTimerTicks();
				m_status=Ready; //reach 50kv, 100ma !
			}
			else{
				m_HvTarget+=m_HvStep;
				if(m_HvTarget>m_WorkingHV) m_HvTarget=m_WorkingHV;
				m_MaTarget+=m_MaStep;
				if(m_MaTarget>m_WorkingMA) m_MaTarget=m_WorkingMA;
				SetHV(m_HvTarget); SetMA(m_MaTarget);
				actionStart=getTimerTicks();
				m_status=WarmingUp;
			}
		}
        break;
    case WarmingUp: //WarmUpXTube() will change XRayTube::m_status to 'WarmingUp'
        actionStart=getTimerTicks(); //HVDEBUG, delete this row in release version
        m_status=Sleep; //HVDEBUG, delete this row in release version

        /* 
		if((m_HvTarget-m_CurrHV<2.0) && (m_MaTarget-m_CurrMA<10.0)) {
			actionStart=getTimerTicks();
			m_status=Sleep;
		}
		else {
			actionLast=0.055*(getTimerTicks()-actionStart);
			if(actionLast>5) {
				gotoxy(70,1);textattr(0x1f);cprintf("TubeError#2");
				CoolDnXTube();
			}
  		} */
		break;
    }
}
void XRayTube::WarmUpXTube() // switchingToTubeReady, switchingToSampling
{
    switch(m_status) {
		case Closed:
			if(CanOpen()){ // change m_status to 'PreOpen', then change to 'Opening', 'StandBy' in function ::Update()
			    Open();
			    m_status = PreOpen;
			} 
			break;
		case StandBy:
		case Ready:
		case CoolingDnHV:
		case CoolingDnMA:
		//case WarmingUp:
			CalculateWarmUpSteps(); // 1.0kv, 10ma
			m_HvTarget += m_HvStep;
			if(m_HvTarget>m_WorkingHV) m_HvTarget=m_WorkingHV;
			m_MaTarget += m_MaStep;
			if(m_MaTarget>m_WorkingMA) m_MaTarget=m_WorkingMA;
			SetHV(m_HvTarget); SetMA(m_MaTarget);
			actionStart=getTimerTicks();
			m_status=WarmingUp;
			break;
	}
}
void XRayTube::CoolDnXTube() { //run this func whenstatus != PreOpen, Opening, StandBy, Closed
	double actionLast;
	switch(m_status) {
		case Ready:
		case WarmingUp:
		case Sleep:
			SetMA(0.0);
			m_MaTarget = 0.0;
		    m_status=CoolingDnMA;
		    actionStart=getTimerTicks();
			break;
		case CoolingDnMA:
		    actionLast=0.055*(getTimerTicks()-actionStart);
			if(actionLast>3){
				SetHV(0.0);
				m_HvTarget = 0.0;
				m_status=CoolingDnHV;
				actionStart=getTimerTicks();
			}
			break;
		case CoolingDnHV:
			actionLast=0.055*(getTimerTicks()-actionStart);
			if(actionLast>3){
		        SetDioBit(DrvPort,DrvBit,FALSE);
		        m_status=Closed;   
		    }
		    break;
		}
	}
void XRayTube::CalculateWarmUpSteps() {
	m_HvStep=1.0;
	m_MaStep=20.0;
}
void XRayTube::CalculateCoolDnSteps() {
	m_HvStep=1.0;
	m_MaStep=20.0;
}
void XRayTube::Show(){
    gotoxy(2,8);
	switch(m_status) {
	    case Closed: //0
			textattr(0x02);cprintf("Closed ");
			break;
		case PreOpen://1
		    textattr(0x02);cprintf("PreOpen");
			break;
		case Opening://2
		    textattr(0x02);cprintf("Opening");
			break;
		case StandBy://3
		    textattr(0x02);cprintf("StandBy");
			break;
		case Ready:  //4
			textattr(0x02);cprintf("Ready  ");
			break;
		case Sleep:
	    case WarmingUp:
	        textattr(0x02);cprintf("Warming");
	        break;
	    case CoolingDnHV:
	    case CoolingDnMA:
	        textattr(0x02);cprintf("Cooling");
	        break;
	    case SlowDown:
	        textattr(0x02);cprintf("SlowDn ");
	        break;
		default:
			textattr(0x02);cprintf("Unkonwn");
	}
	textattr(0x02);
	gotoxy(2,9); cprintf("HV =%5.2lfkV /%5.2lfkV", m_CurrHV,m_WorkingHV);
	gotoxy(2,10);cprintf("MA =%5.2lfmA /%5.2lfmA", m_CurrMA,m_WorkingMA);
	gotoxy(2,11);cprintf("H2500=%5.2lfV /%5.2lfV",m_H2500CurrentHV,m_H2500WorkingHV);

	//DEBUG
	gotoxy(2,12);textattr(0x06);
	cprintf("DA: %3.1f, %3.1f, %3.1f",m_HvCode,m_MaCode,m_H2500Code);
	gotoxy(2,13);textattr(0x06);
	cprintf("AD: HV=%3.1f, HI=%3.1f, STD=%3.1f, HV2500=%3.1f",HvReadCode,MaReadCode,StdReadCode,H2500ReadCode);

	gotoxy(26,8);textattr(0x06);
	cprintf("AD: V24=%3.1f, V12=%3.1f, V5=%3.1f",V24ReadCode,V12ReadCode,V5ReadCode);
}

void XRayTube::ReadPower24Vol(){
    float fcode;
    if(ReadAd(Power24vCh,fcode)){
        Power24Vol = fcode*24/10; 
		if(Power24Vol<0) Power24Vol=0;
		V24ReadCode = fcode;
	}
}
void XRayTube::ReadPower5Vol(){
	float fcode;
	if(ReadAd(Power5vCh,fcode)){
		Power5Vol = fcode*5/10;
		if(Power5Vol<0) Power5Vol=0;
		V5ReadCode = fcode;
	}
}
void XRayTube::ReadPower12Vol(){
	float fcode;
	if(ReadAd(Power12vCh,fcode)){
		Power12Vol = fcode*12/10;
		if(Power12Vol<0) Power12Vol=0;
		V12ReadCode = fcode;
	}
}
void XRayTube::ReadH2500HV(){
    float fcode;
    if(ReadAd(H2500ReadCh,fcode)){ // fcode: 0~10v
        m_H2500CurrentHV = fcode*2500/10;
		if(m_H2500CurrentHV<0) m_H2500CurrentHV=0;
		H2500ReadCode = fcode;
    }
}
void XRayTube::ReadStdHV(){
    float fcode;
    if(ReadAd(StdReadCh,fcode)){ // fcode: 0~10v
        m_StdCurrHV = fcode*10/10;
		if(m_StdCurrHV<0) m_StdCurrHV=0;
		StdReadCode = fcode;
	}
}
void XRayTube::ReadHV(){
	float fcode;
	if(ReadAd(HvReadAdCh,fcode)) { // fcode: 0~10v
		m_CurrHV = fcode*50/10;    //50KV
		if(m_CurrHV<0) m_CurrHV=0;
		HvReadCode = fcode;
	}
}

void XRayTube::ReadMA(){
	float fcode;
	if(ReadAd(MaReadAdCh,fcode)) { // fcode: 0~10v
		m_CurrMA=fcode*1000/10;    //1000ma
		if(m_CurrMA<0) m_CurrMA=0;
		MaReadCode = fcode;
	}
}
void XRayTube::SetH2500HV(float hv){
	m_H2500Code=hv*5.0/3000;  //3000v
	if(m_H2500Code<0.0) m_H2500Code=0.0;
	else if(m_H2500Code>5.0) m_H2500Code=5.0;
	DaOut(H2500CtrlCh,m_H2500Code); //m_HvCode: 0~5
}
void XRayTube::SetHV(float hv){
    m_HvCode=hv*5.0/50.0;  //50.00kv
	if(m_HvCode<0.0) m_HvCode=0.0;
	else if(m_HvCode>5.0) m_HvCode=5.0;
	DaOut(HvCtrlDaCh,m_HvCode); //m_HvCode: 0~5

}
void XRayTube::SetMA(float ma){
    m_MaCode=ma*5.0/1000;  //1000ma
	if(m_MaCode<0.0) m_MaCode=0.0;
	else if(m_MaCode>5.0) m_MaCode=5.0;
	DaOut(MaCtrlDaCh,m_MaCode); //m_HvCode: 0~5
}

BOOL XSampleLid::Initialize(){
	return TRUE;
}
BOOL XSampleLid::CanOpen(){
    return TRUE;
}
BOOL XSampleLid::CanClose(){
    return TRUE;
}
void XSampleLid::Open(){}
void XSampleLid::Close(){}
void XSampleLid::Update(){
    AnalogUpdate();
}
void XSampleLid::Show(){
    gotoxy(2,4);
	switch(Lid1OpenedStatus) {
		case TRUE:
			textattr(0x02);cprintf("Lid1 Opend ");
			break;
		case FALSE:
			textattr(0x02);cprintf("Lid1 Closed");
			break;
	}
	gotoxy(2,5);
	switch(Lid2OpenedStatus) {
		case TRUE:
			textattr(0x02);cprintf("Lid2 Opend ");
			break;
		case FALSE:
			textattr(0x02);cprintf("Lid2 Closed");
			break;
	}
	textattr(0x07);
}
void XSampleLid::ReadSampleLidPos()
{
    Lid1OpenedStatus = GetDioBitHigh(Lid1Port,Lid1Bit);
    Lid2OpenedStatus = GetDioBitHigh(Lid2Port,Lid2Bit);
}

BOOL XShamber::Initialize(){
	return TRUE;
}
BOOL XShamber::CanOpen(){
	//can open fan
	if(FanOnStatus) return FALSE;
	else return TRUE;
	//return !FanOnStatus;
}
BOOL XShamber::CanClose(){
    //can close fan
    return FanOnStatus;
}
void XShamber::Open(){
    SetDioBit(FanPort,FanBit,TRUE);
    FanOnStatus = TRUE;
}
void XShamber::Close(){
    SetDioBit(FanPort,FanBit,FALSE);
    FanOnStatus = FALSE;
}
void XShamber::Update(){
    AnalogUpdate();
    ReadTemperature();
    ReadAngle();
}
void XShamber::ReadTemperature()
{
    float fcode;
    if(ReadAd(temp1ReadCh,fcode)){
		m_CurrTemp1 = fcode*10/4096;
		temp1Code = fcode;
	}
	if(ReadAd(temp2ReadCh,fcode)){
		m_CurrTemp2 = fcode*10/4096;
		temp2Code = fcode;
    }
}
void XShamber::ReadAngle()
{
    float fcode;
    if(ReadAd(angleReadCh,fcode)){
		CurrAngle = fcode*10/4096;
		angleCode = fcode;
	}
}

void XShamber::Show(){
	gotoxy(2,15);
	switch(FanOnStatus) {
		case TRUE:
			textattr(0x02);cprintf("Fan Opend ");
			break;
		case FALSE:
			textattr(0x02);cprintf("Fan Closed");
			break;
		default:
			textattr(0x02);cprintf("Unknow    ");
			//break;
	}
	gotoxy(2,16);textattr(0x02);
	cprintf("%4.2f C, %4.2f C",m_CurrTemp1,m_CurrTemp2);
	gotoxy(2,17);textattr(0x02);
	cprintf("Angle=%5.3f C",CurrAngle);
	gotoxy(2,18);textattr(0x06);
	cprintf("ADCode=%4.2f, %4.2f, %4.2f",temp1Code,temp2Code,angleCode);
}

BOOL XBeamStop::Initialize(){
	return TRUE;
}
BOOL XBeamStop::CanOpen(){
    switch(ShutOpenedStatus) { //FALSE=CLOSE,TRUE=Open
	case TRUE: //opend
		return FALSE;
	case FALSE://closed
		return m_pInstrument->CanOpenBeamStop(); //lid1/lid2 closed
	}
	return FALSE;
}
BOOL XBeamStop::CanClose(){
    return TRUE;
}
void XBeamStop::Open(){
    SetDioBit(CtrlOpenPort,CtrlOpenBit,TRUE);
    SetDioBit(CtrlClosePort,CtrlCloseBit,FALSE);
    ShutOpenedStatus = TRUE; //change status
}
void XBeamStop::Close(){
    SetDioBit(CtrlClosePort,CtrlCloseBit,TRUE);
    SetDioBit(CtrlOpenPort,CtrlOpenBit,FALSE);
    ShutOpenedStatus = FALSE;//change status
}
void XBeamStop::Update(){
    AnalogUpdate();
}
void XBeamStop::Show(){
	gotoxy(56,4);
	switch(ShutOpenedStatus) {
		case TRUE:
			textattr(0x02);cprintf("Opend ");
			break;
		case FALSE:
			textattr(0x02);cprintf("Closed");
			break;
		default:
			textattr(0x02);cprintf("Unknow");
	}    
}
void XBeamStop::ReadBeamStopPos()
{
    OpenPos = GetDioBitHigh(openPosPort,openPosBit);
    ClosePos= GetDioBitHigh(closePosPort,closePosBit);
}
    
BOOL XLight::Initialize(){
	return TRUE;
}
void XLight::SetPanel1(BOOL action){
    if(action != Pan1OnStatus) {
        SetDioBit(Pan1Port,Pan1Bit,action);
        Pan1OnStatus = action;
    }
}
void XLight::SetPanel2(BOOL action){
    if(action != Pan2OnStatus) {
        SetDioBit(Pan2Port,Pan2Bit,action);
        Pan2OnStatus = action;
    }
}
void XLight::SetPanel3(BOOL action){
    if(action != Pan3OnStatus) {
        SetDioBit(Pan3Port,Pan3Bit,action);
        Pan3OnStatus = action;
    }
}
void XLight::SetPanel4(BOOL action){
    if(action != Pan4OnStatus) {
        SetDioBit(Pan4Port,Pan4Bit,action);
        Pan4OnStatus = action;
    }
}
void XLight::SetPanel5(BOOL action){
    if(action != Pan5OnStatus) {
        SetDioBit(Pan5Port,Pan5Bit,action);
        Pan5OnStatus = action;
    }
}
void XLight::SetLamp1(BOOL action){
    if(action != Lamp1OnStatus) {
        SetDioBit(Lamp1Port,Lamp1Bit,action);
        Lamp1OnStatus = action;
    }
}
void XLight::SetLamp2(BOOL action){
    if(action != Lamp2OnStatus) {
        SetDioBit(Lamp2Port,Lamp2Bit,action);
        Lamp2OnStatus = action;
    }
}
void XLight::SetLamp3(BOOL action){
    if(action != Lamp3OnStatus) {
        SetDioBit(Lamp3Port,Lamp3Bit,action);
        Lamp3OnStatus = action;
    }
}
void XLight::SetLamp4(BOOL action){
    if(action != Lamp4OnStatus) {
        SetDioBit(Lamp4Port,Lamp4Bit,action);
        Lamp4OnStatus = action;
    }
}
void XLight::Show(){
	gotoxy(55,8);
	textattr(0x02);cprintf("Panel(%u,%u,%u,%u,%u)",Pan1OnStatus,Pan2OnStatus,Pan3OnStatus,Pan4OnStatus,Pan5OnStatus);
	gotoxy(55,9);
	textattr(0x02);cprintf("Lamp(%d,%d,%d,%d)",(int)Lamp1OnStatus,(int)Lamp2OnStatus,(int)Lamp3OnStatus,(int)Lamp4OnStatus);
}
void XLight::CloseAll(){}

BOOL XKey::Initialize(){
	return TRUE;
}
void XKey::Show(){
    gotoxy(28,4);
    if(Key1OnStatus){
        textattr(0x02);cprintf("Key1: ON ");
    }
    else{
        textattr(0x02);cprintf("Key1: OFF");
    }
    
    gotoxy(28,5);
    if(Key1OnStatus){
        textattr(0x02);cprintf("Key2: ON ");
    }
    else{
        textattr(0x02);cprintf("Key2: ON ");
    }    
    
}
void XKey::ReadAllKeyStatus()
{
	Key1OnStatus = GetDioBitHigh(Key1Port,Key1Bit);
	Key1OnStatus = GetDioBitHigh(Key2Port,Key2Bit);;
}