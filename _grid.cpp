// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "_grid.h"

// Dispatch interfaces referenced by this interface
#include "_Cell.h"
#include "_Calendar.h"
#include "_Chart.h"
#include "_column.h"
#include "_ComboBox.h"
#include "Font.h"
#include "_HPageBreaks.h"
#include "_images.h"
#include "_PageSetup.h"
#include "_range.h"
#include "_reporttitles.h"
#include "_selection.h"
#include "_vpagebreaks.h"

/////////////////////////////////////////////////////////////////////////////
// C_Grid

IMPLEMENT_DYNCREATE(C_Grid, CWnd)

/////////////////////////////////////////////////////////////////////////////
// C_Grid properties

/////////////////////////////////////////////////////////////////////////////
// C_Grid operations

C_Cell C_Grid::GetActiveCell()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803005e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_Cell(pDispatch);
}

BOOL C_Grid::GetAllowUserReorderColumn()
{
	BOOL result;
	InvokeHelper(0x6803005d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetAllowUserReorderColumn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803005d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetAllowUserResizing()
{
	BOOL result;
	InvokeHelper(0x6803005c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetAllowUserResizing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803005c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetAllowUserSort()
{
	BOOL result;
	InvokeHelper(0x6803005b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetAllowUserSort(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6803005b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long C_Grid::GetAppearance()
{
	long result;
	InvokeHelper(0x6803005a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetAppearance(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803005a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL C_Grid::GetAutoRedraw()
{
	BOOL result;
	InvokeHelper(0x68030059, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetAutoRedraw(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030059, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long C_Grid::GetBackColor1()
{
	unsigned long result;
	InvokeHelper(0x68030058, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColor1(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030058, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColor2()
{
	unsigned long result;
	InvokeHelper(0x68030057, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColor2(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030057, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColorBkg()
{
	unsigned long result;
	InvokeHelper(0x68030056, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColorBkg(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030056, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColorFixed()
{
	unsigned long result;
	InvokeHelper(0x68030055, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColorFixed(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030055, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColorFixedSel()
{
	unsigned long result;
	InvokeHelper(0x68030054, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColorFixedSel(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030054, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColorScrollBar()
{
	unsigned long result;
	InvokeHelper(0x68030053, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColorScrollBar(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030053, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBackColorSel()
{
	unsigned long result;
	InvokeHelper(0x68030052, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBackColorSel(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030052, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetBorderColor()
{
	unsigned long result;
	InvokeHelper(0x68030051, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetBorderColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030051, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long C_Grid::GetBottomRow()
{
	long result;
	InvokeHelper(0x68030050, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

C_Calendar C_Grid::GetCalendar()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803004f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_Calendar(pDispatch);
}

C_Cell C_Grid::Cell(long Row, long Col)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300a3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Row, Col);
	return C_Cell(pDispatch);
}

unsigned long C_Grid::GetCellBorderColor()
{
	unsigned long result;
	InvokeHelper(0x6803004e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetCellBorderColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803004e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

C_Chart C_Grid::Chart(long Row, long Col)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300a4, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Row, Col);
	return C_Chart(pDispatch);
}

long C_Grid::GetCols()
{
	long result;
	InvokeHelper(0x6803004d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetCols(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803004d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

C_Column C_Grid::Column(long Col)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600300a5, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Col);
	return C_Column(pDispatch);
}

C_ComboBox C_Grid::ComboBox(long Col)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600300a6, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Col);
	return C_ComboBox(pDispatch);
}

long C_Grid::GetDateFormat()
{
	long result;
	InvokeHelper(0x6803004c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDateFormat(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803004c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

COleFont C_Grid::GetDefaultFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803004b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return COleFont(pDispatch);
}

void C_Grid::SetRefDefaultFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6803004b, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
		 newValue);
}

short C_Grid::GetDefaultRowHeight()
{
	short result;
	InvokeHelper(0x6803004a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDefaultRowHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6803004a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL C_Grid::GetDisplayClientBorder()
{
	BOOL result;
	InvokeHelper(0x68030049, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDisplayClientBorder(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030049, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetDisplayErrorMessage()
{
	BOOL result;
	InvokeHelper(0x68030048, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDisplayErrorMessage(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030048, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetDisplayFocusRect()
{
	BOOL result;
	InvokeHelper(0x68030047, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDisplayFocusRect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030047, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetDisplayRowIndex()
{
	BOOL result;
	InvokeHelper(0x68030046, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetDisplayRowIndex(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030046, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetShowResizeTips()
{
	BOOL result;
	InvokeHelper(0x68030045, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetShowResizeTips(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030045, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL C_Grid::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x68030044, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030044, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long C_Grid::GetEnterKeyMoveTo()
{
	long result;
	InvokeHelper(0x68030043, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetEnterKeyMoveTo(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030043, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString C_Grid::GetFilename()
{
	CString result;
	InvokeHelper(0x68030042, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long C_Grid::GetFixedCols()
{
	long result;
	InvokeHelper(0x68030041, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetFixedCols(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030041, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetFixedRowColStyle()
{
	long result;
	InvokeHelper(0x68030040, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetFixedRowColStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030040, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetFixedRows()
{
	long result;
	InvokeHelper(0x6803003f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetFixedRows(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803003f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long C_Grid::GetForeColorFixed()
{
	unsigned long result;
	InvokeHelper(0x6803003e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetForeColorFixed(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803003e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long C_Grid::GetGridColor()
{
	unsigned long result;
	InvokeHelper(0x6803003d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetGridColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803003d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL C_Grid::GetGridLiness()
{
	BOOL result;
	InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetGridLiness(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

C_HPageBreaks C_Grid::GetHPageBreaks()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803003c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_HPageBreaks(pDispatch);
}

long C_Grid::GetHWnd()
{
	long result;
	InvokeHelper(0x6803003b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

C_Images C_Grid::GetImages()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803003a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_Images(pDispatch);
}

CString C_Grid::GetLastErrorMessage()
{
	CString result;
	InvokeHelper(0x68030039, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void C_Grid::SetLastErrorMessage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68030039, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long C_Grid::GetLeftCol()
{
	long result;
	InvokeHelper(0x68030038, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetLeftCol(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030038, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetMouseCol()
{
	long result;
	InvokeHelper(0x68030037, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Grid::GetMouseRow()
{
	long result;
	InvokeHelper(0x68030036, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL C_Grid::GetMultiSelect()
{
	BOOL result;
	InvokeHelper(0x68030035, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetMultiSelect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030035, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

C_PageSetup C_Grid::GetPageSetup()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x68030034, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_PageSetup(pDispatch);
}

C_Range C_Grid::Range(long FirstRow, long FirstCol, long LastRow, long LastCol)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x600300a7, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		FirstRow, FirstCol, LastRow, LastCol);
	return C_Range(pDispatch);
}

BOOL C_Grid::GetReadOnly()
{
	BOOL result;
	InvokeHelper(0x68030033, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::SetReadOnly(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030033, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long C_Grid::GetReadOnlyFocusRect()
{
	long result;
	InvokeHelper(0x68030032, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetReadOnlyFocusRect(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030032, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

C_ReportTitles C_Grid::GetReportTitles()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x68030031, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_ReportTitles(pDispatch);
}

long C_Grid::GetRowHeight(long Row)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030030, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Row);
	return result;
}

void C_Grid::SetRowHeight(long Row, long nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x68030030, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Row, nNewValue);
}

long C_Grid::GetRightCol()
{
	long result;
	InvokeHelper(0x6803002f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long C_Grid::GetRows()
{
	long result;
	InvokeHelper(0x6803002e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetRows(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803002e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetScrollBars()
{
	long result;
	InvokeHelper(0x6803002d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetScrollBars(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803002d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetScrollBarStyle()
{
	long result;
	InvokeHelper(0x6803002c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetScrollBarStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803002c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

C_Selection C_Grid::GetSelection()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6803002b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_Selection(pDispatch);
}

long C_Grid::GetSelectionMode()
{
	long result;
	InvokeHelper(0x6803002a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetSelectionMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6803002a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short C_Grid::GetSelLength()
{
	short result;
	InvokeHelper(0x68030029, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void C_Grid::SetSelLength(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x68030029, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short C_Grid::GetSelStart()
{
	short result;
	InvokeHelper(0x68030028, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void C_Grid::SetSelStart(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x68030028, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetTopRow()
{
	long result;
	InvokeHelper(0x68030027, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void C_Grid::SetTopRow(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68030027, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long C_Grid::GetTotalPages()
{
	long result;
	InvokeHelper(0x68030026, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

C_VPageBreaks C_Grid::GetVPageBreaks()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x68030025, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return C_VPageBreaks(pDispatch);
}

void C_Grid::AboutBox()
{
	InvokeHelper(0x600300a8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void C_Grid::AddChart(long Row, long Col)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300a9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Row, Col);
}

void C_Grid::AddItem(LPCTSTR Text, BOOL DrawCell)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x600300aa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, DrawCell);
}

VARIANT C_Grid::AutoFitRowHeight(long Row)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600300ab, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Row);
	return result;
}

BOOL C_Grid::DirectPrint(const VARIANT& PageNo)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x600300ac, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&PageNo);
	return result;
}

BOOL C_Grid::ExportToHTML(LPCTSTR Filename)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300ad, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Filename);
	return result;
}

BOOL C_Grid::ExportToXML(LPCTSTR Filename)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300ae, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Filename);
	return result;
}

BOOL C_Grid::ExportToExcel(LPCTSTR Filename, BOOL ExportFixedRow, BOOL ExportFixedCol)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x600300af, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Filename, ExportFixedRow, ExportFixedCol);
	return result;
}

BOOL C_Grid::LoadFromXML(LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300b0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFileName);
	return result;
}

BOOL C_Grid::NewFile()
{
	BOOL result;
	InvokeHelper(0x600300b1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

C_Cell C_Grid::HitTest(long X, long Y)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300b2, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		X, Y);
	return C_Cell(pDispatch);
}

BOOL C_Grid::HttpDownloadFile(LPCTSTR strURL, LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x600300b3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strURL, strFileName);
	return result;
}

BOOL C_Grid::HttpOpenFile(LPCTSTR strURL)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300b4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strURL);
	return result;
}

BOOL C_Grid::HttpLoadFromXML(LPCTSTR strURL)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300b5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strURL);
	return result;
}

BOOL C_Grid::OpenFile(LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300b6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFileName);
	return result;
}

void C_Grid::InsertCol(long StartCol, long Count)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300b7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 StartCol, Count);
}

void C_Grid::InsertRow(long StartRow, long Count)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300b8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 StartRow, Count);
}

BOOL C_Grid::PrintDialog()
{
	BOOL result;
	InvokeHelper(0x600300b9, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void C_Grid::PrintPreview(const VARIANT& Zoom)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x600300ba, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Zoom);
}

void C_Grid::PrintPreviewVirtualGrid(const VARIANT& Zoom)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x600300bb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Zoom);
}

void C_Grid::Refresh()
{
	InvokeHelper(0x600300bc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void C_Grid::RemoveChart(long Row, long Col)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x600300bd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Row, Col);
}

void C_Grid::RemoveItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x600300be, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

BOOL C_Grid::SaveChartAsBitmap(long Row, long Col, LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x600300bf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Row, Col, strFileName);
	return result;
}

BOOL C_Grid::SaveFile(LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300c0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFileName);
	return result;
}

BOOL C_Grid::SetRegisterInformation(LPCTSTR RegisteredOwner, LPCTSTR SerialNumber)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x600300c1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		RegisteredOwner, SerialNumber);
	return result;
}