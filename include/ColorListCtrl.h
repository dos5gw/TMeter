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
 * Desc: ��ɫ�б��࣬�ɷֱ����ø����б����������ɫ�ͱ�����ɫ
 * ʹ�ú�CListCtrl��ͬ�������������
 * // ����һ��Ԫ�����ɫ
 * void SetItemColor(int iItem, int iSubItem, COLORREF clrText, COLORREF clrBgr);
 * // ����һ�е���ɫ
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

// ��ɫ�б���
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
	
	// #################################################### ���г�Ա����
public:
	// ����һ��Ԫ�����ɫ
	void SetItemColor(int iItem, int iSubItem, COLORREF clrText, COLORREF clrBgr);
	// ����һ�е���ɫ
	void SetItemColor(int iItem, COLORREF clrText, COLORREF clrBgr);
	// �����ɫ����
	void RemoveAllColor();
	void DeleteAllItems();
	BOOL DeleteItem( int nItem ); 
	
	// ##################################################### ���г�Ա����
public:
	
	// ##################################################### ˽�г�Ա����
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
