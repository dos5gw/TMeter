#if !defined(AFX_COLORLISTCTRL_H__EBB4E22F_EB10_43AF_9BB5_EB138E1A24C3__INCLUDED_)
#define AFX_COLORLISTCTRL_H__EBB4E22F_EB10_43AF_9BB5_EB138E1A24C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListCtrl.h : header file
//
/* ========================================================================
 * ClassName: CColorListCtrl
 * Author: wuhuiran(06-04-10)
 * Desc: 颜色列表类，可分别设置各个列表项的字体颜色和背景颜色
 * 使用和CListCtrl相同，添加两个函数
 * // 设置一单元格的颜色
 * void SetItemColor(int iItem, int iSubItem, COLORREF clrText, COLORREF clrBgr);
 * // 设置一行的颜色
 * void SetItemColor(int iItem, COLORREF clrText, COLORREF clrBgr);
 ============================================================================*/

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl window
#include <afxtempl.h>

class CListItemInfo
{
public:
	CListItemInfo();
	~CListItemInfo();

public:
	int m_iItem;
	int m_iSubItem;
	COLORREF m_clrText;
	COLORREF m_clrBgr;
};

typedef CArray<CListItemInfo, CListItemInfo> ARRListItemInfo;

// 颜色列表类
class CColorListCtrl : public CListCtrl
{
// Construction
public:
	CColorListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorListCtrl();
	
	// #################################################### 公有成员函数
public:
	// 设置一单元格的颜色
	void SetItemColor(int iItem, int iSubItem, COLORREF clrText, COLORREF clrBgr);
	// 设置一行的颜色
	void SetItemColor(int iItem, COLORREF clrText, COLORREF clrBgr);
	// 清空颜色设置
	void RemoveAllColor();
	void DeleteAllItems();
	BOOL DeleteItem( int nItem ); 
	
	// ##################################################### 公有成员变量
public:
	
	// ##################################################### 私有成员变量
private:
	ARRListItemInfo m_arrItemInfo;

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);	
//	afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTCTRL_H__EBB4E22F_EB10_43AF_9BB5_EB138E1A24C3__INCLUDED_)
