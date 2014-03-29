#pragma once

typedef enum Status {Sampling,TubeReady,Idle,SwitchToSampling,SwitchToTubeReady,SwitchToIdle,Unknown} XStatus;
typedef enum ComponentStatus {Opened,Closed} XComponentStatus;
typedef enum InstrumentOccupied {NONE_O,STBTEST,MEASURE} XOccupied;

class CXrfInstrument
{
	//friend class CMdiMeasureView;
public:
	CXrfInstrument(void);
	~CXrfInstrument(void);
	BOOL LoadHardwareSettings();
	BOOL WriteSettingsToFiles();
	void SetInstrumentStatus(BYTE status);
	XStatus GetInstrumentStatus(){ return m_status; }

	//XOccupied GetInstrumentOccupied( return m_occupied;)
	void SetOccupied(BOOL ocuupied,CString occupyer);
	BOOL CheckOccupied(); //return m_bOccupied and popup messagebox
	BOOL CheckOccupied(CString ocp);

public:
	BOOL bSystemParamLoaded;
	XStatus m_status;
	BOOL m_bOccupied;
	CString m_Occupyer;

	int m_SerialPortNum;
	DWORD m_SerialPortBaud;

	// AD/DA
	float m_WorkingHv,m_WorkingMa,m_H2500WorkingHv;
	float m_CurrentHv,m_CurrentMa,m_H2500CurrentHv;
	float m_SafeHv,m_SafeMa;
	float m_H2500Step;  // m_H2500Step mean how much vol changed alias one channel

	// DIO
	BOOL PanelLight[5]; 
	BOOL LampLight[4];
	BOOL m_BeamStopOpenPos,m_BeamStopClosePos;
	BOOL m_SampleLidOpenPos,m_SampleLidClosePos;

	XComponentStatus m_FanStatus;  // 1:Closed, 0:Opened, 3Unknown
	XComponentStatus m_XRayTubeStatus; 
	XComponentStatus m_BeamStopStatus; 
	XComponentStatus m_SampleLidStatus; 

	// AD
	float m_CurrentHtStdVol,m_Current24Vol,m_Current12Vol,m_Current5Vol;
	float m_CurrentAngle;
	float m_Temperature1,m_Temperature2;
	float m_UThresholdTemp,m_LThresholdTemp;

	// Video Card
	int m_InputChannel;
	int m_Brightness;
	int m_Contrast;
	int m_Hue;
	int m_Saturation;

	// Measurement Hardware Parameter
	short TargetAtomicNumber;
	float TargetTakeOffAngle;
	float Hv30HardCode;
	float Hv40HardCode;
	float Hv50HardCode;
	float WindowThickness; // in millimeters
	float IncidentAngle;
	float EmergentAngle;
	float FilterNiThick;   // in millimeters
	float FilterAlThick;

	float Coll1Current;
	float Coll2Current;
	float Coll3Current;
	float Coll4Current;

private:
	//XOccupied m_occupied;

};
