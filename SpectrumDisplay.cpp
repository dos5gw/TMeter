// SpectrumDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "TMeter.h"
#include "SpectrumDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_SPEC_COUNT 12

static int _color_index = 0;

/*COLORREF reverse order RGB*/
COLORREF _color_list[MAX_SPEC_COUNT] = {
	0x00FF00, //0 Green
	0x6A6AFF, //7 IndianRed1
	0xFFFF00, //8 Cyan
	0x008B8B, //2 Brown
	0x85E6EE, //1 Khaki2
	0xFF6EB4,  //10 Violet
	0xFF1010, //4 Dark Blue
	0xA020F0, //9 Purple
	0x458B74, //11 Aquamarine4
	0x00A5FF, //5 Orange
	0x0000EE, //3 Red2
	0xB5B5B5  //6 grey71
};

COLORREF GetNewColor()
{
	if(++_color_index > MAX_SPEC_COUNT-1) _color_index = 0;
	return _color_list[_color_index];
}

COLORREF GetNewColorAt(int i)
{
	if(i > MAX_SPEC_COUNT-1) i = 0;
	return _color_list[i];
}

/////////////////////////////////////////////////////////////////////////////
// CSpectrumDisplay

CSpectrumDisplay::CSpectrumDisplay()
{
	//yExtendFactor=1;
	//xExtendFactor=1;
	m_xSpace=2;
	m_ySpace=4;
	m_Cursor=1;
	m_canMoveXAxis=FALSE;
	m_beNeedFilled=TRUE;
	m_Axis_Y.SetExtendFactor(1.1);
	m_Axis_X.SetScale(0,SPEC_SIZE);
	m_baseLine=10;
	m_topLine=245;
	m_beSetZoneMode=FALSE;
}

CSpectrumDisplay::~CSpectrumDisplay()
{
	m_MemDC.SelectObject(m_pOldBitmap);
	m_Bitmap.DeleteObject();
	m_MemDC.DeleteDC();
	FreeAnalysisLineArray();
	RemoveAllContrastSpec();
}


BEGIN_MESSAGE_MAP(CSpectrumDisplay, CWnd)
	//{{AFX_MSG_MAP(CSpectrumDisplay)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectrumDisplay message handlers
void CSpectrumDisplay::UpdateMaxCount()
{
	// see CSpectrumInfo<_TSPEC,MAX>::CalculatePeak()
	if((pSpectrum->m_totalTimes==0)||(pSpectrum->m_currentTimes==0))
	{
		maxCount=0;
	}
	else
	{
		maxCount = (pSpectrum->m_maxValue)/(pSpectrum->m_currentTimes*10);
	}
}

void CSpectrumDisplay::OnPaint() 
{
	CPaintDC dc(this);
	GetClientRect(m_rcClient);
	if (m_MemDC.GetSafeHdc()!=NULL) 
	{
		dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
	}
}

void CSpectrumDisplay::ExtendYAxis()
{
	m_Axis_Y.extendFactor /= 2.0;
	Update();
}

void CSpectrumDisplay::ContractYAxis()
{
	m_Axis_Y.extendFactor *= 2.0;
	Update();
}

void CSpectrumDisplay::ExtendXAxis()
{
	m_canMoveXAxis=TRUE;
	m_Axis_X.extendFactor = 1.0;
	m_Axis_X.SetScale(0,128);
	Update();
}

void CSpectrumDisplay::ContractXAxis()
{
	m_canMoveXAxis=FALSE;
	m_Axis_X.extendFactor = 1.0;
	m_Axis_X.SetScale(0,SPEC_SIZE);
	Update();
}

void CSpectrumDisplay::XAxisMoveLeft()
{
	if(!m_canMoveXAxis) return;
	if(m_Axis_X.minRange-20>0)
	{
		m_Axis_X.minRange -= 20;
		m_Axis_X.maxRange -= 20;
	}
	else
	{
		m_Axis_X.minRange=0;
		m_Axis_X.maxRange=128;
	}
	//m_Axis_X.SetScale(0,128);
	Update();
}

void CSpectrumDisplay::XAxisMoveRight()
{	
	if(!m_canMoveXAxis) return;
	if(m_Axis_X.maxRange+20<SPEC_SIZE)
	{
		m_Axis_X.minRange += 20;
		m_Axis_X.maxRange += 20;
	} 
	else
	{
		m_Axis_X.minRange =SPEC_SIZE/2;
		m_Axis_X.maxRange =SPEC_SIZE;
	}
	Update();
}

void CSpectrumDisplay::FillCurve()
{
	m_beNeedFilled = !m_beNeedFilled;
	Update();
}

void CSpectrumDisplay::ChangeCurrentCursor(int cursor)
{
	m_Cursor = cursor;
	Update();
}

