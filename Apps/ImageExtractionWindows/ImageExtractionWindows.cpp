#include  "StdAfx.h"
#include "FirstIncludes.h"
// ImageExtractionWindows.cpp : Defines the class behaviors for the application.
//
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include  "MemoryDebug.h"

using namespace  std;

typedef wchar_t WCHAR;

#include "stdafx.h"

#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;

#include "DataBase.h"
#include "InstrumentDataFileManager.h"
#include "MLClass.h"
using namespace  MLL;

#include  "Instrument.h"
#include  "SipperVariables.h"
using namespace SipperHardware;

#include "ExtractionParms.h"
using namespace ImageExtractionManager;


#include "ImageExtractionWindows.h"
#include "ImageExtractionWindowsDlg.h"



// -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_03.spr   -A 0  -CPD 3  -min 300  -max 0  -Pre On   -d  D:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_03    -R D:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_03\Station8D_03_c0.txt   -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_03.spr  -SF Sipper3    -x  -c D:\TrainingApp\DataFiles\TrainingModels\etpB_training.cfg
// -A 0  -CPD 3  -D D:\TrainingApp\ExtractedImages\Station1G_10-xxxx\  -ipd 1000  -min 500  -max 0  -Pre On  -R D:\TrainingApp\ExtractedImages\Station1G_10-xxxx\Station1G_10_c0.txt  -S D:\TrainingApp\SipperFiles\Station1G_10.spr  -SF Sipper3  -C D:\TrainingApp\DataFiles\TrainingModels\etpB_training.cfg  -X

// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\GG1002005S_01  -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\GG1002005S_01\GG1002005S_01_c0.txt  -S F:\Pices\SipperFiles\GG1002\GG1002005S\GG1002005S_01.spr  -SF Sipper3  -C D:\Pices\DataFiles\TrainingModels\GulfOilBroad2_Discreate6_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes 

/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsApp

BEGIN_MESSAGE_MAP(CImageExtractionWindowsApp, CWinApp)
	//{{AFX_MSG_MAP(CImageExtractionWindowsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsApp construction
CImageExtractionWindowsApp::CImageExtractionWindowsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

  uint32  zed = 0;
}


CImageExtractionWindowsApp::~CImageExtractionWindowsApp()
{

}



/////////////////////////////////////////////////////////////////////////////
// The one and only CImageExtractionWindowsApp object
CImageExtractionWindowsApp theApp;




/////////////////////////////////////////////////////////////////////////////
// CImageExtractionWindowsApp initialization
BOOL CImageExtractionWindowsApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


  bool    cmdLineGood = false;
  KKStr  errorMessage;

  osRunAsABackGroundProcess ();
  RunLog  log;

  ExtractionParmsPtr  multiThreadedExtractionParms = NULL;
  SipperImageExtractionParmsPtr  imageExtractionParms = 
               new SipperImageExtractionParms ("ImageExtraction", 
                                               m_lpCmdLine, 
                                               true,               // Running as Windows App
                                               errorMessage, 
                                               cmdLineGood,
                                               log
                                              );

  if  (imageExtractionParms->MultiThreaded ())
  {
    multiThreadedExtractionParms 
      = new ExtractionParms ("ImageExtraction", 
                              m_lpCmdLine, 
                              true,               // Running as Windows App
                              errorMessage, 
                              cmdLineGood,
                              log
                             );
    delete  imageExtractionParms;
    imageExtractionParms = NULL;
  }

  if  (!errorMessage.Empty ())
  {
    int returnCd = MessageBox (NULL,
                               errorMessage.Str (),
                               "Invalid Parameters",
                               MB_OK
                              );
    imageExtractionParms->DisplayExampleCmdLine ();
    delete  imageExtractionParms;          imageExtractionParms         = NULL;
    delete  multiThreadedExtractionParms;  multiThreadedExtractionParms = NULL;
    return  false;
  }

  {
    CImageExtractionWindowsDlg dlg;

    dlg.ImageExtractionParms (imageExtractionParms);                 /**< 'dlg' will take ownership of 'imageExtractionParms'.          */
    dlg.MultiTreadedExtractionParms (multiThreadedExtractionParms);  /**< 'dlg' will take ownership of 'multiThreadedExtractionParms'.  */

	  m_pMainWnd = &dlg;
	  int nResponse = dlg.DoModal();
	  if (nResponse == IDOK)
	  {
		  // TODO: Place code here to handle when the dialog is
		  //  dismissed with OK
	  }
	  else if (nResponse == IDCANCEL)
	  {
		  // TODO: Place code here to handle when the dialog is
		  //  dismissed with Cancel
	  }
  }

  //delete  imageExtractionParms;
  imageExtractionParms = NULL;

  //InstrumentData::FinalCleanUp ();
  //Instrument::FinalCleanUp ();
  //FeatureFileIO::FinalCleanUp ();
  //FileDesc::FinalCleanUp ();
  //InstrumentDataFileManager::FinalCleanUp ();
  //MLClass::FinalCleanUp ();
  //DataBase::FinalCleanUp ();
  //SipperVariables::FinalCleanUp ();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
