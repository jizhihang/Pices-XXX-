#ifndef  _PICESINTERFACE_OSSERVICES_
#define  _PICESINTERFACE_OSSERVICES_

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

//using namespace PicesCSharpClasses;


#include  "..\\BaseLibrary\\OSservices.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  /**
   @brief The Managed version of the OSServices methods defined in BaseLibrary.
   @details
    This class is the managed equivelent to the functions defined in OSservices 
    in BaseLibrary.  In some cases we implement the code in the unmanaged world 
    in others we encapsulate the Unmanaged call.
   */
  public ref class  OSservices
  {
  public:
    OSservices ();

    static String^   AddSlash (String^ s);

    static bool      CreateDirectory (String^  dirName);

    static bool      CreateDirectoryPath (String^  _pathName);  /**< Will create the whole Directory Path, not just the final part of the path. */

    static bool      DirectoryExists (String^  dirName);

    static String^   GetFileExtension (String^  fullFileName);

    static  array<String^>^   GetListOfFiles (String^  dirPath,
                                              String^  fileSpec
                                             );

    static String^   GetPathPartOfFile (String^  fullFileName);

    static String^   GetParentDirectoryOfDirPath (String^  path);

    static String^   GetRootNameOfDirectory (String^ path);

    static long      HexStrToLong (String^ argument);

    /**
     *@brief  Look for a specified file in a subdirectory structure.
     *@param[in]  fileName  Name of file we are looking for.
     *@param[in]  srcDir    Sub Directory tree we want to search.
     *@return  Full directory path to where first occurance of fileName is located.  If not 
     *         found will return back nullptr.
     */
    static String^   LookForFile (String^  fileName,
                                  String^  srcDir
                                 );

    static void      RenameFile (String^ oldFileName,
                                 String^ newFileName
                                );

    static String^   RemoveExtension (String^  fileName);

    static String^   GetRootName (String^ fileName);

    static String^   GetRootNameWithExtension (String^  fullFileName);


    static KKU::DateTime     SystemDateTimeToKKuDateTime (System::DateTime  sytemDateTime);

    static System::DateTime  KKuDateTimeToSystemDateTime (const KKU::DateTime&  dateTime);

  };  /* OSservices */

}
#endif
