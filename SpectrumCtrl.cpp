# include <math.h>
#include "stdafx.h"
#include "SpectrumCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

# include "Vector.h"
# include "Matrix.h"
# include <math.h>
/*
void CAxis::ScaleAxis(double min,double max,double base) {
	double stepLen,vt,length;
	int jep,n;
	if(max<min) {
		vt=max;max=min;min=vt;
		}
	length=max-min;
	if(length<=1.0) {
		for(jep=0;;jep++) {
			stepLen=pow(base,-jep);
			if(length>stepLen) {
				scaleFormat.Format("%c%d.%dlf",'%',jep+2,jep);
				break;
				}
			}
		}
	else {
		for(jep=0;;jep++) {
			stepLen=pow(base,jep);
			if(length<=stepLen) {
				jep--;
				stepLen=pow(base,jep);
				if(length<10) scaleFormat.Format("%c%d.1lf",'%',jep+2);
				else scaleFormat.Format("%c%d.0lf",'%',jep);
				break;
				}
			}
		}
	BOOL minReady=FALSE;
	BOOL maxReady=FALSE;
	for(n=-(int)base;;n++) {
		if((!minReady)&&(n*stepLen>min)) {
			minScale=(n-1)*stepLen;
			minReady=TRUE;
			}
		if((!maxReady)&&(n*stepLen>=max)) {
			maxScale=n*stepLen;
			maxReady=TRUE;
			}
		if(minReady&&maxReady) break;
		}
	mainStep=stepLen;
	mainSteps=(int)((maxScale-minScale+1.0e-6)/mainStep);
	switch(int(base)) {
		case 10:
			switch(mainSteps) {
				case 1:
					subSteps=10;
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					subSteps=5;
					break;
				default:
					subSteps=2;
					break;
				}
			break;
		case 4:
			subSteps=4;
			break;
		case 8:
			subSteps=2;
			break;
		default:
			subSteps=1;
			break;
		}
	}

double CAxis::GetWidth() {
	return maxScale-minScale;
	}
*/
CSpectrumCtrlEx::CSpectrumCtrlEx(){
	m_Axis_X.SetTitle(_T("Palse Height (0 - 1024)"));
	m_Axis_Y.SetTitle(_T("Palse Count (cps)"));
	pSpectrum=NULL;
	yExtendFactor=1;
	m_drawMode=NormalMode;
	m_bAllowDeepConfig=FALSE;
	
	m_xSpace=2;
	m_gap=1;
	m_ySpace=4;
	m_leftMargin=5;m_rightMargin=1;
	m_topMargin=1;m_bottomMargin=1;
	m_headerHeight=2;
	m_footerHeight=2;
	}

CSpectrumCtrlEx::~CSpectrumCtrlEx() {
	}


BEGIN_MESSAGE_MAP(CSpectrumCtrlEx, CWnd)
	//{{AFX_MSG_MAP(CSpectrumCtrlEx)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ZOOMINY,OnZoomInY)
	ON_COMMAND(ID_ZOOMOUTY,OnZoomOutY)
	ON_COMMAND(ID_ACTSIZE,OnActSize)
	ON_COMMAND(ID_REDRAW,OnRedraw)
	ON_COMMAND(ID_PRINT,Print)
	ON_COMMAND(ID_SAVEBKG,OnSaveBkgroud)
	ON_COMMAND(ID_SAVESPECT,OnSaveSpectrum)
	ON_COMMAND(ID_LOADSPECT,OnLoadSpectrum)
	ON_COMMAND(ID_STANDSPEC,OnLockSpectrum)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSpectrumCtrlEx::OnSaveBkgroud() {
	CString wdir;
	((CNewEdXrfApp*)AfxGetApp())->GetWorkDirectory(wdir);
	CString filen=wdir+"./Spectrums/"+pSpectrum->GetChannelAt(0)->m_element+"Base.DAB";
	pSpectrum->SaveSpectrum(filen);
	((CNewEdXrfApp *)AfxGetApp())->RestoreCwd();
	}
