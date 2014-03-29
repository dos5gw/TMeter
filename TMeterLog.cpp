// TMeterLog.cpp: implementation of the CTMeterLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tmeter.h"
#include "TMeterLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTMeterLog::CTMeterLog()
{

}

CTMeterLog::~CTMeterLog()
{

}

void CTMeterLog::WriteCalcIntensity2Log()
{
	
}

void CTMeterLog::WriteCalibWz2Log()
{
	CStdioFile iniFile;
	CString outBuff;
	CTMeterApp* pApp = (CTMeterApp*)AfxGetApp();
	CString file = pApp->m_pWorkZone->GetFullLogPath();
	file += "\\Calibrate.log";

	CTime now = GetCurrentTime();
	if(iniFile.Open(file,CFile::modeCreate|CFile::modeWrite))
	{
		outBuff.Format("%s \n",now.Format("%Y-%m-%d %H:%M:%S"));
		iniFile.WriteString(outBuff);

		//outBuff.Format();
		//iniFile.WriteString(outBuff);
		
		
		iniFile.Close();
	}
}

void CTMeterLog::WriteMeasureWz2Log()
{

}
