#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <math.h>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace std;

#include "ImageIO.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;

#include "SipperCruise.h"
#include "SipperDeployment.h"
#include "SipperFile.h"
#include "SipperVariables.h"
using namespace  SipperHardware;

#include "DataBase.h"
#include "ImageFeatures.h"
using namespace MLL;


#include "FullSizeImagesInstall.h"


using namespace  FullSizeImagesInstall_DataNameSpace;

// -SrcDir  D:\Pices\Cruises\WB1101\GpsData



void  FullSizeImagesInstall::SipperFileResults::PrintHeader (ostream& o)
{
  o << ""       << "\t" << "Sipper" << "\t" << ""          << "\t" << "SipperFile" << "\t" << ""             << "\t" << ""           << "\t"  << ""           << endl;
  o << "Sipper" << "\t" << "File"   << "\t" << "Sipper"    << "\t" << "Already In" << "\t" << "SipperFile"   << "\t" << "SipperFile" << "\t"  << "SipperFile" << endl;
  o << "File"   << "\t" << "Image"  << "\t" << "File"      << "\t" << "FullSize"   << "\t" << "Missing From" << "\t" << "Images"     << "\t"  << "Images"     << endl;
  o << "Name"   << "\t" << "Count"  << "\t" << "ThumbNail" << "\t" << "Table"      << "\t" << "FullSize"     << "\t" << "Lost"       << "\t"  << "Recoverd"   << endl;
}


void  FullSizeImagesInstall::SipperFileResults::Print (ostream&  o)
{
  o << sipperFileName                   << "\t"
    << sipperFileImageCount             << "\t"
    << sipperFileThumbNail              << "\t"
    << sipperFileAlreadyInFullSizeTable << "\t"
    << sipperFileMissingFromFullSize    << "\t"
    << sipperFileImagesLost             << "\t"
    << sipperFileImagesRecoverd
    << endl;
}


FullSizeImagesInstall::SipperFileResults&  FullSizeImagesInstall::SipperFileResults::operator+= (const SipperFileResults& right)
{
  sipperFileImageCount             += right.sipperFileImageCount;
  sipperFileThumbNail              += right.sipperFileThumbNail;
  sipperFileAlreadyInFullSizeTable += right.sipperFileAlreadyInFullSizeTable;
  sipperFileMissingFromFullSize    += right.sipperFileMissingFromFullSize;
  sipperFileImagesLost             += right.sipperFileImagesLost;
  sipperFileImagesRecoverd         += right.sipperFileImagesRecoverd;
  return *this;
}