void CSpectrumCtrlEx::OnSaveSpectrum() {
	CString wdir;
	((CNewEdXrfApp*)AfxGetApp())->GetWorkDirectory(wdir);
	CString filen=pSpectrum->GetChannelAt(0)->m_element+::GetTimeFromDog().Format("%Y%m%d");
	CFileDialog fileDlg(FALSE,"DAT",filen,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"谱文件(*.DAT)|*.DAT||");
	fileDlg.m_ofn.lpstrInitialDir=wdir+"\\Spectrums\\";
	if(fileDlg.DoModal()==IDOK) {
		filen=fileDlg.GetFileName();
		pSpectrum->SaveSpectrum(filen);
		((CNewEdXrfApp *)AfxGetApp())->RestoreCwd();
		}
	}

void CSpectrumCtrlEx::OnLoadSpectrum() {
	CString wdir;
	((CNewEdXrfApp*)AfxGetApp())->GetWorkDirectory(wdir);
	CString filen=pSpectrum->GetChannelAt(0)->m_element+"*.DAT";
	CFileDialog fileDlg(TRUE,"DAT",filen,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"谱文件(*.DAT)|*.DAT||");
	fileDlg.m_ofn.lpstrInitialDir=wdir+"\\Spectrums\\";
	if(fileDlg.DoModal()==IDOK) {
		filen=fileDlg.GetFileName();
		pSpectrum->LoadSpectrum(filen);
		Update();
		((CNewEdXrfApp *)AfxGetApp())->RestoreCwd();
		}
	}

void CSpectrumCtrlEx::OnLockSpectrum() {
	pSpectrum->TuggleLock();
	Update();
	}	


void CSpectrumCtrlEx::InvalidateCtrl() {
	CClientDC dc(this);
	GetClientRect(m_rcClient); 
	if (m_MemDC.GetSafeHdc() == NULL) {
		m_MemDC.CreateCompatibleDC(&dc);
		m_Bitmap.CreateCompatibleBitmap(&dc,1024,768);
		m_MemDC.SelectObject(m_Bitmap);
		m_MemDC.SetBkColor(RGB(100,100,100));
		CBrush bkBrush(RGB(200,200,200));
		m_MemDC.FillRect(m_rcClient,&bkBrush);
	}
	InvalidateRect(m_rcClient, TRUE);
}


void CSpectrumCtrlEx::OnPaint() {
	CPaintDC dc(this);
	GetClientRect(m_rcClient);
	if (m_MemDC.GetSafeHdc()!=NULL) 
		dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);
	}

