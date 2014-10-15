#ifndef  _CLASSSUMMARY_
#define _CLASSSUMMARY_

#include  "BasicTypes.h"

#include  "MLClass.h"
#include  "LLoydsEntry.h"
#include  "RunLog.h"

using namespace  MLL;


class  ClassSummary
{
public:
  typedef  KKU::uchar   uchar;
  typedef  KKU::uint32    uint32;
  typedef  KKU::uint32  uint32;
  typedef  KKU::ushort  ushort;

  typedef  ClassSummary*   ClassSummaryPtr;

  ClassSummary ();

  ClassSummary (MLClassConstPtr  _mlClass,
                LLoydsEntryListPtr  _lloydsBins,
                float               _realDataU2Stat,
                float               _z_Score
               );

  ~ClassSummary ();

  const KKStr&               ClassName ()       const;

  const  MLClassConstPtr  MLClass ()  const  {return mlClass;}

  int32                      LLoydsBin (uint32 binNum) const;

  int32                      LLoydsBinSize (uint32 lloydsEntryIDX) const;

  LLoydsEntryPtr             LLoydsEntryByIndex (uint32 idx);

  LLoydsEntryPtr             LLoydsEntryByBinSize (int32 binSize);

  uint32                     NumOfLLoydsEntries ()  const;

  uint32                     NumOfLLoydsBins () const; // Returns the largest number of lloydsBins in 'lloydsEntries'


  float                      U2Stat ()  const  {return realDataU2Stat;}

  float                      ZScore ()  const  {return z_Score;}



private:
  MLClassConstPtr  mlClass;
  LLoydsEntryListPtr  lloydsEntries;

  float               realDataU2Stat;
  float               z_Score;
};


typedef  ClassSummary::ClassSummaryPtr  ClassSummaryPtr;



class  ClassSummaryList:  public  KKQueue<ClassSummary>
{
public:
  typedef  KKU::uchar   uchar;
  typedef  KKU::uint32    uint32;
  typedef  KKU::uint32  uint32;
  typedef  KKU::ushort  ushort;

  ClassSummaryList (RunLog&  _log);


  MLClassConstListPtr  GetListOfClasses ()  const;

  VectorInt  LLoydsBinSizes ()  const;

  void  SaveLLoydsBinsData (const KKStr&  fileName,
                            const KKStr&  srcDirName,
                            int32         lastScanLine,
                            int32           baseLLoydsBinSize
                           );


  void  SpatialOverlapReport (ostream&  o)  const;

  void  SummaryReport (ostream&  r)  const;


private:
  uint32  NumOfLLoydsBins ()  const;


  float SpatialOverlapIndex (int32              binSize,
                             ClassSummaryPtr  c1,
                             ClassSummaryPtr  c2
                            )  const;


  RunLog& log;
};


#endif
