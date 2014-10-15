#include  "StdAfx.h"
#include  "FirstIncludes.h"
// ImageExtractionWindowsDlg.cpp : implementation file
//

// fftw.lib  cv.lib  highgui.lib

#include  <time.h>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <map>
#include  <vector>
#include  <string>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

typedef wchar_t WCHAR;

using namespace  std;
using namespace  KKU;

#include "ClassStatistic.h"
#include "OSservices.h"
#include "SipperImageExtractionParms.h"
#include "SipperImageExtraction.h"


#include "ExtractionManager.h"
#include "ExtractionManagerStats.h"
using namespace  ImageExtractionManager;


#include "stdafx.h"
#include "ImageExtractionWindows.h"
#include "ImageExtractionWindowsDlg.h"
//#include ".\imageextractionwindowsdlg.h"

#define  ImageExtractionTimerID  100

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsDlg dialog

CImageExtractionWindowsDlg::CImageExtractionWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageExtractionWindowsDlg::IDD, pParent),

      imageExtractionThread          (NULL),
      imageExtractionParms           (NULL),
      multiTreadedExtractionParms    (NULL),
      multiThreadedExtractionManager (NULL),

      extractionStopped              (false),
      extractionStoppedMsg           (),
      extractionCrashed              (false),
      extractionCanceled             (false),

      terminateNow (false)
      , m_class1Count(_T(""))
      , m_class1Name(_T(""))
      , m_class2Name(_T(""))
      , m_class3Name(_T(""))
      , m_class4Name(_T(""))
      , m_class5Name(_T(""))
      , m_class6Name(_T(""))
      , m_class7Name(_T(""))
      , m_class8Name(_T(""))
      , m_class9Name(_T(""))
      , m_class10Name(_T(""))
      , m_class11Name(_T(""))
      , m_class12Name(_T(""))
      , m_class2Count(_T(""))
      , m_class3Count(_T(""))
      , m_class4Count(_T(""))
      , m_class5Count(_T(""))
      , m_class6Count(_T(""))
      , m_class7Count(_T(""))
      , m_class8Count(_T(""))
      , m_class9Count(_T(""))
      , m_class10Count(_T(""))
      , m_class11Count(_T(""))
      , m_class12Count(_T(""))
      , m_class0Name(_T(""))
      , m_class0Count(_T(""))
      , m_class13Name(_T(""))
      , m_class13Count(_T(""))
      , m_class14Name(_T(""))
      , m_class14Count(_T(""))
      , m_configFileName(_T(""))
      , m_statusMessage(_T(""))
{
	//{{AFX_DATA_INIT(CImageExtractionWindowsDlg)
  m_linesExtracted  = "";
	m_bytesExtracted  = "";
	m_imagesExtracted = "";
	m_imagesWritten   = "";
	m_sipperFileSize  = "";
	m_minimumSize = 0;
	m_maximumSize = 0;
  m_database       = _T("");
  m_sipperFileName = _T("");
	m_destinationDirectory = _T("");
	m_saveFrames = FALSE;
	m_extractFeatures = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



CImageExtractionWindowsDlg::~CImageExtractionWindowsDlg ()
{
  delete  imageExtractionParms;             imageExtractionParms           = NULL;
  delete  multiTreadedExtractionParms;      multiTreadedExtractionParms    = NULL;
  delete  multiThreadedExtractionManager;   multiThreadedExtractionManager = NULL;
}



void CImageExtractionWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CImageExtractionWindowsDlg)
  DDX_Control(pDX, IDCANCEL, m_cancel);
  DDX_Control(pDX, IDC_ExtractProgress, m_extractProgress);
  DDX_Text(pDX, IDC_LinesExtracted, m_linesExtracted);
  DDX_Text(pDX, IDC_BytesExtracted, m_bytesExtracted);
  DDX_Text(pDX, IDC_ImagesExtracted, m_imagesExtracted);
  DDX_Text(pDX, IDC_ImagesWritten, m_imagesWritten);
  DDX_Text(pDX, IDC_sipperFileSize, m_sipperFileSize);
  DDX_Text(pDX, IDC_MinimumSize, m_minimumSize);
  DDX_Text(pDX, IDC_MaximumSize, m_maximumSize);
  DDX_Text(pDX, IDC_Database, m_database);
  DDX_Text(pDX, IDC_SipperFile, m_sipperFileName);
  DDX_Text(pDX, IDC_DestinationDirectory, m_destinationDirectory);
  DDX_Check(pDX, IDC_SaveFrames, m_saveFrames);
  DDX_Check(pDX, IDC_ExtractFeatures, m_extractFeatures);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_Class1Count, m_class1Count);
  DDX_Text(pDX, IDC_Class1Name, m_class1Name);
  DDX_Text(pDX, IDC_Class2Name, m_class2Name);
  DDX_Text(pDX, IDC_Class3Name, m_class3Name);
  DDX_Text(pDX, IDC_Class4Name, m_class4Name);
  DDX_Text(pDX, IDC_Class5Name, m_class5Name);
  DDX_Text(pDX, IDC_Class6Name, m_class6Name);
  DDX_Text(pDX, IDC_Class7Name, m_class7Name);
  DDX_Text(pDX, IDC_Class8Name, m_class8Name);
  DDX_Text(pDX, IDC_Class9Name, m_class9Name);
  DDX_Text(pDX, IDC_Class10Name, m_class10Name);
  DDX_Text(pDX, IDC_Class11Name, m_class11Name);
  DDX_Text(pDX, IDC_Class12Name, m_class12Name);
  DDX_Text(pDX, IDC_Class2Count, m_class2Count);
  DDX_Text(pDX, IDC_Class3Count, m_class3Count);
  DDX_Text(pDX, IDC_Class4Count, m_class4Count);
  DDX_Text(pDX, IDC_Class5Count, m_class5Count);
  DDX_Text(pDX, IDC_Class6Count, m_class6Count);
  DDX_Text(pDX, IDC_Class7Count, m_class7Count);
  DDX_Text(pDX, IDC_Class8Count, m_class8Count);
  DDX_Text(pDX, IDC_Class9Count, m_class9Count);
  DDX_Text(pDX, IDC_Class10Count, m_class10Count);
  DDX_Text(pDX, IDC_Class11Count, m_class11Count);
  DDX_Text(pDX, IDC_Class12Count, m_class12Count);
  DDX_Text(pDX, IDC_Class0Name, m_class0Name);
  DDX_Text(pDX, IDC_Class0Count, m_class0Count);
  DDX_Text(pDX, IDC_Class13Name, m_class13Name);
  DDX_Text(pDX, IDC_Class13Count, m_class13Count);
  DDX_Text(pDX, IDC_Class14Name, m_class14Name);
  DDX_Text(pDX, IDC_Class14Count, m_class14Count);
  DDX_Text(pDX, IDC_ConfigFileName, m_configFileName);
  DDX_Text(pDX, IDC_StatusMessage, m_statusMessage);
}