BOOL CSpectrumDisplay::PreChangeCursor(int& channel)
{
	//int cursor;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if(rcShowSpectrum.PtInRect(point))
	{
		if((point.x-rcShowSpectrum.left)<1) return FALSE;
		// point to channel :
		channel = (int)((point.x-rcShowSpectrum.left)/xResolution + m_Axis_X.minRange);
		if(channel>255) channel=255;
		else if(channel<0) channel=0;
		ChangeCurrentCursor(channel);
		return TRUE;
		//UpdateStaticControls();
	}
	return FALSE;
}

BOOL CSpectrumDisplay::ElementExistInArray(CString element)
{
	CAnalysisLine* pEle;
	int no = m_AnalysisLineArray.GetSize();
	for(int i=0;i<no;i++)
	{
		pEle = m_AnalysisLineArray.GetAt(i);
		if(element.CompareNoCase(pEle->Name)==0)
		{
			//remove this node
			m_AnalysisLineArray.RemoveAt(i);
			delete pEle;
			return TRUE;
		}
	}
	return FALSE;
}

void CSpectrumDisplay::AddAnalysisLines(CString element,double kaChannel,double kbChannel)
{
	if(ElementExistInArray(element)) // find all node in array , if node has already exist, remove it and return TRUE
	{
		return;
	}
	else
	{
		CAnalysisLine* pLine = new CAnalysisLine(element,kaChannel,kbChannel,GetNewColor());
		m_AnalysisLineArray.Add(pLine);
	}
}

void CSpectrumDisplay::CleanAnalysisLines()
{
	//delete all node 
}

void CSpectrumDisplay::FreeAnalysisLineArray()
{
	CAnalysisLine* pLine;
	if(!m_AnalysisLineArray.IsEmpty())
	{
		int no = m_AnalysisLineArray.GetSize();
		for(int i=0;i<no;i++)
		{
			pLine=m_AnalysisLineArray.GetAt(i);
			delete pLine;
		}
		m_AnalysisLineArray.RemoveAll();
	}
}

void CSpectrumDisplay::Update()
{
	InvalidateCtrl();
	UpdateMaxCount();
	GetClientRect(m_rcClient);
	DrawBitmap(&m_MemDC,m_rcClient); //modified by Chen @2012-06-15	
	Invalidate(FALSE); //Do Not Erase m_rcClient background, and send WM_PAINT to hWnd
}

void CSpectrumDisplay::InvalidateCtrl()
{
	CClientDC dc(this);
	GetClientRect(m_rcClient); 
	if (m_MemDC.GetSafeHdc() == NULL) 
	{
		m_MemDC.CreateCompatibleDC(&dc); //Need DeleteDC() ?
		m_Bitmap.CreateCompatibleBitmap(&dc,m_rcClient.Width(),m_rcClient.Height());
		m_pOldBitmap=m_MemDC.SelectObject(&m_Bitmap); //@ 2012-06-15
		m_MemDC.SetBkColor(RGB(100,100,100));
		CBrush bkBrush(RGB(255,255,255));
		m_MemDC.FillRect(m_rcClient,&bkBrush);
	}
	InvalidateRect(m_rcClient, TRUE); //Erase m_rcClient background, and send WM_PAINT to hWnd
}

void CSpectrumDisplay::AttachSpectrumInfo(CSpectrumInfo<DWORD,SPEC_SIZE> *p)
{
	pSpectrum=p;
}

void CSpectrumDisplay::OnSize(UINT nType, int cx, int cy) 
{
	//CStatic::OnSize(nType, cx, cy);
	Update();
}

