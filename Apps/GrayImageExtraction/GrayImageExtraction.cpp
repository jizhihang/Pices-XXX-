#include "FirstIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#ifdef  WIN32
#include <ostream>
#include <windows.h>
#endif

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;

#include "InstrumentDataFileManager.h"
using namespace SipperHardware;

#include "Classifier2.h"
#include "DataBase.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "SipperFile.h"
#include "TrainingProcess2.h"
using namespace MLL;


#include "SipperImageExtraction.h"
#include "SipperImageExtractionParms.h"

#include "ExtractionParms.h"
#include "ExtractionManager.h"
using namespace  ImageExtractionManager;





//  -s F:\hrs0206\hrs020607.spr   -d  K:\v1\Plankton\SipperImages2002Cruise\hrs0206\hrs020607  -m 300

//  -S C:\SipperFiles\2003_July_Cruise\SipperFiles_2003_July_12\HRS_test_0302.spr  -d  C:\TrainingApp\ExtractedImages\2003_July\July_12\HRS_test_0302  -F  -M 2000

//  -s  C:\SipperFiles\2003_July_Cruise\SipperFiles_2003_July_12\test03_b02.spr   -d c:\test03_b02 -a 1  -sf 1bit -f  -m 1000

//  -s  C:\SipperFiles\2003_July_Cruise\SipperFiles_2003_July_12\test03_b02.spr   -d c:\test03_b02 -a 0  -f  -m 1000


// -s "C:\Documents and Settings\All Users\Documents\HRS0303test101.spr"   -d  C:\TrainingApp\ExtractedImages\2003_July\July_15\HRS0303test101-c0-500  -m 500  -a 0 

//  -s  e:\test0303.spr   -d c:\test0303 -a 0  -f  -m 1000 


//  -s C:\SipperFiles\2003_July_Cruise\SipperFiles_July14\HRS030101.spr -d C:\TrainingApp\ExtractedImages\HRS030101   -m 500  -a 0  -X  

// -s  C:\users\kkramer\GradSchool\Plankton\SipperFiles\2003_July_Cruise\SipperFiles_July14\HRS030101.spr  -d C:\users\kkramer\GradSchool\Plankton\ExtractedImages\HRS030101  -m 500 -x


// -s  C:\bigpine\Plankton\SipperFiles\2003_July_Cruise\SipperFiles_July14\HRS030205.spr  -d C:\TrainingApp\ExtractedImages\HRS030205 -r TestExtraction.txt -min 500 -max 2000 -x

// -s C:\users\kkramer\GradSchool\Plankton\SipperFiles\HRS020705.spr  -d C:\TrainingApp\ExtractedImages\HRS020705b -min 300 -max 2000 -x

// -s C:\TrainingApp\SipperFiles\2nd04.spr   -d  c:\Temp\2nd04  -min 100 -max 100000  

// -s C:\TrainingApp\SipperFiles\Test101.spr   -d  C:\TrainingApp\ExtractedImages\Test101  -min 100 -max 100000   -f

// -s C:\bigpine\Plankton\SipperFiles\2003_July_Cruise\SipperFiles_July14\HRS030208.spr   -d  C:\TrainingApp\ExtractedImages\HRS030208b  -min 500   -x

// -s C:\TrainingApp\SipperFiles\gps_test_01.spr  -d C:\TrainingApp\ExtractedImages\gps_test_01.spr

// -s C:\TrainingApp\SipperFiles\hrs0204\hrs020403.spr  -d C:\TrainingApp\ExtractedImages\hrs020403 -x

//  -s C:\TrainingApp\SipperFiles\egmont_SW_03.spr -d    C:\TrainingApp\ExtractedImages\egmont_SW_03-b.spr -C C:\TrainingApp\DataFiles\TrainingModels\Daniel.cfg  -x
// -s  C:\TrainingApp\SipperFiles\hrs020205.spr  -d C:\TrainingApp\ExtractedImages\hrs020205 -x  -config C:\TrainingApp\DataFiles\TrainingModels\Spain_deep.cfg
// -s  C:\TrainingApp\SipperFiles\4096\hrs020405.spr  -d C:\TrainingApp\ExtractedImages\hrs020405 -x -config C:\TrainingApp\DataFiles\TrainingModels\SimpleModel.cfg

