#if !defined(AFX__IMAGES_H__4C9769E0_4999_4663_B3DC_DF2B5D9DD0B5__INCLUDED_)
#define AFX__IMAGES_H__4C9769E0_4999_4663_B3DC_DF2B5D9DD0B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class C_Image;

/////////////////////////////////////////////////////////////////////////////
// C_Images wrapper class

class C_Images : public COleDispatchDriver
{
public:
	C_Images() {}		// Calls COleDispatchDriver default constructor
	C_Images(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_Images(const C_Images& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Add(LPCTSTR FilePath, LPCTSTR Key);
	void Remove(const VARIANT& Key);
	long Count();
	C_Image Item(const VARIANT& Key);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__IMAGES_H__4C9769E0_4999_4663_B3DC_DF2B5D9DD0B5__INCLUDED_)