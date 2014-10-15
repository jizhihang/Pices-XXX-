#include  "FirstIncludes.h"
#include <stdio.h>
#include <ctype.h>

#include  <time.h>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <map>
#include  <vector>

#ifdef WIN32
#include <windows.h>
#endif

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace  std;


#include "BasicTypes.h"
#include "BMPImage.h"
#include "ImageIO.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
#include "StatisticalFunctions.h"
using namespace  KKU;


#include "InstrumentData.h"
#include "InstrumentDataManager.h"
#include "InstrumentDataFileManager.h"
#include "SipperBuff.h"
#include "SipperBuffBinary.h"
#include "SipperBuff3Bit.h"
#include "Sipper3Buff.h"
#include "Sipper3RevBuff.h"
using namespace SipperHardware;


#include "Classifier2.h"
#include "DataBase.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "SipperBlob.h"
#include "TrainingProcess2.h"
using namespace  MLL;



#include "SipperImage.h"
#include "SipperImageExtraction.h"


// -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_02.spr   -A 0  -CPD 3  -min 300  -max 0  -Pre On   -d  D:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_02    -R C:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_02\Station8D_02_c0.txt   -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_02.spr  -SF Sipper3    -x  -c D:\TrainingApp\DataFiles\TrainingModels\etpB_training.cfg

// D:\TrainingApp\ExtractedImages\Station1FF_05-Old
// -A 0  -CPD 3  -D D:\TrainingApp\ExtractedImages\Station1FF_05_new\  -min 300  -max 0  -Pre On  -R d:\TrainingApp\ExtractedImages\Station1FF_05_new\Station1FF_15_c0.txt  -S D:\TrainingApp\SipperFiles\Panama\Station1F\Station1FF_05.spr  -SF Sipper3    -x
// -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_02.spr   -A 0  -CPD 3  -min 300  -max 0  -Pre On   -d  C:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_02    -R C:\TrainingApp\ExtractedImages\Panama\Station8D\Station8D_02\Station8D_02_c0.txt   -S D:\TrainingApp\SipperFiles\Panama\Station8D\Station8D_02.spr  -SF Sipper3    -x  -c D:\TrainingApp\DataFiles\TrainingModels\etpB_training.cfg

//  -s C:\SipperFiles\2003_July_Cruise\SipperFiles_July14\HRS030207.spr      -d  C:\TrainingApp\ExtractedImages\Panama\Station8D\
//  -s C:\TrainingApp\SipperFiles\hrs020407.spr   -d  C:\TrainingApp\ExtractedImages\hrs020407  -x


//  -s C:\SipperFiles\4096\hrs020401.spr   -d  C:\Sipper4096Images\hrs020401 -r C:\Sipper4096Images\hrs020401_NoSave.txt

//  -s C:\SipperFiles\BinaryWeHope\HRS_02_02_02.spr   -d  C:\Dest\HRS_02_02_02 -r  C:\Dest\HRS_02_02_02\HRS_02_02_02.txt  -m 200 -sf Binary -c 1

//  -s C:\SipperFiles\BinaryWeHope\E01b_05net7.spr   -d  C:\Dest\E01b_05net7 -r  C:\Dest\E01b_05net7\E01b_05net7.txt  -m 200 -sf Binary -c 1

//  -s C:\SipperFiles\2002_Sept_Grayscale\hrs0206\hrs020601.spr   -d  K:\v1\Plankton\SipperImages2002Cruise\hrs0206\hrs020601_500  -m 500

//  -s C:\SipperFiles\2002_Sept_Grayscale\hrs0206\hrs020605.spr   -d  C:\temp\hrs020605_300_C_drive  -m 300


//  -s "C:\Documents and Settings\All Users\Documents\Billmoving01.spr"   -d  C:\TrainingApp\ExtractedImages\2003_July\July_12\Billmoving01  -m 2000 -f -sf 1Bit

//  -s C:\SipperFiles\2003_July_Cruise\SiupperFiles_July15\HRS0303test101.spr    -d  C:\TrainingApp\ExtractedImages\HRS0303test101   -m 1000  -a 0 -X

// 2005-July-23   - New Siper 3 file test
// -S  C:\TrainingApp\SipperFiles\Tampa_Bay_2005-07-07-A02.spr  -D C:\TrainingApp\ExtractedImages\Sipper3_Test  -sf Sipper3


// 2005-Aug-12   - Test SIPPER 3 data reports
// -S  C:\TrainingApp\SipperFiles\Test_2005-08-10B_UART_01.spr  -D C:\TrainingApp\ExtractedImages\Test_2005-08-10B_UART_01.spr  -sf Sipper3

// 2005-Sep-13   - Test SIPPER 3 data reports
// -S  C:\TrainingApp\SipperFiles\test_2005-09-09test-d08.spr  -D C:\TrainingApp\ExtractedImages\test_2005-09-09test-d08  -sf Sipper3  -t 160000

// -S  K:\v1\Plankton\SipperFiles\hrs0204\hrs020401.spr  -D C:\TrainingApp\ExtractedImages\hrs020401-300  -sf 3bit  


// -C C:\TrainingApp\DataFiles\TrainingModels\OnePercentAndGreaterModel.cfg -CountOnly -x -s C:\TrainingApp\SipperFiles\hrs020501.spr -x -d C:\TrainingApp\ExtractedImages\hrs020501
/*
  cout << "  GrayImageExtraction  -s   <sipper file name>"                   << endl
       << "                       -d   <destination directory.>"             << endl
       << "                       -r   <report file name>"                   << endl
       << "                       -m   <minimum size/ defaults 100>"         << endl
       << "                       -min <minimum size/ defaults 100>"         << endl
       << "                       -max <maximum size/ ignored by default>"   << endl
       << "                       -f   <save raw frames>"                    << endl
       << "                       -sf  <1Bit | 3Bit(Default)>"               << endl
       << "                       -c   <Configuration Model>"                << endl
       << "                       -t   <start scan line>"                    << endl
       <<                                                                       endl
       << "  -A    <Camera Number>"                
*/

// -s C:\TrainingApp\SipperFiles\egmont_SW_03.spr  -d C:\TrainingApp\ExtractedImages\egmont_SW_03_TestCPD-zzzzz -cpd 3  -c C:\TrainingApp\DataFiles\TrainingModels\FiveBigOnes.cfg  -x

//-s C:\TrainingApp\SipperFiles\primavista_05.spr -d C:\TrainingApp\ExtractedImages\primavista_05-b   -Min 1000 -x

// -c C:\TrainingApp\DataFiles\TrainingModels\SimpleModel.cfg -s C:\TrainingApp\SipperFiles\HRS030207.spr  -d C:\TrainingApp\ExtractedImages\HRS030207_c   -Min 300 -x

//  -CPD 3  -D C:\TrainingApp\ExtractedImages\VTest1_02  -min 500  -max 0  -Pre On  -R C:\TrainingApp\ExtractedImages\VTest1_02\VTest1_02.txt  -S C:\TrainingApp\SipperFiles\VTest1_02.spr    -X  -SF Sipper3

//  -s "C:\TrainingApp\SipperFiles\Test2007Aug10_02.spr"  -d "d:\TrainingApp\ExtractedImages\Test2007Aug10_02"  -min 500  -cpd 3  -SaveBefore  -FO

//  -s "C:\TrainingApp\SipperFiles\Test2007Aug10_02.spr"  -d "d:\TrainingApp\ExtractedImages\Test2007Aug10_02"  -min 500  -cpd 3  -SaveBefore  -FO

// -A 0  -CPD 3  -D d:\TrainingApp\ExtractedImages\F1\  -min 500  -max 0  -Pre On  -R d:\TrainingApp\ExtractedImages\F1\F1_c0.txt  -S D:\TrainingApp\SipperFiles\Panama\F1.spr  -SF Sipper3  -C d:\TrainingApp\DataFiles\TrainingModels\SimpleModel.cfg  -X

// -A 0  -CPD 3  -D d:\TrainingApp\ExtractedImages\Station1FF_15_New\  -min 300  -max 0  -Pre On  -R d:\TrainingApp\ExtractedImages\Station1FF_15_New\Station1FF_15_c0.txt  -S D:\TrainingApp\SipperFiles\Panama\Station1F\Station1FF_15.spr  -SF Sipper3    -X

// -A 0  -CPD 3  -D d:\TrainingApp\ExtractedImages\Station2_01\  -min 300  -max 0  -Pre On  -R d:\TrainingApp\ExtractedImages\Station2_01.spr\Station2_01.txt  -S D:\TrainingApp\SipperFiles\Station2_01.spr  -SF Sipper3    -X  -c etpC_training.cfg

// -A 0  -CPD 3  -D D:\TrainingApp\ExtractedImages\ETP\FR3_02\  -ipd 5000  -min 200  -max 0  -Pre On  -R D:\TrainingApp\ExtractedImages\ETP\FR3_02\FR3_02.txt  -S D:\SipperFiles\Flume_2008-03-14\FR3_02.spr  -SF Sipper3    -X


// -c WFS_Tuned.cfg  -s C:\TrainingApp\SipperFiles\ETP2008\ETP2008_1A\ETP08_1A_01.spr  -d d:\Temp\TestImageExtraction\ -min 300 -ipd 5000  -x


//  -c etpB_training.cfg -s C:\TrainingApp\SipperFiles\Station8E_02.spr   -d c:\temp\ImageExtraction\Station8E_02  -x  

// -c etpC_training.cfg  -s D:\TrainingApp\SipperFiles\Station8D_02.spr  -d d:\Temp\Station8D_02_WithReduction\ -min 300 -ipd 5000  -x

//  -s D:\TrainingApp\SipperFiles\ETP2008\ETP2008_1AA_01.spr -d d:\Temp\TestImageExtraction\ETP2008_1AA_01\  -min 300 -ipd 5000  -x  -c etp_hierarchy

// -c etp_hierarchy  -s F:\Pices\SipperFiles\ETP2008_4bA_04.spr  -d d:\Temp\TestImageExtraction\ETP2008_4bA_04\  -min 250 -ipd 5000  -x  

//  -c WFS_Binary_Tuned.cfg    -s D:\Pices\SipperFiles\ETP2008\ETP2008_1A\ETP2008_1A_01.spr  -d d:\Temp\ExtractedImages\ETP2008_1A_01  -min 250  -ipd 5000  -x  -CountOnly

//  -c etp08_drew.cfg    -s D:\Pices\SipperFiles\ETP2008\ETP2008_8B\ETP2008_8B_01.SPR  -d d:\Pices\ExtractedImages\ETP2008\ETP2008_8B  -min 250  -ipd 5000 -x     -countonly

//  -c etp08_drew.cfg    -s C:\Pices\SipperFiles\TeatCruise\TestCruise_01  -d d:\Pices\ExtractedImages\TestCruise\TestCruise1 -min 250  -ipd 5000 -x     -countonly

//  -c megalibrary_1   -s  C:\Pices\SipperFiles\HRS02\hrs020508.spr -d c:\Pices\ExtractedImages\HRS02\hrs020508 -Countonly  -min 250 -x

// -s D:\Pices\SipperFiles\ETP2008\ETP2008_8E\ETP2008_8E_13.spr  -min 250  -c