// -s C:\TrainingApp\SipperFiles\egmontSW03.spr -d C:\TrainingApp\ExtractedImages\egmontSW03   -CountOnly

// -s C:\TrainingApp\SipperFiles\VTest1_02.spr  -d C:\TrainingApp\ExtractedImages\VTest1_02  -f  -x
// -A 0  -CPD 3  -D C:\TrainingApp\ExtractedImages\Day2_soak2_14\  -min 500  -max 0  -Pre On  -R C:\TrainingApp\ExtractedImages\Day2_soak2_14\Day2_soak2_14_c0.txt  -S K:\v1\Plankton\SipperFiles\2007_08\Day2_soak2_14.spr  -SF Sipper3  -C C:\TrainingApp\DataFiles\TrainingModels\Simple.cfg  -X

// -s D:\TrainingApp\SipperFiles\Station8D_02.spr  -d D:\TrainingApp\ExtractedImages\Station8D_02  -min 350 -ipd 3000  -config etpB_training  -x

// -A 0  -CPD 3  -D d:\TrainingApp\ExtractedImages\ETP\Station1C\Station1C_12\  -ipd 1000  -min 300  -max 0  -Pre On  -R d:\TrainingApp\ExtractedImages\ETP\Station1C\Station1C_12\Station1C_12_c0.txt  -S D:\TrainingApp\SipperFiles\etp\Station1C\Station1C_12.spr  -SF Sipper3    -X

// -s  C:\Pices\SipperFiles\WB1008DSH09A_05.spr  -d C:\Pices\ExtractedImages\DSH09\WB1008DSH09A_05  -r C:\Pices\ExtractedImages\DSH09\WB1008DSH09A_05.txt -min 250 -c oil.cfg -x -co

// -s  C:\Pices\SipperFiles\ETP2008\ETP2008_1D\ETP2008_1D_03.spr  -d D:\Pices\ExtractedImages\ETP2008_1D\ETP2008_1D_03  -r D:\Pices\ExtractedImages\ETP2008_1D\ETP2008_1D_03\ETP2008_1D_03.txt -min 250 -c etp_08_BFS.cfg -x -co
// -s  C:\Pices\SipperFiles\ETP2008\ETP2008_1E\ETP2008_1E_01.spr  -d D:\Pices\ExtractedImages\ETP2008_1E\ETP2008_1E_01  -r D:\Pices\ExtractedImages\ETP2008_1E\ETP2008_1E_01\ETP2008_1E_01.txt -min 250 -c etp_08_BFS.cfg -x -co

// -s D:\Pices\SipperFiles\SML751001\34\SMP751001034_West_08.spr  -d D:\Pices\ExtractedImages\SMP751001034_West_08  -r D:\Pices\ExtractedImages\SMP751001034_West_08\SMP751001034_West_08.txt -min 150 -c GulfOilBroad2_Discreate_Dual.cfg -co

void  MakeASipperWithDupImages ()
{
  FILE* in = osFOPEN ("C:\\TrainingApp\\SipperFiles\\HRS020705.spr", "rb");
  if  (!in)
  {
    cerr << "Could not open SIPPER file." << endl;
    exit (-1);
  }

  FILE* out = osFOPEN ("C:\\TrainingApp\\SipperFiles\\HRS020705_Dup.spr", "wb");

  int  buffSize = 1000000;
  KKU::uchar*  buff = new KKU::uchar[buffSize];

  int recsRead  =  (int)fread (buff, buffSize, 1, in);
  if  (recsRead <= 0)
  {
  	cerr << endl << endl << endl
  	     << "MakeASipperWithDupImages  The fread failed." << endl
  	     << endl;
  }
  
  int recsWritten =  (int)fwrite (buff, buffSize, 1, out); 
  recsWritten =  (int)fwrite (buff, buffSize, 1, out); 
  fclose (in);
  fclose (out);
}  /* MakeASipperWithDupImages */



