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
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

#include "PicesKKStr.h"
#include "OSservices.h"


using namespace  PicesInterface;


using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace KKU;
using namespace System::Windows::Forms;



OSservices::OSservices (void)
{
}



String^  OSservices::AddSlash (String^ s)
{
  if  (s->Length == 0)
    return  String::Empty;
      
  char lastCh = (char)s[s->Length - 1];
  if   ((lastCh == '/')  ||  (lastCh == '\\'))
    return  s;
        
  return  (s + "\\");
}   /* AddSlash */



bool  OSservices::DirectoryExists (String^  dirName)
{
  bool  directoryExists = osValidDirectory (PicesKKStr::SystemStringToKKStr (dirName));
  return  directoryExists;
}  /* DirectoryExists */




bool   OSservices::CreateDirectory (String^  dirName)
{
  bool  created = osCreateDirectory (PicesKKStr::SystemStringToKKStr (dirName));
  return  created;
}  /* CreateDirectory */



bool   OSservices::CreateDirectoryPath (String^  _pathName)  // Will create the whole Directory Path,
{                                                            // not just the final part of the path.

  return  osCreateDirectoryPath (PicesKKStr::SystemStringToKKStr (_pathName));
}




long  OSservices::HexStrToLong (String^  argument)
{
  unsigned __int64  uiHex = 0;

  try
  {
    // Convert hex string to unsigned integer
    uiHex = System::Convert::ToUInt64 (argument, 16);
    // Format it and show as a string				
  }
  catch (Exception^ exception)
  {
    // Show overflow message and return
    MessageBox::Show ("Overflow\n" + exception->ToString ());
  }

  return  (long)uiHex;
}  /* HexStrToLong */







void  OSservices::RenameFile (String^ oldFileName,
                              String^ newFileName
                             )
{
  // First delete new file if it exists; to get it out of the way
  try
  {
    FileInfo^ f = gcnew FileInfo (newFileName);
    if  (f->Exists)
    {
      f->Delete ();
    }
  }
  catch (Exception^ e1)
  {
    MessageBox::Show ("Error trying to rename '" + 
                      oldFileName + 
                      "'" +
                      "to '" + 
                      newFileName +
                      "'  " + 
                      e1->ToString ()
                     );
  }

  // We can now rename the oldFileName to the newFileName
  try
  {
    FileInfo^ f = gcnew FileInfo (oldFileName);
    if (f->Exists)
    {
      f->MoveTo (newFileName);
    }
  }
  catch (Exception^ e1)
  {
    MessageBox::Show ("Error trying to rename '" + oldFileName + "'" +
                      "to '" + newFileName +"'  " + e1->ToString ()
                     );
  }
}  /* RenameFile */



String^  OSservices::RemoveExtension (String^  fileName)
{
  return  PicesKKStr::KKStrToSystenStr (osRemoveExtension (PicesKKStr::SystemStringToKKStr (fileName)));
}  /* RemoveExtension */





String^ OSservices::GetRootName (String^ fileName)
{
  return  PicesKKStr::KKStrToSystenStr (osGetRootName (PicesKKStr::SystemStringToKKStr (fileName)));
}


String^   OSservices::GetRootNameWithExtension (String^  fullFileName)
{
  return  PicesKKStr::KKStrToSystenStr (osGetRootNameWithExtension (PicesKKStr::SystemStringToKKStr (fullFileName)));
}  /* GetRootNameWithExtension */



String^  OSservices::GetFileExtension (String^  fullFileName)
{
  return  PicesKKStr::KKStrToSystenStr (osGetFileExtension (PicesKKStr::SystemStringToKKStr (fullFileName)));
}




String^  OSservices::GetPathPartOfFile (String^  fullFileName)
{
  return PicesKKStr::KKStrToSystenStr (osGetPathPartOfFile (PicesKKStr::SystemStringToKKStr (fullFileName)));
}




String^   OSservices::GetParentDirectoryOfDirPath (String^ path)
{
  KKStr  s = PicesKKStr::SystemStringToKKStr (path);
  KKStr  parentDir = osGetParentDirectoryOfDirPath (s);
  return  PicesKKStr::KKStrToSystenStr (parentDir);
}




String^   OSservices::GetRootNameOfDirectory (String^ path)
{
  // Strating with last chracter in string searcgh back until we find a seperating
  // character such as a slash or ":".

  KKStr  s = PicesKKStr::SystemStringToKKStr (path);
  KKStr  rootName = osGetRootNameOfDirectory (s);
  return  PicesKKStr::KKStrToSystenStr (rootName);
}




KKU::DateTime  OSservices::SystemDateTimeToKKuDateTime (System::DateTime  sytemDateTime)
{
  return  KKU::DateTime   (sytemDateTime.Year,
                           sytemDateTime.Month,
                           sytemDateTime.Day,
                           sytemDateTime.Hour,
                           sytemDateTime.Minute,
                           sytemDateTime.Second
                          );
}



System::DateTime  OSservices::KKuDateTimeToSystemDateTime (const KKU::DateTime&  dateTime)
{
  const DateType&  d (dateTime.Date ());
  const TimeType&  t (dateTime.Time ());


  System::DateTime dt;
  if  (d.Year () < 1)
  {
    return System::DateTime (1900, 1, 1);
  }

  try 
  {
    dt = System::DateTime (d.Year (), d.Month (), d.Day (), t.Hour (), t.Minute (), t.Second ());
  }
  catch  (Exception^  )
  {
    dt = System::DateTime (1900, 1, 1);
  }

  return  dt;

  //if  ((d.Year () < 1980)  ||  (d.Month < 1)  ||  (d.Month > 12)  ||  (d.Day < 1)  || (d.Day > 31))
  //  return  System::DateTime (0, 1, 1);
  //
  //return  System::DateTime (d.Year (), d.Month (), d.Day (), t.Hour (), t.Minute (), t.Second ());
}


String^  OSservices::LookForFile (String^  fileName,
                                  String^  srcDir
                                 )
{
  if  ((fileName == nullptr)  ||  (srcDir == nullptr))
    return nullptr;

  KKStr  location = osLookForFile (PicesKKStr::SystemStringToKKStr (fileName), PicesKKStr::SystemStringToKKStr (srcDir));
  return  PicesKKStr::KKStrToSystenStr (location);
}  /* LookForFile */




array<String^>^   OSservices::GetListOfFiles (String^  dirPath,
                                              String^  fileSpec
                                             )
{
  DirectoryInfo^ di = nullptr;
  try
  {
    di = gcnew DirectoryInfo(dirPath);
  }
  catch (Exception ^)
  {
    di = nullptr;
  }

  if  (di == nullptr)
    return  nullptr;


  array<FileInfo^>^  files = di->GetFiles(fileSpec);
  if  (files == nullptr)
    return  nullptr;

  array<String^>^  results = gcnew array<String^> (files->Length);
  for  (int  x = 0; x < files->Length;  x++)
  {
    results[x] = files[x]->Name;
  }

  return  results;
}  /* GetListOfFiles */




