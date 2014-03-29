#include "stdafx.h"
#include "ComWnd.h"
#include "CRC32.H"

CComWnd::CComWnd()
{
	m_pCurCommand=NULL;
	m_bResetError=FALSE;
	m_bWriting=FALSE;
	m_bComError=FALSE;
}

BOOL COneCommand::AddToReceiveBuffer(BYTE byte) {
	m_ReceiveBuffer.Add(byte);
	if(m_bFirstByte) {
		m_bFirstByte=FALSE;
		if(byte==m_FailedByte) {
			m_bCommandExcuted=TRUE;
			m_status=Failed;
			return TRUE;
			}
		}
	if(m_ReceiveBuffer.GetSize()==m_nExpectedBytes) {
		m_bCommandExcuted=TRUE;
		if(m_bNeedCrcCheck) {
			if(CheckReceiveCrc()) 
				m_status=Success;
			else 
				m_status=CrcError;
			}
		else m_status=Success;
		return TRUE;
		}
	else return FALSE;
	}


UINT ReadThread(LPVOID param) {
	CComWnd *pComWnd=(CComWnd *)param;
	BOOL Ready=FALSE;
	BYTE ch;
	int loop=0;
	while(!Ready) {
		loop++;
		if(pComWnd->m_serial.Read(&ch,1)!=1) {
			pComWnd->m_pCurCommand->m_bCommandExcuted=TRUE;
			pComWnd->m_pCurCommand->m_status=OverTime;	
			pComWnd->m_bComError=TRUE;
			break;
			}
		if(pComWnd->m_pCurCommand->AddToReceiveBuffer(ch)) { // this function will return true when receive Expected Bytes
			Ready=TRUE;	
			}
		}
	return 0;
	}

UINT WriteThread(LPVOID param) {
	CComWnd *pComWnd=(CComWnd *)param;
	BYTE ch;int i=0,no=pComWnd->m_pCurCommand->m_CommandBuffer.GetSize();
	while(i<no) {
		ch=pComWnd->m_pCurCommand->m_CommandBuffer.GetAt(i++);
		if(pComWnd->m_serial.Write(&ch,1)!=1) {
			pComWnd->m_pCurCommand->m_bCommandExcuted=TRUE;
			pComWnd->m_pCurCommand->m_status=OverTime;	
			pComWnd->m_bComError=TRUE;
			break;		
			}
		}
	pComWnd->m_bWriting=FALSE;
	return 0;
	}

BOOL CComWnd::ExcuteCommand(COneCommand *pCmd) {
	m_pCurCommand=pCmd;
	m_nTimerCount=0;
	m_serial.ClearReadBuffer();
	m_serial.ClearWriteBuffer();
	m_bWriting=TRUE;m_bComError=FALSE;
	AfxBeginThread(WriteThread,this,THREAD_PRIORITY_NORMAL);
	while(m_bWriting) 
		Sleep(1);
	if(m_bComError) 
		return FALSE;
	if(m_pCurCommand->PassExcuteEvent()) {
		AfxBeginThread(ReadThread,this,THREAD_PRIORITY_NORMAL);
		}
	return TRUE;
	}


BOOL COneCommand::CheckReceiveCrc() {
	return CheckCrc32(m_ReceiveBuffer,m_crc32);
	}


void COneCommand::AddByteToComamnd(BYTE byte,BOOL ClearCmd,BOOL createCrc) {
	if(ClearCmd) m_CommandBuffer.RemoveAll();
	m_CommandBuffer.Add(byte);
	if(createCrc) CreateCommandCrc();		
	}

void COneCommand::CreateCommandCrc() {
	GenerateCRC32(m_CommandBuffer);
	}


void COneCommand::PrepareCommand(BYTE success, BYTE failed, int bytesExpected, int timeout,BOOL needCrc) {
	m_SuccessByte=success;
	m_FailedByte=failed;
	m_nExpectedBytes=bytesExpected;
	m_bCommandExcuted=FALSE;
	m_nTimeOut=timeout;
	m_bNeedCrcCheck=needCrc;
	m_ReceiveBuffer.RemoveAll();
	m_bFirstByte=TRUE;
	}

void COneCommand::ConstructCommand(CByteArray &cmd,BOOL createCrc,BYTE success, BYTE failed, int bytesExpected, int timeout,BOOL needCrc) {
	m_CommandBuffer.RemoveAll();
	m_CommandBuffer.Append(cmd);
	if(createCrc) CreateCommandCrc();	
	m_SuccessByte=success;
	m_FailedByte=failed;
	m_nExpectedBytes=bytesExpected;
	m_bCommandExcuted=FALSE;
	m_nTimeOut=timeout;
	m_bNeedCrcCheck=needCrc;
	}

CByteArray& COneCommand::GetAnswer() {
	m_status=Passed;
	return m_ReceiveBuffer;
	}


CmdStatus COneCommand::GetStatus(BOOL clear) {
	return m_status;
	}

BOOL COneCommand::GetReceivedBuffer(CByteArray &buffer) {
	BOOL ret;int i,size;BYTE src,dst;
	if((size=m_ReceiveBuffer.GetSize())!=buffer.GetSize()) {
		buffer.RemoveAll();
		buffer.Append(m_ReceiveBuffer);
		return TRUE;
		}
	else {
		ret=FALSE;
		for(i=0;i<size;i++) {
			src=m_ReceiveBuffer.GetAt(i);
			dst=buffer.GetAt(i);
			if(src!=dst) {
				buffer.SetAt(i,src);				
				ret=TRUE;
				}
			}
		return ret;
		}
	}

BOOL COneCommand::CheckTimeOut(int t) {
	if(t>=m_nTimeOut) {
		m_bCommandExcuted=TRUE;
		m_status=OverTime;	
		return TRUE;
		}
	else return FALSE;
	}

BOOL COneCommand::PassExcuteEvent() {
	if(m_nExpectedBytes==0) {
		m_status=Success;
		return FALSE;
		}
	else {
		m_status=Excuting;
		return TRUE;
		}
	}


BOOL COneCommand::CommandExcuted(CmdStatus &status) {
	status=m_status;
	return m_bCommandExcuted;
	}

BOOL CComWnd::OpenSerialPort() {
	if(!m_serial.IsOpen()) {
		m_serial.Open(m_port,m_baud,CSerialPort::NoParity,8,CSerialPort::OneStopBit,CSerialPort::NoFlowControl,FALSE);
		m_serial.SetReadTimeout(0,100,100);
		m_serial.SetWriteTimeout(50,50);
		}
	return TRUE;
	}


BOOL CComWnd::ReconfigComport(int port, DWORD baud) {
	m_port=port;m_baud=baud;
	if(m_serial.IsOpen()) m_serial.Close();
	return OpenSerialPort();
	}

CComWnd::~CComWnd()
{
}
