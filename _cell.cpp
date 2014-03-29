// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "_cell.h"

// Dispatch interfaces referenced by this interface
#include "font.h"


/////////////////////////////////////////////////////////////////////////////
// C_Cell properties

/////////////////////////////////////////////////////////////////////////////
// C_Cell operations

long C_Cell::GetRow()
{
	long result;
	InvokeHelper(0x68030018, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Cell::GetCol()
{
	long result;
	InvokeHelper(0x68030017, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString C_Cell::GetTag()
{
	CString result;
	InvokeHelper(0x68030015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void C_Cell::SetTag(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68030015, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString C_Cell::GetText()
{
	CString result;
	InvokeHelper(0x68030014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void C_Cell::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68030014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString C_Cell::GetImageKey()
{
	CString result;
	InvokeHelper(0x68030013, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long C_Cell::GetAlignment()
{
	long result;
	InvokeHelper(0x68030012, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Cell::SetAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long C_Cell::GetBackColor()
{
	unsigned long result;
	InvokeHelper(0x68030011, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Cell::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030011, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Cell::GetForeColor()
{
	unsigned long result;
	InvokeHelper(0x68030010, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Cell::SetForeColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL C_Cell::GetLocked()
{
	BOOL result;
	InvokeHelper(0x6803000f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Cell::SetLocked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803000f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long C_Cell::GetBorder(long Edge)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803000e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Edge);
	return result;
}

void C_Cell::SetBorder(long Edge, long nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6803000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Edge, nNewValue);
}

long C_Cell::GetCellType()
{
	long result;
	InvokeHelper(0x6803000d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Cell::SetCellType(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803000d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void C_Cell::SetFocus()
{
	InvokeHelper(0x60030019, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void C_Cell::EnsureVisible()
{
	InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

COleFont C_Cell::GetFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return COleFont(pDispatch);
}

BOOL C_Cell::GetPrintable()
{
	BOOL result;
	InvokeHelper(0x6803000b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Cell::SetPrintable(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803000b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Cell::GetWrapText()
{
	BOOL result;
	InvokeHelper(0x6803000a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Cell::SetWrapText(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Cell::GetMergeCell()
{
	BOOL result;
	InvokeHelper(0x68030009, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Cell::SetImage(LPCTSTR ImageKey)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ImageKey);
}

void C_Cell::Refresh()
{
	InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL C_Cell::GetBooleanValue()
{
	BOOL result;
	InvokeHelper(0x68030008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short C_Cell::GetIntegerValue()
{
	short result;
	InvokeHelper(0x68030007, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

long C_Cell::GetLongValue()
{
	long result;
	InvokeHelper(0x68030006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

float C_Cell::GetSingleValue()
{
	float result;
	InvokeHelper(0x68030005, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

double C_Cell::GetDoubleValue()
{
	double result;
	InvokeHelper(0x68030004, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

long C_Cell::GetLeft()
{
	long result;
	InvokeHelper(0x68030003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Cell::GetTop()
{
	long result;
	InvokeHelper(0x68030002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Cell::GetWidth()
{
	long result;
	InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Cell::GetHeight()
{
	long result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
