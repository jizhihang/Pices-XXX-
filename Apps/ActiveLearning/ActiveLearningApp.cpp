#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "ActiveLearning.h"


#include  "OSservices.h"

#include  "MemoryDebug.h"



// -c GrayScale.cfg  -r  Report.txt    -o  PROBABILITY

//  -c FiveBigest.cfg  -o  Probability  -g FiveBigest -M All -i 5 -k 10  -r K:\Plankton\StatusReports\2003-12-01\OneAtATime_400PerClassValidation_Take.txt



// ImageNameList  trainingImages;




void  CopyDirectory (KKStr  srcDir,
                     KKStr  destDir
                    )
{
  cout << "Copying Contents of '" << srcDir << "'" << endl;

 
  osAddLastSlash (srcDir);
  osAddLastSlash (destDir);

  osCreateDirectory (destDir);

  KKStr  fileSpec (srcDir);
  fileSpec << "*.*";

  KKStrListPtr  fileList = osGetListOfFiles (fileSpec);

  if  (fileList)
  {
    StringListIterator  flIDX (*fileList);

    for  (flIDX.CurPtr (); flIDX.CurPtr (); ++flIDX)
    {
      KKStr  fileName = flIDX.CurPtr ();

      osCopyFileBetweenDirectories (fileName, srcDir, destDir);
      // trainingImages.AddEntry (new ImageName (fileName, destDir));
    }
  }


  KKStrListPtr  subDirectories = osGetListOfDirectories (fileSpec);
  StringListIterator  sdIDX (*subDirectories);
  
  for  (sdIDX.Reset ();  sdIDX.CurPtr (); ++sdIDX)
  {
    KKStr  dirName (*(sdIDX.CurPtr ()));

    KKStr  newSrcDir (srcDir);
    newSrcDir << dirName << DS;

    KKStr  newDestDir (srcDir);
    newDestDir << dirName << DS;

    CopyDirectory (srcDir, destDir);
  }
}  /* CopyDirectory */





int  main (int argc,  char** argv)
{

  #if  defined (WIN32)  &&  defined (_DEBUG)  && !defined(_NO_MEMORY_LEAK_CHECK_)
    // _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );  
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
  #endif

  ActiveLearning activeLearner (argc, argv);

  if  (activeLearner.Abort ())
  {
    cerr << endl
         << "main     activeLearner constructor failed,  Abort flag set." << endl
         << endl;
  }
  else
  {
    cout << endl
         << "main   Starting ProcessAllJobs"
         << endl;

    activeLearner.ProcessAllJobs ();
  }

  MLClass::FinalCleanUp ();
  FileDesc::FinalCleanUp ();


  return 0;
}
