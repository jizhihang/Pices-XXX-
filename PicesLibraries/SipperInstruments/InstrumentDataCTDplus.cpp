#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
#include  <map>
//#include  <ostream>
#include  <vector>

#include  "MemoryDebug.h"
using namespace std;

#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include  "InstrumentDataCTDplus.h"

#include  "InstrumentDataManager.h"
#include  "SipperFile.h"
using namespace SipperHardware;




InstrumentDataCTDplus::InstrumentDataCTDplus (InstrumentDataManagerPtr _manager,
                                              const KKStr&             _reportDir,
                                              bool                     _text,
                                              int32                    _instrumentId
                                             ):

    InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, false),
    curTextLine              (),
    curTextLineStartScanLine (-1),
    lastTextLine             (),
    prevEolChar              (0)
{
  AssignCtdExternalInstruments ();
}



InstrumentDataCTDplus::~InstrumentDataCTDplus ()
{
  report << curTextLine;
}



void  InstrumentDataCTDplus::AssignCtdExternalInstruments ()
{
  // Ideally I should be getting these assignemnets from the SipperFiles table in the database.

  if  (manager->SipperFile ())
  {
    ctdExt0Code = manager->SipperFile ()->CtdExt0 ();
    ctdExt1Code = manager->SipperFile ()->CtdExt1 ();
    ctdExt2Code = manager->SipperFile ()->CtdExt2 ();
    ctdExt3Code = manager->SipperFile ()->CtdExt3 ();
    return;
  }

  KKStr  fullName = manager->SipperFileName ();
  KKStr  rootName = osGetRootName (fullName);

  ctdExt0Code = "TRN";
  ctdExt1Code = "OXG";
  ctdExt2Code = "FLO";
  ctdExt3Code = "TUR";

  if  (rootName == "ETP2008_1B_01")
  {
    ctdExt0Code = "TRN";
    ctdExt1Code = "OXG";
    ctdExt2Code = "FLO";
    ctdExt3Code = "TUR";
    return;
  }

  else if  (rootName.SubStrPart (0, 9) == "ETP2008_1B")
  {
    ctdExt0Code = "";
    ctdExt1Code = "OXG";
    ctdExt2Code = "FLO";
    ctdExt3Code = "TUR";
  }

  if  ((rootName.SubStrPart (0, 4) == "ETP08")     ||
       (rootName.SubStrPart (0, 6) == "ETP2008")
      )
  {
    ctdExt0Code = "TRN";
    ctdExt1Code = "OXG";
    ctdExt2Code = "FLO";
    ctdExt3Code = "TUR";

    if  ((rootName.SubStrPart (0, 9) == "ETP2008_1B")  ||
         (rootName.SubStrPart (0, 9) == "ETP2008_1C")  ||
         (rootName.SubStrPart (0, 9) == "ETP2008_1D")  ||
         (rootName.SubStrPart (0, 9) == "ETP2008_1E")  ||
         (rootName == "ETP2008_4a_01")
        )
    {
      ctdExt1Code = "";
    }
    return;
  }

  if  (rootName == "OnDeckTest_02")
  {
    ctdExt0Code = "TRN";
    ctdExt1Code = "OXG";
    ctdExt2Code = "FLO";
    ctdExt3Code = "TUR";
    return;
  }

  if  (rootName.SubStrPart (0, 6).EqualIgnoreCase ("Station"))
  {
    ctdExt0Code = "OXG";
    ctdExt1Code = "TRN";
    ctdExt2Code = "FLO";
    ctdExt3Code = "";
    return;
  }

  if  ((rootName.SubStrPart (0,2) == "B07")  ||
       (rootName.SubStrPart (0,1) == "GG")   ||
       (rootName.SubStrPart (0,1) == "SD")   ||
       (rootName.SubStrPart (0,1) == "WB")   ||
       (rootName.SubStrPart (0,1) == "SM")
      )
  {
    ctdExt0Code = "TRN";
    ctdExt1Code = "OXG";
    ctdExt2Code = "FLO";
    ctdExt3Code = "TUR";
    return;
  }

  if  ((rootName.SubStrPart (0,2) == "sip"))
  {
    ctdExt0Code = "OXG";
    ctdExt1Code = "";
    ctdExt2Code = "TRN";
    ctdExt3Code = "FLO";
    return;
  }

  // All other files
  ctdExt0Code = "OXG";
  ctdExt1Code = "";
  ctdExt2Code = "TRN";
  ctdExt3Code = "FLO";
  return;
}  /* AssignCtdExternalInstruments */




/*!
 \ brief  Parses one string of CTD data.  Will parse out seperate fields.
 */
void  InstrumentDataCTDplus::ProcessCTDdata (const KKStr&  txt)
{
  if  (txt.Len () < 94)
    return;

  const char* txtZ = txt.Str ();
  
  if  ((txtZ[0] == '=')  &&  (txtZ[1] == '='))
  {
    // We have a seperator line,  igonore
    return;
  }

  if  ((txtZ[0] == '-')  &&  (txtZ[1] == '-'))
    return;

  if  ((txtZ[0] == '*')  &&  (txtZ[1] == '*'))
    return;
 
  if  ((txtZ[0] == ' ')  &&  (txtZ[1] == ' '))
    return;
 
  
  bool  valid;
  CTD_Plus_Data ctdData (txt, manager->Latitude (), valid, ctdExt0Code, ctdExt1Code, ctdExt2Code, ctdExt3Code);
  if  (valid)
    manager->CTDdataPlus (curTextLineStartScanLine, ctdData);
  else
    cerr << "Invalid data" << endl;

}  /* ProcessCTDdata */




