#ifndef COM_WND_H
#define COM_WND_H

#include <afxtempl.h>
#include "SerialPort.h"

enum CmdStatus{Excuting,Success,Failed,OverTime,CrcError,Passed};
enum CommandType{ConfigBX,LoadBX,ChgTimes,Reset,Confirm,ReadAll,SetAll,ChgStatus,ReadSpectrum,ClrFault,ReleaseVacuum};

union FloatBytes 
{
	float real;
	BYTE byte[4];
};

union ShortBytes 
{
	short real;
	BYTE byte[2];
};

class COneCommand : public CObject
{
public:
	CommandType m_Type;
	CByteArray m_CommandBuffer;
	CByteArray m_ReceiveBuffer;
	BYTE m_SuccessByte;
	BYTE m_FailedByte;
	int m_nExpectedBytes;
	BOOL m_bNeedCrcCheck;
	int m_nTimeOut;
	CmdStatus m_status;
public:
	DWORD m_crc32;
	BOOL CommandExcuted(CmdStatus& status);
	BOOL PassExcuteEvent();
	BOOL CheckTimeOut(int t);
	BOOL m_bCommandExcuted;
	void ConstructCommand(CByteArray& cmd,BOOL createCrc,BYTE success,BYTE failed,int bytesExpected,int timeout,BOOL needCrc);
	BOOL GetReceivedBuffer(CByteArray& buffer);
	CmdStatus GetStatus(BOOL clear);
	CByteArray& GetAnswer();
	void PrepareCommand(BYTE success, BYTE failed, int bytesExpected, int timeout,BOOL needCrc);
	void CreateCommandCrc();
	void AddByteToComamnd(BYTE byte,BOOL ClearCmd=FALSE,BOOL createCrc=FALSE);
	BOOL CheckReceiveCrc();
	BOOL AddToReceiveBuffer(BYTE byte);
private:
	BOOL m_bFirstByte;
};

class CComWnd //: public CStatic
{
	friend UINT ReadThread(LPVOID param);
	friend UINT WriteThread(LPVOID param);
	int m_port;
	DWORD m_baud;

public:
	CComWnd();
	BOOL ReconfigComport(int port,DWORD baud);
	BOOL OpenSerialPort();
	BOOL ExcuteCommand(COneCommand *pCommand);

	inline void setPortNum(int num){ m_port= num; }
	inline void setPortBaud(DWORD baud){ m_baud= baud; }
	inline int  getPortNum(){ return m_port; }
	inline DWORD getPortBaud(){ return m_baud; }
	BOOL getComError(){ return m_bComError; }

	BOOL m_bResetError;
	BOOL m_bComError;
	BOOL m_bWriting;
	int  m_nTimerCount;

private:
	CSerialPort m_serial;
	COneCommand *m_pCurCommand;

public:
	virtual ~CComWnd();
};

class CCommandTrack : public CTypedPtrArray<CObArray,COneCommand*> 
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
};

#endif