// C:\Pices\SipperFiles\SML751001\37
// D:\Pices\SipperFiles\ETP2008\ETP2008_1F
// -s ETP2008_1F_02.spr  -min 150  -d c:\Pices\ExtractedImages\ETP2008\ETP2008_1F\ETP2008_1F_02  -x  -c ETP08_station1_harvest_Binary.cfg  

// C:\Pices\SipperFiles\SML751001\37


// -s C:\Pices\SipperFiles\ETP2008\ETP2008_1F\ETP2008_1F_02.spr  -min 250 -c C:\Pices\DataFiles\TrainingModels\ETP08_station1_harvest.cfg  -x -co
// -s C:\Pices\SipperFiles\ETP2008\ETP2008_1F\ETP2008_1F_02.spr  -min 250   -x -co

// -s C:\Pices\SipperFiles\SML751001\37\SMP751001037_01.spr  -min 150 -c Oil  -x -co -d c:Pices\ExtractedImages\SMP751001\SMP751001037\SMP751001037_01

// -s C:\Pices\SipperFiles\ETP2008\ETP2008_1D\ETP2008_1D_01.spr  -min 150 -c ETP08_station1_harvest.cfg -x -co -d c:Pices\ExtractedImages\ETP2008\ETP2008_1D\ETP2008_1D_01


// -s D:\Users\kkramer\GradSchool\SipperProject\SipperSoftware\Apps\SipperFileEncoder\Shrimp.spr  -min 150 -d D:\Pices\ExtreactedImages\AEOS\Shrimp\

// -s C:\Pices\SipperFiles\WB1101\WB1101NT24\WB1101NT24A_01.spr  -d D:\Pices\ExtractedImages\WB1101\WB1101NT24\WB1101NT24A_01  -co -x -c ETP08_station1_harvest.cfg  -min 250


// -D C:\Pices\ExtractedImages\SMP751001034_02\  -ipd 1000  -min 100  -R C:\Pices\ExtractedImages\SMP751001034_02\SMP751001034_02_c0.txt  -S E:\Pices\SipperFiles\SMP751001\SMP751001_034\East\SMP751001034_02.spr  -SF Sipper3  -C Oil_MFS  -X  -CountOnly


// -s C:\Pices\SipperFiles\ETP2008\ETP2008_1A\ETP08_1A_02.spr -d C:\Pices\ExtractedImages\ETP08_1A_02\ETP08_1A_02 -co -x -c etp_08_BFS
// -s D:\Pices\SipperFiles\ETP2008\ETP2008_8E\ETP2008_8E_05.spr  -d D:\Pices\ExtractedImages\ETP2008\ETP2008_8E\ETP2008_8E_05  -x -c SixClasses.cfg
// -s D:\Pices\SipperFiles\ETP2008\ETP2008_8E\ETP2008_8E_05.spr  -d D:\Pices\ExtractedImages\ETP2008\ETP2008_8E\ETP2008_8E_05  -x -c SixClasses.cfg  -Min 100 -Max 250

// -s E:\Pices\SipperFiles\ETP2008\ETP2008_1A\ETP08_1A_02.spr  -d  E:\Pices\ExtractedImages\ETP2008\ETP2008_1A -co -x -c NRDA_small.cfg
// -s E:\Pices\SipperFiles\SMP7510010\SMP751001034\Part1\SMP751001034_01.spr  -d  E:\Pices\ExtractedImages\SMP7510010\SMP751001034_01\ -co -x -c NRDA_small.cfg  -min 250 

// -s E:\Pices\SipperFiles\SMP7510010\SMP751001034\Part1\SMP751001034_01.spr  -d  E:\Pices\ExtractedImages\SMP7510010\SMP751001034_01\ -co -x -c NRDA_small.cfg  -min 250 

// -s D:\Pices\SipperFiles\ETP2008\Part1\ETP2008_1A\ETP08_1A_02.spr     -d D:\Pices\ExtractedImages\ETP2008\ETP08_1A_02     -x   -c SixClasses.cfg  -co -min 150
// -s D:\Pices\SipperFiles\ETP2008\Part1\ETP2008_1A\ETP2008_1AA_02.spr  -d D:\Pices\ExtractedImages\ETP2008\ETP2008_1AA_02  -x   -c SixClasses.cfg  -co -min 150


// -s E:\Pices\SipperFiles\SMP7510010\SMP751001034\Part3\SMP751001034_West_08.spr  -d D:\Pices\ExtractedImages\SMP751001034\SMP751001034_West_08 -x -c SixClasses.cfg  -co -min 350
// -CPD 3  -D E:\Pices\ExtractedImages\WB0813PCB02_01\  -ipd 1000  -min 250  -max 0  -Pre On  -R E:\Pices\ExtractedImages\WB0813PCB02_01\WB0813PCB02_01_c0.txt  -S C:\Pices\SipperFiles\WB0813\WB0813PCB02\WB0813PCB02_01.spr  -SF Sipper3  -C E:\Pices\DataFiles\TrainingModels\NRDA_SMP751001_37_BFS.cfg  -ScanLineStart 3103522  -X  -CountOnly  -MultiThreaded Yes 


// -A 0  -CPD 3  -D C:\Pices\ExtractedImages\BE0612ST03_18\  -ipd 1000  -min 250  -max 0  -Pre On  -R C:\Pices\ExtractedImages\BE0612ST03_18\BE0612ST03_18_c0.txt  -S BE0612ST03_18.spr  -SF Sipper3  -C C:\Pices\DataFiles\TrainingModels\Gulf_no_oil_MFS.cfg  -ScanLineEnd 4697356  -CountOnly  -MultiThreaded Yes -DataBase Default

//  -d D:\Pices\ExtractedImages\BE0612ST03_18  -min 250 -s BE0612ST03_18.spr -c Gulf_no_oil_MFS  -CountOnly
// F:\Pices\SipperFiles\USF\BE0612\BE0612ST03


// -CPD 3  -d E:\Pices\ExtractedImages\WB0813DSH10S4_01   -min 250  -s WB0813DSH10S4_01.spr  -SF S4   -c GulfOilBroad_MFS  -X  -CountOnly  -MultiThreaded Yes 
// "E:\Pices\SipperFiles\WB2013-08_Sipper4\lir.20.SIPPER 4 August 2013 Trials.DSH09.UDR 1"


// -CPD 3  -d E:\Pices\ExtractedImages\WB0813DSH08_0006\  -ipd 1000  -min 500  -max 0  -Pre On  -S WB0813DSH08_0006.spr  -SF Sipper4Bit  -C E:\Pices\DataFiles\TrainingModels\GulfOilBroad_MFS.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase COT
// E:\Pices\SipperFiles\WB0813_S4\WB0813-S4_DSH09

inline  int  Max (int x1,  int x2,  int x3, int x4,  int x5,  int x6)
{
  int  r = Max (x1, x2);

  if  (x3 > r)  r = x3;
  if  (x4 > r)  r = x4;
  if  (x5 > r)  r = x5;
  if  (x6 > r)  r = x6;

  return  r;
}  /* Max */





inline  int  Max (int x1,  int x2,  int x3,  
                  int x4,  int x5,  int x6,  
                  int x7,  int x8,  int x9
                 )
{
  int  r;
  
  if  (x1 > x2)
    r = x1;
  else 
    r = x2;

  if  (x3 > r)  r = x3;
  if  (x4 > r)  r = x4;
  if  (x5 > r)  r = x5;
  if  (x6 > r)  r = x6;
  if  (x7 > r)  r = x7;
  if  (x8 > r)  r = x8;
  if  (x9 > r)  r = x9;

  return  r;
}  /* Max */




/**
 @brief  Returns the number of different neighobors.  'x1' - 'x9' are the blob id's of the neighbors you want to check.
 */
int  CountPixs (int x1,  int x2,  int x3,  
                int x4,  int x5,  int x6,  
                int x7,  int x8,  int x9
               )
{
  int  count = 0;
  vector<int>  ids;

  if  (x1 >= 0)
    ids.push_back (x1);
  if  (x2 >= 0)
    ids.push_back (x2);
  if  (x3 >= 0)
    ids.push_back (x3);
  if  (x4 >= 0)
    ids.push_back (x4);
  if  (x5 >= 0)
    ids.push_back (x5);
  if  (x6 >= 0)
    ids.push_back (x6);
  if  (x7 >= 0)
    ids.push_back (x7);
  if  (x8 >= 0)
    ids.push_back (x8);
  if  (x9 >= 0)
    ids.push_back (x9);

  if  (ids.size () < 1)
    return 0;

  sort (ids.begin (), ids.end ());
  int lastId = -1;

  for  (KKU::uint x = 0;  x < ids.size ();  x++)
  {
    if  (ids[x] != lastId)
    {
      count++;
      lastId = ids[x];
    }
  }

  return  count;
}






// I added this code as a one time fix to update all the Instrument Data tables after I made sure that the
// Sipper file definitions were correct.
void  UpdateInstrumentData (DataBasePtr  dbConn,
                            KKStr        sipperFileName,
                            RunLog&      log
                           )
{
  bool   cancelFlag = false;

  SipperFilePtr sf = dbConn->SipperFileRecLoad (sipperFileName);
  if  (!sf)
    return;

  cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

  KKU::uint  lastScanLine = 0;
  KKU::uint  nextScanLine = 0;

  InstrumentDataListPtr  id = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile (sipperFileName, sf, cancelFlag, log);;
  if  (id != NULL)
  {
    dbConn->InstrumentDataSaveListForOneSipperFile (sf->SipperFileName (), *id, cancelFlag);

    InstrumentDataList::iterator  idx;
    for  (idx = id->begin ();  idx != id->end ();  idx++)
    {
      InstrumentDataPtr  id = *idx;
      nextScanLine = id->ScanLine ();
      dbConn->ImageUpdateInstrumentDataFields (id, osGetRootName (sipperFileName), lastScanLine, nextScanLine + 100);
      lastScanLine = nextScanLine;
    }
  }

  delete  id;
  id = NULL;
}  /* UpdateInstrumentData */




// I added this code as a onetime fix to update all the Instrument Data tables after I made sure that the
// Sipper file definitions were correct.
void  UpdateInstrumentData (DataBasePtr  dbConn,
                            RunLog&      log
                           )
{
  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad ("ETP2008", "", "");

  bool  cancelFlag = false;

  SipperFileList::iterator  idx;

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

    UpdateInstrumentData (dbConn, sf->SipperFileName (), log);
  }

  delete  sipperFiles;
  sipperFiles = NULL;
}  /* UpdateInstrumentData */





// I added this code as a one time fix to update all the Instrument Data tables after I made sure that the
// Sipper file definitions were correct.
void  ReFreshInstrumentData (DataBasePtr  dbConn,
                             RunLog&      log
                            )
{
  InstrumentDataFileManager::InitializePush ();
  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad ("ETP2008", "", "");

  SipperFileList::iterator  idx;

  bool   cancelFlag = false;
  char   msgBuff[1024];

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    if  (sf->CruiseName ().EqualIgnoreCase ("SMP751001"))
    {
      cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;
      dbConn->InstrumentDataReFreshSipperFile (sf->SipperFileName (), cancelFlag, msgBuff, sizeof (msgBuff));
    }
  }

  InstrumentDataFileManager::InitializePop ();

}  /* ReFreshInstrumentData */




