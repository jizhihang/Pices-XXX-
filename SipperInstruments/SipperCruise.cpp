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


#include "SipperCruise.h"

using namespace SipperHardware;


SipperCruise::SipperCruise ():
	  cruiseName   (),
    shipName     (),
	  description  (),
    dateStart    (osGetLocalDateTime ().Date ()),
	  dateEnd      (osGetLocalDateTime ().Date ()),
	  location     (),
	  objective    (),
	  principal    (),
	  researchOrg  ()
{
}

SipperCruise::~SipperCruise ()
{
}



SipperCruiseList::SipperCruiseList (bool  _owner):
    KKQueue<SipperCruise> (_owner)

{
}

    
SipperCruiseList::~SipperCruiseList ()
{
}


// If there are any errors in '_cruiseName' will return description of error otherwise will return a empty string.
KKStr  SipperCruise::ValidateCruiseName (const KKStr&  _cruiseName)   
{
  if  (_cruiseName.Empty ())
    return  "Cruise Name can not be left blank!";

  if  (_cruiseName.Len () > 10)
    return  "Cruise Name can not be longer than 10 characters.";

  KKStr  invalidChars = "";
  for  (int32 x = 0;  x < _cruiseName.Len ();  x++)
  {
    char c = toupper (_cruiseName[x]);
    if  ((c >= 'A')  &&  (c <= 'Z'))  continue;
    if  ((c >= '0')  &&  (c <= '9'))  continue;
    if  (c == '_')                    continue;

    if  (invalidChars.Len () > 0)
      invalidChars << ", ";
    invalidChars.Append (c);
  }

  if  (invalidChars.Len () > 0)
    return  KKStr ("Invalid Characters[") + invalidChars + "]";

  return "";
}  /* ValidateCruiseName*/



class  SipperCruiseList::CruiseNameComparator
{
public:
  bool  operator() (SipperCruisePtr  left,  
                    SipperCruisePtr  right
                   )
  {
    return  (left->CruiseName ().CompareIgnoreCase (right->CruiseName ()) < 0);
  }
};  /* CruiseNameComparator */



void  SipperCruiseList::SortByCruiseName ()
{
  CruiseNameComparator  comparator;
  sort (begin (), end (), comparator);
}