BEGIN_MESSAGE_MAP(CImageExtractionWindowsDlg, CDialog)
	//{{AFX_MSG_MAP(CImageExtractionWindowsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
  ON_STN_CLICKED(IDC_MinimumImageSizeLabel, OnStnClickedMinimumimagesizelabel2)
  ON_EN_CHANGE(IDC_MinimumSize, OnEnChangeMinimumsize)
  ON_BN_CLICKED(IDC_ExtractFeatures, OnBnClickedExtractfeatures)
  ON_EN_CHANGE(IDC_MaximumSize, OnEnChangeMaximumsize)
  ON_EN_CHANGE(IDC_Class1Name, OnEnChangeClass1name)
  ON_EN_CHANGE(IDC_Class0Name, OnEnChangeClass0name)
  ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsDlg message handlers




BOOL CImageExtractionWindowsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

    SetTimer (ImageExtractionTimerID, 1000, NULL);

    UpdateDialogStaticVariables ();

    StartImageExtraction ();

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CImageExtractionWindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}





// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageExtractionWindowsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}





// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageExtractionWindowsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}





void CImageExtractionWindowsDlg::OnCancel() 
{
  // TODO: Add extra cleanup here

 
  m_cancel.EnableWindow (false);
 
  int returnCd = MessageBox ("Do you want to Cancel Image Extraction ?",
                             "Cancel Image Extraction",
                             MB_YESNO
                            );

  if  (returnCd == IDYES)
  {
    if  (imageExtractionParms)
      imageExtractionParms->Cancel (true);

    if  (multiThreadedExtractionManager)
      multiThreadedExtractionManager->CancelProcessing (0);

    cancelRequestStartTime = osGetLocalDateTime ();

    /*
      if   (::WaitForSingleObject (this->imageExtractionThread->m_hThread, 10000) == WAIT_OBJECT_0)
      {
        // Thread Canceled Successfully
      
      }

      CDialog::OnCancel();
    */
  }
  else
  {
    m_cancel.EnableWindow (true);
  }
}




