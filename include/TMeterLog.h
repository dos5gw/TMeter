// TMeterLog.h: interface for the CTMeterLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMETERLOG_H__86AB5A0E_FAF6_4FC6_86FD_34550FBA942C__INCLUDED_)
#define AFX_TMETERLOG_H__86AB5A0E_FAF6_4FC6_86FD_34550FBA942C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTMeterLog  
{
public:
	CTMeterLog();
	virtual ~CTMeterLog();
	void WriteCalcIntensity2Log();
	void WriteCalibWz2Log();
	void WriteMeasureWz2Log();

};

#endif // !defined(AFX_TMETERLOG_H__86AB5A0E_FAF6_4FC6_86FD_34550FBA942C__INCLUDED_)
