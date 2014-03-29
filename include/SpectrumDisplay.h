#if !defined(AFX_SPECTRUMDISPLAY_H__4CCA43F9_2F48_4403_BD90_EAD4E89A346F__INCLUDED_)
#define AFX_SPECTRUMDISPLAY_H__4CCA43F9_2F48_4403_BD90_EAD4E89A346F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpectrumDisplay.h : header file
//
#define MAXSIZE 256
#include <afxtempl.h>
#include "SpectrumInfo.h"
#include "NewAxis.h"
#include "TMeterTypeDefine.h"

//typedef enum MOVEPOS {LEFT,RIGHT} MovePos;
extern COLORREF _color_list[];
COLORREF GetNewColor();
COLORREF GetNewColorAt(int i);

class CAnalysisLine : public CObject
{
public:
	CAnalysisLine()
	{
		KaChannel=0.0;
		KbChannel=0.0;
		Name="NC";
		Color=0x000000;
	}
	CAnalysisLine(CString element,double kaChannel,double kbChannel,COLORREF color):Name(element)
	{
		KaChannel=kaChannel;
		KbChannel=kbChannel; 	
		Color=color;
	}
	~CAnalysisLine()
	{
	}
public:
	double KaChannel;
	double KbChannel;
	CString Name;
	COLORREF Color;
};

class CAnalysisLineArray : public CTypedPtrArray<CObArray,CAnalysisLine*>
{
public:
	BOOL IsEmpty() {return (GetSize()==0);}
};

/////////////////////////////////////////////////////////////////////////////
// CSpectrumDisplay window
class CSpectrumDisplay : public CStatic
//class CSpectrumDisplay : public CWnd
{
// Construction
public:
	CSpectrumDisplay();

// Attributes
public:
	CNewAxis m_Axis_X;
	CNewAxis m_Axis_Y;
	int m_baseLine;
	int m_topLine;
	BOOL m_beNeedFilled;

// Operations
public:
	CAnalysisLineArray m_AnalysisLineArray; //元素能量线
	void AddAnalysisLines(CString element,double kaChannel,double kbChannel);
	void CleanAnalysisLines();
	BOOL ElementExistInArray(CString element);
	void FreeAnalysisLineArray();

	CTypedPtrArray<CObArray,CSpectrumInfo<DWORD,SPEC_SIZE>*> m_ContrastSpectrum; //叠加谱图, 2012-07-09
	BOOL AddToContrastSpec(CSpectrumInfo<DWORD,SPEC_SIZE> *p, CString file); 
	void RemoveAllContrastSpec();
	void RemoveLastContrastSpec();

	void Update();
	void InvalidateCtrl();
	void AttachSpectrumInfo(CSpectrumInfo<DWORD,SPEC_SIZE> *p);
	void DrawAll(CDC* pDC,CRect rect);
	void UpdateMaxCount(); // change value of 

	void DrawBitmap(CDC* pDC,CRect rect);
	void DrawBackground(CDC* pDC,CRect rect,CRect specRect);
	void DrawAxis(CDC* pDC,CRect rect,CRect specRect);
	void DrawSpectrum(CDC* pDC,CRect rect,CRect specRect);
	void DrawContrastSpectrum(CDC* pDC,CRect rect,CRect specRect); //@2012-07-09
	void DrawSketchSpectrum(CDC* pDC,CRect rect,CRect specRect);   //@2012-07-10
	void DrawAnalysisLines(CDC* pDC,CRect rect,CRect specRect);

	void DrawSpectrumPolygon(CDC* pDC,CRect rect,CRect specRect); //@2012-07-11
	void DrawContrastSpectrumPolygon(CDC* pDC,CRect rect,CRect specRect);

	void ExtendYAxis();  //+X
	void ContractYAxis();//-X
	void ExtendXAxis();  //+Y
	void ContractXAxis();//-Y
	void XAxisMoveLeft();
	void XAxisMoveRight();
	void SetDafaultScale();
	void FillCurve();
	void ChangeCurrentCursor(int cursor);
	BOOL PreChangeCursor(int& channel);
	void ZeroSpectrum();
	void SetZoneMode(BOOL b){m_beSetZoneMode=b;}

private:
	CSpectrumInfo<DWORD,SPEC_SIZE> *pSpectrum;
	CDC      m_MemDC;
	CBitmap  m_Bitmap;
	CBitmap* m_pOldBitmap;
	CRect	 m_rcClient;  // the static rect of chart control
	CFont    m_font;
	CSize    m_CharSize;
	int m_xSpace,m_ySpace;
	//double yExtendFactor,xExtendFactor;
	double maxCount;
	//MovePos m_movePos;
	BOOL m_canMoveXAxis;
	BOOL m_beSetZoneMode;
	int m_moveStep;
	int m_Cursor;
	CRect rcShowSpectrum;

	double xResolution;
	double yResolution;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectrumDisplay)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpectrumDisplay();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpectrumDisplay)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRUMDISPLAY_H__4CCA43F9_2F48_4403_BD90_EAD4E89A346F__INCLUDED_)
