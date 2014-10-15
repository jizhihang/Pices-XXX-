#if !defined(_CLASSSTATISTIC_)

#include "BasicTypes.h"
#include "Str.h"

#include "MLClass.h"


namespace MLL
{
  /**
   *@brief Used by routines that retrieve Class statistics from FeatureVectorList instances.
   */
  class  ClassStatistic 
  {
  public:
    typedef  KKU::uint32  uint32;

    ClassStatistic (const ClassStatistic&  right);

    ClassStatistic (MLClassConstPtr  _mlClass,
                    uint32              _count
                   );

    uint32              Count      ()  const {return  count;}
    MLClassConstPtr  MLClass ()  const {return  mlClass;}

    void                Increment  ()         {count++;}

    const KKStr&        Name       ()  const;

    const  ClassStatistic&  operator+= (const ClassStatistic&  right);

  private:
    MLClassConstPtr  mlClass;   /**< Does not own the mlClass object.                 */

    uint32              count;        /**< Represents the number of imageFeatures objects in a
                                       * a FeatureVectorList that point to mageClass
                                       */
  };

  typedef  ClassStatistic*  ClassStatisticPtr;


  class  ClassStatisticList:  public KKQueue<ClassStatistic> 
  {
  public:
    ClassStatisticList (bool  _owner);

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    ClassStatisticPtr  LookUpByImageClass (MLClassConstPtr  mlClass)  const;

    void  SortByClassName  ();
    void  SortByCount  ();

    void  PushOnBack  (ClassStatisticPtr  stat);
    void  PushOnFront (ClassStatisticPtr  stat);

    void  PrintReport (ostream& r);

    int32 operator[]  (MLClassConstPtr  mlClass);
    const  ClassStatisticList&  operator+= (const ClassStatisticList&  right);

  private:
    class  ClassStatisticSortComparrison;

    std::map<MLClassConstPtr, ClassStatisticPtr>  imageClassIndex;

    class  ClassStatSortByCount;
  };  /* ClassStatisticList */

  typedef  ClassStatisticList*  ClassStatisticListPtr;


}  /* namespace MLL */

#define  _CLASSSTATISTIC_

#endif

