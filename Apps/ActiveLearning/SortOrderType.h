#ifndef  _SORTORDERTYPE_
#define  _SORTORDERTYPE_


#include  "Str.h"

typedef enum  
   {
     NULL_SortOrder,
     DistanceToBorderOrder,
     MixedRandomProbability,
     ProbabilityOrder,
     BreakTieOrder,
     ProbabilityLittleRandom,
     RandomOrder
   }  
   SortOrderType;



KKStr   SortOrderDescr (SortOrderType  sortMethod);

KKStr   SortOrderDescrShort (SortOrderType  sortMethod);


SortOrderType   SortOrderTypeFromStr (KKStr  sortMethodStr);

#endif
