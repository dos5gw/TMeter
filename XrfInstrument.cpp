#include "StdAfx.h"
#include "TMeter.h"
#include "XrfInstrument.h"

CXrfInstrument::CXrfInstrument()
{
	bSystemParamLoaded=FALSE;
	m_status=Unknown; //SwitchToIdle;
	m_bOccupied=FALSE;
	//m_Occupyer="NONE";

	m_WorkingHv = m_WorkingMa = m_H2500WorkingHv = 0.0;
	m_CurrentHv = m_CurrentMa = m_H2500CurrentHv = 0.0;

	m_SafeHv=10.0; //Kv
	m_SafeMa=50.0; //Ma
	m_CurrentHtStdVol=0.0;

	m_FanStatus=Closed; 
	m_XRayTubeStatus=Closed; 
	m_BeamStopStatus=Closed; 
	m_SampleLidStatus=Closed; 

	m_Current24Vol=m_Current12Vol=m_Current5Vol=0.0;
	m_CurrentAngle=0.0;
	m_Temperature1=m_Temperature2=0.0;

	m_UThresholdTemp=38.0;
	m_LThresholdTemp=35.0;

	m_SerialPortNum=1;
	m_SerialPortBaud=28800;

	m_InputChannel=1;
	m_Brightness=255;
	m_Contrast=255;
	m_Hue=255;
	m_Saturation=300;

	// Measurement Hardware Parameter, Default Value
	TargetAtomicNumber = 74; //W Target
	TargetTakeOffAngle = 20.f;
	Hv30HardCode = 30.0;
	Hv40HardCode = 40.0;
	Hv50HardCode = 50.0;
	WindowThickness = 0.3f;     
	IncidentAngle = 45.f;
	EmergentAngle = 45.f;
	FilterNiThick = 0.f;
	FilterAlThick = 0.f; 

	Coll1Current= 0.f;
	Coll2Current= 0.f;
	Coll3Current= 0.f;
	Coll4Current= 0.f;
}

