#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>

#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace System;
using namespace KKU;

#include  "OSservices.h"
#include  "Cruise.h"
#include  "CruiseList.h"

using namespace  PicesInterface;


CruiseList::CruiseList (String^  baseDir)
{
}


Cruise^  CruiseList::LookUpByName (String^  name)
{
  String^  nameLower = name->ToLower ();
  for each  (Cruise^ cruise in (*this))
  {
    if  (cruise->Name->ToLower () == nameLower)
       return  cruise;
  }

  return  nullptr;
}  /* LookUpByName */





void  CruiseList::Load (String^ baseDir)
{
  String^  dirName = OSservices::AddSlash (baseDir) + "DataFiles\\Cruises\\";

  
  DirectoryInfo^  di = nullptr;
  
  try  {di = gcnew DirectoryInfo (dirName);}  catch  (Exception^) {di = nullptr;}
  if  (di == nullptr)
     return;

  array<FileInfo^>^  files = di->GetFiles (".txt");
 
  if  (files == nullptr)
     return;

  for each  (FileInfo^ fi in files)
  {
    Cruise^  cruise = nullptr;
    String^  fileName = dirName + fi->Name;
        
    try {cruise = gcnew Cruise (fileName);}  catch (Exception^) {cruise = nullptr;}
    if  (cruise != nullptr)
       Add (cruise);
  }
}  /* Load */