void CSpectrumDisplay::DrawBitmap(CDC* pDC,CRect rect)
{
	int fontWidth=rect.Width()/60;
	if(fontWidth<10) fontWidth=10;
	m_font.CreateFont(-fontWidth,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_TT_ALWAYS | CLIP_LH_ANGLES,DEFAULT_QUALITY, FF_MODERN | TMPF_TRUETYPE,"Arial");
	CFont* pOldFont=m_MemDC.SelectObject(&m_font);
	m_CharSize=m_MemDC.GetTextExtent("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	m_CharSize.cx/=62;
	m_xSpace=m_CharSize.cx/2;
	m_ySpace=m_CharSize.cy/2;

	m_Axis_Y.SetScale(0,maxCount);

	rcShowSpectrum=rect;
	rcShowSpectrum.left += m_CharSize.cx*m_Axis_Y.nJep + m_xSpace + 10;
	rcShowSpectrum.bottom -= m_CharSize.cy + m_ySpace +10;

	DrawBackground(pDC,rect,rcShowSpectrum);
	DrawAxis(pDC,rect,rcShowSpectrum);

	//DrawSpectrum(pDC,rect,rcShowSpectrum); // modified @2012-07-11
	DrawSpectrumPolygon(pDC,rect,rcShowSpectrum); 

	//DrawContrastSpectrum(pDC,rect,rcShowSpectrum); // modified @2012-07-11
	DrawContrastSpectrumPolygon(pDC,rect,rcShowSpectrum); 

	DrawSketchSpectrum(pDC,rect,rcShowSpectrum); // added @2012-07-09
	DrawAnalysisLines(pDC,rect,rcShowSpectrum);

	m_MemDC.SelectObject(pOldFont);
	m_font.DeleteObject();
}

void CSpectrumDisplay::DrawBackground(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	if(pDC->GetSafeHdc() != NULL) {
		pDC->FillRect(rect,&CBrush(RGB(200,200,200)));
		pDC->FillRect(rcShowSpectrum,&CBrush(RGB(240,255,240)));
	}
}

void CSpectrumDisplay::DrawAxis(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int i;
	int xpos,ypos;
	CString scale;
	CSize scaleSize;
	CPen pen_Grid(PS_SOLID,1,RGB(220,220,220));

	if(pDC->GetSafeHdc() != NULL) {
		pDC->SetBkColor(RGB(200,200,200)); //set bk Color for font
		pDC->SetTextColor(RGB(0,0,0));     //set text Color for font

		CPen *pOldPen=pDC->SelectObject(&pen_Grid);
		//draw x-axis
		xResolution=rcShowSpectrum.Width()/((m_Axis_X.maxRange-m_Axis_X.minRange)*m_Axis_X.extendFactor);
		pDC->MoveTo(rcShowSpectrum.left,rcShowSpectrum.bottom);
		pDC->LineTo(rcShowSpectrum.right,rcShowSpectrum.bottom);
		for(i=1;i<=m_Axis_X.nSteps;i++)
		{
			xpos = (int)(i*m_Axis_X.mainStep * xResolution);
			pDC->MoveTo((rcShowSpectrum.left+xpos),rcShowSpectrum.bottom);
			pDC->LineTo((rcShowSpectrum.left+xpos),rcShowSpectrum.top);
			//scale.Format(m_Axis_X.scaleFormat,(m_Axis_X.mainStep*i - m_Axis_X.minRange));
			scale.Format("%3d",(int)(m_Axis_X.mainStep*i+m_Axis_X.minRange));
			scaleSize=pDC->GetTextExtent(scale);
			xpos=xpos+scaleSize.cx/2 + m_xSpace*2;
			ypos=rcShowSpectrum.bottom + m_ySpace;
			if(xpos>rcShowSpectrum.right-scaleSize.cx) xpos=rcShowSpectrum.right-scaleSize.cx;
			pDC->TextOut(xpos,ypos,scale);
		}
		//draw y-axis
		yResolution=rcShowSpectrum.Height()/((m_Axis_Y.maxRange-m_Axis_Y.minRange)*m_Axis_Y.extendFactor); // 2012-06-11
		pDC->MoveTo(rcShowSpectrum.left,rcShowSpectrum.bottom);
		pDC->LineTo(rcShowSpectrum.left,rcShowSpectrum.top);
		for(i=1;i<=m_Axis_Y.nSteps;i++)
		{
			ypos = (int)(i*m_Axis_Y.mainStep*yResolution);
			pDC->MoveTo(rcShowSpectrum.left,(rcShowSpectrum.bottom-ypos));
			pDC->LineTo(rcShowSpectrum.right,(rcShowSpectrum.bottom-ypos));
			scale.Format(m_Axis_Y.scaleFormat,m_Axis_Y.mainStep*i);
			scale.Format(m_Axis_Y.scaleFormat,m_Axis_Y.mainStep*i);
			scaleSize=pDC->GetTextExtent(scale);
			xpos=rect.left+m_xSpace/2; //rcShowSpectrum.left - (scaleSize.cx+m_xSpace);
			ypos=rcShowSpectrum.bottom-ypos; //ypos-scaleSize.cy/2;
			pDC->TextOut(xpos,ypos,scale);
		}
		pDC->SelectObject(pOldPen);
	}
	pen_Grid.DeleteObject();
}

void CSpectrumDisplay::DrawSpectrumPolygon(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	double count;
	int xpos,ypos,i;
	CBrush br_SteelBlue(RGB(70,130,180));
	CBrush br_CadetBlue(RGB(122,197,205));
	CRgn rgn,rgnLeft,rgnRight;
	CPoint pt[SPEC_SIZE];
	if(m_baseLine<0) m_baseLine=0;
	if(m_topLine>SPEC_SIZE-1) m_topLine=SPEC_SIZE-1;

	if(pDC->GetSafeHdc() != NULL) {

		//if(!pSpectrum->m_bNeedDisplay || !m_beNeedFilled) goto skipDrawPolygon;
		if(pSpectrum->m_bNeedDisplay && m_beNeedFilled){
			for(i=1;i<SPEC_SIZE-1 ;i++)
			{
				count = pSpectrum->GetCountClone(i);
				if(count<0) count=0;
				pt[i].x=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution);
				pt[i].y=(int)(rcShowSpectrum.bottom - count*yResolution);
			}
			pt[0].x = rcShowSpectrum.left;
			pt[0].y = rcShowSpectrum.bottom;
			pt[SPEC_SIZE-1].x = rcShowSpectrum.left;
			pt[SPEC_SIZE-1].y = rcShowSpectrum.bottom;
			
			rgn.CreatePolygonRgn(pt,SPEC_SIZE,ALTERNATE); //rgn.CreatePolygonRgn(pt,SPEC_SIZE,WINDING);
			if((m_topLine-m_baseLine)<=3) {
				pDC->FillRgn(&rgn, &br_CadetBlue); // m_topLine be closed to m_baseLine
			}
			else{
				pDC->FillRgn(&rgn, &br_SteelBlue);
				
				//fill [i]<m_baseLine, and [i]>m_topLine
				pt[m_baseLine+1].x = pt[m_baseLine].x;
				pt[m_baseLine+1].y = rcShowSpectrum.bottom;
				rgnLeft.CreatePolygonRgn(pt,m_baseLine+2,ALTERNATE);
				pDC->FillRgn(&rgnLeft, &br_CadetBlue);
				
				pt[m_topLine-1].x = pt[m_topLine].x;
				pt[m_topLine-1].y = rcShowSpectrum.bottom;
				rgnRight.CreatePolygonRgn(&pt[m_topLine-1],SPEC_SIZE-m_topLine+1,ALTERNATE);
				pDC->FillRgn(&rgnRight, &br_CadetBlue);
			}
		}

		CString text;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		xpos=rcShowSpectrum.left+m_xSpace;
		ypos=rcShowSpectrum.top+m_ySpace; 
		text.Format("Peak=%4.2lf",pSpectrum->m_peakPosDword);
		pDC->TextOut(xpos,ypos,text);

		ypos=ypos+m_ySpace*3;
		text.Format("Precision=%4.2lf%c",pSpectrum->m_precision,'%');
		pDC->TextOut(xpos,ypos,text);

		ypos=ypos+m_ySpace*3;
		text.Format("All Cps=%3.1lf",pSpectrum->m_cps);
		pDC->TextOut(xpos,ypos,text);

		ypos=ypos+m_ySpace*3;
		text.Format("Seconds=%d",10*pSpectrum->m_currentTimes);
		pDC->TextOut(xpos,ypos,text);
	}
		
	br_SteelBlue.DeleteObject();
	br_CadetBlue.DeleteObject();
	rgn.DeleteObject();
	rgnLeft.DeleteObject();
	rgnRight.DeleteObject();
}