BOOL CXrfInstrument::LoadHardwareSettings()
{
	CString inBuff; int findPos; CStdioFile iniFile;
	CString setting;

	if(iniFile.Open("Hardware\\XrfSettings.INI",CFile::modeRead)) 
	{
		while(iniFile.ReadString(inBuff)) 
		{
			if(inBuff.Find("SerialPortNum")!=-1) { //1
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_SerialPortNum=atoi(setting);
				//m_SerialPortInstance.setPortNum(atoi(setting));
			}
			else if(inBuff.Find("SerialPortBaud")!=-1) { //2
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_SerialPortBaud=atoi(setting);
				//m_SerialPortInstance.setPortBaud(atoi(setting));
			}
			else if(inBuff.Find("WorkingHv")!=-1) { //3
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_WorkingHv=(float)atof(setting);	
			}
			else if(inBuff.Find("WorkingMa")!=-1) { //4
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_WorkingMa=(float)atof(setting);	
			}
			else if(inBuff.Find("SafeHv")!=-1) { //5
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_SafeHv=(float)atof(setting);	
			}
			else if(inBuff.Find("SafeMa")!=-1) { //6
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_SafeMa=(float)atof(setting);	
			}
			else if(inBuff.Find("H2500Working")!=-1) { //7
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_H2500WorkingHv=(float)atof(setting);	
			}
			else if(inBuff.Find("H2500Step")!=-1) {     //added @2012-08-28 
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_H2500Step=(float)atof(setting);	
			}
			else if(inBuff.Find("V_InputChannel")!=-1) {//added @2012-07-16
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_InputChannel=atoi(setting);
			}
			else if(inBuff.Find("V_Brightness")!=-1) {
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_Brightness=atoi(setting);
			}
			else if(inBuff.Find("V_Contrast")!=-1) {
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_Contrast=atoi(setting);
			}
			else if(inBuff.Find("V_Hue")!=-1) {
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_Hue=atoi(setting);
			}
			else if(inBuff.Find("V_Saturation")!=-1) {
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				m_Saturation=atoi(setting);
			}

			// newly added @2012-08-27 
			else if(inBuff.Find("Coll1Current")!=-1) { // Meas. Param 1
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Coll1Current=(float)atof(setting);
			}
			else if(inBuff.Find("Coll2Current")!=-1) { // Meas. Param 2
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Coll2Current=(float)atof(setting);
			}
			else if(inBuff.Find("Coll3Current")!=-1) { // Meas. Param 3
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Coll3Current=(float)atof(setting);
			}
			else if(inBuff.Find("Coll4Current")!=-1) { // Meas. Param 4
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Coll4Current=(float)atof(setting);
			}
			else if(inBuff.Find("Hv30HardCode")!=-1) { // Meas. Param 5
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Hv30HardCode=(float)atof(setting);
			}
			else if(inBuff.Find("Hv40HardCode")!=-1) { // Meas. Param 6
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Hv40HardCode=(float)atof(setting);
			}
			else if(inBuff.Find("Hv50HardCode")!=-1) { // Meas. Param 7
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				Hv50HardCode=(float)atof(setting);
			}
			else if(inBuff.Find("FilterThicknessAl")!=-1) { // Meas. Param 8
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				FilterAlThick=(float)atof(setting);
			}
			else if(inBuff.Find("FilterThicknessNi")!=-1) { // Meas. Param 9
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				FilterNiThick=(float)atof(setting);
			}
			else if(inBuff.Find("TubeWindowThickness")!=-1) { // Meas. Param 10
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				WindowThickness=(float)atof(setting);
			}
			else if(inBuff.Find("TargetAtomicNumber")!=-1) { // Meas. Param 11
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				TargetAtomicNumber=atoi(setting);
			}
			else if(inBuff.Find("XrayTargetTakeOffAngle")!=-1) { // Meas. Param 12
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				TargetTakeOffAngle=(float)atof(setting);
			}
			else if(inBuff.Find("XrayIncidentAngle")!=-1) { // Meas. Param 13
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				IncidentAngle=(float)atof(setting);
			}
			else if(inBuff.Find("XrayEmergentAngle")!=-1) { // Meas. Param 14
				findPos=inBuff.Find('=')+1;
				setting=inBuff.Mid(findPos);
				EmergentAngle=(float)atof(setting);
			}
		}
		bSystemParamLoaded=TRUE;
		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CXrfInstrument::WriteSettingsToFiles()
{
	CString outBuff;CStdioFile iniFile;
	CString setting;
	CString path = ((CTMeterApp*)AfxGetApp())->GetApplicationDir() +  "\\Hardware\\XrfSettings.INI";

	if(iniFile.Open(path,CFile::modeCreate|CFile::modeWrite)) {

		setting.Format("SerialPortNum=%d\n",m_SerialPortNum);    //1
		iniFile.WriteString(setting);

		setting.Format("SerialPortBaud=%lu\n",m_SerialPortBaud); //2
		iniFile.WriteString(setting);

		setting.Format("WorkingHv=%5.3f\n",m_WorkingHv); //3
		iniFile.WriteString(setting);

		setting.Format("WorkingMa=%6.3f\n",m_WorkingMa); //4
		iniFile.WriteString(setting);

		setting.Format("SafeHv=%5.3f\n",m_SafeHv);       //5
		iniFile.WriteString(setting);

		setting.Format("SafeMa=%6.3f\n",m_SafeMa);       //6
		iniFile.WriteString(setting);

		setting.Format("H2500Working=%6.3f\n",m_H2500WorkingHv); //7
		iniFile.WriteString(setting);

		setting.Format("H2500Step=%6.3f\n",m_H2500Step);        //8
		iniFile.WriteString(setting);

		setting.Format("V_InputChannel=%d\n",m_InputChannel);
		iniFile.WriteString(setting);

		setting.Format("V_Brightness=%d\n",m_Brightness);  
		iniFile.WriteString(setting);

		setting.Format("V_Contrast=%d\n",m_Contrast); 
		iniFile.WriteString(setting);

		setting.Format("V_Hue=%d\n",m_Hue);         
		iniFile.WriteString(setting);

		setting.Format("V_Saturation=%d\n",m_Saturation);  
		iniFile.WriteString(setting);


		// newly added @2012-08-27 
		setting.Format("Coll1Current=%6.3f\n",Coll1Current);  // Meas. Param 1
		iniFile.WriteString(setting);

		setting.Format("Coll2Current=%6.3f\n",Coll2Current);  // Meas. Param 2
		iniFile.WriteString(setting);

		setting.Format("Coll3Current=%6.3f\n",Coll3Current);  // Meas. Param 3
		iniFile.WriteString(setting);

		setting.Format("Coll4Current=%6.3f\n",Coll4Current);  // Meas. Param 4
		iniFile.WriteString(setting);

		setting.Format("Hv30HardCode=%6.3f\n",Hv30HardCode);  // Meas. Param 5
		iniFile.WriteString(setting);

		setting.Format("Hv40HardCode=%6.3f\n",Hv40HardCode);  // Meas. Param 6
		iniFile.WriteString(setting);

		setting.Format("Hv50HardCode=%6.3f\n",Hv50HardCode);  // Meas. Param 7
		iniFile.WriteString(setting);

		setting.Format("FilterThicknessAl=%5.3f\n",FilterAlThick);  // Meas. Param 8
		iniFile.WriteString(setting);

		setting.Format("FilterThicknessNi=%5.3f\n",FilterNiThick);  // Meas. Param 9
		iniFile.WriteString(setting);

		setting.Format("TubeWindowThickness=%5.3f\n",WindowThickness);  // Meas. Param 10
		iniFile.WriteString(setting);

		setting.Format("TargetAtomicNumber=%d\n",TargetAtomicNumber);  // Meas. Param 11
		iniFile.WriteString(setting);

		setting.Format("XrayTargetTakeOffAngle=%4.2f\n",TargetTakeOffAngle);  // Meas. Param 12
		iniFile.WriteString(setting);

		setting.Format("XrayIncidentAngle=%4.2f\n",IncidentAngle);  // Meas. Param 13
		iniFile.WriteString(setting);

		setting.Format("XrayEmergentAngle=%4.2f\n",EmergentAngle);  // Meas. Param 14
		iniFile.WriteString(setting);


		iniFile.Close();
		return TRUE;
	}
	return FALSE;
}

void CXrfInstrument::SetInstrumentStatus(BYTE status)
{
	// Sampling(0),TubeReady(1),Idle(2),SwitchToSampling(3),SwitchToTubeReady(4),SwitchToIdle(5)
	switch(status)
	{
	case 0x00:
		m_status = Sampling;
		break;
	case 0x01:
		m_status = TubeReady;
		break;
	case 0x02:
		m_status = Idle;
		break;
	case 0x03:
		m_status = SwitchToSampling;
		break;
	case 0x04:
		m_status = SwitchToTubeReady;
		break;
	case 0x05:
		m_status = SwitchToIdle;
		break;
	default:
		m_status = Unknown;
	}
}

void CXrfInstrument::SetOccupied(BOOL ocuupied,CString occupyer)
{
	m_bOccupied=ocuupied;
	m_Occupyer=occupyer;
}

BOOL CXrfInstrument::CheckOccupied() 
{
	if(m_bOccupied)
	{
		CString info; 
		info.LoadString(IDS_INFO_OCUUPIED_INFO);
		info = info + m_Occupyer;
		AfxMessageBox(info);
	}
		
	return m_bOccupied;
}

BOOL CXrfInstrument::CheckOccupied(CString ocp)
{
	if(m_bOccupied && m_Occupyer==ocp) return TRUE;
	return FALSE;
}

CXrfInstrument::~CXrfInstrument()
{

}