void  UpdateInstrumentDataFieldsInFeatrureRecords (DataBasePtr  dbConn,
                                                   RunLog&      log
                                                  )
{
  InstrumentDataFileManager::InitializePush ();

  bool  cancelFlag = false;

  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad ("", "", "");

  if  (!sipperFiles)
    return;

  SipperFileList::iterator  idx;

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

    ImageFeaturesListPtr data = dbConn->FeatureDataGetOneSipperFile (sf->SipperFileName (), NULL, 'P', false, cancelFlag);

    ImageFeaturesList::iterator  idx2;
    for  (idx2 = data->begin ();  idx2 != data->end ();  idx2++)
    {
      ImageFeaturesPtr fd = *idx2;

      KKStr  imageFileName = osGetRootName (fd->ImageFileName ());

      InstrumentDataPtr  id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, cancelFlag, log);
      if  (id)
      {
        if  ((id->Depth        () != fd->Depth        ())  ||
             (id->Fluorescence () != fd->Fluorescence ())  ||
             (id->Salinity     () != fd->Salinity     ())  ||
             (id->Oxygen       () != fd->Oxygen       ())
            )
        {
          fd->Depth        (id->Depth        ());
          fd->Fluorescence (id->Fluorescence ());
          fd->Salinity     (id->Salinity     ());
          fd->Oxygen       (id->Oxygen       ());

          dbConn->FeatureDataUpdate (NULL,   // NULL = Not providing DatabseImage; get ImageID from Images Table using ImageFileName in "fd".
                                     fd
                                    );
        }
      }
    }

    delete  data;
    data = NULL;
  }

  delete  sipperFiles;

  InstrumentDataFileManager::InitializePop ();
}  /* UpdateInstrumentDataFieldsInFeatrureRecords */






void  SipperImageExtraction::CreateAllSipperFileRecs (DataBasePtr  dbConn,
                                                      RunLog&      log
                                                     )
{
  VectorKKStr  sipperFileNanes = InstrumentDataFileManager::GetListOfSipperFiles (log);

  VectorKKStr::iterator  idx;

  for  (idx = sipperFileNanes.begin ();  idx != sipperFileNanes.end ();  idx++)
  {
    KKStr  fn = *idx;
    KKStr  rn = osGetRootName (fn);

    KKStr  first3Chars = rn.SubStrPart (0, 2).ToLower ();
    if  ((first3Chars == "sta")  ||  (first3Chars == "etp"))
    {
      KKStr  fullFileName = InstrumentDataFileManager::GetFullSipperFileName (rn);

      SipperFilePtr  sipperFileRec = new SipperFile (rn);

      sipperFileRec->AssignCtdExternalInstruments (rn);

      dbConn->SipperFileInsert (*sipperFileRec);
      delete  sipperFileRec;
      sipperFileRec = NULL;
    }
  }
}  /* CreateAllSipperFileRecs */
                                                  





SipperImageExtraction::SipperImageExtraction (SipperImageExtractionParms&  _parms,
                                              bool&                        _successful,
                                              RunLog&                      _log
                                             ):

  connectedPixelDist         (_parms.ConnectedPixelDist ()),
  parms                      (_parms),
  fileDesc                   (FeatureFileIOPices::NewPlanktonFile (_log)),
  veryLargeImageSize         (10000),
  imagesWrittenInFrame       (0),
  frame                      (NULL),
  frameArea                  (NULL),
  frameRowByteOffset         (NULL),
  origFrame                  (NULL),
  origFrameArea              (NULL),
  workFrame                  (NULL),
  workFrameArea              (NULL),
  totPixels                  (0),
  frameNum                   (0),
  frameSipperRow             (0),
  endOfFileReachedInLastCall (false),

  blobs                      (NULL),
  numOfBlobsInFrame          (0),
  maxBlobsPerFrame           (50000),

  scanLineEnd                (0),
  scanLineStart              (0),

  blobIds                    (NULL),
  blobIdsArea                (NULL),
  trainer                    (NULL),
  classifier                 (NULL),
  lastRowInFrame             (0),
  lastRowUsed                (0),
  maxFrameHeight             (0),
  nextFrameStartRow          (0),
  numImagesWritten           (0),
  pixsInRows                 (NULL),
  
  reportFile                 (),
  sipperBuff                 (NULL),
  sipperRootName             (),
  instrumentDataManager      (NULL),
  unKnownMLClass          (NULL),
  dupImageDetector           (NULL),
  duplicateImageDir          (),
  duplicateImages            (NULL),
  imageManager               (NULL),
  log                        (_log),

  dbConn                     (NULL),
  sipperFileRec              (NULL)

{
  log.Level (10) << "SipperImageExtraction::SipperImageExtraction" << endl;

  _successful = true;

  if  (parms.UpdateDataBase ())
  {
    dbConn = new DataBase (parms.DataBaseServer (), log);
    if  (!dbConn->Valid ())
    {
      log.Level (-1) << endl << "SipperImageExtraction    *** Could not connect to Database ***" << endl << endl;

      _successful = false;
      parms.StatusMessage ("Could not connect to Database   *** " + dbConn->ServerDescription () + " ***");
      log.Level (-1) << endl << endl << endl 
                     << "SipperImageExtraction      *** ERROR ***"  << endl
                     << endl
                     << "                      Failed to connect to Database[" << dbConn->ServerDescription () << "]" <<  endl
                     << endl;
      return;
    }
  }


  InstrumentDataFileManager::InitializePush ();

 
  //UpdateInstrumentData (dbConn, "B0710ST004A_06.spr", log);
  //exit (-1);

  //ReFreshInstrumentData (dbConn, log);
  //exit (-1);

  //dbConn->InstrumentDataLoadAllSipperFiles ();

  scanLineEnd   = parms.ScanLineEnd   ();
  scanLineStart = parms.ScanLineStart ();

  KKStr  siperFileRootName = osGetRootName (parms.SipperFileName ());

  if  (parms.UpdateDataBase ())
    sipperFileRec = dbConn->SipperFileRecLoad (siperFileRootName);

  if  (sipperFileRec != NULL)
  {
    if  ((sipperFileRec->ExtractionScanLineStart () > 0)  &&  (scanLineStart == 0))
      scanLineStart = sipperFileRec->ExtractionScanLineStart ();

    if  ((sipperFileRec->ExtractionScanLineEnd () > 0)  &&  (scanLineEnd == 0))
      scanLineEnd = sipperFileRec->ExtractionScanLineEnd ();
  }
  else
  {
    // This entry must exist;  otherwise the inserts to the Images table will fail.

    sipperFileRec = new SipperFile (parms.SipperFileName ());

    sipperFileRec->StationName   ("1");
    sipperFileRec->DeploymentNum ("D");

    sipperFileRec->AssignCtdExternalInstruments (osGetRootName (parms.SipperFileName ()));

    DateTime  refTimeStamp (2008, 10, 1, 0, 0, 0);
    DateTime  sipperFileTimeStamp  = osGetFileDateTime (parms.SipperFileName ());
    long long  fileSizeInBytes = osGetFileSize (parms.SipperFileName ());
    sipperFileRec->SizeInBytes (fileSizeInBytes);
    sipperFileRec->DateTimeStart (sipperFileTimeStamp);

    sipperFileRec->Sp0 (SipperHardware::Instrument::LookUpByShortName ("CTD"));
    sipperFileRec->Sp1 (SipperHardware::Instrument::LookUpByShortName ("P-R"));
    sipperFileRec->Sp2 (SipperHardware::Instrument::LookUpByShortName ("BAT"));

    sipperFileRec->CtdExt0 ("TRN");
    sipperFileRec->CtdExt1 ("OXG");
    sipperFileRec->CtdExt2 ("FLO");
    sipperFileRec->CtdExt3 ("TUR");

    sipperFileRec->ScanRate (30000.0f);

    if  (parms.UpdateDataBase ())
      dbConn->SipperFileInsert (*sipperFileRec);
  }

  if  (scanLineEnd < 1)
    scanLineEnd = uint32_max;

  if  ((!parms.RefreshDataBaseImages ())  &&  parms.UpdateDataBase ())
  {
    // We do not erase database when we are ReFreshingDataBaseImages.

    // Since we are re-extracting the SIPPER file we need to remove all data from 
    // previous Image Extraction.
    //dbConn->ImagesEraseSipperFile (parms.SipperFileName ());
    dbConn->ImagesEraseSipperFileSizeRange (parms.SipperFileName (), parms.MinImageSize (), parms.MaxImageSize ());
  }

  {
    InstrumentDataListPtr   instrumentData = InstrumentDataFileManager::GetInstrumentDataForSipperFile 
                                             (parms.SipperFileName (), 
                                              sipperFileRec, 
                                              true,   // true = Fource re-extraction of data from SipperFiles.
                                              parms.CancelField (), 
                                              log
                                             );

    if  ((instrumentData)  &&  (!parms.RefreshDataBaseImages ())  &&  parms.UpdateDataBase ())
      dbConn->InstrumentDataSaveListForOneSipperFile (parms.SipperFileName (), *instrumentData, parms.CancelField ());
  }

  // Make sure Output Dircectory Exists
  osCreateDirectoryPath (parms.OutputRootDir ());
  
  KKStr  logFileName = osAddSlash (parms.OutputRootDir ()) + osGetRootName (_parms.SipperFileName ()) + ".log";
  log.AttachFile (logFileName);

  reportFile.open   (parms.ReportFileName ().Str ());

  reportFile << "Command Line [" << parms.CmdLine () << "]" << endl << endl << endl;
  log.Level (10) << endl << endl << endl
                 << "Command Line[" << parms.CmdLine () << "]" << endl 
                 << endl << endl;

  KKStr  instrumentReportDirectory = osAddSlash (parms.OutputRootDir ()) + "InstumentData";

  instrumentDataManager = new InstrumentDataManager (_parms.SipperFileName (), sipperFileRec, instrumentReportDirectory, log);

  sipperBuff = SipperBuff::CreateSipperBuff (parms.FileFormat (), 
                                             parms.SipperFileName (), 
                                             parms.CameraNum (),
                                             instrumentDataManager,
                                             log
                                            );

  if  (sipperBuff)
  {
    if  (!sipperBuff->Opened ())
    {
      delete  sipperBuff;
      sipperBuff = NULL;
    }
  }

  if  (sipperBuff == NULL)
  {
    log.Level (-1) << endl << "SipperImageExtraction    *** Could not open SipperFile[" << parms.SipperFileName () << "] ***" << endl << endl;

    _successful = false;
    parms.StatusMessage ("Could not open SIPPER File[" + parms.SipperFileName () + "]");
    log.Level (-1) << endl << endl << endl 
                   << "SipperImageExtraction      *** ERROR ***"  << endl
                   << endl
                   << "                      Failed to open Sipper File[" << parms.SipperFileName () << "]" << endl
                   << endl;
    return;
  }

  frameNum          = 0;
  lastRowInFrame    = 0;
  lastRowUsed       = 0;
  maxFrameHeight    = 0;
  nextFrameStartRow = 0;

  imagesWrittenInFrame = 0;

  parms.PixelsRead    (0);
  parms.PixelsWritten (0);

  parms.ImagesExtracted (0);
  parms.ImagesWritten (0);

  imageManager = new SipperExtractionImageManager (fileDesc, 
                                                   parms.OutputRootDir (),
                                                   parms.ExtractFeatureData (),
                                                   veryLargeImageSize,
                                                   (!parms.ConfigFileName ().Empty ()),  // We are going to clasify images,
                                                   parms.CountOnly (),
                                                   parms.ImagesPerDirectory (),
                                                   log
                                                  );

  KKStr  driveLetter, path, extension;

  osParseFileSpec (parms.SipperFileName (),
                   driveLetter,
                   path,
                   sipperRootName,
                   extension
                  );

  AllocateFrame ();

  if  (scanLineStart > 0)
  {
    sipperBuff->SkipToScanLine (scanLineStart);
    log.Level (10)  << "Done Skipping records" << endl;

    reportFile << endl
               << "Skipped to Line[" << parms.ScanLineStart () << "] in Sipper File." << endl
               << endl;
  }


  unKnownMLClass   = MLClass::CreateNewMLClass ("UnKnown");

  if  (parms.ExtractFeatureData ())
  {
    dupImageDetector = new DuplicateImages (fileDesc, log);
    duplicateImages  = new ImageFeaturesList (fileDesc, true, log, 100);
    duplicateImageDir = osAddSlash (parms.OutputRootDir ()) + "DuplicateImages";
    osCreateDirectoryPath (duplicateImageDir);

    if  (!parms.ConfigFileName ().Empty ())
    {
      parms.StatusMessage ("Building Training Model");
      trainer = new TrainingProcess2 (parms.ConfigFileName (),
                                     NULL,
                                     fileDesc, 
                                     log, 
                                     &reportFile, 
                                     false,             // false = DON'T force model rebuild.
                                     true,              // true = Check for duplicate images in training data.
                                     parms.CancelField (),
                                     parms.StatusMessageField ()
                                    );

      if  (parms.CancelField ())
      {
        parms.StatusMessage ("Image Extraction Canceled.");
        log.Level (0) << endl
                      << "SipperImageExtraction::SipperImageExtraction        *** Canceled ***" << endl
                      << endl;
      }

      else if  (trainer->Abort ())
      {
        _successful = false;

        parms.StatusMessage ("Error building Classifier2 for ConfigFileName[" + parms.ConfigFileName () + "].");

        log.Level (-1) << endl
                       << endl
                       << "SipperImageExtraction   *** ERROR ***  Building Traiing Process" << endl
                       << endl;
        parms.Cancel (true);  // Can not continue. 
      }

      else
      {
        log.Level (20) << "SipperImageExtraction::SipperImageExtraction   Classifier2 Created Sucessfully." << endl;
        parms.StatusMessage ("");
        classifier = new Classifier2 (trainer, log);
      }
    }
  }

  log.Level (10) << "SipperImageExtraction::SipperImageExtraction  Exiting" << endl;
}




