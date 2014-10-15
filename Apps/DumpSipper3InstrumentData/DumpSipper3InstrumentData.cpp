#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#include  "MemoryDebug.h"


using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;

#include  "FileDesc.h"
using namespace  MLL;


#include "DumpSipper3InstrumentData.h"


//  sf C:\TrainingApp\SipperFiles\feb1906.spr -rf c:\temp\feb1906.txt

//  -sf C:\TrainingApp\SipperFiles\feb1801.spr -rf c:\temp\feb1801.txt



typedef struct 
{
    uchar pix0      : 1;
    uchar pix1      : 1;
    uchar pix2      : 1;
    uchar pix3      : 1;
    uchar pix4      : 1;
    uchar pix5      : 1;
    uchar pix6      : 1;
    uchar pix7      : 1;
    uchar pix8      : 1;
    uchar pix9      : 1;
    uchar pix10     : 1;
    uchar pix11     : 1;

    uchar grayScale : 1;  // 0 = B/W
                          // 1 = GrayScale

    uchar raw       : 1;  // 1 = Data is Raw 
                          // 0 = Compressed Run-Length

    uchar eol       : 1;  // 1 = End of Line Encountered, 
                          // 0 = End of Line not Encountered

    uchar imageData : 1;  // 0 = Instrument Data
                          // 1 = Image Data

}  Sipper3Rec;





DumpSipper3InstrumentData::DumpSipper3InstrumentData (int     argc,
                                                      char**  argv
                                                     ):
  Application  (argc, argv),
  report       (NULL),
  sipperFile   (NULL)
{
  ProcessCmdLineParameters (argc, argv);

  if  (sipperFileName.Empty ())
  {
    cerr << endl
         << endl
         << "You must provide the name of a Sipper3 file to process." << endl
         << endl;
    DisplayCmdLine ();
    osWaitForEnter ();
    exit (-1);
  }

  if  (reportFileName.Empty ())
  {
    KKStr   dirPath, rootName, extension;
    osParseFileName (sipperFileName, dirPath, rootName, extension);
    reportFileName = osAddSlash (dirPath) + rootName + ".txt";
  }

  report = new ofstream (reportFileName.Str ());
  if  (!report->is_open ())
  {
    cerr << endl
         << "*** ERROR ***,   Opening Report File[" << reportFileName << "]" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  OpenSipperFile ();
}



DumpSipper3InstrumentData::~DumpSipper3InstrumentData ()
{
  if  (report)
    if  (report->is_open ())
      report->close ();
  delete  report;

  if  (sipperFile)
  {
    fclose (sipperFile);
  }
}




void  DumpSipper3InstrumentData::DisplayCmdLine ()
{
  cout << endl
       << "DumpSipper3InstrumentData   -SF <Sipper File Name>     -RF <Report File Name>"            << endl
       <<                                                                                               endl
       << "               -SF    Requird,  name of sipper file to extract data from."                << endl
       << "               -RF    Optional,  will default to SipperFile Name with '.txt' extension."  << endl
       << endl;
}  /* DisplayCmdLine */





bool  DumpSipper3InstrumentData::ProcessCmdLineParameter (char    parmSwitchCode, 
                                                          KKStr  parmSwitch, 
                                                          KKStr  parmValue
                                                         )
{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-REPORT")     ||
       (parmSwitch == "-REPORTFILE") ||
       (parmSwitch == "-RF")         ||
       (parmSwitch == "-R") 
      )
  {
    reportFileName = parmValue;
  }

  else 
  if  ((parmSwitch == "-S")          ||
       (parmSwitch == "-SIPPERFILE") ||
       (parmSwitch == "-SF")         
      )
  {
    sipperFileName = parmValue;
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */




void  DumpSipper3InstrumentData::OpenSipperFile ()
{
  sipperFile = osFOPEN (sipperFileName.Str (), "rb");

  if  (!sipperFile)
  {
    cerr << endl
         << "***ERROR***,    Opening File[" << sipperFileName << "]" << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }
}  /* Open */





uchar  DumpSipper3InstrumentData::GetNextSipperRec (bool&  moreRecs)
{
  Sipper3Rec  sipperRec;
  size_t      recsRead;
  bool        imageData;
  bool        raw;
  bool        eol;
  bool        grayScale;
  
  uchar  sensorId = 0;

  do
  {
    do {
      recsRead =  fread (&sipperRec, 2, 1, sipperFile);
      if  (recsRead < 1)
      {
        moreRecs = false;
        return 0;
     }

      byteOffset = byteOffset + 2;
      imageData   = sipperRec.imageData;
      raw         = sipperRec.raw;
      eol         = sipperRec.eol;
      grayScale   = sipperRec.grayScale;
  //}  while  ((imageData)  ||  (!grayScale));
    }  while  (imageData);

    sensorId   = sipperRec.raw       * 32 + 
               sipperRec.grayScale * 16 + 
               sipperRec.pix11     * 8  +
               sipperRec.pix10     * 4  +
               sipperRec.pix9      * 2  +
               sipperRec.pix8;
  }  while  (sensorId != 16);

  uchar asciiChar = sipperRec.pix7 * 128  +  
                    sipperRec.pix6 * 64   +
                    sipperRec.pix5 * 32   +
                    sipperRec.pix4 * 16   +
                    sipperRec.pix3 * 8   +
                    sipperRec.pix2 * 4   +
                    sipperRec.pix1 * 2   +
                    sipperRec.pix0;

  moreRecs = true;

  return  asciiChar;
}  /* GetNextSipperRec */





void  DumpSipper3InstrumentData::ProcessSipperFile ()
{
  bool  moreRecs;
  uchar asciiChar;

  asciiChar = GetNextSipperRec (moreRecs);

  while  (moreRecs)
  {
    *report << asciiChar;
    asciiChar = GetNextSipperRec (moreRecs);
  } 
}



int  main (int  argc,  char**  argv)
{
  DumpSipper3InstrumentData  app (argc, argv);
  if  (app.Abort ())
    return -1;

  app.ProcessSipperFile ();
}  /* main */