void CSpectrumCtrlEx::DrawSpike(CDC* pDC,CRect rect) {
	int i,j;CString scale,text;
	CPen pen_Black(PS_SOLID, 1, RGB(0,0,0));
	CPen pen_Dark(PS_SOLID, 1, RGB(220,220,220));
	CPen pen_Light(PS_SOLID, 1, RGB(240,240,240));

	int fontWidth=8;

	switch(m_drawMode) {
	case IconMode:
	case NormalMode:
		fontWidth=12;	
		break;
	case ZoomMode:
		fontWidth=rect.Width()/50;	
		if(fontWidth<12) fontWidth=12;
		break;
		}
	m_fnt.CreateFont(-fontWidth, 0, 0, 0, FW_DONTCARE, 
        FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
        CLIP_TT_ALWAYS | CLIP_LH_ANGLES,DEFAULT_QUALITY, 
        FF_MODERN | TMPF_TRUETYPE,"Arial");
    CFont* pOldFont=m_MemDC.SelectObject(&m_fnt);
	m_CharSize=m_MemDC.GetTextExtent("abcdefghijklmnopqrstuvwwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	m_CharSize.cx/=62;
	m_xSpace=m_CharSize.cx/2;
	m_ySpace=m_CharSize.cy/2;
	
	// 开始画进度条
	CRect progressRect,fillProgRerct;
	CBrush bkBrush(RGB(200,200,200));
	pDC->FillRect(rect,&bkBrush);
	pDC->Draw3dRect(rect,RGB(20,20,20),RGB(255,255,255));

	CRect rcShowSpectrum=rect;
	rcShowSpectrum.DeflateRect(1,1,1,1);
	progressRect=rcShowSpectrum;
	progressRect.bottom=progressRect.top+m_CharSize.cy/2;
	pDC->Draw3dRect(progressRect,RGB(0,0,0),RGB(255,255,255));
	rcShowSpectrum.top=progressRect.bottom+1;

	CPen* pOldPen = pDC->SelectObject(&pen_Dark);
	progressRect.DeflateRect(1,1,1,1);
	if(pSpectrum->m_bFinished) pDC->FillRect(progressRect,&CBrush(RGB(255,128,128)));
	else {
		pDC->FillRect(progressRect,&CBrush(RGB(255,255,200)));
		fillProgRerct=progressRect;
		int fillProgWidth=int(pSpectrum->m_AllTimeSecond*progressRect.Width()/pSpectrum->m_SetTimeSecond);
		fillProgRerct.right=fillProgRerct.left+fillProgWidth;
		if(fillProgRerct.right>=progressRect.right) fillProgRerct.right=progressRect.right;
		pDC->FillRect(fillProgRerct,&CBrush(RGB(0,128,255)));
		}
	// 画进度条结束 

	
	if(m_drawMode!=IconMode) { // 
		rcShowSpectrum.left+=m_CharSize.cx*3+2*m_xSpace;
		rcShowSpectrum.right-=m_xSpace;
		rcShowSpectrum.top+=m_CharSize.cy;
		rcShowSpectrum.bottom-=m_CharSize.cy+2*m_ySpace;
		}
	if(pDC->GetSafeHdc() != NULL) {
		pDC->FillRect(rcShowSpectrum,&CBrush(RGB(255,255,240)));
		pDC->SetBkColor(RGB(200,200,200));
		pDC->SetTextColor(RGB(0,0,0));
		m_Axis_X.SetRange(0,1024.0,8.0); //横坐标按8的幂数增长
		m_Axis_Y.SetRange(0.0,maxCount*yExtendFactor); //纵坐标按10的幂数增长
		int xpos,ypos;CSize scaleSize;
		if(m_drawMode==IconMode) goto skipAxisX; 
		for(i=0;i<=m_Axis_X.mainSteps;i++) { // 开始画刻度X
			xpos=rcShowSpectrum.left + i*rcShowSpectrum.Width()/m_Axis_X.mainSteps;
			if(i!=0&&i!=m_Axis_X.mainSteps) pDC->SelectObject(&pen_Dark);
			else pDC->SelectObject(&pen_Black);

			pDC->MoveTo(xpos,rcShowSpectrum.top);
			pDC->LineTo(xpos,rcShowSpectrum.bottom); //竖网格
			
			//刻度字体
			scale.Format(m_Axis_X.scaleFormat,m_Axis_X.minScale+i*m_Axis_X.mainStep);
			scaleSize=pDC->GetTextExtent(scale);
			xpos=xpos-scaleSize.cx/2;ypos=rcShowSpectrum.bottom+m_ySpace;
			if(xpos>rcShowSpectrum.right-scaleSize.cx) xpos=rcShowSpectrum.right-scaleSize.cx;
			pDC->TextOut(xpos,ypos,scale);
			if(i!=m_Axis_X.mainSteps) {
				for(j=1;j<m_Axis_X.subSteps;j++) {
					xpos+=rcShowSpectrum.Width()/(m_Axis_X.mainSteps*m_Axis_X.subSteps);
					pDC->SelectObject(&pen_Light);
					pDC->MoveTo(xpos,rcShowSpectrum.bottom-1);
					pDC->LineTo(xpos,rcShowSpectrum.top+1);
					}
				}
			}	
skipAxisX:
		if(m_drawMode==IconMode) goto skipAxisY;
		for(i=0;i<=m_Axis_Y.mainSteps;i++) { //画Y轴
			ypos=rcShowSpectrum.bottom-i*rcShowSpectrum.Height()/m_Axis_Y.mainSteps;
			if(i!=0&&i!=m_Axis_Y.mainSteps) pDC->SelectObject(&pen_Dark);
			else pDC->SelectObject(&pen_Black);
			pDC->MoveTo(rcShowSpectrum.left+1,ypos);
			pDC->LineTo(rcShowSpectrum.right,ypos);
			scale.Format(m_Axis_Y.scaleFormat,m_Axis_Y.minScale+i*m_Axis_Y.mainStep);
			scaleSize=pDC->GetTextExtent(scale);
			xpos=rcShowSpectrum.left-(scaleSize.cx+m_xSpace);
			ypos=ypos-scaleSize.cy/2;
			pDC->TextOut(xpos,ypos,scale);
			if(i!=m_Axis_Y.mainSteps) {
				for(j=1;j<m_Axis_Y.subSteps;j++) {
					ypos-=rcShowSpectrum.Height()/(m_Axis_Y.mainSteps*m_Axis_Y.subSteps);
					pDC->SelectObject(&pen_Light);
					pDC->MoveTo(rcShowSpectrum.left+1,ypos);
					pDC->LineTo(rcShowSpectrum.right-1,ypos);
					}
				}
			}
skipAxisY:
		// 开始画谱........
		double count; COLORREF color;
		BOOL isZoneBodor=FALSE,isDynZoneBorder=FALSE;
		if(pSpectrum==NULL) return;
		CPen *pen=new CPen(PS_SOLID,1,RGB(0,0,0));	
		pDC->SelectObject(pen);
		int xposf=0,yposf=0;
		BOOL draw=FALSE,FirstPoint=TRUE;
		COLORREF prevColor=RGB(0,0,0);
		for(i=0;i<1024;i++) {
			pSpectrum->GetReff(i,count,color,isZoneBodor,isDynZoneBorder);
			if(color!=prevColor) {
				prevColor=color;
				pDC->SelectObject(pOldPen);		
				if(pen) delete pen;
				pen=new CPen(PS_SOLID,1,color);
				pDC->SelectObject(pen);
				}
			ypos=rcShowSpectrum.bottom-1+(int)((count-m_Axis_Y.minScale)*(double)(rcShowSpectrum.top-rcShowSpectrum.bottom)/(m_Axis_Y.maxScale-m_Axis_Y.minScale));
			if(ypos<rcShowSpectrum.top) ypos=rcShowSpectrum.top;
			xpos=rcShowSpectrum.left+rcShowSpectrum.Width()*i/(int)(m_Axis_X.GetWidth());
			if(FirstPoint) {
				xposf=xpos;
				yposf=ypos;
				FirstPoint=FALSE;
				}
			else {
				for(int xp=xposf;xp<xpos;xp++) {	
					pDC->MoveTo(xp,rcShowSpectrum.bottom-1);
					pDC->LineTo(xp,ypos);
					}
				xposf=xpos;
				yposf=ypos;
				}
			if(isZoneBodor) {
				pDC->MoveTo(xpos,rcShowSpectrum.bottom-1);
				pDC->LineTo(xpos,rcShowSpectrum.top);
				}
			if(isDynZoneBorder) {
//				pDC->MoveTo(xpos,ypos-10);pDC->LineTo(xpos,ypos-30);
				pDC->MoveTo(xpos,ypos-10);pDC->LineTo(xpos-3,ypos-20);
				pDC->MoveTo(xpos,ypos-10);
				pDC->LineTo(xpos+3,ypos-20);pDC->LineTo(xpos-3,ypos-20);
				}
			}

		//开始画曲线
		xposf=0;yposf=0;draw=FALSE;FirstPoint=TRUE;
		for(i=0;i<1024;i++) {
			count=pSpectrum->GetCountClone(i);
			if(count>1.0e-5) draw=TRUE;
			else draw=FALSE;
			if(draw) {
				xpos=rcShowSpectrum.left+rcShowSpectrum.Width()*i/(int)(m_Axis_X.GetWidth());
				ypos=rcShowSpectrum.bottom-1+
					(int)((count-m_Axis_Y.minScale)*(double)(rcShowSpectrum.top-rcShowSpectrum.bottom)/(m_Axis_Y.maxScale-m_Axis_Y.minScale));	
				if(ypos<rcShowSpectrum.top) ypos=rcShowSpectrum.top; 
				else if(ypos>rcShowSpectrum.bottom) ypos=rcShowSpectrum.bottom;
				if(FirstPoint) {
					xposf=xpos;yposf=ypos;
					FirstPoint=FALSE;
					}
				else {
					pDC->MoveTo(xposf,yposf);
					pDC->LineTo(xpos,ypos);
					xposf=xpos;yposf=ypos;
					}	
				}
			}

		int i,nZone=pSpectrum->GetChannelNo(),pos=rcShowSpectrum.top+5;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		switch(m_drawMode) {
		case ZoomMode:
			xpos=rcShowSpectrum.left+m_xSpace*2;ypos=rcShowSpectrum.top+m_ySpace;
			text.Format("通道:%d",pSpectrum->infoID);
			pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
			text.Format("采样:%d/%d",int(pSpectrum->m_AllTimeSecond),int(pSpectrum->m_SetTimeSecond));
			pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
			if(pSpectrum->m_bAutoDriftCorrect) text.Format("基准峰位:%5.1f,自动校正",pSpectrum->m_peekbase);
			else text="无峰位漂移校正";
			pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
			for(i=0;i<nZone;i++) {
				text.Format("元素:%s",pSpectrum->GetChannelAt(i)->m_element);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("计数率:%-7.2lf/s",pSpectrum->GetChannelAt(i)->m_countcps);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("基线:%d",pSpectrum->GetChannelAt(i)->m_baseline);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("顶线:%d",pSpectrum->GetChannelAt(i)->m_topline);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("峰位:%5.1lf",pSpectrum->GetChannelAt(i)->m_peek);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("分辨率:%5.2lf%c",pSpectrum->GetChannelAt(i)->m_precision,'%');
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("峰修系数:%7.5lf",pSpectrum->GetChannelAt(i)->m_CorrectRestrains);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("校正:K=%7.5lf",pSpectrum->GetChannelAt(i)->m_cpsCorrectFactorSlope);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				text.Format("校正:B=%7.3lf",pSpectrum->GetChannelAt(i)->m_cpsCorrectFactorCut);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				}
			if(pSpectrum->m_bLocked) {
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(200,0,0));	
				pDC->TextOut(xpos,ypos,"Locked");
				}
			break;
		case IconMode:
			xpos=rcShowSpectrum.left+m_xSpace/2;ypos=rcShowSpectrum.top+m_ySpace/2;
			//text.Format("Ch%d",pSpectrum->infoID);
			//pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
			for(i=0;i<nZone;i++) {
				text.Format("%s",pSpectrum->GetChannelAt(i)->m_element);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				}
			if(pSpectrum->m_bLocked) {
				pDC->SetTextColor(RGB(200,0,0));	
				pDC->TextOut(rcShowSpectrum.right-(m_xSpace+m_CharSize.cx),rcShowSpectrum.top+m_ySpace,"L");
				}
			break;
		default:
			xpos=rcShowSpectrum.left+m_xSpace;ypos=rcShowSpectrum.top+m_ySpace;
			text.Format("Ch=%d %d/%d",pSpectrum->infoID,int(pSpectrum->m_AllTimeSecond),int(pSpectrum->m_SetTimeSecond));
			pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
			for(i=0;i<nZone;i++) {
				text.Format("%s:%-7.2lf",pSpectrum->GetChannelAt(i)->m_element,pSpectrum->GetChannelAt(i)->m_countcps);
				pDC->TextOut(xpos,ypos,text);ypos+=m_CharSize.cy+m_ySpace;
				}
			if(pSpectrum->m_bLocked) {
				pDC->SetTextColor(RGB(200,0,0));	
				pDC->TextOut(rcShowSpectrum.right-(m_xSpace+m_CharSize.cx*4),rcShowSpectrum.top+m_ySpace,"Lock");
				}
			break;
			}
		pDC->SelectObject(pOldPen);
		delete pen;	
		}	
	m_MemDC.SelectObject(pOldFont);
	//m_fnt.Detach();
	m_fnt.DeleteObject();
	}

