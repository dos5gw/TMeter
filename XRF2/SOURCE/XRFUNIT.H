# ifndef __XRFUNIT_H__
# define __XRFUNIT_H__
# include <dos.h>
# include "TypeDef.h"
# include "PM511P.H"
//# include "WatchDog.h"
# include "Fuzzy.h"
# include "string.h"
class XRayTube;
class XSampleLid;
class XShamber;
class XBeamStop;
class XLight;
class XKey;

union FloatByte {
	BYTE byte[4];
	float real;
};
union ShortByte {
	BYTE byte[2];
	unsigned int real;
};

class XInstrument {
private:
    XRayTube*   m_pXRayTube;
	XSampleLid* m_pSampleLid;
	XShamber*   m_pShamber;
	XBeamStop*  m_pBeamStop;
	XLight* m_pLight;
	XKey*   m_pKey;
	BOOL ReceivingBxConfig;
	DWORD crcBxConfig,ReceivingBxConfigStart;
	//BOOL ReceivingTiConfig;
	//DWORD crcTiConfig,ReceivingTiConfigStart;
	//BOOL ReceivingTubeConfig; //ext
	
public:
	enum Status {Sampling,TubeReady,Idle,SwitchToSampling,SwitchToTubeReady,SwitchToIdle,StandBy,SwitchToStandBy} m_status;
	BOOL FaultEncountered;
	BOOL CheckInstrumentStatus(Status targetStatus);
	BOOL CanOpenXRayTube();
	BOOL CanOpenBeamStop();
public:
	XInstrument();
	~XInstrument();
	void ScreenLayOut();
	void Run();
	BOOL CanStartSample();
    void StartSample();
    
	void Update();
	void Show();
	void ShowComStatus();
};

class XUnit {
protected:
	XInstrument* m_pInstrument;
	DWORD actionStart;
	char strError[40];
	BOOL newError;
public:
	XUnit(XInstrument* pParent) {
		m_pInstrument=pParent;
		actionStart=0;
		strError[0]='\0';
		newError=FALSE;
		}
public:
	//virtual BOOL Initialize();
	//virtual BOOL CanOpen();
	//virtual BOOL CanClose();
	//virtual void Open();
	//virtual void Close();
	//virtual void Update();
	//virtual void Show();

};
	
//1
class XRayTube //:public XUnit
{
public:
	XRayTube(XInstrument* p){
		m_pInstrument = p;
	    HvCtrlDaCh = 3; MaCtrlDaCh = 2;
		HvReadAdCh = 5; MaReadAdCh = 4; StdReadCh = 9;
	    DrvPort = 0; DrvBit = 3;

	    m_status = Closed;
		H2500CtrlCh =0; H2500ReadCh =3;
		Power24vCh = 8; Power5vCh = 0; Power12vCh = 1;

	    //newly added
	    m_SafeHV = 10;
		m_SafeMA = 50;  // safe: 10kv, 50ma
	    m_StdCurrHV = 0.0;
	    m_CurrHV = m_CurrMA = 0.0; 
	    m_HvTarget = m_MaTarget = 0.0;
	    m_WorkingHV = m_WorkingMA = 0.0;
	    m_H2500WorkingHV = 1800;
	    m_H2500CurrentHV = 0.0;
	    m_HvCode = m_MaCode = m_H2500Code = 0;
	    HvReadCode=MaReadCode=StdReadCode=H2500ReadCode=V24ReadCode=V12ReadCode=V5ReadCode = 0.0;
	    
	    Power24Vol = Power12Vol = Power5Vol = 0.0;
	    actionStart=0; m_delaySeconds = 2.0;
	    
	    m_maxWaitingSec = 0.0;
	    m_bSlowDownAdjust =  m_bWarmUpAdjust = FALSE;
        m_HvStep = 1.0;
        m_MaStep = 10;
        bH2500VolDaout=TRUE;
	};
	DWORD actionStart;
	double m_delaySeconds,m_maxWaitingSec;
	XInstrument* m_pInstrument;
    int HvCtrlDaCh, MaCtrlDaCh;
    int HvReadAdCh, MaReadAdCh, StdReadCh;
    int H2500CtrlCh, H2500ReadCh;
    int Power24vCh,Power5vCh,Power12vCh;
    int DrvPort,DrvBit;
    