SipperImageExtraction::~SipperImageExtraction ()
{
  // uint x;
  log.Level (10) << "SipperImageExtraction::~SipperImageExtraction" << endl;

  parms.StatusMessage ("SipperImageExtraction::~SipperImageExtraction");

  InstrumentDataFileManager::InitializePop ();

  delete  dbConn;  dbConn = NULL;

  delete  instrumentDataManager;  instrumentDataManager = NULL;

  delete  sipperBuff;  sipperBuff = NULL;

  delete  imageManager;  imageManager = NULL;

  delete  frameArea;
  delete  blobIdsArea;

  delete  workFrameArea;
  delete  workFrame;

  delete  origFrame;
  delete  origFrameArea;

  if  (blobs)
  {
    for  (uint x = 0;  x < maxBlobsPerFrame;  x++)
    {
      delete  blobs[x];
      blobs[x] = NULL;
    }
    delete  blobs;
    blobs = NULL;
  }

  delete  frame;
  delete  frameRowByteOffset;
  delete  blobIds;
  delete  pixsInRows;
  delete  dupImageDetector;
  delete  duplicateImages;
  delete  trainer;
  delete  classifier;
  delete  sipperFileRec;
}




KKStr  SipperImageExtraction::SipperFileFormatStr ()
{
  return  SipperFileFormatToStr (parms.FileFormat ());
}  /* SipperFileFormatStr */



void  SipperImageExtraction::AllocateFrame ()
{
  uint x;

  maxFrameHeight = MAXLINELEN;
  SipperBlob::MaxFrameHeight (maxFrameHeight);
  SipperBlob::MaxFrameWidth  (MAXLINELEN);

  lastRowUsed    = 0;

  totPixels = maxFrameHeight * MAXLINELEN;

  frameArea   = new uchar [totPixels];
  blobIdsArea = new int   [totPixels];

  frame              = new uchar* [maxFrameHeight];
  frameRowByteOffset = new uint64 [maxFrameHeight];
  blobIds            = new int*   [maxFrameHeight];
  pixsInRows         = new uint   [maxFrameHeight];

  if  (!parms.MorphOperations ().Empty ())
  {
    workFrameArea = new uchar [totPixels];
    workFrame     = new uchar*[maxFrameHeight];
    origFrameArea = new uchar [totPixels];
    origFrame     = new uchar*[maxFrameHeight];
  }

  nextFrameStartRow = maxFrameHeight;

  int  curPixel = 0;

  for  (x = 0; x < maxFrameHeight; x++)
  {
    frame[x] = &(frameArea[curPixel]);
    memset (frame[x], 0, MAXLINELEN);
    frameRowByteOffset[x] = 0;

    if  (workFrameArea)
    {
      workFrame[x] = &(workFrameArea[curPixel]);
      memset (workFrame[x], 0, MAXLINELEN);

      origFrame[x] = &(origFrameArea[curPixel]);
      memset (origFrame[x], 0, MAXLINELEN);
    }

    blobIds[x] = &(blobIdsArea[curPixel]);

    pixsInRows[x] = 0;
    curPixel += MAXLINELEN;
  }

  parms.LinesExtracted (0);

  parms.BytesExtracted  (0);
  parms.LinesExtracted  (0);
  parms.ImagesExtracted (0);
  parms.ImagesWritten   (0);

  blobs = new SipperBlobPtr[maxBlobsPerFrame];
  for  (x = 0;  x < maxBlobsPerFrame;  x++)
    blobs[x] = new SipperBlob ();
}  /* AllocateFrame */





void  SipperImageExtraction::GetNextFrame (bool&  moreFrames)
{
  uint  col;
  uint  colCount[MAXLINELEN];
  bool  flow;
  uint  lineSize = 0;
  uint  x;

  uint  rowCount = 0;

  for  (x = 0; x < MAXLINELEN; x++)
    colCount[x] = 0;

  frameSipperRow = sipperBuff->CurRow ();

  //if  (nextFrameStartRow >= maxFrameHeight)
  if  (nextFrameStartRow >= lastRowUsed)
  {
    // There are no lines left over from a previous call to
    // this function.

    if  (endOfFileReachedInLastCall) 
    {
      moreFrames = false;
      return;
    }
  }

  else
  {
    // We have left over lines from previous GetNextFrame

    uchar*  tempRow;
    uint    tempPixsInRow;
    uint64  tempRowByteOffset;

    for  (x = nextFrameStartRow; x <= lastRowUsed; x++)
    {
      tempRow           = frame              [x];
      tempRowByteOffset = frameRowByteOffset [x];
      tempPixsInRow     = pixsInRows         [x];

      frame              [x] = frame              [rowCount];
      frameRowByteOffset [x] = frameRowByteOffset [rowCount];
      pixsInRows         [x] = pixsInRows         [rowCount];

      frame              [rowCount] = tempRow;
      frameRowByteOffset [rowCount] = tempRowByteOffset;
      pixsInRows         [rowCount] = tempPixsInRow;

      for  (col = 0; col < MAXLINELEN; col++)
      {
        if  (tempRow[col] > 0)
          colCount[col]++;
      }

      frameSipperRow--;  // Since we are using lines that have already
                         // been read from sipper buff we need to subtract
                         // these lines from prev frame from our starting 
                         // frame starting position.
      rowCount++;
    }

    lastRowUsed       = rowCount - 1;
    lastRowInFrame    = lastRowUsed;
    nextFrameStartRow = maxFrameHeight;
  }

  bool  moreLines = false;

  if  (!endOfFileReachedInLastCall) 
  {
    moreLines = true;

    while  (moreLines  &&  (rowCount < maxFrameHeight))
    {
      frameRowByteOffset[rowCount] = sipperBuff->ByteOffset ();
      sipperBuff->GetNextLine (frame[rowCount], 
                               MAXLINELEN, 
                               lineSize, 
                               colCount, 
                               pixsInRows[rowCount], 
                               flow
                              );
      if  ((lineSize <= 0)  ||  (sipperBuff->CurRow () > scanLineEnd))
      {
        endOfFileReachedInLastCall = true;
        moreLines = false;
      }
      else
      {
        rowCount++;
        parms.IncrementLinesExtracted ();
      }
    }
  }

  if  (!moreLines)
  {
    endOfFileReachedInLastCall = true;
    if  (rowCount < 10)
    {
      moreFrames = false;
      return;
    }
  }
  
  lastRowUsed    = rowCount - 1;
  lastRowInFrame = lastRowUsed;

  while  (rowCount < maxFrameHeight)
  {
    memset (frame[rowCount], 0, MAXLINELEN);
    pixsInRows[rowCount] = 0;
    rowCount++;
  }

  if  (!parms.FramesOnly ())
    SearchForCleanBreak ();

  if  (parms.SaveFrames ())
    SaveFrame ("Before");

  if  (parms.PreProcess ())
    EliminatePosibleLines (colCount);

  if  (!parms.MorphOperations ().Empty ())
    PerformMorphOperations ();

  if  (parms.SaveFramesAfter ())     // kk 2005-03-03,  as per andrew only before frames.
    SaveFrame ("After");
}  /* GetNextFrame */





void  SipperImageExtraction::SearchForCleanBreak ()
{
  // A Clean Break is defined by having three rows in a row that do not 
  // have any pixels occupied.

  lastRowInFrame = lastRowUsed;

  int     blankRowsInARow = 0;


  if  ((pixsInRows[lastRowInFrame]     == 0)  &&
       (pixsInRows[lastRowInFrame - 1] == 0))
  {
    // The Frame ends with a clean break so lets set next Frame to 
    // maxFrameHeight indicating that there is nothing to carry over
    // and back lastRowInFrame and lastRowUsed to first non blank 
    // Row.

    lastRowInFrame--;
    while  ((lastRowInFrame > 100) && (pixsInRows[lastRowInFrame] == 0))
      lastRowInFrame--;

    nextFrameStartRow = maxFrameHeight;
    lastRowUsed = lastRowInFrame;
  }

  else
  {
    // Since the last row has some pixels we need to search back for 
    // a clean break in the frame.

    while  ((lastRowInFrame > 1000)  &&  (blankRowsInARow < 3))
    {
      if  (pixsInRows[lastRowInFrame] == 0)
      {
        blankRowsInARow++;
      }
      else
      {
        blankRowsInARow = 0;
        nextFrameStartRow = lastRowInFrame;
      }

      lastRowInFrame--;
    }

    if  ((lastRowInFrame <= 1000)  &&  (blankRowsInARow < 3))
    {
      // Since we could not find a seperation between images lets look
      // for were the row with least pixels.
      uint  rowWithLeastPixels = lastRowInFrame;
      uint  leastNumOfPixels   = 999999;
      uint  x;
      for  (x = lastRowInFrame; x < lastRowUsed; x++)
      {
        if  (pixsInRows[x] < leastNumOfPixels )
        {
          rowWithLeastPixels = x;
          leastNumOfPixels = pixsInRows[x];
        }
      }

      lastRowInFrame = rowWithLeastPixels;
      nextFrameStartRow = lastRowInFrame + 1;
    }
    else
    {
      nextFrameStartRow = lastRowInFrame + 1;

      // We now have a Good Starting Position for the next Frame.
      // So now lets look for the ending position of the prev frame.
      while  ((lastRowInFrame > 100) && (pixsInRows[lastRowInFrame] == 0))
        lastRowInFrame--;
    }
  }
}  /* SearchForCleanBreak */






