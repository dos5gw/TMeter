// ColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl

CListItemInfo::CListItemInfo()
{
	m_iItem = -1;
	m_iSubItem = -1;
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBgr = ::GetSysColor(COLOR_WINDOW);
}

CListItemInfo::~CListItemInfo()
{
}

CColorListCtrl::CColorListCtrl()
{
	m_arrItemInfo.RemoveAll();
}

CColorListCtrl::~CColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdrawList)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomdrawList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl message handlers
// 自绘函数
void CColorListCtrl::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
    // Take the default processing unless we set this to something else below.
    *pResult = 0;
	
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		 *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else  if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
        // This is the prepaint stage for an item. Here's where we set the
        // item's text color. Our return value will tell Windows to draw the
        // item itself, but it will use the new color we set here.
        // We'll cycle the colors through red, green, and light blue.
        COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT);
		COLORREF clrBgr = ::GetSysColor(COLOR_WINDOW);
		
		int itemp = pLVCD->nmcd.dwItemSpec;
		int iSubItem = pLVCD->iSubItem;

		for(int i = 0; i < m_arrItemInfo.GetSize(); i++)
		{
			CListItemInfo listInfo = m_arrItemInfo[i];
			if((listInfo.m_iItem == itemp) && listInfo.m_iSubItem == iSubItem)
			{
				crText = listInfo.m_clrText;
				clrBgr = listInfo.m_clrBgr;
			}
		}

        // Store the color back in the NMLVCUSTOMDRAW struct.
        pLVCD->clrText = crText;
		pLVCD->clrTextBk = clrBgr;
		
        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
	}
}


// 设置一单元格的颜色
void CColorListCtrl::SetItemColor(int iItem, int iSubItem, COLORREF clrText, COLORREF clrBgr)
{
	CListItemInfo listInfo;
	listInfo.m_iItem = iItem;
	listInfo.m_iSubItem = iSubItem;
	listInfo.m_clrText = clrText;
	listInfo.m_clrBgr = clrBgr;

	m_arrItemInfo.Add(listInfo);
	Invalidate();
	UpdateWindow();
}

// 设置一行的颜色
void CColorListCtrl::SetItemColor(int iItem, COLORREF clrText, COLORREF clrBgr)
{
	CHeaderCtrl* pHeaderCtrl = NULL;
	pHeaderCtrl = GetHeaderCtrl();
	if(!pHeaderCtrl)
	{
		return;
	}

	int nCount = pHeaderCtrl->GetItemCount();

	for(int i = 0; i < nCount; i++)
	{
		SetItemColor(iItem, i, clrText, clrBgr);
	}
}

// 清空颜色设置
void CColorListCtrl::RemoveAllColor()
{
	m_arrItemInfo.RemoveAll();
	Invalidate();
	UpdateWindow();
}

// 重载函数CListCtrl::DeleteAllItems()
void CColorListCtrl::DeleteAllItems()
{
	CListCtrl::DeleteAllItems();
	RemoveAllColor();

}

// 重载BOOL CListCtrl::DeleteItem(int nItem)
BOOL CColorListCtrl::DeleteItem( int nItem )
{
	BOOL bRes = CListCtrl::DeleteItem(nItem);

	if(bRes)
	{
		for(int i = 0; i < m_arrItemInfo.GetSize(); i++)
		{
			if(m_arrItemInfo[i].m_iItem == nItem)
			{
				m_arrItemInfo.RemoveAt(i);
				i--;
			}
		}
	}

	return bRes;
}