    //enum Status {Closed,PreOpen,Opening,StandBy,WarmingUp,CoolingDn,Sleep,Ready,SlowDown} m_status;
	enum Status {Closed,PreOpen,Opening,StandBy,Ready,WarmingUp,CoolingDnHV,CoolingDnMA, Sleep,SlowDown} m_status;
    float m_StdCurrHV;
    float m_CurrHV,m_CurrMA;
    float m_SafeHV,m_SafeMA;
    float m_HvTarget,m_MaTarget;
    float m_WorkingHV,m_WorkingMA;
    float m_H2500WorkingHV,m_H2500CurrentHV;
    float m_HvCode,m_MaCode,m_H2500Code; // DA write code
    float HvReadCode,MaReadCode,StdReadCode,H2500ReadCode,V24ReadCode,V12ReadCode,V5ReadCode; //AD read code
	float Power24Vol,Power12Vol,Power5Vol;
public:
    //newly added
    BOOL m_bSlowDownAdjust;
	BOOL m_bWarmUpAdjust;
	float m_HvStep,m_MaStep;
	BOOL bH2500VolDaout; //add @2012-06-16
    
    BOOL Initialize(); 
    BOOL CanOpen();    
    BOOL CanClose(); 
    BOOL CanSafetyClose();  
    void Open();       
	void Close();
	BOOL CheckReady();
    void Update();     
    void Show();  
    void WarmUpXTube(); 
    void CoolDnXTube();    
    void CalculateWarmUpSteps();
    void CalculateCoolDnSteps();
    void ReadHV();
    void ReadMA();
    
    void ReadPower24Vol();
    void ReadPower5Vol();
    void ReadPower12Vol();
    
    void ReadH2500HV();
    void ReadStdHV();
    
    void SetHV(float hv);
    void SetMA(float ma);
    void SetH2500HV(float hv);
    ~XRayTube()
    {
		Close();
    }
};

//2
class XSampleLid //:public XUnit
{
public:
	XSampleLid(XInstrument* p){
	    m_pInstrument = p;
	    Lid1Port = 1; Lid1Bit =5 ;
        Lid2Port = 1; Lid2Bit =6;
        Lid1OpenedStatus = Lid2OpenedStatus =FALSE;
	};
	XInstrument* m_pInstrument;
    BOOL GetLid1Pos();
    BOOL GetLid2Pos();
private:
    int Lid1Port, Lid1Bit;
    int Lid2Port, Lid2Bit;
    
public:
    BOOL Lid1OpenedStatus,Lid2OpenedStatus; //status, 1=opend
    BOOL Initialize(); 
    BOOL CanOpen();    
    BOOL CanClose();   
    void Open();       
    void Close();      
    void Update();     
	void Show();
	void ReadSampleLidPos();
    ~XSampleLid()
    {
		//Close();
    }
};

//3
class XShamber //:public XUnit
{
public:
	XShamber(XInstrument* p){
	    m_pInstrument = p;
	    FanPort =0; FanBit =2;
        angleReadCh = 11;
	    temp1ReadCh = 10; temp2ReadCh =2;
	    FanOnStatus = FALSE;
		m_CurrTemp1 = m_CurrTemp2 = 0.0;
		CurrAngle = 0.0;
		temp1Code = temp2Code = 0.0;
		angleCode = 0.0;
		m_DLimitTemp = 36.0;
		m_ULimitTemp = 38.0;
		
	};
	XInstrument* m_pInstrument;
private:
    int angleReadCh;
    int temp1ReadCh, temp2ReadCh;
    int FanPort,FanBit;  
    
public:
    BOOL FanOnStatus; //status, 1=working, 0=stop
    float m_CurrTemp1,m_CurrTemp2;
	float CurrAngle;
	float temp1Code,temp2Code;
	float angleCode;
	float m_ULimitTemp,m_DLimitTemp;
	
    BOOL Initialize(); 
    BOOL CanOpen();    
    BOOL CanClose();   
    void Open();       
    void Close();      
    void Update();     
    void Show();
    void ReadTemperature();
    void ReadAngle();
    ~XShamber()
    {
        Close();
    }
};

