#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace KKU;

#include  "OSservices.h"
#include  "Cruise.h"

using namespace  PicesInterface;



Cruise::Cruise ():
    name        (String::Empty),
    location    (String::Empty),
    objective   (String::Empty),
    principal   (String::Empty),
    researchOrg (String::Empty),
    dateStart   (DateTime::Now),
    dateEnd     (DateTime::Now),
    notes       (String::Empty)
{
}


Cruise::Cruise  (String^  fileName):
    name        (String::Empty),
    location    (String::Empty),
    objective   (String::Empty),
    principal   (String::Empty),
    researchOrg (String::Empty),
    dateStart   (DateTime::Now),
    dateEnd     (DateTime::Now),
    notes       (String::Empty)
{
  try  {Load (fileName);}
  catch  (Exception^ e)
  {
    throw e;
  }
}




void  Cruise::Save (String^ baseDir)
{
  String^  cruiseDir = OSservices::AddSlash (baseDir)   + "DataFiles\\Cruises\\";
  String^  fileName =  OSservices::AddSlash (cruiseDir) + name;
  OSservices::CreateDirectoryPath (cruiseDir);

  StreamWriter^  sw = nullptr;
  try  {sw = gcnew StreamWriter (fileName, false);}
  catch  (Exception^ e)  
  {
    sw = nullptr;
    MessageBox::Show ("Error occured trying to create Cruise Description file[" + fileName + "]" + "\n\n" +
                      e->ToString (),
                      "Cruise.Save[" + fileName + "]",
                      MessageBoxButtons::OK
                     );
    return;
  }
      
  sw->WriteLine ("[CRUISE_DESCRIPTION]");
  sw->WriteLine ("Location    = "  + location);
  sw->WriteLine ("Objective   = "  + objective);
  sw->WriteLine ("Principal   = "  + principal);
  sw->WriteLine ("ResearchOrg = "  + researchOrg);
  sw->WriteLine ("DateStart   = "  + dateStart->ToString ("yyyy/mm/dd"));
  sw->WriteLine ("DateEnd     = "  + dateEnd->ToString ("yyyy/mm/dd"));

  array<String^>^  noteLines = notes->Split ('\n');
  for each  (String^ noteLine in noteLines)
  {
    sw->WriteLine ("Notes      = " + noteLine);
  }

  sw->Close ();
  delete  sw;
  sw = nullptr;
}  /* Save */



void  Cruise::Load (String^ fileName)
{
  StreamReader^  sr = nullptr;
      
  try  
  {
    sr = gcnew StreamReader (fileName);
  }  
  catch (Exception^ e) 
  {
    throw gcnew Exception ("Cruise.Load FileName[" + fileName + "]", e);
  }

  if  (sr == nullptr)
    throw gcnew Exception ("Cruise.Load  FileName[" + fileName +"]  Could not open file.");

    
  while  (!sr->EndOfStream)
  {
    String^ nextLine = sr->ReadLine ();
    array<String^>^ fields = nextLine->Split ('=');
    int x = fields->Length;
    if  (x > 1)
    {
      String^  fieldName   = ((String^)fields->GetValue (0))->Trim ()->ToUpper ();
      String^  fieldValue  = ((String^)fields->GetValue (1))->Trim ();
      ProcessField (fileName, fieldValue);
    }
  }
    
  sr->Close ();
  sr = nullptr;
}  /* Load */




Cruise::DateTime^  Cruise::StrToDateTime (String^  s)
{
  IFormatProvider^ format = gcnew System::Globalization::CultureInfo ("", true);
  DateTime^ dt = DateTime::ParseExact (s, "yyyy/mm/dd", format);
  delete  format;
  format = nullptr;
  return  dt;
}




void  Cruise::ProcessField (String^  fieldName, 
                            String^  fieldValue
                           )
{

  if  (fieldName == "LOCATION")
    location = fieldValue;

  else if  (fieldName == "Objective")
    objective   = fieldValue;

  else if  (fieldName == "PRINCIPAL")
    principal   = fieldValue;

  else if  (fieldName == "RESEARCGORG")
    researchOrg = fieldValue;

  else if  (fieldName == "DATESTART")
    dateStart   = StrToDateTime (fieldValue);
  
  else if  (fieldName == "DATEEND")
    dateEnd     = StrToDateTime (fieldValue);

  else if  (fieldName == "NOTES")
    notes += fieldValue + "\n";
}  /* ProcessField */
  
