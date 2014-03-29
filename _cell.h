#if !defined(AFX__CELL_H__498F7DD4_C5DD_42B3_8CAD_5DDEB7FC4C2F__INCLUDED_)
#define AFX__CELL_H__498F7DD4_C5DD_42B3_8CAD_5DDEB7FC4C2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// C_Cell wrapper class

class C_Cell : public COleDispatchDriver
{
public:
	C_Cell() {}		// Calls COleDispatchDriver default constructor
	C_Cell(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_Cell(const C_Cell& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetRow();
	long GetCol();
	CString GetTag();
	void SetTag(LPCTSTR lpszNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	CString GetImageKey();
	long GetAlignment();
	void SetAlignment(long nNewValue);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long newValue);
	unsigned long GetForeColor();
	void SetForeColor(unsigned long newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	long GetBorder(long Edge);
	void SetBorder(long Edge, long nNewValue);
	long GetCellType();
	void SetCellType(long nNewValue);
	void SetFocus();
	void EnsureVisible();
	COleFont GetFont();
	BOOL GetPrintable();
	void SetPrintable(BOOL bNewValue);
	BOOL GetWrapText();
	void SetWrapText(BOOL bNewValue);
	BOOL GetMergeCell();
	void SetImage(LPCTSTR ImageKey);
	void Refresh();
	BOOL GetBooleanValue();
	short GetIntegerValue();
	long GetLongValue();
	float GetSingleValue();
	double GetDoubleValue();
	long GetLeft();
	long GetTop();
	long GetWidth();
	long GetHeight();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__CELL_H__498F7DD4_C5DD_42B3_8CAD_5DDEB7FC4C2F__INCLUDED_)
