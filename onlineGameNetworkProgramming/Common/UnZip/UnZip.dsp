# Microsoft Developer Studio Project File - Name="UnZip" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=UnZip - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UnZip.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UnZip.mak" CFG="UnZip - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UnZip - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "UnZip - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Common/UnZip", TYAAAAAA"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "UnZip - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "UnZip - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "ZLIB_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\UnZipd.lib"

!ENDIF 

# Begin Target

# Name "UnZip - Win32 Release"
# Name "UnZip - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Unzip.cpp
# End Source File
# Begin Source File

SOURCE=.\UnzipFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipAutoBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipBigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipCentralDir.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipException.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipFileHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipInternalInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\ZUBaseFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnZip.h
# End Source File
# Begin Source File

SOURCE=.\UnzipFile.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\ZipArchive.h
# End Source File
# Begin Source File

SOURCE=.\ZipAutoBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ZipBigFile.h
# End Source File
# Begin Source File

SOURCE=.\ZipCentralDir.h
# End Source File
# Begin Source File

SOURCE=.\ZipException.h
# End Source File
# Begin Source File

SOURCE=.\ZipFile.h
# End Source File
# Begin Source File

SOURCE=.\ZipFileHeader.h
# End Source File
# Begin Source File

SOURCE=.\ZipInternalInfo.h
# End Source File
# Begin Source File

SOURCE=.\ZipStorage.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# Begin Source File

SOURCE=.\ZUBaseFile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# Begin Source File

SOURCE=.\zlib.lib
# End Source File
# End Target
# End Project
