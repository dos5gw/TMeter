// ConfOneStdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tmeter.h"
#include "ConfOneStdDlg.h"
#include "OneLayerDefine.h"
# include "_cell.h"
# include "_column.h"
# include "_combobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfOneStdDlg dialog


CConfOneStdDlg::CConfOneStdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfOneStdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfOneStdDlg)
	m_StdName = _T("");
	//}}AFX_DATA_INIT
	m_bNewFile = TRUE;
	m_pStdSample = NULL;
}


void CConfOneStdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfOneStdDlg)
	DDX_Control(pDX, IDC_STDGRID, m_StdGrid);
	DDX_Text(pDX, IDC_STD_TAG, m_StdName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfOneStdDlg, CDialog)
	//{{AFX_MSG_MAP(CConfOneStdDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfOneStdDlg message handlers

BOOL CConfOneStdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(m_bNewFile) SetWindowText("输入新标样");
	else SetWindowText("修改标样");

	C_ComboBox comboBox;
	m_StdGrid.SetRegisterInformation("CN00277","0YGRA-NFRGD-OMS2F-YKATO-UKIC3");
	m_StdGrid.SetAutoRedraw(FALSE);
	m_StdGrid.SetAllowUserSort(TRUE);
	m_StdGrid.SetShowResizeTips(FALSE);

	m_StdGrid.SetBackColor1(RGB(255,251,247));
	m_StdGrid.SetBackColor2(RGB(251,255,247));
	m_StdGrid.SetBackColorBkg(RGB(200,200,200)); 
	m_StdGrid.SetScrollBarStyle(0);

	m_StdGrid.SetCols(4); //m_StdGrid.SetFixedCols(1);
	m_StdGrid.SetRows(1); //m_StdGrid.SetFixedRows(1);
	m_StdGrid.Cell(0,1).SetText("镀层/元素");
	m_StdGrid.Cell(0,2).SetText("厚度(um)/含量(%)");
	m_StdGrid.Cell(0,3).SetText("模式");
	
	m_StdGrid.Column(3).SetCellType(1);
	m_StdGrid.Column(3).SetUserSortIndicator(3);
	comboBox=m_StdGrid.ComboBox(3); // col 2
	comboBox.AddItem("存在");
	comboBox.AddItem("不存在");
	comboBox.AddItem("无限厚度");

	FillGrid();
	m_StdName = m_pStdSample->m_tag;
	UpdateData(FALSE); //Variable -> Interface
	
	return TRUE;  
}

void CConfOneStdDlg::LinkValue(BOOL bNew,CStandardSample* ptr)
{
	m_bNewFile = bNew;
	m_pStdSample = ptr;
}

void CConfOneStdDlg::FillGrid()
{
	//m_bInitialized = FALSE;
	CString text;
	m_StdGrid.ShowWindow(SW_HIDE);
	m_StdGrid.SetAutoRedraw(FALSE);	
	while(m_StdGrid.GetRows()>1) m_StdGrid.RemoveItem(1); // clean exist rows
	
	int row = 1;
	OneLayerDefine* pOneLayer;
	int nLayer = m_pStdSample->m_layerArray.GetSize();
	for(int i=0;i<nLayer;i++)
	{
		pOneLayer = m_pStdSample->m_layerArray.GetAt(i);
		text.Format("%d",i+1);
		m_StdGrid.AddItem(text,FALSE);
		
		text.Format("D%d ",i+1);
		text += pOneLayer->GetLayerName();
		m_StdGrid.Cell(row,1).SetText(text);          // (1)layer name
		m_StdGrid.Cell(row,1).SetLocked(TRUE);
		
		if(pOneLayer->m_isSaturated)
		{
			m_StdGrid.Cell(row,2).SetText(" -- ");
		}
		else
		{
			text.Format("%6.3lf",pOneLayer->m_thickness); // (2)layer thickness
			m_StdGrid.Cell(row,2).SetText(text);
		}

		
		
		if(pOneLayer->m_isExist)                      // (3)layer mode
		{
			m_StdGrid.Cell(row,3).SetText("存在"); 
		} 
		else 
		{
			m_StdGrid.Cell(row,3).SetText("不存在");
		}
		if(pOneLayer->m_isSaturated)
		{
			m_StdGrid.Cell(row,3).SetText("无限厚度");
		}
		row++;
		
		
		//if(pOneLayer->m_isExist){
		if(pOneLayer->m_contentMode==0) //0,calculate  1,correct  2,known
		{
			int nElem = pOneLayer->m_elementList.GetSize();
			for(int j=0;j<nElem;j++)
			{
				CClassicalElement *pOneEle = pOneLayer->m_elementList.GetAt(j);
				text.Format("%d",i+1);
				m_StdGrid.AddItem(text,FALSE);
				
				text.Format("   -- %s",pOneEle->Name);
				m_StdGrid.Cell(row,1).SetText(text);  // (1)Element Name
				m_StdGrid.Cell(row,1).SetLocked(TRUE); 
				
				text.Format("%5.2f",pOneEle->Content);  // (2)Element Content
				m_StdGrid.Cell(row,2).SetText(text);
				
				m_StdGrid.Cell(row,3).SetText("-");
				m_StdGrid.Cell(row,3).SetLocked(TRUE);
				
				row++;
			}
		}
		//} // END IF LAYER EXIST
		
		
		//if(pOneLayer->m_isSaturated) break;
	}
	
	//m_StdGrid.ExpandColumnsToFit();
	m_StdGrid.SetAutoRedraw(TRUE);	
	m_StdGrid.Refresh();
	m_StdGrid.ShowWindow(SW_SHOW);
	//OnSelchangeMethodcombo();
	
	//m_bInitialized = TRUE;
}

void CConfOneStdDlg::OnOK() 
{
	UpdateData(TRUE); //Interface -> Variable

	// Standard Sample's Name is necessary
	if(m_StdName.IsEmpty())
	{
		AfxMessageBox("请输入标样名称\n");
		return;
	}

	int row = 1;
	CString text;
	OneLayerDefine* pOneLayer;
	m_pStdSample->m_tag = m_StdName;
	int nLayer = m_pStdSample->m_layerArray.GetSize();
	for(int i=0;i<nLayer;i++)
	{
		pOneLayer = m_pStdSample->m_layerArray.GetAt(i);
		pOneLayer->m_thickness = (float)(m_StdGrid.Cell(row,2).GetDoubleValue());

		text = m_StdGrid.Cell(row,3).GetText();
		text.TrimLeft();text.TrimRight();
		if(text=="存在")
			pOneLayer->m_isExist = TRUE;
		else if(text=="不存在")
			pOneLayer->m_isExist = FALSE;
		else if(text=="无限厚度")
			pOneLayer->m_isSaturated = TRUE;

		row++;

		if(pOneLayer->m_contentMode==0) //0,calculate  1,correct  2,known
		{
			int nElem = pOneLayer->m_elementList.GetSize();
			for(int j=0;j<nElem;j++)
			{
				CClassicalElement *pOneEle = pOneLayer->m_elementList.GetAt(j);
				pOneEle->Content = (float)(m_StdGrid.Cell(row,2).GetDoubleValue());
				row++;
			}
		}
		//pOneLayer->CalcDensity();
		pOneLayer->CalcAverageDensity();
	}
	
	CDialog::OnOK();
}

void CConfOneStdDlg::OnCancel() 
{
	if(m_bNewFile) delete m_pStdSample;
	CDialog::OnCancel();
}