void  InstrumentDataCTDplus::ReportInstrumentData (uint32 curScanLine,
                                                   uchar  sensorData
                                                  )
{
  curTextLineStartScanLine = curScanLine;

  if  ((sensorData == '\n')  ||  (sensorData == '\r'))
  {
    char  lastEolChar = prevEolChar;
    prevEolChar = sensorData;

    if  (lastEolChar == '\n')
    {
      if  (sensorData == '\r')
        return;
    }

    else if  (lastEolChar == '\r')
      if  (sensorData == '\n')
        return;

    if  (!curTextLine.Empty ())
      ProcessCTDdata (curTextLine);

    report << curTextLineStartScanLine << "\t" << curTextLine << endl;
    lastTextLine = curTextLine;
    curTextLine = "";
    return;
  }

  prevEolChar = 0;
  if  (curTextLine.Empty ())
    curTextLineStartScanLine = curScanLine;

  curTextLine.Append ((char)sensorData);

  if  (curTextLine.Len () > 365)
  {
    // The CTD data-stream seams to be missing '\r' and '\f'.  This happens when all 4 voltages are selected for output.
    // We will have to identify individual CTD data rows from the data-stream.  We will use the month fields to help us
    // locate a fixed point in the records.
    //
    // Similar logic had to be implemented in "InstrumentDataReportCTD.cs".
    ParseTxtBlock (curTextLine);
  }
}  /* ReportInstrumentData */



/*  some example CTD Data
28.4069,  5.48627,   17.249, 4.2988, 3.3123, 0.0967, 0.0816,  33.7479, 1541.195, 30 Oct 2007, 21:52:42,  21.3204,  9.9 
28.4070,  5.48617,   17.247, 4.2993, 3.3116, 0.0972, 0.0797,  33.7472, 1541.194, 30 Oct 2007, 21:52:42,  21.3198,  9.9 
28.4072,  5.48616,   17.244, 4.2986, 3.3089, 0.0996, 0.0795,  33.7470, 1541.194, 30 Oct 2007, 21:52:43,  21.3196, 10.0 
28.4071,  5.48620,   17.249, 4.2993, 3.3093, 0.0995, 0.0788,  33.7473, 1541.194, 30 Oct 2007, 21:52:43,  21.3198,  9.9 
28.4072,  5.48620,   17.257, 4.3007, 3.3099, 0.0994, 0.0796,  33.7472, 1541.195, 30 Oct 2007, 21:52:43,  21.3198, 10.0
28.4075,  5.48617,   17.268, 4.2999, 3.3098, 0.1021, 0.0823,  33.7469, 1541.195, 30 Oct 2007, 21:52:44,  21.3194,  9.9
*/

/*!
  \fn ParseTxtBlock   Will use month field in a CTD data stream to locate the start and end of a CTD data block.
 */
void  InstrumentDataCTDplus::ParseTxtBlock (KKStr&  block)
{
  int32 lineStartIdx = 0;
  int32 lineEndIdx   = 0;

  while  (block.Len () > 122)
  {
    int32  monthIdx = LocateAMonthNameStr (block);

    if  (monthIdx < 0)
    {
      if  (block.Len () > 118)
        block = "";
      return;
    }

    lineStartIdx = monthIdx - 85;
    lineEndIdx   = monthIdx + 33;

    if  (lineStartIdx < 0)
    {
      if  (lineEndIdx >= block.Len ())
      {
        block = "";
        return;
      }
      block = block.SubStrPart (lineEndIdx + 1);
    }

    else if  (lineEndIdx >= block.Len ())
    {
      // We will return what is left in block; figuring that more data will be appended to i
      return;
    }

    else
    {
      KKStr  lastLine = block.SubStrPart (lineStartIdx, lineEndIdx);
      block = block.SubStrPart (lineEndIdx + 1);
      ProcessCTDdata (lastLine);
    }
  }
}  /* ParseTxtBlock */




int32 InstrumentDataCTDplus::LocateAMonthNameStr (const KKStr& str)
{
  int32 idx = 0;
  int32 lastIdx = str.Len () - 6;
  const char* s = str.Str ();

  char  c0, c1, c2;

  while  (idx < lastIdx)
  {
    c0 = s[idx];
    if  ((c0 >= 'A')  &&  (c0 < 'Z'))
    {
      c1 = s[idx + 1];
      c2 = s[idx + 2];

      if  ((c0 == 'J')  && (c1 == 'a')  &&  (c2 =='n'))
        return idx;

      if  ((c0 == 'F')  && (c1 == 'e')  &&  (c2 =='b'))
        return idx;

      if  ((c0 == 'M')  && (c1 == 'a')  &&  (c2 =='r'))
        return idx;

      
      if  ((c0 == 'A')  && (c1 == 'p')  &&  (c2 =='r'))
        return idx;

      if  ((c0 == 'M')  && (c1 == 'a')  &&  (c2 =='y'))
        return idx;

      if  ((c0 == 'J')  && (c1 == 'u')  &&  (c2 =='n'))
        return idx;

      
      if  ((c0 == 'J')  && (c1 == 'u')  &&  (c2 =='l'))
        return idx;

      if  ((c0 == 'A')  && (c1 == 'u')  &&  (c2 =='g'))
        return idx;

      if  (((c0 == 'S')  && (c1 == 'p')  &&  (c2 =='t'))  ||  ((c0 == 'S')  && (c1 == 'e')  &&  (c2 =='p')))
        return idx;

      
      if  ((c0 == 'O')  && (c1 == 'c')  &&  (c2 =='t'))
        return idx;

      if  ((c0 == 'N')  && (c1 == 'o')  &&  (c2 =='v'))
        return idx;

      if  ((c0 == 'D')  && (c1 == 'e')  &&  (c2 =='c'))
        return idx;
    }

    idx++;
  }

  return -1;

}  /* LocateAMonthNameStr */