void CSpectrumCtrlEx::Update() {
	if(pSpectrum->m_bModified) {
		InvalidateCtrl();
		UpdateMaxCount();  //得到maxCount, 峰顶
		GetClientRect(m_rcClient);
		DrawSpike(&m_MemDC,m_rcClient);
		Invalidate(FALSE); //不擦除背景,OnPaint(), OnPaint中贴图
	}
}



void CSpectrumCtrlEx::UpdateMaxCount() {	
	BOOL newMax=FALSE;
	if(pSpectrum==NULL||pSpectrum->m_AllTimeSecond==0) {
		maxCount=1;
		newMax=TRUE;
		}
	else {
		maxCount=0;
		for(int i=0;i<1023;i++) {
			if(pSpectrum->InPeekZone(i)&&pSpectrum->spectrum[i]>maxCount) {//InPeekZone 在感应区内
				maxCount=pSpectrum->spectrum[i];
				newMax=TRUE;
				}	
			}
		maxCount/=pSpectrum->m_AllTimeSecond;
		if(maxCount<0.1) maxCount=0.1;
		}
	}

void CSpectrumCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) {
	ClientToScreen(&point);
	CMenu menu,subMenu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_ZOOMINY, "纵放大");
	menu.AppendMenu(MF_STRING, ID_ZOOMOUTY, "纵压缩");
	menu.AppendMenu(MF_STRING, ID_ACTSIZE, "最  佳");
	menu.AppendMenu(MF_SEPARATOR,0,"");
	menu.AppendMenu(MF_STRING, ID_REDRAW, "刷  新");
	menu.AppendMenu(MF_SEPARATOR,0,"");
	menu.AppendMenu(MF_STRING, ID_PRINT, "打印谱");

	if(m_bAllowDeepConfig) {
		menu.AppendMenu(MF_SEPARATOR,0,"");
		menu.AppendMenu(MF_STRING|(pSpectrum->m_bLocked?MF_CHECKED:MF_UNCHECKED),ID_STANDSPEC,"锁定谱");
	
		menu.AppendMenu(MF_STRING,ID_SAVESPECT,"存谱图");
		menu.AppendMenu(MF_STRING,ID_LOADSPECT,"调谱图");
		}
	menu.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}

