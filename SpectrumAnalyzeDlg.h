#if !defined(AFX_SPECTRUMANALYZEDLG_H__10D50CE2_33BA_47B3_BAC1_A6C907C05FE4__INCLUDED_)
#define AFX_SPECTRUMANALYZEDLG_H__10D50CE2_33BA_47B3_BAC1_A6C907C05FE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpectrumAnalyzeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzeDlg dialog

class CSpectrumAnalyzeDlg : public CDialog
{
// Construction
public:
	CSpectrumAnalyzeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpectrumAnalyzeDlg)
	enum { IDD = IDD_SPECTRUMDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectrumAnalyzeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpectrumAnalyzeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRUMANALYZEDLG_H__10D50CE2_33BA_47B3_BAC1_A6C907C05FE4__INCLUDED_)