void CImageExtractionWindowsDlg::OnTimer  (UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

  if  (nIDEvent != ImageExtractionTimerID)
  {
    CDialog::OnTimer (nIDEvent);
  }
  else
  {
    if  (extractionStopped)
      OnTimerExtractionStopped ();
    
    else if  (imageExtractionParms)
      OnTimerSingleThreaded ();

    else if  (multiTreadedExtractionParms)
      OnTimerMultiThreaded ();
  }
}  /* OnTimer */



void  CImageExtractionWindowsDlg::OnTimerExtractionStopped ()
{
  m_cancel.EnableWindow (false);

  KillTimer (ImageExtractionTimerID);
    
  if  (extractionCanceled)
  {
    MessageBox ("Image Extraction has been Canceled",
                "Image Extraction",
                MB_OK
               );
  }
  
  else if  (extractionCrashed)
  {
    MessageBox ("Image Extraction      *** CRASHED ***",
                "Image Extraction",
                MB_OK
               );
  }

  else
  {
    if  (!osIsBackGroundProcess ())
      int returnCd = MessageBox ("Image Extraction has Completed",
                                 "Image Extraction",
                                 MB_OK
                                );
  }

  CDialog::OnCancel();
}  /* OnTimerExtractionStopped */




void  CImageExtractionWindowsDlg::OnTimerUpdateClassStats (const ClassStatisticListPtr  classStats)
{
  int  classIDX = 0;
  ClassStatisticList::const_iterator  idx = classStats->begin ();

  while  ((classIDX < 15)  &&  (idx != classStats->end ()))
  {
    ClassStatisticPtr  stat = *idx;

    const char*  name = stat->MLClass ()->Name ().Str ();
    KKU::uint  count = stat->Count ();

    KKU::KKStr countStr = StrFormatInt (count, "ZZZ,ZZZ,ZZ0");
    const char*  countCstr = countStr.c_str ();

    switch  (classIDX)
    {
    case  0:  m_class0Name  = name;
              m_class0Count = countCstr;
              break;

    case  1:  m_class1Name  = name;
              m_class1Count = countCstr;
              break;

    case  2:  m_class2Name  = name;
              m_class2Count = countCstr;
              break;

    case  3:  m_class3Name  = name;
              m_class3Count = countCstr;
              break;

    case  4:  m_class4Name  = name;
              m_class4Count = countCstr;
              break;

    case  5:  m_class5Name  = name;
              m_class5Count = countCstr;
              break;

    case  6:  m_class6Name  = name;
              m_class6Count = countCstr;
              break;

    case  7:  m_class7Name  = name;
              m_class7Count = countCstr;
              break;

    case  8:  m_class8Name  = name;
              m_class8Count = countCstr;
              break;

    case  9:  m_class9Name  = name;
              m_class9Count = countCstr;
              break;

    case 10:  m_class10Name  = name;
              m_class10Count = countCstr;
              break;

    case 11:  m_class11Name  = name;
              m_class11Count = countCstr;
              break;

    case 12:  m_class12Name  = name;
              m_class12Count = countCstr;
              break;

    case 13:  m_class13Name  = name;
              m_class13Count = countCstr;
              break;

    case 14:  m_class14Name  = name;
              m_class14Count = countCstr;
              break;
    }

    classIDX++;
    idx++;
  }
}  /* OnTimerUpdateClassStats */