void CSpectrumCtrlEx::OnZoomInY() {
	yExtendFactor*=0.618;
	Update();
	}

void CSpectrumCtrlEx::OnZoomOutY() {
	yExtendFactor*=1.618;
	Update();
	}



void CSpectrumCtrlEx::OnActSize() {
	yExtendFactor=1.0;
	Update();
	}

void CSpectrumCtrlEx::OnRedraw() {
	pSpectrum->Calculate(TRUE,FALSE);
//	pSpectrum->Smooth();
	Update();
	}

void CSpectrumCtrlEx::OnSize(UINT nType,int cx,int cy) {
	Update();
	}

void CSpectrumCtrlEx::AttachSpectrumInfo(CSpectrumInfo *p) {
	pSpectrum=p;
	}




void CSpectrumCtrlEx::Print() {
    CDC dc;
    CPrintDialog printDlg(FALSE);
    if (printDlg.DoModal()!=IDOK)return;
    dc.Attach(printDlg.GetPrinterDC());
    dc.m_bPrinting = TRUE;
    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);
    DOCINFO di;
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;
    BOOL bPrintingOK = dc.StartDoc(&di);
    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++) {
        dc.StartPage();                  
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);             
        bPrintingOK = (dc.EndPage() > 0);
		}
    OnEndPrinting(&dc, &Info);   
    if (bPrintingOK) dc.EndDoc();
    else dc.AbortDoc();                          
    dc.Detach();  
	}



