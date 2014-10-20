#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;


//#include "MLClass.h"

//#include "PicesKKStr.h"


#include  "PicesClass.h"
#include  "PicesPrediction.h"

using namespace PicesInterface;


PicesPrediction::PicesPrediction  (PicesClass^ _mlClass,
                                   int         _votes,
                                   double      _probability
                                  ):
      mlClass  (_mlClass),
      probability (_probability),
      votes       (_votes)
{}



String^  PicesPrediction::ClassName::get ()  
{
  if  (mlClass == nullptr)
    return String::Empty;
  else
    return  mlClass->Name;
}
    

void  PicesPrediction::Refresh (PicesPrediction^  p)
{
  mlClass  = p->MLClass;
  probability = p->Probability;
  votes       = p->Votes;
}
