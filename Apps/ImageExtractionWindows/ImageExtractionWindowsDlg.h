// ImageExtractionWindowsDlg.h : header file
//
#include "afxwin.h"

#include "DateTime.h"
#include "Str.h"
using namespace  KKU;

#include "SipperImageExtractionParms.h"

#include "PreDefinedClasses.h"
using namespace  ImageExtractionManager;


#if !defined(AFX_IMAGEEXTRACTIONWINDOWSDLG_H__597AC46B_831A_46B0_A9C2_D951CEB8C88E__INCLUDED_)
#define AFX_IMAGEEXTRACTIONWINDOWSDLG_H__597AC46B_831A_46B0_A9C2_D951CEB8C88E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsDlg dialog

class CImageExtractionWindowsDlg : public CDialog
{
// Construction
public:
	CImageExtractionWindowsDlg(CWnd* pParent = NULL);	// standard constructor

  ~CImageExtractionWindowsDlg ();


// Dialog Data
	//{{AFX_DATA(CImageExtractionWindowsDlg)
	enum { IDD = IDD_IMAGEEXTRACTIONWINDOWS_DIALOG };
	CButton	m_cancel;
	CProgressCtrl	m_extractProgress;
	CString       m_linesExtracted;
	CString       m_bytesExtracted;
	CString       m_imagesExtracted;
	CString       m_imagesWritten;
	CString       m_sipperFileSize;
	int		m_minimumSize;
	int		m_maximumSize;
  CString m_database;
	CString	m_sipperFileName;
	CString	m_destinationDirectory;
	BOOL	m_saveFrames;
	BOOL	m_extractFeatures;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageExtractionWindowsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL



public:
    void  ImageExtractionParms           (SipperImageExtractionParmsPtr  _imageExtractionParms);            /**< Takes ownership of '_imageExtractionParms'.           */
    void  MultiTreadedExtractionParms    (ExtractionParmsPtr             _multiTreadedExtractionParms);     /**< Takes ownership of '_multiTreadedExtractionParms'.    */
    void  MultiThreadedExtractionManager (ExtractionManagerPtr           _multiThreadedExtractionManager);  /**< Takes ownership of '_multiThreadedExtractionManager'. */

    void  ExtractionStopped              (bool   _extractionStopped)    {extractionStopped    = _extractionStopped;}
    void  ExtractionStoppedMsg           (string _extractionStoppedMsg) {extractionStoppedMsg = _extractionStoppedMsg;}
    void  ExtractionCrashed              (bool   _extractionCrashed)    {extractionCrashed    = _extractionCrashed;}
    void  ExtractionCanceled             (bool   _extractionCanceled)   {extractionCanceled   = _extractionCanceled;}

    ExtractionParmsPtr             MultiTreadedExtractionParms ()  const  {return multiTreadedExtractionParms;}
    SipperImageExtractionParmsPtr  ImageExtractionParms        ()  const  {return imageExtractionParms;}

    void  ManageImageExtractionMultiThreaded ();


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImageExtractionWindowsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  void      UpdateDialogStaticVariables ();
	void      StartImageExtraction();
  DateTime  cancelRequestStartTime;
  bool      terminateNow;  // Used when the 'imageExtractionThread' does not terminate in a timely manner.
                           // this flag will let the main thread know to stop right now.

  CWinThread*                    imageExtractionThread;
  SipperImageExtractionParmsPtr  imageExtractionParms;

  ExtractionParmsPtr             multiTreadedExtractionParms;
  ExtractionManagerPtr           multiThreadedExtractionManager;

  bool                           extractionStopped;
  std::string                    extractionStoppedMsg;
  bool                           extractionCrashed;
  bool                           extractionCanceled;
  
  void  OnTimerExtractionStopped ();
  void  OnTimerUpdateClassStats  (const ClassStatisticListPtr  classStats);
  void  OnTimerSingleThreaded    ();
  void  OnTimerMultiThreaded     ();

public:
  afx_msg void OnStnClickedMinimumimagesizelabel2();
  afx_msg void OnEnChangeMinimumsize();
  afx_msg void OnBnClickedExtractfeatures();
  afx_msg void OnEnChangeMaximumsize();
  afx_msg void OnEnChangeClass1name();
  CString m_class1Count;
  CString m_class1Name;
  CString m_class2Name;
  CString m_class3Name;
  CString m_class4Name;
  CString m_class5Name;
  CString m_class6Name;
  CString m_class7Name;
  CString m_class8Name;
  CString m_class9Name;
  CString m_class10Name;
  CString m_class11Name;
  CString m_class12Name;
  CString m_class2Count;
  CString m_class3Count;
  CString m_class4Count;
  CString m_class5Count;
  CString m_class6Count;
  CString m_class7Count;
  CString m_class8Count;
  CString m_class9Count;
  CString m_class10Count;
  CString m_class11Count;
  CString m_class12Count;
  CString m_class0Name;
  CString m_class0Count;
  CString m_class13Name;
  CString m_class13Count;
  CString m_class14Name;
  CString m_class14Count;
  CString m_configFileName;
  CString m_statusMessage;
  afx_msg void OnEnChangeClass0name();
  afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEEXTRACTIONWINDOWSDLG_H__597AC46B_831A_46B0_A9C2_D951CEB8C88E__INCLUDED_)