void CSpectrumDisplay::DrawContrastSpectrumPolygon(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int num = m_ContrastSpectrum.GetSize();
	if(num == 0) return;

	//int xpos,ypos;
	double count;
	//BOOL bNeedDraw=FALSE;
	//int maxX=(int)m_Axis_X.maxRange;
	//int minX=(int)m_Axis_X.minRange;
	
	CPoint pt[SPEC_SIZE];
	CBrush brush(RGB(70,130,180));
	CSpectrumInfo<DWORD,SPEC_SIZE> *pSpec=NULL;
	
	if(pDC->GetSafeHdc() != NULL) {
		for(int loop=0;loop<num;loop++){
			pSpec = m_ContrastSpectrum.GetAt(loop);
			if(!pSpec->m_bNeedDisplay) continue;
			if(m_beNeedFilled){
				CRgn rgn;
				brush.DeleteObject();
				brush.CreateSolidBrush(pSpec->m_color);
				for(int i=1;i<SPEC_SIZE-1 ;i++)
				{
					count = pSpec->GetCountClone(i);
					if(count<0) count=0;
					pt[i].x=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution);
					pt[i].y=(int)(rcShowSpectrum.bottom - count*yResolution);
				}
				pt[0].x = rcShowSpectrum.left;
				pt[0].y = rcShowSpectrum.bottom;
				pt[SPEC_SIZE-1].x = rcShowSpectrum.left;
				pt[SPEC_SIZE-1].y = rcShowSpectrum.bottom;

				rgn.CreatePolygonRgn(pt,SPEC_SIZE,ALTERNATE);
				pDC->FillRgn(&rgn, &brush);
				rgn.DeleteObject();
			} // end if(m_beNeedFilled)
		}
	}
	brush.DeleteObject();
}

