#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

#include "MemoryDebug.h"


#include "BasicTypes.h"
#include "Str.h"
using namespace KKU;

#include  "GradeClassification.h"



int main( int argc, char **argv )
{
  GradeClassification gc (argc, argv);
  if ( gc.Abort() )
  {
    return (-1);
  }

  gc.Grade ();

  return (0);
}
