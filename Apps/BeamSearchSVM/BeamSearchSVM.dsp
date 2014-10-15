# Microsoft Developer Studio Project File - Name="BeamSearchSVM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=BeamSearchSVM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BeamSearchSVM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BeamSearchSVM.mak" CFG="BeamSearchSVM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BeamSearchSVM - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "BeamSearchSVM - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BeamSearchSVM - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Zi /Od /I "c:\Program Files\OpenCV\cv\include" /I "C:\Program Files\OpenCV\otherlibs\highgui" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fftw.lib cv.lib highgui.lib /nologo /subsystem:console /machine:I386 /libpath:"C:\Program Files\OpenCV\lib"
# SUBTRACT LINK32 /map /debug

!ELSEIF  "$(CFG)" == "BeamSearchSVM - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\Library" /I "..\fftw" /I "c:\Program Files\OpenCV\cv\include" /I "C:\Program Files\OpenCV\otherlibs\highgui" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "KKDEBUG" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fftw_debug.lib cv.lib highgui.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\fftw" /libpath:"C:\Program Files\OpenCV\lib"

!ENDIF 

# Begin Target

# Name "BeamSearchSVM - Win32 Release"
# Name "BeamSearchSVM - Win32 Debug"
# Begin Source File

SOURCE=.\BeamSearchSVM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Blob.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\BMPImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLine.txt
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

SOURCE=..\..\Library\ImageClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\ImageFeatures.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\NormalizationParms.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\OSservices.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\Raster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Library\RunLog.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleRun.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleRunList.cpp
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
# Begin Source File

SOURCE=..\FeatureCalculation\fftw\Release\fftw.lib
# End Source File
# End Target
# End Project