void  SipperImageExtraction::BuildColCount (uint  colCount[])
{
  int   col;
  uint  row;

  for  (col = 0; col < MAXLINELEN; col++)
  {
    colCount[col] = 0;
  }

  uchar*  rowContent = NULL;
  for  (row = 0; row <= lastRowInFrame; row++)
  {
    rowContent = frame[row];

    for  (col = 0; col < MAXLINELEN; col++)
    {
      if  (rowContent[col] > 0)
        colCount[col]++;
    }
  }
}  /* BuildColCount */




void  SipperImageExtraction::EliminatePosibleLines (uint  colCount[])
{
  uint  endCol;
  bool  endColFound;
  uint  col;
  uint  row;
  uint  startCol;
  uint  threshold = lastRowInFrame  / 6;
  if  (threshold < 300)
    threshold = 300;

  uint  x;

  BuildColCount (colCount);


  for  (col = 1; col < (MAXLINELEN - 1); col++)
  {
    if  (colCount[col] > threshold)
    {
      startCol = col;
      endCol   = col;
      endColFound = false;
      col++;

      while  ((col < (MAXLINELEN - 1))  &&  (!endColFound))
      {
        if  (colCount[col] > threshold)
        {
          endCol = col;
          col++;
        }
        else
        {
          endColFound = true;
        }
      }

      int  width = 1 + endCol - startCol;

      if  (width > MaxArtifactLineWidth)
        continue;

      // Now that we defined a Possible Collumn lets get rid of the stupid Line.

      uchar*  rowContent = NULL;
      for  (row = 0; row <= lastRowUsed; row++)
      {
        rowContent = frame[row];

        if  (((rowContent[startCol - 1] == 0) ||  (startCol <= 1))  &&  
             ((rowContent[endCol   + 1] == 0) ||  (endCol   >= (MAXLINELEN - 2))))
        {
          for  (x = startCol; x <= endCol; x++)
          {
            if  (rowContent[x] > 0)
            {
              rowContent[x] = 0;
              pixsInRows[row]--;
            }
          }
        }
      }
    }
  }  /* end of for col */
}  /* EliminatePosibleLines */




void  SipperImageExtraction::SaveFrame (const KKStr& suffix)
{
  uint  col;
  uint  row;

  RasterSipperPtr  frameImage = new RasterSipper (lastRowInFrame + 1, 4096, false);
  uchar**  grayArea = frameImage->Green ();

  for  (row = 0; row < lastRowInFrame; row++)
  {
    for  (col = 0; col < MAXLINELEN; col++)
    {
      grayArea[row][col] = frame[row][col];
    }
  }
  
  KKStr  frameName = "Frame_" + StrFormatInt (frameNum, "0000") + "_" +
                      StrFormatInt (frameSipperRow, "00000000")  + "_" +
                      suffix +
                      ".bmp";

  imageManager->SaveFrame (frameName, frameImage, parms.Colorize ());

  delete  frameImage;
}  /* SaveFrame */






void  SipperImageExtraction::PerformErosion ()
{
  uint  row, col;

  uchar*  curRow = NULL;
  uchar*  nextRow = NULL;
  uchar*  lastRow = NULL;
  uchar*  workRow = NULL;

  for  (row = 1; row < lastRowInFrame; row++)
  {
    curRow  = frame[row];
    lastRow = frame[row - 1];
    nextRow = frame[row + 1];
    workRow = workFrame[row];

    for  (col = 1;  col < (MAXLINELEN - 1);  col++)
    {
      if  (curRow[col] == 0)
      {
        workRow[col] = 0;
      }
      else
      {
        if  ((lastRow[col-1] == 0)  ||
             (lastRow[col  ] == 0)  ||
             (lastRow[col+1] == 0)  ||

             (curRow [col-1] == 0)  ||
             (curRow [col+1] == 0)  ||

             (nextRow[col-1] == 0)  ||
             (nextRow[col  ] == 0)  ||
             (nextRow[col+1] == 0)
            )

          workRow[col] = 0;
        else
          workRow[col] = curRow[col];
      }
    }
  }

  // Lets copy back
  for  (row = 1; row < lastRowInFrame; row++)
  {
    curRow  = frame     [row];
    workRow = workFrame [row];

    for  (col = 1;  col < (MAXLINELEN - 1);  col++)
    {
      curRow[col] = workRow[col];
    }
  }
}  /* PerformErosion */




void  CountAndTotal (KKU::uchar    c,
                     KKU::ushort&  count,
                     KKU::ushort&  total
                    )
{
  if  (c > 0)
  {
    count++;
    total += c;
  }
}  /* CountAndTotal */




void  SipperImageExtraction::PerformDialation ()
{
  uint  row, col;

  uchar*  curRow = NULL;
  uchar*  nextRow = NULL;
  uchar*  lastRow = NULL;
  uchar*  workRow = NULL;

  ushort  count, total;

  for  (row = 1; row < lastRowInFrame; row++)
  {
    lastRow = frame[row - 1];
    curRow  = frame[row];
    nextRow = frame[row + 1];
    workRow = workFrame[row];

    for  (col = 1;  col < (MAXLINELEN - 1);  col++)
    {
      if  (curRow[col] != 0)
      {
        workRow[col] = curRow[col];
      }
      else
      {
        if  ((lastRow[col-1] == 0)  &&
             (lastRow[col  ] == 0)  &&
             (lastRow[col+1] == 0)  &&

             (curRow [col-1] == 0)  &&
             (curRow [col+1] == 0)  &&

             (nextRow[col-1] == 0)  &&
             (nextRow[col  ] == 0)  &&
             (nextRow[col+1] == 0)
            )
        {
          workRow[col] = 0;
        }
        else
        {
          count = total = 0;
          CountAndTotal (lastRow[col-1], count, total);
          CountAndTotal (lastRow[col  ], count, total);
          CountAndTotal (lastRow[col+1], count, total);

          CountAndTotal (curRow [col-1], count, total);
          CountAndTotal (curRow [col+1], count, total);

          CountAndTotal (nextRow[col-1], count, total);
          CountAndTotal (nextRow[col  ], count, total);
          CountAndTotal (nextRow[col+1], count, total);

          workRow[col] = total / count;
        }
      }
    }
  }

  // Lets copy back
  for  (row = 1; row < lastRowInFrame; row++)
  {
    curRow  = frame     [row];
    workRow = workFrame [row];

    for  (col = 1;  col < (MAXLINELEN - 1);  col++)
    {
      curRow[col] = workRow[col];
    }
  }
}  /* PerformDialation */





void  SipperImageExtraction::PerformOpening ()
{
  PerformErosion ();
  PerformDialation ();
}  /* PerformOpening */




void  SipperImageExtraction::PerformClosing ()
{
  PerformDialation ();
  PerformErosion ();
}  /* PerformOpening */



void  SipperImageExtraction::PerformMorphOperations ()
{
  KKStr  ops = parms.MorphOperations ();

  memcpy (origFrameArea, frameArea, totPixels);
  
  while  (!ops.Empty ())
  {
    KKStr  nextOp = ops.ExtractToken (",\n\r\t");
    nextOp.Upper ();
    if  (nextOp == "O")
      PerformOpening ();

    else if  (nextOp == "C")
      PerformClosing ();

    else if  (nextOp == "D")
      PerformDialation ();

    else if  (nextOp == "E")
      PerformErosion ();
  }

}  /* PerformMorphOperations */




SipperBlobPtr  SipperImageExtraction::NewBlob (uint  rowTop,
                                               uint  colLeft
                                              )
{
  if  (numOfBlobsInFrame >= maxBlobsPerFrame)
  {
    // This is some really crap code that I am adding here.  
    // the whole way that 'blobs' is being managed needs to be 
    // reworked.
    uint  x = 0;

    // Lets go and re-use an older smaller blob

    while  (x < numOfBlobsInFrame)
    {
      if  (blobs[x]->Id () == -1)
      {
        // We found a unused blob that is not being used any more
        
        SipperBlob&  blob = *(blobs[x]);

        blob.id = x;
        //  numOfBlobsInFrame++;  // We don't increment this since we are making use 
                                  // of an older blob that has been merged.

        blob.rowTop     = rowTop;
        blob.rowBot     = rowTop;

        blob.colLeft    = colLeft;
        blob.colRight   = colLeft; 
        blob.pixelCount = 0;
        blob.explored   = false;
        blob.neighbors.clear ();
        blob.neighborIdLastAdded = -1;
        return  &blob;
      }

      x++;
    }

    {
      // There are no empty slots in 'blobs'   w2e are going to have to expand it.
      uint  newMaxBlobsPerFrame  = maxBlobsPerFrame + 10000;

      log.Level (10) << "SipperImageExtraction::NewBlob    maxBlobsPerFrame[" << maxBlobsPerFrame << "] Exceeded  New Size[" << newMaxBlobsPerFrame << "]." << endl;
      
      SipperBlobPtr*  newBlobs = new SipperBlobPtr[newMaxBlobsPerFrame];

      uint  blobIDX = 0;
      while  (blobIDX < maxBlobsPerFrame)
      {
        newBlobs[blobIDX] = blobs[blobIDX];
        blobs[blobIDX] = NULL;
        blobIDX++;
      }

      delete[]  blobs;  blobs = NULL;

      while  (blobIDX < newMaxBlobsPerFrame)
      {
        newBlobs[blobIDX] = new SipperBlob ();
        blobIDX++;
      }

      blobs = newBlobs;
      maxBlobsPerFrame = newMaxBlobsPerFrame;
    }
  }

  SipperBlob&  blob = *(blobs[numOfBlobsInFrame]);

  blob.id = numOfBlobsInFrame;
  numOfBlobsInFrame++;

  blob.rowTop     = rowTop;
  blob.rowBot     = rowTop;

  blob.colLeft    = colLeft;
  blob.colRight   = colLeft; 
  blob.pixelCount = 0;
  blob.explored   = false;
  blob.neighbors.clear ();
  blob.neighborIdLastAdded = -1;

  return  &blob;
}  /* NewBlob */







