#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <map>

#include  "MemoryDebug.h"
using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include  "DataBaseLogEntry.h"
using namespace MLL;




DataBaseLogEntry::DataBaseLogEntry ():
  logEntryId        (0),
  progCode          (""),
  progName          (""),
  dateTimeCompiled  (),
  cmdLine           (""),
  compName          (""),
  dataBaseUserName  (""),
  compUserName      (""),
  dateTimeStart     (),
  dateTimeStartUtc  (),
  dateTimeEnd       (),
  cpuTimeUsed       (),
  completionStatus  (""),
  sipperFileName    ("")
{
}


DataBaseLogEntry::DataBaseLogEntry (const KKStr&     _progCode,
                                    const KKStr&     _progName,
                                    const DateTime&  _dateTimeCompiled,
                                    const KKStr&     _cmdLine,
                                    const KKStr&     _compName,
                                    const KKStr&     _compUserName,
                                    const KKStr&     _sipperFileName   /**<  Root name of Sipper File */
                                   ):
  logEntryId        (0),
  progCode          (_progCode),
  progName          (_progName),
  dateTimeCompiled  (_dateTimeCompiled),
  cmdLine           (_cmdLine),
  compName          (_compName),
  dataBaseUserName  (""),
  compUserName      (_compUserName),
  dateTimeStart     (),
  dateTimeStartUtc  (),
  dateTimeEnd       (),
  cpuTimeUsed       (),
  completionStatus  (""),
  sipperFileName    (_sipperFileName)

{
}





DataBaseLogEntry::~DataBaseLogEntry ()
{
}



DataBaseLogEntryList::DataBaseLogEntryList (bool _owner):
      KKQueue<DataBaseLogEntry> (_owner)
{
}


DataBaseLogEntryList::~DataBaseLogEntryList ()
{
}



class  DataBaseLogEntryList::ProgCodeComparison
{
public:
  ProgCodeComparison () {}

  bool  operator()  (DataBaseLogEntryPtr  p1,
                     DataBaseLogEntryPtr  p2
                    )
  {
    if  (p1->ProgCode () < p2->ProgCode ())
      return true;

    else if  (p1->ProgCode () > p2->ProgCode ())
      return false;

    return  (p1->DateTimeStart () < p2->DateTimeStart ());
  }
};



void  DataBaseLogEntryList::SortByProgCode ()
{
  ProgCodeComparison  c;
  sort (begin (), end (), c);
}  /* SortByProgCode */

