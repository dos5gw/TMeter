#ifndef __SPECTRUMCTRL_H__
#define __SPECTRUMCTRL_H__

#include <afxtempl.h>
# include "Axis.h"

enum DrawMode {IconMode,NormalMode,ZoomMode};

# include "SpectrumInfo.h"
class CSpectrumCtrlEx : public CWnd {
public:
	CSpectrumCtrlEx();
	void InvalidateCtrl();
	void DrawSpike(CDC* pDC,CRect rect);
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineChartCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public: 
	BOOL DrawAndSaveBitmap(CString filen,int width,int height,int colordepth);
	CFont m_fnt;
	BOOL m_bAllowDeepConfig;
	void SetDrawMode(DrawMode mode) {
		m_drawMode=mode;
		}
	void AttachSpectrumInfo(CSpectrumInfo *p);
	CSpectrumInfo * pSpectrum;
	void UpdateMaxCount();
	CAxis m_Axis_Y;
	CAxis m_Axis_X;
	void Update();
	void Print();
	virtual ~CSpectrumCtrlEx();
	// Generated message map functions

	afx_msg void OnZoomInY();
	afx_msg void OnZoomOutY();
	afx_msg void OnActSize();
	afx_msg void OnSaveBkgroud();
	afx_msg void OnSaveSpectrum();
	afx_msg void OnLoadSpectrum();
	afx_msg void OnLockSpectrum();
	afx_msg void OnRedraw();

	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
    virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

protected:
	//{{AFX_MSG(CLineChartCtrl)
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CDC      m_MemDC;
	CBitmap  m_Bitmap;
private:
	CRect m_rcClient;
	double maxCount;
	double yExtendFactor;
	DrawMode m_drawMode;


	int m_gap;
	int m_footerHeight;
	int m_headerHeight;
	int m_topMargin,m_bottomMargin;
	int m_leftMargin,m_rightMargin;
	int m_xSpace,m_ySpace;
	CSize m_CharSize,m_PaperSize;
	CFont m_PrinterFont;
	CString m_footer;
	};


/////////////////////////////////////////////////////////////////////////////
#endif