void CSpectrumDisplay::DrawSpectrum(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int i;
	int xpos,ypos;
	CString text;
	double count;
	BOOL bNeedDraw=FALSE;
	int maxX=(int)m_Axis_X.maxRange;
	int minX=(int)m_Axis_X.minRange;

	int xposf=0,yposf=0;
	BOOL FirstPoint = TRUE;

	CPen pen_Spec_SteelBlue(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(70,130,180)); 
	CPen pen_Spec_CadetBlue(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(122,197,205));

	if(pDC->GetSafeHdc() != NULL) {
		CPen *pOldPen=pDC->SelectObject(&pen_Spec_SteelBlue); //set default color
		if(!pSpectrum->m_bNeedDisplay || !m_beNeedFilled) goto skipDraw;

		for(i=minX;i<maxX;i++)
		{
			if((i<m_baseLine)||(i>m_topLine)) pDC->SelectObject(&pen_Spec_CadetBlue);
			else pDC->SelectObject(&pen_Spec_SteelBlue);
			count=pSpectrum->GetCountClone(i);
			if(count>1.0e-5) bNeedDraw=TRUE;
			else bNeedDraw=FALSE;
			if(bNeedDraw)
			{
				int nWidth=0;
				while(nWidth<xResolution)
				{
					//xpos=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution + nWidth);
					xpos=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution - xResolution/2.0 +0.5 + nWidth); // @2012-07-08
					ypos=(int)(rcShowSpectrum.bottom - count*yResolution);
					pDC->MoveTo(xpos,rcShowSpectrum.bottom-1);
					pDC->LineTo(xpos,ypos);
					nWidth++;
				}
			}
		}
skipDraw: 		
		//draw spectrum information
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		xpos=rcShowSpectrum.left+m_xSpace;
		ypos=rcShowSpectrum.top+m_ySpace; 
		text.Format("Peak=%4.2lf",pSpectrum->m_peakPosDword);
		pDC->TextOut(xpos,ypos,text);
		ypos=ypos+m_ySpace*3;
		text.Format("Precision=%4.2lf%c",pSpectrum->m_precision,'%');
		pDC->TextOut(xpos,ypos,text);
		ypos=ypos+m_ySpace*3;
		text.Format("Cps=%3.1lf",pSpectrum->m_cps);
		pDC->TextOut(xpos,ypos,text);
		ypos=ypos+m_ySpace*3;
		text.Format("Seconds=%d",10*pSpectrum->m_currentTimes);
		pDC->TextOut(xpos,ypos,text);
		//ypos=ypos+m_ySpace*3;
		//text.Format("Cursor Cps=%3.1lf",pSpectrum->GetCountClone(m_Cursor));
		//pDC->TextOut(xpos,ypos,text);
		//ypos=ypos+m_ySpace*3;
		//text.Format("K Alpa=%3.1lf",m_Cursor*22.1/170);
		//pDC->TextOut(xpos,ypos,text);
		pDC->SelectObject(pOldPen);
	}
	pen_Spec_SteelBlue.DeleteObject();
	pen_Spec_CadetBlue.DeleteObject();
}

void CSpectrumDisplay::DrawContrastSpectrum(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int num = m_ContrastSpectrum.GetSize();
	if(num == 0) return;
	int i;
	int xpos,ypos;
	double count;
	BOOL bNeedDraw=FALSE;
	int maxX=(int)m_Axis_X.maxRange;
	int minX=(int)m_Axis_X.minRange;
	CSpectrumInfo<DWORD,SPEC_SIZE> *pSpec=NULL;
	CPen pen_Spec(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(0,0,0)); 

	if(pDC->GetSafeHdc() != NULL) {
		CPen *pOldPen=pDC->SelectObject(&pen_Spec); //set default colour
		for(int loop=0;loop<num;loop++)
		{
			pSpec = m_ContrastSpectrum.GetAt(loop);
			if(!pSpec->m_bNeedDisplay) continue;
			pen_Spec.DeleteObject();
			pen_Spec.CreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,pSpec->m_color);
			pDC->SelectObject(&pen_Spec); 
			if(m_beNeedFilled)
			{
				for(i=minX;i<maxX;i++)
				{
					//if((i<pSpectrum->m_baseline)||(i>pSpectrum->m_topline)) pDC->SelectObject(&pen_Spec_CadetBlue);
					//else pDC->SelectObject(&pen_Spec_SteelBlue);
					count=pSpec->GetCountClone(i);
					if(count>1.0e-5) bNeedDraw=TRUE;
					else bNeedDraw=FALSE;
					if(bNeedDraw)
					{
						int nWidth=0;
						while(nWidth<xResolution)
						{
							//xpos=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution + nWidth);
							xpos=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution - xResolution/2.0 +0.5 + nWidth); // @2012-07-08
							ypos=(int)(rcShowSpectrum.bottom - count*yResolution);
							pDC->MoveTo(xpos,rcShowSpectrum.bottom-1);
							pDC->LineTo(xpos,ypos);
							nWidth++;
						}
					}
				}
			}//end if(m_beNeedFilled)
		}
		pDC->SelectObject(pOldPen);
	}
	pen_Spec.DeleteObject();
}

