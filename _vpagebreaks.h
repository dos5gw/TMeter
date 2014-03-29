#if !defined(AFX__VPAGEBREAKS_H__D92409CA_6297_42F5_9DFA_9AAD5BBE9EA0__INCLUDED_)
#define AFX__VPAGEBREAKS_H__D92409CA_6297_42F5_9DFA_9AAD5BBE9EA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class C_PageBreak;

/////////////////////////////////////////////////////////////////////////////
// C_VPageBreaks wrapper class

class C_VPageBreaks : public COleDispatchDriver
{
public:
	C_VPageBreaks() {}		// Calls COleDispatchDriver default constructor
	C_VPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_VPageBreaks(const C_VPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Add(long Col);
	void Remove(long Index);
	void Clear();
	long Count();
	C_PageBreak Item(long Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__VPAGEBREAKS_H__D92409CA_6297_42F5_9DFA_9AAD5BBE9EA0__INCLUDED_)
