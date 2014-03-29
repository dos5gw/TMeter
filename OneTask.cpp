// OneTask.cpp: implementation of the COneTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tmeter.h"
#include "OneTask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



/////////////////////////////////////////////////////////////////////////////
// COneTask
COneTask::COneTask()
{
	m_Type=1; 
	m_TimeOut=1;
	m_Executed=FALSE;
	//m_Finished=FALSE;
	m_SampleFinished=FALSE;

	m_Index=0;
}

COneTask::COneTask(int type,int timeout,CString tag,int index) :m_Tag(tag)
{
	m_Type = type;
	m_TimeOut = timeout;
	m_Executed=FALSE;
	//m_Finished=FALSE;
	m_SampleFinished=FALSE;
	m_Index = index;
}

COneTask::~COneTask()
{

}



/////////////////////////////////////////////////////////////////////////////
// CTaskTrack
CTaskTrack::CTaskTrack()
{
	index = 0;
}

CTaskTrack::~CTaskTrack()
{

}

COneTask* CTaskTrack::GetPreTask()
{
	COneTask* ptr;
	if(index==0) 
		ptr=GetAt(index);
	else
		ptr=GetAt(--index);

	return ptr;
}

COneTask* CTaskTrack::GetNextTask()
{
	COneTask* ptr;
	int num = GetSize();
	if(index == num-1)
		ptr=GetAt(index);
	else
		ptr=GetAt(++index);
	return ptr;
}

COneTask* CTaskTrack::GetCurrentTask()
{
	return GetAt(index);
}


BOOL CTaskTrack::ReachTail()
{
	int num = GetSize();
	return (index == (num-1))?TRUE:FALSE;
}