void  SipperImageExtraction::MergeBlobIds (uint destBlobId,
                                           uint srcBlobId
                                          )
{                               
  if  (destBlobId > numOfBlobsInFrame)
  {
    KKStr err;
    err << "***ERROR*** destBlobId[" << destBlobId << "] out of Range (0 - " << numOfBlobsInFrame << ")";
    osDisplayWarning (err);
    log.Level (-1) << err << endl;
    osWaitForEnter ();
    exit (-1);
  }


  if  (srcBlobId > numOfBlobsInFrame)
  {
    KKStr err;
    err << "***ERROR*** srcBlobId[" << srcBlobId << "] out of Range (0 - " << numOfBlobsInFrame << ")";
    osDisplayWarning (err);
    log.Level (-1) << err << endl;
    exit (-1);
  }

  SipperBlob&  destBlob  = *(blobs[destBlobId]);
  SipperBlob&  srcBlob   = *(blobs[srcBlobId]);

  int  col;
  int  row;

  int  rowBot   = srcBlob.rowBot;
  int  colRight = srcBlob.colRight;

  for  (row = srcBlob.rowTop; row <= rowBot; row++)
  {
    for  (col = srcBlob.colLeft; col <= colRight; col++)
    {
      if  (blobIds[row][col] == int (srcBlobId))
        blobIds[row][col] = destBlobId;
    }
  }

  destBlob.rowTop     = Min (destBlob.rowTop,   srcBlob.rowTop);
  destBlob.rowBot     = Max (destBlob.rowBot,   srcBlob.rowBot);
  destBlob.colLeft    = Min (destBlob.colLeft,  srcBlob.colLeft);
  destBlob.colRight   = Max (destBlob.colRight, srcBlob.colRight);

  destBlob.pixelCount = destBlob.pixelCount + srcBlob.pixelCount;

  srcBlob.id = -1;
}  /* MergeBlobIds */







void  SipperImageExtraction::ExtractFrames ()
{
  log.Level (10) << "SipperImageExtraction::ExtractFrames" << endl;

  DateTime  elaspedTime;
  DateTime  endTime;
  DateTime  startTime = osGetLocalDateTime ();

  double  startCPUsecs = osGetSystemTimeUsed ();

  reportFile << endl;

  reportFile << "SipperImageExtraction - Version[" << VERSION << "]."  << endl 
             << endl;

  reportFile << "Source Sipper File    [" << sipperBuff->FileName     ()  << "]." << endl;
  reportFile << "File Format           [" << SipperFileFormatStr      ()  << "]." << endl;
  reportFile << "Camera Number         [" << (int)parms.CameraNum     ()  << "]." << endl;
  reportFile << "Destination Directory [" << parms.OutputRootDir      ()  << "]." << endl;
  reportFile << "Minimum Image Size    [" << parms.MinImageSize       ()  << "]." << endl;
  reportFile << "Maximum Image Size    [" << parms.MaxImageSize       ()  << "]." << endl;
  reportFile << "Start Scan Row        [" << parms.ScanLineStart      ()  << "]." << endl;
  reportFile << "End   Scan Row        [" << parms.ScanLineEnd        ()  << "]." << endl;
  reportFile << "MorphOperations       [" << parms.MorphOperations    ()  << "]." << endl;
  reportFile << "Connected Pixel Dist  [" << parms.ConnectedPixelDist ()  << "]." << endl;
  reportFile << "CountOnly             [" << (parms.CountOnly () ?"Yes":"No") << "]." << endl;
  
  reportFile << endl;

  reportFile << "Start Time: " << startTime << endl;
  
  frameNum = 0;
  bool  moreFrames = true;

  GetNextFrame (moreFrames);
  while  (moreFrames  &&  (!parms.Cancel ()))
  {
    log.Level (10) << "Processing Frame [" << StrFormatInt (frameNum, "0") << "]." << endl; 

    if  (parms.FramesOnly ())
    {
      parms.BytesExtracted (sipperBuff->ByteOffset ());
    }
    else
    {
      ProcessFrame ();
    }

    if  (parms.Cancel ())
    {
      log.Level (10) << "ExtractFrames   Inside Loop, Cacel request made." << endl;
      break;
    }

    frameNum++;
    GetNextFrame (moreFrames);
  }

  log.Level (10) << "ExtractFrames   Done Extracting Frames." << endl;
  
  if  (parms.Cancel ())
  {
    reportFile << endl
               << endl
               << endl;
    reportFile << "****************************************************************************"  << endl;
    reportFile << "*                                                                          *"  << endl;
    reportFile << "*                   TERMINATING EXTRACTION AT USER REQUEST                 *"  << endl;
    reportFile << "*                                                                          *"  << endl;
    reportFile << "****************************************************************************"  << endl;
    reportFile << endl
               << endl
               << endl;
  }
  else
  {
    // Will now close out all subdirectories being managed by imageManager and write the 
    // final 'Features' data file for all images in to root destination directory.
    imageManager->Flush ();
  }

  double  doneCPUsecs    = osGetSystemTimeUsed ();
  double  systemTimeUsed = doneCPUsecs - startCPUsecs;


  endTime =  osGetLocalDateTime ();
  elaspedTime = endTime - startTime;

  reportFile << endl; 
  reportFile << "Total CPU Time    [" << StrFormatDouble (systemTimeUsed, "zzz,zz0.0") << "]." << endl; 
  reportFile << "End Time          [" << endTime                << "]." << endl;
  reportFile << "Total Elasped Time[" << elaspedTime.Time ()    << "]." << endl;
  reportFile << "Total Elasped Time[" << elaspedTime.Seconds () << "]." << endl 
             << endl;

  reportFile << "Totals" << endl << endl;
  double  avgSize;
  double  imagesWrittenAvgSize;
  double  imagesNotWrittenAvgSize;


  uint  imagesNotWritten = parms.ImagesExtracted () - parms.ImagesWritten ();
  uint  pixelsNotWritten = parms.PixelsRead ()      - parms.PixelsWritten ();

  if  (parms.ImagesExtracted () > 0)
    avgSize = parms.PixelsRead () / parms.ImagesExtracted ();
  else
    avgSize = 0;


  if  (parms.ImagesWritten () > 0)
    imagesWrittenAvgSize = parms.PixelsWritten () / parms.ImagesWritten ();
  else
    imagesWrittenAvgSize = 0;

  if  (imagesNotWritten > 0)
    imagesNotWrittenAvgSize = pixelsNotWritten / imagesNotWritten;
  else
    imagesNotWrittenAvgSize = 0;

  reportFile << "Total Lines["     << StrFormatInt    (sipperBuff->CurRow (),    "zzz,zzz,zz0")   << "]   "
             << "Total Images["    << StrFormatInt    (parms.ImagesExtracted (), "zzz,zzz,zz0")   << "]   "
             << "Avg Size["        << StrFormatDouble (avgSize,                  "zzz,zz0.0")     << "]   "
             << "Images Written["  << StrFormatInt    (parms.ImagesWritten (),   "zzz,zzz,zz0")   << "]   "
             << "Avg Size["        << StrFormatDouble (imagesWrittenAvgSize,     "zzz,zz0.0")     << "]"
             << endl
             << endl;

  reportFile << "Total Images Not Written[" << imagesNotWritten << "]   "
             << "Avg Size[" << imagesNotWrittenAvgSize << "]"
             << endl
             << endl;

  reportFile << "Very Large Image Threshold[" << StrFormatInt (veryLargeImageSize, "zzz,zz@") << "]  "
             << endl
             << endl;

  reportFile << "Bytes Dropped[" << StrFormatInt (sipperBuff->BytesDropped (), "zz,zzz,zz0") << "]." << endl << endl;

  reportFile << "Count By Size:" << endl << endl;

  imageManager->ReportStatistics (reportFile);

  if  (dbConn != NULL)
  {
    long long  sizeInBytes = (long long)osGetFileSize (parms.SipperFileName ());
    dbConn->SipperFileUpdateFileSizeStats (osGetRootName (parms.SipperFileName ()), sizeInBytes, sipperBuff->CurRow ());
  }

  if  (!parms.FramesOnly ())
  {
    VectorUlong*  scanLinesPerMeterDepth =  instrumentDataManager->ScanLinesPerMeterDepth ();
    KKStr  depthReportFileName = osRemoveExtension (parms.ReportFileName ()) + "_Depth.txt";
    ofstream depthReport (depthReportFileName.Str ());
    depthReport << "Command Line [" << parms.CmdLine      () << "]" << endl;
    depthReport << "Date         [" << osGetLocalDateTime () << "]" << endl << endl;
    imageManager->ReportStatsByDepth (depthReport, scanLinesPerMeterDepth);
    depthReport.close ();
    delete  scanLinesPerMeterDepth;  scanLinesPerMeterDepth = NULL;
  }
}  /* ExtractFrames */





void  SipperImageExtraction::InitailzieBlobIds ()
{
  uint  row, col;

  for  (row = 0; row < maxFrameHeight; row++)
  {
    for  (col = 0; col < MAXLINELEN; col++)
      blobIds[row][col] = -1;
  }
}  /* InitailzieBlobIds */





void  SipperImageExtraction::LocateBlobsUsingConnectedDistanceOf3 ()
{
  uchar*        curRow           = NULL;
  int*          curRowBlobIds    = NULL;
  int*          prev1RowBlobIds  = NULL;
  int*          prev2RowBlobIds  = NULL;
  int*          prev3RowBlobIds  = NULL;

  uint          col = 3;
  uint          row = 3;

  SipperBlobPtr curBlob    = NULL;
  int           curBlobId  = 0;
  int           nearBlobId = 0;

  uint          blankColsInARow = 0;

  for  (row = connectedPixelDist; row < lastRowInFrame; row++)
  {
    col = connectedPixelDist;

    curRow           = frame[row];
    curRowBlobIds    = blobIds[row];
    prev1RowBlobIds  = blobIds[row - 1];
    prev2RowBlobIds  = blobIds[row - 2];
    prev3RowBlobIds  = blobIds[row - 3];

    curBlob = NULL;

    if  (pixsInRows[row] > 0)
    {
      while  (col < (MAXLINELEN - 3))
      {
        if  (curRow[col] > 0)
        {
          blankColsInARow = 0;

          // Check Upper Left 1st
          nearBlobId = Max (prev1RowBlobIds[col],
                            prev2RowBlobIds[col],
                            prev3RowBlobIds[col],
                            prev1RowBlobIds[col - 1],
                            prev2RowBlobIds[col - 1],
                            prev3RowBlobIds[col - 1],
                            prev1RowBlobIds[col - 2],
                            prev2RowBlobIds[col - 2],
                            prev1RowBlobIds[col - 3]
                           );

          if  (nearBlobId < 0)
          {
            // Check Upper Right Pixels.
            nearBlobId = Max (prev1RowBlobIds[col + 3],
                              prev1RowBlobIds[col + 2],
                              prev2RowBlobIds[col + 2],
                              prev1RowBlobIds[col + 1],
                              prev2RowBlobIds[col + 1],
                              prev3RowBlobIds[col + 1]
                             );
          }

          if  (curBlob)
          {
            if  (nearBlobId >= 0)
            {
              // There is an image with-in a couple of pixels

              if  (nearBlobId != curBlobId)
              {
                // The near SipperBlob is different than the curBlob;  so the two
                // blobs are really one.  We will merge them into one blob.
                //MergeBlobIds (curBlobId, nearBlobId);
                blobs[nearBlobId]->AddNeighbor (curBlob);
                curBlob->AddNeighbor (blobs[nearBlobId]);
              }
            }

            curRowBlobIds[col] = curBlobId;
            curBlob->colRight  = Max (curBlob->colRight, col);
            curBlob->rowBot    = Max (curBlob->rowBot,   row);
            curBlob->colLeft   = Min (curBlob->colLeft,  col);
            curBlob->rowTop    = Min (curBlob->rowTop,   row);

            curBlob->pixelCount++;
          }
          else   
          {
            // No Current SipperBlob
            if  (nearBlobId >= 0)
            {
              curBlob   = blobs[nearBlobId];
              curBlobId = curBlob->id;
            }
            else
            {
              curBlob = NewBlob (row, col);
              curBlobId = curBlob->id;
            }

            curRowBlobIds[col] = curBlobId;
            curBlob->colRight  = Max (curBlob->colRight, col);
            curBlob->rowBot    = Max (curBlob->rowBot,   row);

            curBlob->colLeft   = Min (curBlob->colLeft, col);
            curBlob->rowTop    = Min (curBlob->rowTop,  row);

            curBlob->pixelCount++;
          }
        }
        else
        {
          if  (curBlob)
          {
            // Check Upper Left 1st
            nearBlobId = Max (prev1RowBlobIds[col],
                              prev2RowBlobIds[col],
                              prev3RowBlobIds[col],
                              prev1RowBlobIds[col - 1],
                              prev2RowBlobIds[col - 1],
                              prev3RowBlobIds[col - 1],
                              prev1RowBlobIds[col - 2],
                              prev2RowBlobIds[col - 2],
                              prev1RowBlobIds[col - 3]
                             );

            if  (nearBlobId >= 0)
            {
              if  (nearBlobId != curBlobId)
              {
                //MergeBlobIds (curBlobId, nearBlobId);
                blobs[nearBlobId]->AddNeighbor (curBlob);
                curBlob->AddNeighbor (blobs[nearBlobId]);
              }
            }
          }

          blankColsInARow++;
          if  (blankColsInARow > connectedPixelDist)
          {
            curBlob = NULL;
            curBlobId = -1;
          }
        }

        col++;
      }
    }
  }
}  /* LocateBlobsUsingConnectedDistanceOf3 */




