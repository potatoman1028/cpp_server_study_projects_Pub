# Microsoft Developer Studio Project File - Name="SyncLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SyncLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SyncLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SyncLib.mak" CFG="SyncLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SyncLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SyncLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Library/SyncLib", VBDAAAAA"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "SyncLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SyncLib___Win32_Release"
# PROP BASE Intermediate_Dir "SyncLib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"Library\Syncu.lib"
# ADD LIB32 /nologo /out:"Library\Sync.lib"

!ELSEIF  "$(CFG)" == "SyncLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SyncLib___Win32_Debug"
# PROP BASE Intermediate_Dir "SyncLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"Library\Syncud.lib"
# ADD LIB32 /nologo /out:"Library\Syncd.lib"

!ENDIF 

# Begin Target

# Name "SyncLib - Win32 Release"
# Name "SyncLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cpp\pattern\Decorator.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\net\InetAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\util\List.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\net\Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\net\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\pattern\Singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\util\Stack.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\String.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Synchronized.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\ThreadManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cpp\pattern\Decorator.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Exception.h
# End Source File
# Begin Source File

SOURCE=.\cpp\net\InetAddress.h
# End Source File
# Begin Source File

SOURCE=.\cpp\util\List.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Object.h
# End Source File
# Begin Source File

SOURCE=.\cpp\net\Packet.h
# End Source File
# Begin Source File

SOURCE=.\cpp\net\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\cpp\pattern\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\cpp\util\Stack.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\String.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Synchronized.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\Thread.h
# End Source File
# Begin Source File

SOURCE=.\cpp\lang\ThreadManager.h
# End Source File
# End Group
# End Target
# End Project
