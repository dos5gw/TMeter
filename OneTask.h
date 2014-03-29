// OneTask.h: interface for the COneTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONETASK_H__F8ED924A_CD55_4457_9F7F_8D754A4EFAA7__INCLUDED_)
#define AFX_ONETASK_H__F8ED924A_CD55_4457_9F7F_8D754A4EFAA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COneTask  : public CObject
{
public:
	int m_Type;  // 1: calib(measure pure element), 2: calib(measure standard), 3,Calculate Intensity, 4, Calibrate(temp.cal)
	int m_TimeOut;

	BOOL m_Executed; // if ture, task will not ExecuteTask()
	//BOOL m_Finished; // if ture, task will be deleted
	BOOL m_SampleFinished;

	CString m_Tag;

	int m_Index; // Identification for CStandardSamples[x]

public:
	void SetExecuted(BOOL b) {m_Executed = b;};

	COneTask();
	COneTask(int type,int timeout,CString tag,int index);
	virtual ~COneTask();

};

class CTaskTrack : public CTypedPtrArray<CObArray,COneTask*> 
{
	int index;
public:
	CTaskTrack();
	~CTaskTrack();
	COneTask* GetPreTask();
	COneTask* GetNextTask();
	COneTask* GetCurrentTask();
	BOOL ReachTail();

	BOOL IsEmpty() {return (GetSize()==0);}
};

#endif // !defined(AFX_ONETASK_H__F8ED924A_CD55_4457_9F7F_8D754A4EFAA7__INCLUDED_)
