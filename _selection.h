#if !defined(AFX__SELECTION_H__A2F95BDA_5F63_4B56_B155_42126AB1EE98__INCLUDED_)
#define AFX__SELECTION_H__A2F95BDA_5F63_4B56_B155_42126AB1EE98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// C_Selection wrapper class

class C_Selection : public COleDispatchDriver
{
public:
	C_Selection() {}		// Calls COleDispatchDriver default constructor
	C_Selection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_Selection(const C_Selection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetBackColor(unsigned long newValue);
	void SetCellType(long nNewValue);
	void SetForeColor(unsigned long newValue);
	void SetAlignment(long nNewValue);
	void SetLocked(BOOL bNewValue);
	void SetPrintable(BOOL bNewValue);
	void SetWrapText(BOOL bNewValue);
	void SetBorders(long Edge, long nNewValue);
	void SetMergeCells(BOOL bNewValue);
	void SetFontName(LPCTSTR lpszNewValue);
	void SetFontSize(float newValue);
	void SetFontBold(BOOL bNewValue);
	void SetFontItalic(BOOL bNewValue);
	void SetFontUnderline(BOOL bNewValue);
	void SetFontStrikethrough(BOOL bNewValue);
	long GetFirstRow();
	long GetLastRow();
	long GetFirstCol();
	long GetLastCol();
	void ClearAll();
	void ClearText();
	void ClearFormat();
	void DeleteByRow();
	void DeleteByCol();
	void Merge();
	VARIANT CopyData();
	VARIANT CutData();
	VARIANT PasteData();
	void InsertRows();
	void InsertCols();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__SELECTION_H__A2F95BDA_5F63_4B56_B155_42126AB1EE98__INCLUDED_)
