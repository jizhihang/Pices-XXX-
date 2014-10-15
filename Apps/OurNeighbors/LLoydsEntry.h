#ifndef  _LLOYDSENTRY_
#define _LLOYDSENTRY_

#include  "BasicTypes.h"
using namespace KKU;

#include  "MLClass.h"
using namespace  MLL;


class  LLoydsEntry 
{
public:
  typedef  KKU::uchar   uchar;
  typedef  KKU::uint32    uint32;
  typedef  KKU::uint32  uint32;
  typedef  KKU::ushort  ushort;

  LLoydsEntry  (int32           _lloydsBinSize,
                VectorIntPtr  _lloydsBins,
                double        _lloydsIndex
               );

  ~LLoydsEntry ();

  int32     LLoydsBin (size_t idx) const;

  int32     LLoydsBinSize  ()  const  {return lloydsBinSize;}

  double  LLoydsIndex    ()  const  {return lloydsIndex;}

  uint32    NumOfLLoydsBins () const  {return  ((lloydsBins == NULL) ? 0 : (uint32)lloydsBins->size ());}

private:
  int32            lloydsBinSize;
  double         lloydsIndex;
  double         lloydsBinsMean;
  VectorIntPtr   lloydsBins;
};

typedef  LLoydsEntry*  LLoydsEntryPtr;



class  LLoydsEntryList: public  KKQueue<LLoydsEntry>
{
public:
  LLoydsEntryList (RunLog&  _log);

  LLoydsEntryPtr  LLoydsEntryByBinSize (int32 binSize)  const;

  void  PushOnBack (LLoydsEntryPtr  _lloydsEntry);


private:
  typedef  map<int32, LLoydsEntryPtr>  LLoydsBinEntryMap;
  typedef  pair<int32, LLoydsEntryPtr> LLoydsBinEntryPair;



  LLoydsBinEntryMap  binEntryMap;

  RunLog&  log;
};



typedef  LLoydsEntryList*  LLoydsEntryListPtr;


#endif