void CSpectrumDisplay::DrawSketchSpectrum(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int i;
	int xpos,ypos,xposf,yposf;
	int maxX=(int)m_Axis_X.maxRange;
	int minX=(int)m_Axis_X.minRange;
	double count;
	CSpectrumInfo<DWORD,SPEC_SIZE> *pSpec=NULL;
	CPen pen_Spec(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(0,0,0)); 
	
	if(pDC->GetSafeHdc() != NULL) {
		CPen *pOldPen=pDC->SelectObject(&pen_Spec);

		int num = m_ContrastSpectrum.GetSize();
		for(int loop=num;loop>=0;loop--)
		{
			if(loop>0) pSpec = m_ContrastSpectrum.GetAt(loop-1);
			else pSpec = pSpectrum; // global spectrum
			if(!pSpec->m_bNeedDisplay) continue;
			pen_Spec.DeleteObject();
			pen_Spec.CreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,pSpec->m_color);
			pDC->SelectObject(&pen_Spec);
			
			//start drawing outline sketch
			BOOL FirstPoint = TRUE;
			for(i=minX;i<maxX;i++) 
			{
				count=pSpec->GetCountClone(i);
				xpos=(int)(rcShowSpectrum.left+(i-m_Axis_X.minRange)*xResolution);
				ypos=(int)(rcShowSpectrum.bottom-count*yResolution);
				if(FirstPoint)
				{
					xposf=xpos;
					yposf=ypos;
					FirstPoint=FALSE;
				} 
				else
				{
					pDC->MoveTo(xposf,yposf);
					pDC->LineTo(xpos,ypos);
					xposf=xpos;
					yposf=ypos;
				}
			}
		}
		pDC->SelectObject(pOldPen);
	}
	pen_Spec.DeleteObject();
}

void CSpectrumDisplay::DrawAnalysisLines(CDC* pDC,CRect rect,CRect rcShowSpectrum)
{
	int no = m_AnalysisLineArray.GetSize();
	//if(no==0) return;
	int xpos,ypos;
	int xposKa,xposKb;
	int yposKa=rcShowSpectrum.bottom - rcShowSpectrum.Height()*1/2;
	int yposKb=rcShowSpectrum.bottom - rcShowSpectrum.Height()*1/8;
	CPen pen_Line(PS_SOLID,1,RGB(0,0,0));
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetTextColor(RGB(0,0,0));

	if(pDC->GetSafeHdc() != NULL) {
		CPen *pOldPen=pDC->SelectObject(&pen_Line);

		xpos=(int)(rcShowSpectrum.left+(m_Cursor-m_Axis_X.minRange)*xResolution);
		ypos=rcShowSpectrum.top;
		pDC->MoveTo(xpos,ypos);
		pDC->LineTo(xpos,rcShowSpectrum.bottom);

		if(m_beSetZoneMode)
		{
			pen_Line.DeleteObject();
			//pen_Line.CreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(122,197,205));
			pen_Line.CreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(70,130,180));
			pDC->SelectObject(&pen_Line);
			xpos=(int)(rcShowSpectrum.left+(m_baseLine-m_Axis_X.minRange)*xResolution);
			ypos=rcShowSpectrum.top;
			pDC->MoveTo(xpos,ypos);
			pDC->LineTo(xpos,rcShowSpectrum.bottom);
			xpos=(int)(rcShowSpectrum.left+(m_topLine-m_Axis_X.minRange)*xResolution);
			ypos=rcShowSpectrum.top;
			pDC->MoveTo(xpos,ypos);
			pDC->LineTo(xpos,rcShowSpectrum.bottom);
		}

		CAnalysisLine* pLine;
		for(int i=0;i<no;i++)
		{
			pLine = m_AnalysisLineArray.GetAt(i);
			pen_Line.DeleteObject();
			pen_Line.CreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,pLine->Color);
			pDC->SelectObject(&pen_Line);     //set curve color
			pDC->SetTextColor(pLine->Color);  //set text color for character
			xposKa=(int)(rcShowSpectrum.left + (pLine->KaChannel - m_Axis_X.minRange)*xResolution);
			xposKb=(int)(rcShowSpectrum.left + (pLine->KbChannel - m_Axis_X.minRange)*xResolution);
			pDC->MoveTo(xposKa,yposKa);
			pDC->LineTo(xposKa,rcShowSpectrum.bottom);
			pDC->TextOut(xposKa,yposKa-m_ySpace*2,pLine->Name);

			pDC->MoveTo(xposKb,yposKb);
			pDC->LineTo(xposKb,rcShowSpectrum.bottom);
		}
		pDC->SetTextColor(RGB(0,0,0)); // set default color for character
		pDC->SelectObject(pOldPen);
	}
	pen_Line.DeleteObject();
}