void  Test ()
{
  RunLog   runLog ("c:\\Temp\\TestTrainingProcess.txt");
  KKStr    configFileName = "etp_hierarchy.cfg";
  FileDescPtr  fd = FeatureFileIOPices::NewPlanktonFile (runLog);
  
  InstrumentDataFileManager::InitializePush ();


  bool*     cancelFlag = new bool (false);
  KKStrPtr  statusMsg  = new KKStr ();


  TrainingProcess2Ptr  trainer = 
              new TrainingProcess2 (configFileName, 
                                   fd, 
                                   runLog, 
                                   false,         // false = Features are not Already Normalized
                                   *cancelFlag, 
                                   *statusMsg
                                  );
  delete  trainer;

}



#include "InstrumentData.h"
#include "InstrumentDataManager.h"
#include "InstrumentDataFileManager.h"
#include "SipperBuff.h"
#include "SipperBuffBinary.h"
#include "SipperBuff3Bit.h"
#include "Sipper3Buff.h"
#include "Sipper3RevBuff.h"



// I added this code as a one time fix to update all the Instrument Data tables after I made sure that the
// Sipper file definitions were correct.
void  ReFreshInstrumentData (RunLog&  log)
{
  DataBase dbConn (log);

  InstrumentDataFileManager::InitializePush ();
  SipperFileListPtr  sipperFiles = dbConn.SipperFileLoad ("ETP2008", "", "");

  SipperFileList::iterator  idx;

  bool   cancelFlag = false;
  char   msgBuff[1024];

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

    dbConn.InstrumentDataReFreshSipperFile (sf->SipperFileName (), cancelFlag, msgBuff, sizeof (msgBuff));
  }

  InstrumentDataFileManager::InitializePop ();
  InstrumentData::FinalCleanUp ();

  exit (-1);

}  /* ReFreshInstrumentData */




void  TestKKFFTW ()
{
  bool  valid = false;
  KKStr  imageName = "E:\\Pices\\TrainingLibraries\\etp_08\\gelatinous_hydromedusae_hydroid\\ETP2008_8A_06_07920105_3528.bmp";
  RasterSipperPtr  r = new RasterSipper (imageName, valid);
  if  (valid)
  {
    RasterSipperPtr  ft1 = r->FastFourier ();
    RasterSipperPtr  ft2 = r->FastFourierKK ();

    float* f1 =  ft1->FourierMagArea ();
    float* f2 =  ft2->FourierMagArea ();

    uint32  numPixels = r->Height () * r->Width ();

    float  largestDelta = 0.0f;

    for  (uint32 x = 0;  x < numPixels;  ++x)
    {
      float  delta = f1[x] - f2[x];
      if  (delta != 0.0f)
      {
        cout << x << "\t" << f1[x] << "\t" <<  f2[x] << "\t" << (delta) << "\t" << largestDelta << endl;
        if  (fabs (delta) > fabs (largestDelta))
          largestDelta = delta;
      }
    }

    delete  ft1;  ft1 = NULL;
    delete  ft2;  ft2 = NULL;
  }

  delete  r;

}  /* TestKKFFTW */



void  CointToScanLine71787452 ()
{
  RunLog runLog;

  SipperFilePtr  sfe = new SipperFile ("WB0813DSH09_05.spr");
  InstrumentDataManagerPtr  instrumentDataManager 
    = new InstrumentDataManager ("C:\\Pices\\SipperFiles\\WB0813\\WB0813DSH09_05.spr", sfe,  runLog);
                   
  SipperBuffPtr  sf = SipperBuff::CreateSipperBuff ("C:\\Pices\\SipperFiles\\WB0813\\WB0813DSH09_05.spr",
                                                    0, //                 _cameraNum,
                                                    instrumentDataManager,
                                                    runLog
                                                   );

  uchar   lineBuff[10000];
  uint32  lineBuffSize = 4096;
  uint32  lineSize = 0;
  uint32  colCount[4096];
  uint32  pixelsInRow = 0;
  bool  flow = false;

  uint32  lineCount = 0;
  sf->GetNextLine (lineBuff, lineBuffSize, lineSize, colCount, pixelsInRow, flow);
  while  (!sf->Eof ())
  {
    ++lineCount;
    if  (lineCount > 71787452)
      break;

    if  ((lineCount % 10000) == 0)
    {
      cout << StrFormatInt (lineCount, "###,###,##0") << "\t" << StrFormatInt (sf->ByteOffset (), "###,###,###,##0") << endl;
    }

    sf->GetNextLine (lineBuff, lineBuffSize, lineSize, colCount, pixelsInRow, flow);
  }

  int64  byteOffset = sf->ByteOffset ();

  delete  sf;
  sf = NULL;
}


