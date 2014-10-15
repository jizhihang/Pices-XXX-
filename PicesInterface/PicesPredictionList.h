#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


#include  "PicesPrediction.h"

namespace  MLL
{
  #if  !defined(_CLASSPROB_)
  class  ClassProb;
  typedef  ClassProb*  ClassProbPtr;
  class  ClassProbList;
  typedef  ClassProbList*  ClassProbListPtr;
  #endif
}



namespace PicesInterface
{
  public ref  class PicesPredictionList:  List<PicesPrediction^>
  {
  public:
    PicesPredictionList ();

    PicesPredictionList (int  size);

    PicesPredictionList (MLL::ClassProbList&  predictions);


    void  SortByProbabilityHighToLow ();
    void  SortByVotingHighToLow ();

  private:
    ref class SortPredictionsByProbability;
    ref class SortPredictionsByVotes;
  };
}