void CSpectrumCtrlEx::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo) {
    ASSERT(pDC && pInfo);
    if (!pDC || !pInfo) return;
	m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
    int nFontSize = -m_PaperSize.cx/60;
    CString strFontName = "宋体";
    m_PrinterFont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
        OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, strFontName);
      
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);
    m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ01234567890"));
	m_CharSize.cx /= 62;
    
	m_ySpace=m_CharSize.cy/4;
	m_xSpace=m_CharSize.cx/4;

    
    pInfo->SetMaxPage(1);
    pInfo->m_nCurPage = 1;       
    pDC->SelectObject(pOldFont);
	}

void CSpectrumCtrlEx::OnPrint(CDC *pDC, CPrintInfo *pInfo) {
	if (!pDC || !pInfo) return;
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);
	// Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_PaperSize);
    pDC->SetViewportExt(m_PaperSize);
	
    //Print Header
	pDC->SetWindowOrg(-m_leftMargin*m_CharSize.cx,0);

	pInfo->m_rectDraw.top=0;
    pInfo->m_rectDraw.left=0;
    pInfo->m_rectDraw.right=m_PaperSize.cx-(m_leftMargin+m_rightMargin)*m_CharSize.cx;
    pInfo->m_rectDraw.bottom=m_headerHeight*m_CharSize.cy;
	PrintHeader(pDC, pInfo);


	//Gap between header and column headings
	pDC->OffsetWindowOrg(0, -m_headerHeight*m_CharSize.cy);
	pDC->OffsetWindowOrg(0, -m_gap*m_CharSize.cy);

	pInfo->m_rectDraw.bottom=m_PaperSize.cy-(m_topMargin+m_bottomMargin+m_headerHeight+m_footerHeight+m_gap)*m_CharSize.cy;
	
	DrawSpike(pDC,pInfo->m_rectDraw);
    
	//Print Footer
	pInfo->m_rectDraw.bottom = m_footerHeight*m_CharSize.cy;
    pDC->SetWindowOrg(-m_leftMargin*m_CharSize.cx, -m_PaperSize.cy + m_footerHeight*m_CharSize.cy);
    PrintFooter(pDC, pInfo);

    pDC->SetWindowOrg(0,0);

    pDC->SelectObject(pOldFont);
	}


