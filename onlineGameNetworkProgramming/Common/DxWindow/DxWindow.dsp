# Microsoft Developer Studio Project File - Name="DxWindow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DxWindow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DxWindow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DxWindow.mak" CFG="DxWindow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DxWindow - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DxWindow - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Common/DxWindow", FYFAAAAA"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "DxWindow - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../Library/DxWindow.lib"

!ELSEIF  "$(CFG)" == "DxWindow - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../Library/DxWindowd.lib"

!ENDIF 

# Begin Target

# Name "DxWindow - Win32 Release"
# Name "DxWindow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Windows\Controls\ButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\CheckBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxSound.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\DxWindowEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\EditBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\FontCatalog.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\FrameView.cpp
# End Source File
# Begin Source File

SOURCE=.\IME\InputMethodsEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\IWindowCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\ListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\ScrollBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Windows\Window.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Windows\Controls\ButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\CheckBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxSound.h
# End Source File
# Begin Source File

SOURCE=.\DirectX\DxSurface.h
# End Source File
# Begin Source File

SOURCE=.\DxWindowEntry.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\EditBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Windows\FontCatalog.h
# End Source File
# Begin Source File

SOURCE=.\Windows\FrameView.h
# End Source File
# Begin Source File

SOURCE=.\IME\InputMethodsEditor.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\IWindowCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\ListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Controls\ScrollBarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Windows\Window.h
# End Source File
# End Group
# End Target
# End Project
