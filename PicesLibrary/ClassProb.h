#if  !defined(_CLASSPROB_)
#define  _CLASSPROB_
/**
 *@class  MLL::ClassProb
 *@brief Used to record probability for a specified class;  and a list of classes.
 *@author  Kurt Kramer
 */

#include "BasicTypes.h"
#include "Str.h"


namespace MLL
{
  #if  !defined(_MLCLASS_)
  class  MLClass;
  typedef  MLClass*         MLClassPtr;
  typedef  MLClass const *  MLClassConstPtr;
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif


  class  ClassProb
  {
  public:
    ClassProb (MLClassConstPtr _classLabel,
               double             _probability,
               float              _votes
              );

    ClassProb (const ClassProb&  _pair);

    MLClassConstPtr  classLabel;
    double              probability;
    float               votes;
  };
  typedef  ClassProb*  ClassProbPtr;



  class  ClassProbList:  public  KKQueue<ClassProb>
  {
  public:
    ClassProbList ();
    ClassProbList (bool owner);
    ClassProbList (const ClassProbList&  pairList);
    void  SortByClassName ();
    void  SortByProbability (bool highToLow = true);
    void  SortByVotes       (bool highToLow = true);


    const ClassProbPtr  LookUp (MLClassConstPtr  targetImageClass)  const;

  private:
    static bool  CompairByClassName (const ClassProbPtr left, const ClassProbPtr right);

    class  ProbabilityComparer;
    class  VotesComparer;
  };
  typedef  ClassProbList*  ClassProbListPtr;

}  /* namespace MML */

#endif