void  CopyOverSipperFileXXX ()
{
  uchar  buff[10240];

  FILE*  i = KKU::osFOPEN ("C:\\Pices\\SipperFiles\\WB0813\\WB0813DSH09_05.spr", "rb");
  FILE*  o = KKU::osFOPEN ("C:\\Pices\\SipperFiles\\WB0813\\WB0813DSH09_05a.spr", "wb");

  int64  bytesToWrite = 1408806768;
  int64  bytesCopied = 0;
  int64  bytesLeft = bytesToWrite;

  while  (bytesLeft > 0)
  {
    int32  bytesThisLoop = sizeof (buff);
    if  (bytesLeft < bytesThisLoop)
      bytesThisLoop = bytesLeft;

    fread  (buff, 1, bytesThisLoop, i);
    fwrite (buff, 1, bytesThisLoop, o);
    bytesLeft -= bytesThisLoop;
  }

  fclose (i);
  fclose (o);
}  /* CopyOverSipperFile */





//**************************************************************************
//*                                                                        *
//*                           The Main Module                              *
//*                                                                        *
//*                                                                        *
//*                                                                        *
//*                                                                        *
//*                                                                        *
//**************************************************************************

int  main (int  argc,  char** argv)
{
  //#if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
   _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  //#endif

  {
    RunLog  log;

 //  CopyOverSipperFileXXX ();
   //exit (-1);


    bool  useMultiThreadedVersion = false;

    //ReFreshInstrumentData (log);
    //exit(-1);

    cout  << "GrayImageExtraction - Version[" << VERSION << "]."  << endl;

    KKStr  errorMessage;
    bool    parmsGood = false;

    SipperImageExtractionParms  parms (argc, 
                                       argv, 
                                       false,         // Don't run as Windows
                                       errorMessage,  // Error message if Parms is bad.
                                       parmsGood,
                                       log
                                      );

    if  (!parmsGood)
    {
      parms.DisplayExampleCmdLine ();
      exit (-1);
    }

    osCreateDirectoryPath (parms.OutputRootDir ());

    if  (parms.MultiThreaded ())
    {
      cout << endl << endl << "Multi-Threaded seleceted." << endl << endl;
      KKStr  errorMessage = "";
      bool   parmsGood    = true;
        
      ExtractionParms emParms (argc, argv, false, errorMessage, parmsGood, log);
      if  (parmsGood)
      {
        int32  numberOfProcessors = osGetNumberOfProcessors ();
        if  (numberOfProcessors < 1)
          numberOfProcessors = 1;

        cout << endl << endl << "Multi-Threaded seleceted: " << numberOfProcessors << endl << endl;

        ExtractionManager  extractionManager ("GrayImageExtraction", emParms, numberOfProcessors, log);
        bool  successful = false;
        extractionManager.ManageTheExtraction (successful);
      }
    }
    else
    {
      cout << endl << endl << "Single-Threaded seleceted." << endl << endl;

      bool  successfull = true;
      SipperImageExtraction  imageExtraction (parms, successfull, log);
      if  (successfull)
        imageExtraction.ExtractFrames ();
    }
  }

  InstrumentDataFileManager::FinalCleanUp ();
  Instrument::FinalCleanUp ();
  FileDesc::FinalCleanUp ();
  MLClass::FinalCleanUp ();
  InstrumentData::FinalCleanUp ();

  //#if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
  //_CrtDumpMemoryLeaks();
  //#endif
  
  return 0;
} /* main */
