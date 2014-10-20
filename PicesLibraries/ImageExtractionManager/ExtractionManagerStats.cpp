#include "FirstIncludes.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
using namespace KKU;


#include "ExtractionManagerStats.h"
using namespace  ImageExtractionManager;


ExtractionManagerStats::ExtractionManagerStats ():
    bytesRead          (0),
    framesRead         (0),
    framesProcessed    (0),
    scanLinesProcessed (0),
    imagesFound        (0),
    imagesClassified   (0),
    imagesUpdated      (0),
    duplicatesDetected (0),
    updateFailures     (0),
    framesOnQueue      (0),
    imagesOnQueue      (0)
{
}



ExtractionManagerStats::ExtractionManagerStats (const ExtractionManagerStats&  stats):
    bytesRead          (bytesRead),
    framesRead         (framesRead),
    framesProcessed    (framesProcessed),
    scanLinesProcessed (scanLinesProcessed),
    imagesFound        (imagesFound),
    imagesClassified   (imagesClassified),
    imagesUpdated      (imagesUpdated),
    duplicatesDetected (duplicatesDetected),
    updateFailures     (updateFailures),
    framesOnQueue      (framesOnQueue),
    imagesOnQueue      (imagesOnQueue)
{
}


ExtractionManagerStats::~ExtractionManagerStats ()
{
}