FullSizeImagesInstall::FullSizeImagesInstall (int     argc, 
                                              char**  argv
                                             ):

   Application  (argc, argv),
   cancelFlag            (false),
   dbConn                (NULL),
   report                (NULL),
   reportFileName        (),
   results               (),
   sipperFiles           (NULL),
   sipperFilesProcessed  (0)
{                  
  if  (argc < 1)   
  {                
    log.Level (-1) << endl << endl 
                   << "No Command Line Parameters were provided." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

}



FullSizeImagesInstall::~FullSizeImagesInstall ()
{
  delete  dbConn;   dbConn = NULL;
  delete  report;   report = NULL;
}




bool  FullSizeImagesInstall::ProcessCmdLineParameter (char    parmSwitchCode, 
                                                      KKStr   parmSwitch, 
                                                      KKStr   parmValue
                                                    )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();


  if  (parmSwitch.EqualIgnoreCase ("-Report")  ||  parmSwitch.EqualIgnoreCase ("-R"))
  {
    reportFileName = parmValue;
    if  (osFileExists (reportFileName))
    {
      log.Level (-1) << endl << endl
                     << "FullSizeImagesInstall::ProcessCmdLineParameter    ***ERROR***" << endl
                     << endl
                     << "             Report File Already Exists[" << reportFileName << "]" << endl
                     << endl;
      Abort (true);
    }
  }

  else
  {
    log.Level (-1) << endl << endl
                   << "FullSizeImagesInstall::ProcessCmdLineParameter    ***ERROR***" << endl
                   << endl
                   << "             Invalid Parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }

	return  !Abort ();
}  /* ProcessCmdLineParameter */




void   FullSizeImagesInstall::DisplayCommandLineParameters ()
{
  log.Level (0) << "FullSizeImagesInstall"                                                           << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "   Application that will scan through all images in the MySQL Images table"      << endl;
  log.Level (0) << "   and verify that a FullSize image exists for those that are larger than"       << endl;
  log.Level (0) << "   their thumbnail version.  The data will be proecssed by SipperFile entries."  << endl;
  log.Level (0)                                                                                      << endl;
}  /* DisplayCommandLineParameters */




void   FullSizeImagesInstall::DiagnoseImage (DataBaseImagePtr  image, 
                                             RasterSipperPtr   fullSizeImage
                                            )
{
  RasterSipperPtr  thumbNail = NULL;

  DataBaseImagePtr  thumbNailEntry = dbConn->ImageLoad (image->ImageId ());
  if  (thumbNailEntry)
    thumbNail = thumbNailEntry->ThumbNail (log);

  RasterSipperPtr recoverd = image->GetOrigImageFromSipperFile (log);

  KKStr  rootName = image->ImageFileName () + "_" + StrFormatInt (image->Height (), "0000") + "-" + StrFormatInt (image->Width (), "0000");
  KKStr  dirName = "C:\\Temp\\PicesImages\\";
  KKU::osCreateDirectoryPath (dirName);

  KKStr  fullRootName = osAddSlash (dirName) + rootName;

  if  (thumbNail)
    KKU::SaveImageGrayscaleInverted8Bit (*thumbNail, fullRootName + "_0ThumbNail.bmp");
  KKU::SaveImageGrayscaleInverted8Bit (*fullSizeImage, fullRootName   + "_1FullSize.bmp");
  KKU::SaveImageGrayscaleInverted8Bit (*recoverd,      fullRootName   + "_2Recoverd.bmp");

  delete  thumbNail;  thumbNail = NULL;
  delete  recoverd;   recoverd  = NULL;

}  /* DiagnoseImage */




void   FullSizeImagesInstall::ProcessSipperFile (SipperFilePtr  sipperFile)
{
  log.Level (10) << "FullSizeImagesInstall::ProcessSipperFile    Starting File[" << sipperFile->SipperFileName () << "]" << endl;

  *report << endl << endl
    << "****************************************************************************************************************************" << endl
    << "****************************************************************************************************************************" << endl
    << endl
    << "Sipper File" << "\t" << sipperFile->SipperFileName () << endl
    << endl;

  int32  sipperFileImageCount             = 0;
  int32  sipperFileThumbNail              = 0;  // number of images where the ThumbNail is the true extracted image.
  int32  sipperFileFullSize               = 0;  // number images that should be saved in ImagesFullSize table.
  int32  sipperFileAlreadyInFullSizeTable = 0;
  int32  sipperFileMissingFromFullSize    = 0;
  int32  sipperFileWrong                  = 0;  // Number images nit saved correctly.
  int32  sipperFileImagesLost             = 0;
  int32  sipperFileImagesRecoverd         = 0;

  int32  totalNumSipperFiles = sipperFiles->size ();

  //if  (sipperFilesProcessed > 267)
  {
    int zed = 9090;

    DataBaseImageListPtr  images 
      = dbConn->ImagesQuery (NULL,                            // imageGroup,
                             sipperFile->SipperFileName (),
                             NULL,                            //mlClass,
                             'P',                             // classKeyToUse, 'P' - Use Prediced Class Key,  'V' - Validated Class */
                             0.0f,                            // probMin,
                             1.0f,                            // probMax,
                             0,                               // sizeMin,
                             0,                               // sizeMax,  0 indicates that there is no upper bound.
                             0,                               // depthMin,
                             0,                               // depthMax,
                             0,                               // restartImageId,  <= 0 indicates start from beginning.
                             -1,                              // limit,    -1 indicates no limit.
                             false,                           // includeThumbnail,
                             cancelFlag
                            );

    if  (images == NULL)
    {
      log.Level (-1) << endl << "FullSizeImagesInstall::ProcessSipperFile    *** NO IMAGES RETRIEVED FROM THE DATABASE ***" << endl;
      *report << "*** No Images returned from database. ***" << endl << endl;
    }
    else
    {
      DataBaseImageList::iterator  idx;
      for  (idx = images->begin ();  idx != images->end ();  ++idx)
      {
        DataBaseImagePtr  image = *idx;

        sipperFileImageCount++;
        if  ((sipperFileImageCount % 500) == 0)
          cout << (sipperFilesProcessed + 1)  << " of " << totalNumSipperFiles << "   " << sipperFileImageCount << " of " << images->size () << "  " << sipperFileImagesRecoverd << "  " << sipperFileImagesLost << endl;

        bool  deleteOldFulSizeImage = false;

        int32  maxDim = Max (image->Height (), image->Width ());
        if  (maxDim <= 100)
        {
          // Since the maximum dimension is less that 100 then the ThumNail in the Images table is the True image as extracted form the sipper file.
          sipperFileThumbNail++;
        }
        else
        {
          ++sipperFileFullSize;

          // The True image is larger than the ThumbNail image;  we will 1st check if is already exists in the ImagesFullSize table.
          // If it does not then we will go to the Sipper File and reconstruct a new one.
          RasterSipperPtr i = dbConn->ImageFullSizeLoad (image->ImageFileName ());
          if  (i == NULL)
          {
            sipperFileMissingFromFullSize++;
          }
          else
          {
            int32  tlRow,tlCol, brRow, brCol;
            i->FindBoundingBox (tlRow, tlCol, brRow, brCol);

            if  ((tlRow > 3)  ||
                 (tlCol > 3)  ||
                 (brRow < (i->Height () - 3))  ||
                 (brCol < (i->Width  () - 3))
                )
            {
              DiagnoseImage (image, i);

              ++sipperFileWrong;
              delete  i;
              i = NULL;
              deleteOldFulSizeImage = true;
            }
            else
            {
              sipperFileAlreadyInFullSizeTable++;
            }
          }

          if  (i == NULL)
          {
            // The image was not in the ImageFullSize table,  we wil now reconstruct the image and save it to the ImageFullSize table.
            try  {i = image->GetOrigImageFromSipperFile (log);}
            catch (...) {i = NULL;}

            if  (i != NULL)
            {
              // Sine we just went to the trouble to load construct the Full Size version of the image from the 
              // SIPPER file we should update the database so that next time we can retrieve without resorting to 
              // the SIPPER file.
              if  (deleteOldFulSizeImage)
                dbConn->ImageFullSizeDelete (image->ImageFileName ());

              sipperFileImagesRecoverd++;
              *report << image->ImageFileName () << "\t" << "Recovered" << endl;
              dbConn->ImageFullSizeSave (image->ImageFileName (), *i);

              ImageFeaturesPtr  fd = new ImageFeatures (*i, image->Class1 (), NULL);

              fd->ImageFileName (image->ImageFileName ());
              ImageFeaturesPtr  existingFeatureData = dbConn->FeatureDataRecLoad (image->ImageFileName ());

              InstrumentDataPtr  id = NULL;
              uint32  frameNum = image->TopLeftRow () / 4096;

              id = dbConn->InstrumentDataGetByScanLine (image->SipperFileName (), 4096 * frameNum);
              if  (id != NULL)
              {
                fd->Depth        (id->Depth        ());
                fd->Fluorescence (id->Fluorescence ());
                fd->Salinity     (id->Salinity     ());
                fd->Oxygen       (id->Oxygen       ());
              }

              if  (existingFeatureData)
              {
                if  (id == NULL)
                {
                  fd->Depth        (existingFeatureData->Depth        ());
                  fd->Fluorescence (existingFeatureData->Fluorescence ());
                  fd->Salinity     (existingFeatureData->Salinity     ());
                  fd->Oxygen       (existingFeatureData->Oxygen       ());
                }
                dbConn->FeatureDataUpdate (image, fd);
                delete  existingFeatureData;
                existingFeatureData = NULL;
              }
              else
              {
                dbConn->FeatureDataInsertRow (image->SipperFileName (), *fd);
              }

              delete  id;  id = NULL;
              delete  fd;  fd = NULL;
            }

            else
            {
              // Image was not recovered.
              sipperFileImagesLost++;
              *report << image->ImageFileName () << "\t" << "***NOT FOUND***" << endl;
              log.Level (-1) << "ProcessSipperFile   ** FAILED **   to recover Image[" << image->ImageFileName () << "]" << endl;
            }
          }

          delete  i;
          i = NULL;
        }
      }
    }

    cout << (sipperFilesProcessed + 1) << " of " << totalNumSipperFiles << "   " << sipperFileImageCount << " of " << images->size () << endl;


    SipperFileResults sippFileResults (sipperFile->SipperFileName (),
                                       sipperFileImageCount,
                                       sipperFileThumbNail,
                                       sipperFileAlreadyInFullSizeTable,
                                       sipperFileMissingFromFullSize,
                                       sipperFileImagesLost,
                                       sipperFileImagesRecoverd
                                      );

    log.Level (10) << "ProcessSipperFile    Sipper File[" << sipperFile->SipperFileName () << "]     Completed." << endl;
    *report << "--------------------------------------------------------------------------------------------" << endl;
    *report << "sipperFileImageCount"             << "\t" << sipperFileImageCount             << endl;
    *report << "sipperFileThumbNail"              << "\t" << sipperFileThumbNail              << endl;
    *report << "sipperFileAlreadyInFullSizeTable" << "\t" << sipperFileAlreadyInFullSizeTable << endl;
    *report << "sipperFileMissingFromFullSize"    << "\t" << sipperFileMissingFromFullSize    << endl;
    *report << "sipperFileImagesLost"             << "\t" << sipperFileImagesLost             << endl;
    *report << "sipperFileImagesRecoverd"         << "\t" << sipperFileImagesRecoverd         << endl;
    *report << "--------------------------------------------------------------------------------------------" << endl;
    *report << endl << endl;

    results.push_back (sippFileResults);

    delete  images;
    images = NULL;
  }

  ++sipperFilesProcessed;
}  /* ProcessSipperFile */




void   FullSizeImagesInstall::Main ()
{
  if  (Abort ())
    return;

  if  (reportFileName.Empty ())
  {

    KKStr  reportDir = osAddSlash (SipperVariables::PicesReportDir ()) + "FullSizeImagesInstall";
    osCreateDirectoryPath (reportDir);
    reportFileName = osAddSlash (reportDir) + "FullSizeImagesInstall_" + osGetLocalDateTime ().YYYYMMDDHHMMSS () + ".txt";
  }

  report = new ofstream (reportFileName.Str ());

  dbConn = new DataBase (log);
  if  (!dbConn->Valid ())
  {
    log.Level (-1) << endl << endl 
      << "FullSizeImagesInstall::Main    ***ERROR***    Failed to connect to database."  << endl
      << endl;
    Abort (true);
    return;
  }

  sipperFiles = dbConn->SipperFileLoad ("", "", "");
  if  (!sipperFiles)
  {
    log.Level (-1) << endl << endl 
      << "FullSizeImagesInstall::Main    ***ERROR***    No SipperFile table entries found."  << endl
      << endl;
    Abort (true);
    return;
  }

  SipperFileList::iterator idx;
  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  ++idx)
  {
    ProcessSipperFile (*idx);
  }

  *report << endl  << endl << endl
    << "***************************************************************************************************" << endl
    << "All Sipper Files Processed" << endl
    << endl;

  *report << "Summary of results" << endl;

  SipperFileResults  totals ("Totals", 0, 0, 0, 0, 0, 0);

  SipperFileResults::PrintHeader (*report);
  for  (uint32 x = 0;  x < results.size ();  ++x)
  {
    results[x].Print (*report);
    totals += results[x];
  }

  *report << endl;

  totals.Print (*report);
  *report << endl;

  delete  report;
  report = NULL;
 }  /* Main */





int  main (int     argc,
           char**  argv
          )
{
  FullSizeImagesInstall  importGpsApp (argc, argv);
  if  (importGpsApp.Abort ())
    return 1;

  importGpsApp.Main ();
  if  (importGpsApp.Abort ())
    return 1;
  else
    return 0;
}
