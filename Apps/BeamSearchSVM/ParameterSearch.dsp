# Microsoft Developer Studio Project File - Name="ParameterSearch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ParameterSearch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ParameterSearch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ParameterSearch.mak" CFG="ParameterSearch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ParameterSearch - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ParameterSearch - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ParameterSearch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\Library" /I "..\OpenCV" /I "..\fftw" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fftw_release.lib cv.lib highgui.lib /nologo /subsystem:console /machine:I386 /libpath:"..\OpenCV" /libpath:"..\fftw"

!ELSEIF  "$(CFG)" == "ParameterSearch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\Library" /I "..\OpenCV" /I "..\fftw" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fftw_release.lib cv.lib highgui.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\OpenCV" /libpath:"..\fftw"

!ENDIF 

# Begin Target

# Name "ParameterSearch - Win32 Release"
# Name "ParameterSearch - Win32 Debug"
# Begin Source File

SOURCE=..\..\Library\BinaryClassParms.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\BMPImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ConfussionMatrix2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ContourFollower.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ConvexHull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\DateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\FeatureNumList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Histogram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ImageClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ImageFeatures.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ImageIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\NormalizationParms.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\OSservices.cpp
# End Source File
# Begin Source File

SOURCE=.\ParameterJob.cpp
# End Source File
# Begin Source File

SOURCE=.\ParameterSearch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Raster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\RunLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Str.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\svm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\SVMModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\SVMparam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\SvmWrapper.cpp
# End Source File
# End Target
# End Project
