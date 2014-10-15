#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <iostream>
#include  <string>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include  "SortOrderType.h"




const char*  sortDescriptions[] =
   {
     "NULL_SortOrder",
     "MixedRandomProbability",
     "ProbabilityOrder",
     "BreakTieOrder",
     "ProbWithLittleRandom",
     "RandomOrder",
     NULL
   };


const char*  sortDescriptionsShort[] =
   {
     "NULL_SortOrder",
     "Mixed",
     "LC",
     "BT",
     "ProbLittleRand",
     "Random",
     NULL
   };



KKStr   SortOrderDescr (SortOrderType  sortMethod)
{
  if  ((sortMethod < 0)  ||  (sortMethod > 5))
    return  KKStr ();

  return  KKStr (sortDescriptions[sortMethod]);
}


KKStr   SortOrderDescrShort (SortOrderType  sortMethod)
{
  if  ((sortMethod < 0)  ||  (sortMethod > 5))
    return  KKStr ();

  return  KKStr (sortDescriptionsShort[sortMethod]);
}




SortOrderType   SortOrderTypeFromStr (KKStr  sortMethodStr)
{
  int  i = 0;


  SortOrderType sortOrder = NULL_SortOrder;

  while  ((sortDescriptions[i] != NULL)  &&  (sortOrder == NULL_SortOrder))
  {
    if  (KKStr::StrEqualNoCase (sortMethodStr.Str (), sortDescriptions[i]))
      sortOrder = (SortOrderType)i;

    i++;
  }

  return  sortOrder;
}