//4
class XBeamStop //:public XUnit
{
public:
	XBeamStop(XInstrument* p){
	    m_pInstrument = p;
		CtrlOpenPort =0; CtrlOpenBit =6; 
		CtrlClosePort =0; CtrlCloseBit =4; 
        openPosPort=1;  openPosBit=3;
        closePosPort=1; closePosBit=4;
        ShutOpenedStatus = FALSE;
        ClosePos = OpenPos = FALSE;

	};
	XInstrument* m_pInstrument;
    //BOOL GetShutOpenStatus();
    //BOOL GetShutCloseStatus();
private:
    int CtrlOpenPort,CtrlOpenBit;
    int CtrlClosePort,CtrlCloseBit; //added @2012-06-16
    int openPosPort, openPosBit;
    int closePosPort,closePosBit;
    
public:
    BOOL ShutOpenedStatus; //status, 0=Close,1=Open shut
    BOOL ClosePos,OpenPos;
    BOOL Initialize(); 
    BOOL CanOpen();    
    BOOL CanClose();   
    void Open();       
    void Close();      
    void Update();     
	void Show();
	void ReadBeamStopPos();
    ~XBeamStop()
    {
        Close();
    }
};

//5
class XLight //:public XUnit{
{
public:
    XLight(){
        Pan1Port =2; Pan1Bit =1;   
        Pan2Port =2; Pan2Bit =2;   
        Pan3Port =2; Pan3Bit =3;   
        Pan4Port =2; Pan4Bit =4;   
        Pan5Port =2; Pan5Bit =5;   
                     
        Lamp1Port =2; Lamp1Bit =6; 
        Lamp2Port =2; Lamp2Bit =7; 
        Lamp3Port =0; Lamp3Bit =0; 
        Lamp4Port =0; Lamp4Bit =1;

        Pan1OnStatus=Pan2OnStatus=Pan3OnStatus=Pan4OnStatus=Pan5OnStatus=FALSE; //0: CLOSED
        //Pan1Action=Pan2Action=Pan3Action=Pan4Action=Pan5Action=FALSE;
        Lamp1OnStatus=Lamp2OnStatus=Lamp3OnStatus=Lamp4OnStatus=FALSE;
        //Lamp1Action=Lamp2Action=Lamp3Action=Lamp4Action=FALSE;
    };
    BOOL Initialize();
    void SetPanel1(BOOL action); //action=TRUE, open
    void SetPanel2(BOOL action);
    void SetPanel3(BOOL action);
    void SetPanel4(BOOL action);
    void SetPanel5(BOOL action);
    void SetLamp1(BOOL action);
    void SetLamp2(BOOL action);
    void SetLamp3(BOOL action);
    void SetLamp4(BOOL action);
	void CloseAll();
	void Show();

	BOOL Pan1OnStatus,Pan2OnStatus,Pan3OnStatus,Pan4OnStatus,Pan5OnStatus;//status,1=on, 0=off
	int Pan1Port, Pan1Bit;
	int Pan2Port, Pan2Bit;
	int Pan3Port, Pan3Bit;
	int Pan4Port, Pan4Bit;
	int Pan5Port, Pan5Bit;

	BOOL Lamp1OnStatus,Lamp2OnStatus,Lamp3OnStatus,Lamp4OnStatus; //status,1=on, 0=off
	int Lamp1Port, Lamp1Bit;
	int Lamp2Port, Lamp2Bit;
	int Lamp3Port, Lamp3Bit;
	int Lamp4Port, Lamp4Bit;
public:
	~XLight()
	{
		CloseAll();
	}
};

//6
class XKey //:public XUnit{
{
public:
	XKey(){
        Key1Port =1; Key1Bit =0; 
        Key2Port =1; Key2Bit =1;
        Key1OnStatus = FALSE;
		Key2OnStatus = FALSE;
	};
	BOOL Initialize();
	void Show();
    //BOOL GetKeyStatus(int n);

    int Key1Port,Key1Bit;
    int Key2Port,Key2Bit;
    BOOL Key1OnStatus,Key2OnStatus; //status
    void ReadAllKeyStatus();
public:
    ~XKey()
    {
        //Close();
    }

};

# endif