int  SipperImageExtraction::GetMaxBlobIdInUpperLeft (int row,
                                                     int col
                                                    )
{
  int  maxBlobId = -1;
  int c, r, startCol;

  startCol = col - 1;

  for (r = row - connectedPixelDist;  r < row;  r++)
  {
    if  (r >= 0)
    {
      for  (c = startCol;  c <= col;  c++)
      {
        if  (c >= 0)
        {
          if  (blobIds[r][c] > maxBlobId)
            maxBlobId = blobIds[r][c];
        }
      }
    }

    startCol--;
  }

  return  maxBlobId;
}  /* GetMaxBlobIdInUpperLeft */





int  SipperImageExtraction::GetMaxBlobIdInUpperRight (int row,
                                                      int col
                                                     )
{
  int  maxBlobId = -1;
  int r, c, endCol;

  endCol = col + 1;
  for  (r = row - connectedPixelDist;  r < row;  r++)
  {
    if  (r >= 0)
    {
      if  (endCol >= MAXLINELEN)
        endCol = MAXLINELEN - 1;

      for  (c = col + 1;  c <= endCol;  c++)
      {
        if  (blobIds[r][c] > maxBlobId)
          maxBlobId = blobIds[r][c];
      }
    }

    endCol++;
  }

  return  maxBlobId;
}  /* GetMaxBlobIdInUpperRight */




void  SipperImageExtraction::LocateBlobsUsingConnectedDistanceNotOf3 ()
{
  log.Level (20) << "SipperImageExtraction::LocateBlobsUsingConnectedDistanceNotOf3" << endl;
  uchar*        curRow           = NULL;
  int*          curRowBlobIds    = NULL;

  uint          col = 1;
  uint          row = 1;

  SipperBlobPtr curBlob           = NULL;
  int           curBlobId         = 0;
  int           nearBlobId        = 0;

  uint          blankColsInARow = 0;

  for  (row = 1; row < lastRowInFrame; row++)
  {
    col = connectedPixelDist;

    curRow           = frame[row];
    curRowBlobIds    = blobIds[row];

    curBlob = NULL;

    if  (pixsInRows[row] > 0)
    {
      while  (col < (MAXLINELEN - 3))
      {
        if  (curRow[col] > 0)
        {
          blankColsInARow = 0;

          // Check Upper Left 1st
          nearBlobId = GetMaxBlobIdInUpperLeft (row, col);

          if  (nearBlobId < 0)
          {
            // Check Upper Right Pixels.
            nearBlobId = GetMaxBlobIdInUpperRight (row, col);
          }

          if  (curBlob)
          {
            if  (nearBlobId >= 0)
            {
              // There is an image with-in a couple of pixels

              if  (nearBlobId != curBlobId)
              {
                // The near SipperBlob is different than the curBlob;  so the two
                // blobs are really one.  

                blobs[nearBlobId]->AddNeighbor (curBlob);
                curBlob->AddNeighbor (blobs[nearBlobId]);
              }
            }

            curRowBlobIds[col] = curBlobId;
            curBlob->colRight  = Max (curBlob->colRight, col);
            curBlob->rowBot    = Max (curBlob->rowBot,   row);
            curBlob->colLeft   = Min (curBlob->colLeft,  col);
            curBlob->rowTop    = Min (curBlob->rowTop,   row);

            curBlob->pixelCount++;
          }
          else   
          {
            // No Current SipperBlob
            if  (nearBlobId >= 0)
            {
              curBlob   = blobs[nearBlobId];
              curBlobId = curBlob->id;
            }
            else
            {
              curBlob = NewBlob (row, col);
              curBlobId = curBlob->id;
            }

            curRowBlobIds[col] = curBlobId;
            curBlob->colRight  = Max (curBlob->colRight, col);
            curBlob->rowBot    = Max (curBlob->rowBot,   row);

            curBlob->colLeft   = Min (curBlob->colLeft, col);
            curBlob->rowTop    = Min (curBlob->rowTop,  row);

            curBlob->pixelCount++;
          }
        }
        else
        {
          if  (curBlob)
          {
            // Check Upper Left 1st
            nearBlobId = GetMaxBlobIdInUpperLeft (row, col);

            if  (nearBlobId >= 0)
            {
              if  (nearBlobId != curBlobId)
              {
                //MergeBlobIds (curBlobId, nearBlobId);
                blobs[nearBlobId]->AddNeighbor (curBlob);
                curBlob->AddNeighbor (blobs[nearBlobId]);
              }
            }
          }

          blankColsInARow++;
          if  (blankColsInARow > connectedPixelDist)
          {
            curBlob = NULL;
            curBlobId = -1;
          }
        }

        col++;
      }
    }
  }  /* row */

  log.Level (20) << "LocateBlobsUsingConnectedDistanceNotOf3    Exiting" << endl;
}  /* LocateBlobsUsingConnectedDistanceNotOf3 */






VectorSipperImages*  SipperImageExtraction::BuildListOfSipperImages (int&  imagesInFrame)
{
  log.Level (20) << "SipperImageExtraction::BuildListOfSipperImages" << endl;
  VectorSipperImages*  sipperImages = new VectorSipperImages ();

  for  (uint blobIDX = 0;  (blobIDX < numOfBlobsInFrame)  &&  (!parms.Cancel ());  blobIDX++)
  {
    SipperBlobPtr  blob = blobs[blobIDX];
    if  (!blob)
    {
      log.Level (-1) << endl << endl 
        << "SipperImageExtraction::BuildListOfSipperImages    blob == NULL" << endl
        << endl;
      continue;
    }

    if  (blob->Explored ())
      continue;

    SipperImagePtr  sipperImage = new SipperImage ();
    sipperImage->Explore (blob);

    bool  skipThisImage = false;

    if  (sipperImage->PixelCount () < 2)
    {
      // We will not consider a single pixel an image.
      skipThisImage = true;
    }
    else
    {
      parms.IncrementImagesExtracted ();
      imagesInFrame++;

      uint  pixelsInImage = sipperImage->PixelCount ();

      parms.IncrementPixelsRead (pixelsInImage);

      // This is were we decide if a particular Sipper Image has met the user criteria 
      if  (pixelsInImage < parms.MinImageSize () || (parms.MaxImageSize() > parms.MinImageSize() && (pixelsInImage > parms.MaxImageSize())))
      {
        skipThisImage = true;
      }
    }

    if  ((!skipThisImage)  &&  (parms.PreProcess ()))
    {
      uint  h = sipperImage->Height ();
      uint  w = sipperImage->Width ();

      // Will Check to see if this is an obvious Vertical Artifact Line.

      if  ((w < 14)  &&  (h > 20))
      {
        float  widthVsHeight = (float)w / (float)h;
        if  (widthVsHeight < 0.08f)
        {
          int  availArea =  w * h;
          float  pixelUsedRatio = (float)sipperImage->PixelCount () / (float)availArea;

          if  (pixelUsedRatio > 0.2f)
          {
            skipThisImage = true;
            /*
            RasterSipperPtr r = sipperImage->GetRaster (frame, blobIds, frameSipperRow);
            KKStr  n = "c:\\Temp\\ArtifactLines\\" + sipperRootName  +
                        "_"  +
                        StrFormatInt (sipperImage->RowTop () + frameSipperRow, "00000000") +
                        "_" +
                        StrFormatInt (sipperImage->ColLeft (), "0000") +
                        ".bmp";
            osCreateDirectory ("c:\\Temp\\ArtifactLines\\");
            SaveImage  (*r, n);
            delete  r;
            */
          }
        }
      }
    }

    if  (skipThisImage)
    {
      delete  sipperImage;
      sipperImage = NULL;
      continue;
    }

    sipperImages->push_back (sipperImage);
  }

  log.Level (20) << "BuildListOfSipperImages    Exiting" << endl;

  return  sipperImages;
}  /* BuildListOfSipperImages */