void CSpectrumCtrlEx::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
    m_PrinterFont.DeleteObject();
	}


void CSpectrumCtrlEx::PrintHeader(CDC *pDC, CPrintInfo *pInfo) {
    CRect   rc(pInfo->m_rectDraw);
    CString strHeaderString;
    CFont   BoldFont;
    LOGFONT lf;
    //create bold font for header and footer
    VERIFY(m_PrinterFont.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
	VERIFY(BoldFont.CreateFontIndirect(&lf));
    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    // print App title on top right margin
    strHeaderString.LoadString(AFX_IDS_APP_TITLE);
    
//	pDC->DrawText(strHeaderString, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    // print parent window title in the centre (Gert Rijs)
    CWnd *pParentWnd = GetParent();
    while (pParentWnd) {
	    pParentWnd->GetWindowText(strHeaderString);
	    if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
		    break;
        pParentWnd = pParentWnd->GetParent();
		}
//	pDC->DrawText(strHeaderString, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

 //   pDC->SelectStockObject(BLACK_PEN);
   // pDC->MoveTo(rc.left, rc.bottom);
//    pDC->LineTo(rc.right, rc.bottom);
}

//print footer with a line and date, and page number
void CSpectrumCtrlEx::PrintFooter(CDC *pDC, CPrintInfo *pInfo) {
    CRect rc(pInfo->m_rectDraw);
/*	CString strFontName = "宋体";
	m_PrinterFont.Delete();
    m_PrinterFont.CreateFont(m_CharSize.cy*3/2, 0,0,0, FW_BOLD, 0,0,0, DEFAULT_CHARSET,
        OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, strFontName);
    
    CFont *pNormalFont = pDC->SelectObject(&m_PrinterFont);
*/
    
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    pDC->DrawText(m_footer,-1,rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
     m_PrinterFont.DeleteObject();
	}	







BOOL CSpectrumCtrlEx::DrawAndSaveBitmap(CString filen, int width, int height, int colordepth) {
	return TRUE;
	}
