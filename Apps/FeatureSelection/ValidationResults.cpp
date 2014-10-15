#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "MemoryDebug.h"

using namespace std;



#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;



#include "ConfusionMatrix2.h"
#include "CrossValidation.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "ValidationResults.h"
#include "BinaryJob.h"
#include "FeatureSelectionTypes.h"
#include "JobRandomSplit.h"
#include "Processor.h"
#include "Results.h"



ValidationResults::ValidationResults (RunLog&  _log):
     Results (_log),
     classedCorrectly     (NULL),
     classedCorrectlySize (0)
{
}


ValidationResults::ValidationResults (FinalResultType            _resultType,
                                      TrainingConfiguration2Ptr  _config,
                                      CrossValidationPtr         _crossValidation,
                                      FeatureVectorListPtr      _trainData,
                                      const KKStr&              _hostName,
                                      int                        _classedCorrectlySize,
                                      bool*                      _classedCorrectly,
                                      JobValidationPtr           _job,
                                      RunLog&                    _log
                                     ):
  Results (_resultType, _config, _crossValidation, _trainData, _hostName, _job, _log),
  classedCorrectly     (NULL),
  classedCorrectlySize (_classedCorrectlySize)

{
  if  (classedCorrectlySize > 0)
  {
    classedCorrectly = new bool[classedCorrectlySize];
    for (int x = 0;  x < classedCorrectlySize;  x++)
      classedCorrectly[x] = _classedCorrectly[x];
  }
}




ValidationResults::~ValidationResults ()
{
  delete  classedCorrectly;
  classedCorrectly = NULL;
}




void  ValidationResults::Write (ostream&  o)
{
  o << "<ValidationResults>" << endl;
  Results::Write (o);

  if  (classedCorrectly)
  {
    o << "ClassedCorrectly" << "\t" << classedCorrectlySize << endl;
    for  (int x = 0;  x < classedCorrectlySize;  x++)
    {
      o << (classedCorrectly[x] ? "Y" : "N");
    }
    o << endl;
  }

  o << "</ValidationResults>" << endl;
}  /* Write */




void  ValidationResults::Read (istream&  i)
{
  delete  classedCorrectly;
  classedCorrectly = NULL;
  classedCorrectlySize = 0;
  
  char  buff[10240];

  i.getline (buff, sizeof (buff));
  KKStr l  (buff);

  while  ((!i.eof ())  &&  (!l.EqualIgnoreCase ("</ValidationResults>")))
  {
    KKStr  fieldName = l.ExtractToken2 ("\t");

    if  (fieldName.EqualIgnoreCase ("<Results>"))
      Results::Read (i);

    else if  (fieldName.EqualIgnoreCase ("ClassedCorrectly"))
    {
      classedCorrectlySize = l.ExtractTokenInt ("\t");
      if  (classedCorrectlySize > 0)
      {
        int  ccIDX = 0;
        int  ch;
        classedCorrectly = new bool[classedCorrectlySize];
        for  (ccIDX = 0;  ccIDX < classedCorrectlySize;  ccIDX++)
        {
          ch = i.get ();
          if  (ch == '\n')
            break;
          classedCorrectly[ccIDX] = (ch == 'Y');
        }
        while  ((ch != '\n')  &&  (!i.eof ()))
          ch = i.get ();
      }
    }

    i.getline (buff, sizeof (buff));
    l = buff;
  }
}  /* Read */





ValidationResultsPtr  ValidationResults::ConstructFromStream (istream&  i,
                                                              RunLog&   log
                                                             )
{
  ValidationResultsPtr  results = new ValidationResults (log);
  results->Read (i);
  return  results;
} /* ConstructFromStream */

