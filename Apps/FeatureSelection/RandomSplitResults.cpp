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


#include "RandomSplitResults.h"
#include "BinaryJob.h"
#include "FeatureSelectionTypes.h"
#include "JobRandomSplit.h"
#include "Processor.h"



RandomSplitResults::RandomSplitResults (RunLog&  _log):
   Results (_log),
   splitNum (0)
{
}


RandomSplitResults::RandomSplitResults (FinalResultType          _resultType,
                                        TrainingConfiguration2Ptr _config,
                                        CrossValidationPtr        _crossValidation,
                                        FeatureVectorListPtr      _trainData,
                                        const KKStr&              _hostName,
                                        JobRandomSplitPtr         _job,
                                        RunLog&                   _log
                                       ):
  Results (_resultType, _config, _crossValidation, _trainData, _hostName, _job, _log),
  splitNum (0)

{
  splitNum = _job->SplitNum ();
}




RandomSplitResults::~RandomSplitResults ()
{
}




void  RandomSplitResults::Write (ostream&  o)
{
  o << "<RandomSplitResults>"                                             << endl
    << "SplitNum"  << "\t" << splitNum    << endl;

  Results::Write (o);

  o << "</RandomSplitResults>" << endl;
}  /* Write */



void  RandomSplitResults::Read (istream&  i)
{
  char  buff[50000];

  i.getline (buff, sizeof (buff));
  KKStr l  (buff);

  while  ((!i.eof ())  &&  (!l.EqualIgnoreCase ("</RandomSplitResults>")))
  {
    KKStr  fieldName = l.ExtractToken2 ("\t");

    if  (fieldName.EqualIgnoreCase ("<Results>"))
      Results::Read (i);

    else if  (fieldName.EqualIgnoreCase ("SplitNum"))
      splitNum = l.ExtractTokenInt ("\t");

    i.getline (buff, sizeof (buff));
    l = buff;
  }

}  /* Read */





RandomSplitResultsPtr  RandomSplitResults::ConstructFromStream (istream&  i,
                                                                RunLog&   log
                                                               )
{
  RandomSplitResultsPtr  results = new RandomSplitResults (log);
  results->Read (i);
  return  results;
} /* ConstructFromStream */





RandomSplitResultsList::RandomSplitResultsList (bool     _owner,
                                                RunLog&  _log
                                               ):
    KKQueue<RandomSplitResults> (_owner),
    log (_log)
{
}



RandomSplitResultsList::RandomSplitResultsList (const RandomSplitResultsList&  results):
    KKQueue<RandomSplitResults> (results.Owner ()),
    log (results.log)
{
  RandomSplitResultsList::const_iterator idx;
  for  (idx = results.begin ();  idx != results.end ();  idx++)
  {
    if  (Owner ())
    {
      PushOnBack (new RandomSplitResults (**idx));
    }
    else
    {
      PushOnBack (*idx);
    }
  }
}




RandomSplitResultsList::RandomSplitResultsList (const KKStr&  resultsFileName,
                                                RunLog&       _log
                                               ):
    KKQueue<RandomSplitResults>(true),
    log (_log)
{
  ifstream i (resultsFileName.Str ());
  if  (i.is_open ())
    Read (i);
  i.close ();
}




RandomSplitResultsList::~RandomSplitResultsList ()
{
}



RandomSplitResultsPtr  RandomSplitResultsList::Average ()  const 
{
  RandomSplitResultsPtr  total = new RandomSplitResults (log);
  RandomSplitResultsList::const_iterator idx;
  for  (idx = begin ();  idx != end ();  idx++)
    (*total) += (**idx);

  if  (QueueSize () > 0)
    (*total) /= QueueSize ();
  return  total;
}  /* Average */




void  RandomSplitResultsList::Read (istream&  i)
{
  char buff[10240];
  i.getline (buff, sizeof (buff));
  while  (!i.eof ())
  {
    KKStr  ln (buff);
    ln.TrimLeft ();
    ln.TrimRight ();

    KKStr  lineName = ln.ExtractToken2 ("\t");

    if  (lineName.EqualIgnoreCase ("<RandomSplitResults>"))
    {
      RandomSplitResultsPtr r = RandomSplitResults::ConstructFromStream (i, log);
      if  (r)
        PushOnBack (r);
    }

    i.getline (buff, sizeof (buff));
  }
}  /* Read */

 