/* @2012-06-12 */
void CSpectrumDisplay::DrawAll(CDC* pDC,CRect rect)
{
	int i;
	int xpos,ypos;
	CString scale,text;
	CSize scaleSize;

	CPen pen_Black(PS_SOLID,1,RGB(0,0,0));
	CPen pen_Grid(PS_SOLID,1,RGB(220,220,220));
	CPen pen_Gray(PS_SOLID,1,RGB(140,140,140));
	COLORREF mfcDefaultBg = RGB(236,233,216); //MFC Default Color

	int fontWidth=rect.Width()/50;
	if(fontWidth<12) fontWidth=12;
	m_font.CreateFont(-fontWidth, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,CLIP_TT_ALWAYS | CLIP_LH_ANGLES,DEFAULT_QUALITY, FF_MODERN | TMPF_TRUETYPE,"Arial");
	CFont* pOldFont=m_MemDC.SelectObject(&m_font);
	m_CharSize=m_MemDC.GetTextExtent("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	m_CharSize.cx/=62;
	m_xSpace=m_CharSize.cx/2;
	m_ySpace=m_CharSize.cy/2;

	m_Axis_Y.SetScale(0,maxCount);

	CRect rcShowSpectrum=rect;
	rcShowSpectrum.left += m_CharSize.cx*m_Axis_Y.nJep + m_xSpace + 10;
	rcShowSpectrum.bottom -= m_CharSize.cy + m_ySpace +10;

	//choose pen's width
	//int penWidth=(int)(rcShowSpectrum.Width()/((m_Axis_X.maxRange-m_Axis_X.minRange)*m_Axis_X.extendFactor) + 1);
	CPen pen_Spec_IndianRed(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(255,106,106)); 
	CPen pen_Spec_CadetBlue(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE|PS_JOIN_BEVEL,1,RGB(122,197,205));
	CPen pen_Spec_Curve(PS_SOLID,1,RGB(0,0,0));

	if(pDC->GetSafeHdc() != NULL) {

		CPen *pOldPen=pDC->SelectObject(&pen_Grid);
		pDC->FillRect(rect,&CBrush(RGB(200,200,200)));
		pDC->FillRect(rcShowSpectrum,&CBrush(RGB(240,255,240)));
		pDC->SetBkColor(RGB(200,200,200)); //set bk Color for font
		pDC->SetTextColor(RGB(0,0,0));     //set text Color for font

		pDC->MoveTo(rect.left,rect.bottom-1);
		pDC->LineTo(rect.right-1,rect.bottom-1);
		pDC->LineTo(rect.right-1,rect.top);
		pDC->LineTo(rect.left,rect.top);
		pDC->LineTo(rect.left,rect.bottom);

		//pDC->SelectObject(&pen_Gray);

		//draw x-axis
		xResolution=rcShowSpectrum.Width()/((m_Axis_X.maxRange-m_Axis_X.minRange)*m_Axis_X.extendFactor);
		pDC->MoveTo(rcShowSpectrum.left,rcShowSpectrum.bottom);
		pDC->LineTo(rcShowSpectrum.right,rcShowSpectrum.bottom);
		for(i=1;i<=m_Axis_X.nSteps;i++)
		{
			xpos = (int)(i*m_Axis_X.mainStep * xResolution);

			pDC->MoveTo((rcShowSpectrum.left+xpos),rcShowSpectrum.bottom);
			pDC->LineTo((rcShowSpectrum.left+xpos),rcShowSpectrum.top);

			//scale.Format(m_Axis_X.scaleFormat,(m_Axis_X.mainStep*i - m_Axis_X.minRange));
			scale.Format("%3d",(int)(m_Axis_X.mainStep*i+m_Axis_X.minRange));
			scaleSize=pDC->GetTextExtent(scale);
			xpos=xpos+scaleSize.cx/2 + m_xSpace*2;
			ypos=rcShowSpectrum.bottom + m_ySpace;
			if(xpos>rcShowSpectrum.right-scaleSize.cx) xpos=rcShowSpectrum.right-scaleSize.cx;
			pDC->TextOut(xpos,ypos,scale);
		}

		//draw y-axis
		yResolution=rcShowSpectrum.Height()/((m_Axis_Y.maxRange-m_Axis_Y.minRange)*m_Axis_Y.extendFactor); // 2012-06-11
		pDC->MoveTo(rcShowSpectrum.left,rcShowSpectrum.bottom);
		pDC->LineTo(rcShowSpectrum.left,rcShowSpectrum.top);
		for(i=1;i<=m_Axis_Y.nSteps;i++)
		{
			ypos = (int)(i*m_Axis_Y.mainStep*yResolution);
			pDC->MoveTo(rcShowSpectrum.left,(rcShowSpectrum.bottom-ypos));
			pDC->LineTo(rcShowSpectrum.right,(rcShowSpectrum.bottom-ypos));
			
			scale.Format(m_Axis_Y.scaleFormat,m_Axis_Y.mainStep*i);
			scale.Format(m_Axis_Y.scaleFormat,m_Axis_Y.mainStep*i);
			scaleSize=pDC->GetTextExtent(scale);
			xpos=rect.left+m_xSpace/2; //rcShowSpectrum.left - (scaleSize.cx+m_xSpace);
			ypos=rcShowSpectrum.bottom-ypos; //ypos-scaleSize.cy/2;
			pDC->TextOut(xpos,ypos,scale);
		}
		/*********draw spectrum & curve @2012-06-15 ***********************************
		double count;
		BOOL bNeedDraw=FALSE;
		BOOL FirstPoint=TRUE;
		int maxX=(int)m_Axis_X.maxRange;
		int minX=(int)m_Axis_X.minRange;
		CBrush specBrush;
		specBrush.CreateSolidBrush(RGB(255,106,106));
		double polygon[maxX-minX+2];
		for(i=minX+1;i<(maxX-minX+1);i++) 
		{
			count=pSpectrum->GetCountClone(i);
			polygon[i].x==(int)(rcShowSpectrum.left+(i-m_Axis_X.minRange)*xResolution);
			polygon[i].y=(int)(rcShowSpectrum.bottom-count*yResolution);
		}
		CRgn specRgn;
		specRgn.CreatePolygonRgn(polygon,maxX-minX+2,ALTERNATE);
		pDC->FillRgn(&specRgn,&specBrush);
		*********************************************************************************/

		/*********draw spectrum & curve*********/
		double count;
		BOOL bNeedDraw=FALSE;
		int maxX=(int)m_Axis_X.maxRange;
		int minX=(int)m_Axis_X.minRange;
		if(!m_beNeedFilled) goto skipFillCurve;
		for(i=minX;i<maxX;i++) //spectrum
		{
			if((i<pSpectrum->m_baseline)||(i>pSpectrum->m_topline)) 
				pDC->SelectObject(&pen_Spec_CadetBlue);
			else 
				pDC->SelectObject(&pen_Spec_IndianRed);

			count=pSpectrum->GetCountClone(i);
			if(count>1.0e-5) bNeedDraw=TRUE;
			else bNeedDraw=FALSE;
			if(bNeedDraw)
			{
				int nWidth=0;
				while(nWidth<xResolution)
				{
					xpos=(int)(rcShowSpectrum.left + (i-m_Axis_X.minRange)*xResolution + nWidth);
					ypos=(int)(rcShowSpectrum.bottom - count*yResolution);
					pDC->MoveTo(xpos,rcShowSpectrum.bottom-1);
					pDC->LineTo(xpos,ypos);
					nWidth++;
				}
			}
		}
skipFillCurve:
		int xposf=0,yposf=0;
		BOOL FirstPoint = TRUE;
		pDC->SelectObject(&pen_Spec_Curve);
		for(i=minX;i<maxX;i++) //curve
		{
			count=pSpectrum->GetCountClone(i);
			xpos=(int)(rcShowSpectrum.left+(i-m_Axis_X.minRange)*xResolution);
			ypos=(int)(rcShowSpectrum.bottom-count*yResolution);
			if(FirstPoint)
			{
				xposf=xpos;
				yposf=ypos;
				FirstPoint=FALSE;
			} 
			else
			{
				pDC->MoveTo(xposf,yposf);
				pDC->LineTo(xpos,ypos);
				xposf=xpos;
				yposf=ypos;
			}
		} 
		
		//draw spectrum information
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		xpos=rcShowSpectrum.left+m_xSpace;
		ypos=rcShowSpectrum.top+m_ySpace; 

		text.Format("PeakPos=%d",pSpectrum->m_peakPos);
		pDC->TextOut(xpos,ypos,text);

		ypos=ypos+m_ySpace*3;
		text.Format("precision=%3.1lf%c",pSpectrum->m_precision,'%');
		pDC->TextOut(xpos,ypos,text);

		ypos=ypos+m_ySpace*3;
		text.Format("cps=%3.1lf",pSpectrum->m_cps);
		pDC->TextOut(xpos,ypos,text);

		pDC->SelectObject(pOldPen);
		//delete pNewPen;	
	}
	m_MemDC.SelectObject(pOldFont);
	m_font.DeleteObject();
}

BOOL CSpectrumDisplay::AddToContrastSpec(CSpectrumInfo<DWORD,SPEC_SIZE> *p,CString file)
{
	int num = m_ContrastSpectrum.GetSize();
	if(num>MAX_SPEC_COUNT) 
	{
		delete p;
		return FALSE;
	}
	p->m_fileName = file;
	p->m_color = GetNewColorAt(num);
	m_ContrastSpectrum.Add(p);
	//Update();
	return TRUE;
}

void CSpectrumDisplay::RemoveAllContrastSpec()
{
	int num = m_ContrastSpectrum.GetSize();
	CSpectrumInfo<DWORD,SPEC_SIZE> *p;
	for(int i=0;i<num;i++)
	{
		p = m_ContrastSpectrum.GetAt(i);
		delete p;
	}
	m_ContrastSpectrum.RemoveAll();
	//Update();
}

void CSpectrumDisplay::RemoveLastContrastSpec()
{
	int num = m_ContrastSpectrum.GetSize();
	if(num<1) return;
	CSpectrumInfo<DWORD,SPEC_SIZE> *p = m_ContrastSpectrum.GetAt(num-1);
	delete p;
	m_ContrastSpectrum.RemoveAt(num-1);
	//Update();
}

void CSpectrumDisplay::SetDafaultScale()
{
	m_canMoveXAxis=FALSE;
	m_Axis_X.SetScale(0,SPEC_SIZE);
	m_Axis_Y.SetExtendFactor(1.1);
	Update();
}

void CSpectrumDisplay::ZeroSpectrum()
{
	pSpectrum->ResetSpectrum();
}