void  SipperImageExtraction::ProcessFrame ()
{
  if  (parms.Cancel ())
    return;

  log.Level (20) << "SipperImageExtraction::ProcessFrame"  << endl;

  imagesWrittenInFrame = 0;

  int imagesInFrame    = 0;

  numOfBlobsInFrame = 0;

  // Initialize SipperBlob ID's
  InitailzieBlobIds ();

  int  imageId = -1;

  //if  (connectedPixelDist == 3)
  //  LocateBlobsUsingConnectedDistanceOf3 ();
  //else
  LocateBlobsUsingConnectedDistanceNotOf3 ();

  VectorSipperImages*  sipperImages = BuildListOfSipperImages (imagesInFrame);


  if  (origFrameArea)
    memcpy (frameArea, origFrameArea, totPixels);


  // Now that we found all the blobs in the frame,  lets select 
  // the candidate ones that are to be written out.

  VectorSipperImages::iterator  siIDX;
  for  (siIDX = sipperImages->begin ();   ((siIDX != sipperImages->end ())  &&  (!parms.Cancel ()));  siIDX++)
  {
    SipperImagePtr  sipperImage = *siIDX;

    bool   noiseLine = false;

    if  ((sipperImage->ColLeft () < 350)  ||  (sipperImage->ColLeft () > 3700))
    {
      uint  width  = 1 + sipperImage->ColRight () - sipperImage->ColLeft ();
      uint  height = 1 + sipperImage->RowBot   () - sipperImage->RowTop  ();

      if  ((height > 90)  &&  (width < 30))
        noiseLine = true;

      else if  ((height > 300)  &&   (width < 40))
        noiseLine = true;

      else if  ((height > 600)  &&   (width < 51))
        noiseLine = true;

      else if  ((height > 1500)  &&   (width < 58))
        noiseLine = true;
    }

    if  (noiseLine)
    {
       //RasterSipperPtr raster = sipperImage->GetRaster (frame, blobIds, frameSipperRow, frameRowByteOffset);
       //KKStr  fn;
       //
       //fn << "C:\\Temp\\NoiseLines\\" << sipperRootName << "_"
       //   << StrFormatInt (sipperImage->RowTop  (), "00000000")
       //   << "_"
       //   << StrFormatInt (sipperImage->ColLeft (), "0000")
       //   << ".bmp";
       //
       //SaveImage  (*raster, fn);
       //delete  raster;
       //raster = NULL;
       continue;
    }


    if  (origFrameArea)
      sipperImage->Dialate (frame, blobIds, 2);

    numImagesWritten++;
    imagesWrittenInFrame++;
    parms.IncrementImagesWritten ();
    parms.IncrementPixelsWritten (sipperImage->PixelCount ());
    parms.BytesExtracted (sipperBuff->ByteOffset ());

    KKStr  imageFileName (40);

    uint  sipperTopRow = sipperImage->RowTop () + frameSipperRow;
    uint  sipperTopCol = sipperImage->ColLeft ();

    imageFileName = sipperRootName;
    imageFileName << "_"
                  << StrFormatInt (sipperTopRow, "ZZ00000000")
                  << "_"
                  << StrFormatInt (sipperTopCol, "0000")
                  << ".bmp";
    

    MLClassConstPtr  predClass = unKnownMLClass;
    ImageFeaturesPtr  image = NULL;
    RasterSipperPtr raster = sipperImage->GetRaster (frame, blobIds, frameSipperRow, frameRowByteOffset);
    
    bool  imageIsDuplicate = false;


    if  (parms.ExtractFeatureData ())
    {
      raster->FileName (imageFileName);

      image = new ImageFeatures (*raster, unKnownMLClass);

      image->ImageFileName (imageFileName);

      InstrumentDataPtr  id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, parms.Cancel (), log);
      if  (id)
      {
        image->FlowRate1    (id->FlowRate1    ());
        image->FlowRate2    (id->FlowRate2    ());
        image->Latitude     (id->Latitude     ());
        image->Longitude    (id->Longitude    ());
        image->Depth        (id->Depth        ());
        image->Salinity     (id->Salinity     ());
        image->Oxygen       (id->Oxygen       ());
        image->Fluorescence (id->Fluorescence ());
      }

      image->SfCentroidCol (image->CentroidCol () + float  (sipperImage->ColLeft ()));
      image->SfCentroidRow (image->CentroidRow () + double (sipperImage->RowTop () + frameSipperRow));
 
      // Before we go and save this Plankton Image, lets make sure that
      // it is not a duplicate of a previous one extracted.  This happens
      // sometimes because sometimes a buffer contents in SIPPER are 
      // written twice, at the end of the file.

      if  (dupImageDetector)
      {
        DuplicateImagePtr  dupImages = dupImageDetector->AddSingleImage (image);
        if  (dupImages)
        {
          log.Level (30) << "ProcessFrame  Duplicate Image Detected[" << image->ImageFileName () << "]." << endl;
          imageIsDuplicate = true;
          duplicateImages->PushOnBack (image);
          reportFile << endl 
                     << "Duplicate Image Detected[" << imageFileName << "]" << endl
                     << "Original Image is       [" << dupImages->FirstImageAdded ()->ImageFileName () << "]" << endl;

          if  (!duplicateImageDir.Empty ())
          {
            // Since image is a duplicate we will need to move it to duplicates directory
            KKStr  newRootName = osGetRootName (dupImages->FirstImageAdded ()->ImageFileName ()) 
                                  + "-" +
                                  osGetRootName (image->ImageFileName ());
            KKStr  newFullFileName = osAddSlash (duplicateImageDir) + newRootName + ".bmp";
            osDeleteFile (newFullFileName);  // Make sure there is no file already there from prev run.
            BmpImage bmpImage (*raster);
            bmpImage.SaveGrayscaleInverted4Bit (newFullFileName);
          }
        }
      }

      if  (!imageIsDuplicate)
      {
        if  (classifier)
        {
          int  numOfWinners;
          bool knownClassOneOfTheWinners  = false;
          double  probability = 0.0f;
          double  breakTie    = 0.0f;

          ImageFeaturesPtr  dupImage = new ImageFeatures (*image);
          predClass = classifier->ClassifyAImage (*dupImage, probability, numOfWinners, knownClassOneOfTheWinners, breakTie);
          image->MLClass     (predClass);
          image->Probability    ((float)probability);
          image->BreakTie       ((float)breakTie);
          image->PredictedClass (predClass);
          delete  dupImage;
        }
      }
    }

    if  (!imageIsDuplicate)
    {
      bool  successful = false;

      MLClassPtr  validatedClass = NULL;
      if  (parms.RefreshDataBaseImages ())
      {
        ReFreshDataBaseImage (imageFileName, 
                              sipperRootName, 
                              raster, 
                              image, 
                              sipperImage->ByteOffset (),
                              sipperTopRow,
                              sipperTopCol
                             );
      }

      else if  ((parms.ExtractFeatureData ())  &&  parms.UpdateDataBase ())
      {
        dbConn->ImageInsert (*raster,
                             osGetRootName (imageFileName),
                             osGetRootName (sipperRootName),
                             sipperImage->ByteOffset (),       // byteOffset,     // byteOffset of SipperRow containing TopLeftRow
                             sipperTopRow,
                             sipperTopCol,
                             raster->Height (),
                             raster->Width  (),
                             sipperImage->PixelCount (),
                             parms.ConnectedPixelDist (),
                             0,                                   // Extracttipon LogId
                             0,
                             (int)(image->CentroidRow () + 0.5f),
                             (int)(image->CentroidCol () + 0.5f),
                             image->PredictedClass (),
                             image->Probability (),
                             NULL,
                             0.0,
                             NULL,
                             image->Depth (),
                             0.0f,              // imageSize
                             NULL,              // sizeCoordinates,
                             imageId,
                             successful
                            );

        if  (!successful)
        {
          log.Level (-1) << endl << endl
                         << "SipperImageExtraction::ProcessFrame    ***ERROR***       Insert Failed" << endl
                         << endl
                         << "ErrorMessage[" << dbConn->LastErrorDesc () << "]" << endl
                         << endl;
        }
        else
        {
          dbConn->FeatureDataInsertRow (sipperRootName, *image);
        }
      }

      try
      {
        imageManager->AddImage (imageFileName, 
                                predClass,
                                sipperImage->PixelCount (),
                                instrumentDataManager->Depth (),
                                image,              // imageManager will take ownershop of image.
                                raster,
                                parms.Colorize ()
                               );
      }
      catch (...)
      {
        cerr << std::endl << std::endl
          << "Exception adding image to ImageManager." << std::endl
          << std::endl;
        char  buff[120];
        cin >> buff;
      }
    }

    delete  raster;  raster = NULL;
  }  /* end of for blobIDX */

  reportFile << "Frame:"       << StrFormatInt (frameNum,             "##,##0")
             << "  Size:"      << StrFormatInt (lastRowInFrame,       "###,##0")
             << "  Images:"    << StrFormatInt (imagesInFrame,        "##,##0")
             << "  Written:"   << StrFormatInt (imagesWrittenInFrame, "##,##0")
             << endl;

  imagesInFrame        = 0;
  imagesWrittenInFrame = 0;

  if  (!parms.RunningAsAWindowsApp ())
  {
    cout << "*STAT*"; 
    cout << sipperBuff->ByteOffset () << "," 
         << sipperBuff->CurRow     () << "," 
         << parms.ImagesExtracted  () << "," 
         << parms.ImagesWritten    ()
         << endl;
  }

  if  (!parms.Cancel ())
  {
    // refresh the current class statistics.
    ClassStatisticListPtr  classStats = imageManager->ClassStats ();
     // 'parms' will take ownership of classStats.
    parms.ClassStats (classStats);
  }

  {
    // Need to clean up sipperImages  list
    VectorSipperImages::iterator  siIDX;
    for  (siIDX = sipperImages->begin ();  siIDX != sipperImages->end ();  siIDX++)
    {
      SipperImagePtr  sipperImage = *siIDX;
      delete  sipperImage;
    }

    delete  sipperImages;  sipperImages = NULL;
  }

  log.Level (20) << "ProcessFrame   Exiting"  << endl;
} /* ProcessFrame */





void  SipperImageExtraction::ReFreshDataBaseImage (const KKStr&      imageFileName, 
                                                   const KKStr&      sipperFileName, 
                                                   RasterSipperPtr   raster, 
                                                   ImageFeaturesPtr  featureVector,
                                                   uint64            byteOffset,
                                                   uint              sipperTopRow,
                                                   uint              sipperTopCol
                                                  )
{
  log.Level (10) << "SipperImageExtraction::ReFreshDataBaseImage  ImageFileName[" << imageFileName << "]." << endl;

  if  (!dbConn)
    return;

  float  centroidRow         = -1.0f;
  float  centroidCol         = -1.0f;
  float  centroidWeightedCol = -1.0f;
  float  centroidWeightedRow = -1.0f;
  int    imageId             = -1;
  int    size                = 0;
  bool   successful          = false;
  int    weight              = 0;

  raster->CalcCentroid (size, weight, centroidRow, centroidCol, centroidWeightedRow, centroidWeightedCol);

  DataBaseImagePtr  existingEntry = dbConn->ImagesLocateClosestImage (imageFileName);
  if  (existingEntry)
  {
    existingEntry->PixelCount ((uint)size);
    existingEntry->CentroidRow ((int)(centroidRow + 0.5f));
    existingEntry->CentroidCol ((int)(centroidCol + 0.5f));
    existingEntry->ByteOffset  (byteOffset);
    existingEntry->TopLeftRow  (sipperTopRow);
    existingEntry->TopLeftCol  (sipperTopCol);
    existingEntry->Height      (raster->Height ());
    existingEntry->Width       (raster->Width  ());

    dbConn->ImageUpdate (*existingEntry, *raster);
  }

  else if  (featureVector)
  {
    dbConn->ImageInsert (*raster,
                         osGetRootName (imageFileName),
                         osGetRootName (sipperRootName),
                         byteOffset,       // byteOffset,     // byteOffset of SipperRow containing TopLeftRow
                         sipperTopRow,
                         sipperTopCol,
                         raster->Height (),
                         raster->Width  (),
                         (uint)(size),
                         parms.ConnectedPixelDist (),
                         0,     // Extraction Log Id
                         0,     // Classification Log Id
                         (uint)(centroidRow + 0.5f),
                         (uint)(centroidCol + 0.5f),
                         featureVector->PredictedClass (),
                         featureVector->Probability (),
                         NULL,
                         0.0f,
                         NULL,
                         featureVector->Depth (),
                         0.0f,              // imageSize
                         NULL,              // sizeCoordinates,
                         imageId,
                         successful
                        );
  }

  delete  existingEntry;
  existingEntry = NULL;
}  /* ReFreshDataBaseImage */