void  CImageExtractionWindowsDlg::OnTimerSingleThreaded ()
{
  if  (imageExtractionParms->Cancel ()  &&  (!imageExtractionParms->DoneExecuting ()))
  {
    DateTime now = osGetLocalDateTime ();
    DateTime elaspedTime = now - cancelRequestStartTime;

    uint64  secsWeHaveBeenWaitingForCancel = elaspedTime.Seconds ();
    if  (secsWeHaveBeenWaitingForCancel > 120)
    {
      // We have waited long enough;  we are just going to kill the thread.
      terminateNow = true;
      return;
    }
  }

  KKStr  bytesExtractedStr  = StrFormatInt64 (imageExtractionParms->BytesExtracted (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  linesExtractedStr  = StrFormatInt (imageExtractionParms->LinesExtracted  (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  imagesExtractedStr = StrFormatInt (imageExtractionParms->ImagesExtracted (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  imagesWrittenStr   = StrFormatInt (imageExtractionParms->ImagesWritten   (), "ZZZ,ZZZ,ZZZ,ZZ0");

  m_bytesExtracted  = bytesExtractedStr.Str  ();
  m_linesExtracted  = linesExtractedStr.Str  ();
  m_imagesExtracted = imagesExtractedStr.Str ();
  m_imagesWritten   = imagesWrittenStr.Str   ();

  m_extractProgress.SetPos ((int)(imageExtractionParms->BytesExtracted () / 1024));
  m_statusMessage = imageExtractionParms->Log ().LastLine ().Str ();

  ClassStatisticListPtr  classStats = imageExtractionParms->ClassStats ();
  if  (classStats)
    OnTimerUpdateClassStats (classStats);

  UpdateData (false);

  if  (terminateNow)
  {
    m_cancel.EnableWindow (false);

    this->KillTimer (ImageExtractionTimerID);
    
    int returnCd = MessageBox ("Image Extraction cancelation was forced.",
                               "Image Extraction",
                               MB_OK
                              );
    CDialog::OnCancel();
  }
}  /* OnTimerSingleThreaded */



void  CImageExtractionWindowsDlg::OnTimerMultiThreaded ()
{
  if  ((multiThreadedExtractionManager == NULL)  ||  (multiTreadedExtractionParms == NULL))
    return;

  if  (multiThreadedExtractionManager->CancelFlag  ()  &&  (!multiThreadedExtractionManager->DoneExecuting ()))
  {
    DateTime now = osGetLocalDateTime ();
    DateTime elaspedTime = now - cancelRequestStartTime;

    uint64  secsWeHaveBeenWaitingForCancel = elaspedTime.Seconds ();
    if  (secsWeHaveBeenWaitingForCancel > 300)
    {
      // We have waited long enough;  we are just going to kill the thread.
      terminateNow = true;
      return;
    }
  }

  ExtractionManagerStats  stats;
  multiThreadedExtractionManager->GetRunTimeStats (stats);

  KKStr  bytesExtractedStr  = StrFormatInt64 (stats.BytesRead          (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  linesExtractedStr  = StrFormatInt   (stats.ScanLinesProcessed (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  imagesExtractedStr = StrFormatInt   (stats.ImagesClassified   (), "ZZZ,ZZZ,ZZZ,ZZ0");
  KKStr  imagesWrittenStr   = StrFormatInt   (stats.ImagesUpdated      (), "ZZZ,ZZZ,ZZZ,ZZ0");

  m_bytesExtracted  = bytesExtractedStr.Str  ();
  m_linesExtracted  = linesExtractedStr.Str  ();
  m_imagesExtracted = imagesExtractedStr.Str ();
  m_imagesWritten   = imagesWrittenStr.Str   ();

  m_extractProgress.SetPos ((int)(stats.BytesRead () / 1024));
  m_statusMessage = multiThreadedExtractionManager->Log ().LastLine ().Str ();

  {
    // We will be getting owneship of 'classStats' so will need to delete when done with.
    ClassStatisticListPtr  classStats = multiThreadedExtractionManager->GetClassStatistics ();
    if  (classStats)
      OnTimerUpdateClassStats (classStats);
    delete  classStats;
    classStats = NULL;
  }

  UpdateData (false);

  // CDialog::OnTimer (nIDEvent);

  if  (terminateNow)
  {
    m_cancel.EnableWindow (false);

    this->KillTimer (ImageExtractionTimerID);
    
    int returnCd = MessageBox ("Image Extraction cancelation was forced.",
                               "Image Extraction",
                               MB_OK
                              );
    CDialog::OnCancel();
  }
}  /* OnTimerMultiThreaded */


void  CImageExtractionWindowsDlg::ManageImageExtractionMultiThreaded ()
{
  if  (multiTreadedExtractionParms == NULL)
    return;

  bool  successful = true;
  {
    successful = true;
    int32  numberOfProcessors = osGetNumberOfProcessors ();
    if  (numberOfProcessors < 1)
      numberOfProcessors = 1;

    RunLog  log;

    multiThreadedExtractionManager = new ExtractionManager ("ImageExtractionWindows",
                                                            *multiTreadedExtractionParms, 
                                                            numberOfProcessors, 
                                                            log
                                                           );

    bool  successful = false;
    multiThreadedExtractionManager->ManageTheExtraction (successful);

    if  (!successful)
    {
      if  (multiThreadedExtractionManager->CancelFlag ())
      {
        ExtractionCanceled (true);
      }
      else if  (multiThreadedExtractionManager->Crashed ())
      {
        ExtractionCrashed (true);
      }
    }

    ExtractionStopped (true);

    osSleep (0.1f);
  }

  // We do not delete 'multiThreadedExtractionManager'  because other methods in this 
  // class may still need to access it.
}  /* ManageImageExtractionMultiThreaded */



UINT   ImageExtractionMultiThreaded (LPVOID pParam)
{
  osRunAsABackGroundProcess ();

  CImageExtractionWindowsDlg* imageExtractionWindowsDlg = (CImageExtractionWindowsDlg*)pParam;

  imageExtractionWindowsDlg->ManageImageExtractionMultiThreaded ();

  return 0;   // thread completed successfully
}  /* ImageExtractionMultiThreaded */



UINT   ImageExtractionSingleThreaded (LPVOID pParam)
{
  osRunAsABackGroundProcess ();

  CImageExtractionWindowsDlg* imageExtractionWindowsDlg = (CImageExtractionWindowsDlg*)pParam;
  if  (imageExtractionWindowsDlg == NULL) //   ||   (!parms->IsKindOf (RUNTIME_CLASS (SipperImageExtractionParms)))
    return 1;   // if pObject is not valid

  SipperImageExtractionParmsPtr  parms = imageExtractionWindowsDlg->ImageExtractionParms ();


  //RunLog  log;


  bool  successful = true;

  {
    successful = true;
    SipperImageExtraction  imageExtraction (*parms, successful, parms->Log ());
    if  (!successful)
    { 
      osDisplayWarning ("Failed to initialize 'SipperImageExtraction'\n" +
                        parms->StatusMessage ()
                       );
    }
    else
    {
      imageExtraction.ExtractFrames ();
    }
  }

    
  if  (parms->Cancel ())
  {
    imageExtractionWindowsDlg->ExtractionCanceled (true);
  }

  else if  (!successful)
  {
    imageExtractionWindowsDlg->ExtractionCrashed (true);
  }

  imageExtractionWindowsDlg->ExtractionStopped (true);


  parms->DoneExecuting (true);

  return 0;   // thread completed successfully
}  /* ImageExtractionSingleThreaded */





void   CImageExtractionWindowsDlg::ImageExtractionParms (SipperImageExtractionParmsPtr  _imageExtractionParms)  
{
  imageExtractionParms = _imageExtractionParms;
}  /* ImageExtractionParms */




void  CImageExtractionWindowsDlg::MultiTreadedExtractionParms (ExtractionParmsPtr  _multiTreadedExtractionParms)
{
  multiTreadedExtractionParms = _multiTreadedExtractionParms;
}


void  CImageExtractionWindowsDlg::MultiThreadedExtractionManager (ExtractionManagerPtr  _multiTreadedExtractionParms)
{
  multiThreadedExtractionManager = _multiTreadedExtractionParms;
}


void   CImageExtractionWindowsDlg::UpdateDialogStaticVariables ()
{
  if  (imageExtractionParms)
  {
    KKStr  sipperFileSizeStr = StrFormatInt64 (imageExtractionParms->SipperFileSize (), "ZZZ,ZZZ,ZZZ,ZZ0");

    m_sipperFileSize       = sipperFileSizeStr.Str ();
    m_sipperFileName       = imageExtractionParms->SipperFileName ().Str ();
    m_database             = imageExtractionParms->DataBaseServerDesc ().Str ();
    m_destinationDirectory = imageExtractionParms->OutputRootDir ().Str ();
    m_saveFrames           = imageExtractionParms->SaveFrames ();
    m_extractFeatures      = imageExtractionParms->ExtractFeatureData ();
    m_minimumSize          = imageExtractionParms->MinImageSize ();
    m_maximumSize          = imageExtractionParms->MaxImageSize ();
    m_configFileName       = osGetRootName (imageExtractionParms->ConfigFileName ()).Str ();

    m_extractProgress.SetRange32 (0, (int)(imageExtractionParms->SipperFileSize () / 1024));
    UpdateData (false);
  }

  else if  (multiTreadedExtractionParms)
  {
    KKStr  sipperFileSizeStr = StrFormatInt64 (multiTreadedExtractionParms->SipperFileSize (), "ZZZ,ZZZ,ZZZ,ZZ0");

    m_sipperFileSize       = sipperFileSizeStr.Str ();
    m_database             = multiTreadedExtractionParms->DataBaseServerDesc ().Str ();
    m_sipperFileName       = multiTreadedExtractionParms->SipperFileName ().Str ();
    m_destinationDirectory = multiTreadedExtractionParms->OutputRootDir ().Str ();
    m_saveFrames           = multiTreadedExtractionParms->SaveFrames ();
    m_extractFeatures      = multiTreadedExtractionParms->ExtractFeatureData ();
    m_minimumSize          = multiTreadedExtractionParms->MinImageSize ();
    m_maximumSize          = multiTreadedExtractionParms->MaxImageSize ();
    m_configFileName       = osGetRootName (multiTreadedExtractionParms->ConfigFileName ()).Str ();

    m_extractProgress.SetRange32 (0, (int)(multiTreadedExtractionParms->SipperFileSize () / 1024));
    UpdateData (false);
  }
}   /* UpdateDialogStaticVariables */




void CImageExtractionWindowsDlg::StartImageExtraction ()
{
  if  (imageExtractionParms)
  {
    imageExtractionThread = AfxBeginThread (ImageExtractionSingleThreaded,
                                            (LPVOID)this
                                           );

  }
  else if  (multiTreadedExtractionParms)
  {
    imageExtractionThread = AfxBeginThread (ImageExtractionMultiThreaded,
                                            (LPVOID)this
                                           );
  }
//                                          THREAD_PRIORITY_NORMAL, 
//                                          2000000
}  /* StartImageExtraction */




void CImageExtractionWindowsDlg::OnStnClickedMinimumimagesizelabel2()
{
  // TODO: Add your control notification handler code here
}


void CImageExtractionWindowsDlg::OnEnChangeMinimumsize()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}

void CImageExtractionWindowsDlg::OnBnClickedExtractfeatures()
{
  // TODO: Add your control notification handler code here
}

void CImageExtractionWindowsDlg::OnEnChangeMaximumsize()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}

void CImageExtractionWindowsDlg::OnEnChangeClass1name()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}

void CImageExtractionWindowsDlg::OnEnChangeClass0name()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}

void CImageExtractionWindowsDlg::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  
  CDialog::OnClose();
}
