# Microsoft Developer Studio Project File - Name="TMeter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TMeter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TMeter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TMeter.mak" CFG="TMeter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TMeter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TMeter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TMeter - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TMeter - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dvrsdk.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TMeter - Win32 Release"
# Name "TMeter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_cell.cpp
# End Source File
# Begin Source File

SOURCE=.\_column.cpp
# End Source File
# Begin Source File

SOURCE=.\_combobox.cpp
# End Source File
# Begin Source File

SOURCE=.\_grid.cpp
# End Source File
# Begin Source File

SOURCE=.\_pagesetup.cpp
# End Source File
# Begin Source File

SOURCE=.\_range.cpp
# End Source File
# Begin Source File

SOURCE=.\AdjustZoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Btnst.cpp
# End Source File
# Begin Source File

SOURCE=.\CalibChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassicalElement.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ComWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ConditionMonitorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfOneStdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CRC32.CPP
# End Source File
# Begin Source File

SOURCE=.\CreateDefmaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateWorkZoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineAppTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineOneLayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineProductTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineUnitTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DefMa.cpp
# End Source File
# Begin Source File

SOURCE=.\DefMADataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DefMAListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EnergyCalibDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\FpCalculator.cpp
# End Source File
# Begin Source File

SOURCE=.\FpMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\Inplaceedit.cpp
# End Source File
# Begin Source File

SOURCE=.\IntensityCalculator.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiCalibDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiCalibView.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiMeasureDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiMeasureView.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiSpectrumDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MdiSpectrumView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTextButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\OneLayerDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\OneResult.cpp
# End Source File
# Begin Source File

SOURCE=.\OneTask.cpp
# End Source File
# Begin Source File

SOURCE=.\PeriodicTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCollimatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\SetCrossParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetMeasHeadParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetXyzParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpectrumAnalyzeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpectrumChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpectrumDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StbTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdSample.cpp
# End Source File
# Begin Source File

SOURCE=.\StdSampleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeTickDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\Titletip.cpp
# End Source File
# Begin Source File

SOURCE=.\TMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\TMeter.rc
# End Source File
# Begin Source File

SOURCE=.\TMeterDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TMeterLog.cpp
# End Source File
# Begin Source File

SOURCE=.\TMeterString.cpp
# End Source File
# Begin Source File

SOURCE=.\TMeterView.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkZoneDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkZoneDef.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkZoneListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XrfInstrument.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_cell.h
# End Source File
# Begin Source File

SOURCE=.\_column.h
# End Source File
# Begin Source File

SOURCE=.\_combobox.h
# End Source File
# Begin Source File

SOURCE=.\_grid.h
# End Source File
# Begin Source File

SOURCE=.\_pagesetup.h
# End Source File
# Begin Source File

SOURCE=.\_range.h
# End Source File
# Begin Source File

SOURCE=.\AdjustZoneDlg.h
# End Source File
# Begin Source File

SOURCE=.\Btnst.h
# End Source File
# Begin Source File

SOURCE=.\CalibChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\CELLRANGE.H
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClassicalElement.h
# End Source File
# Begin Source File

SOURCE=.\ComWnd.h
# End Source File
# Begin Source File

SOURCE=.\ConditionMonitorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfOneStdDlg.h
# End Source File
# Begin Source File

SOURCE=.\CRC32.H
# End Source File
# Begin Source File

SOURCE=.\CreateDefmaDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateWorkZoneDlg.h
# End Source File
# Begin Source File

SOURCE=.\DefineAppTab.h
# End Source File
# Begin Source File

SOURCE=.\DefineOneLayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\DefineProductTab.h
# End Source File
# Begin Source File

SOURCE=.\DefineUnitTab.h
# End Source File
# Begin Source File

SOURCE=.\DefMa.h
# End Source File
# Begin Source File

SOURCE=.\DefMADataSet.h
# End Source File
# Begin Source File

SOURCE=.\DefMAListDlg.h
# End Source File
# Begin Source File

SOURCE=.\EnergyCalibDlg.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\FpCalculator.h
# End Source File
# Begin Source File

SOURCE=.\FpMethod.h
# End Source File
# Begin Source File

SOURCE=.\GRIDDROPTARGET.H
# End Source File
# Begin Source File

SOURCE=.\INPLACEEDIT.H
# End Source File
# Begin Source File

SOURCE=.\IntensityCalculator.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MATRIX.H
# End Source File
# Begin Source File

SOURCE=.\MdiCalibDoc.h
# End Source File
# Begin Source File

SOURCE=.\MdiCalibView.h
# End Source File
# Begin Source File

SOURCE=.\MdiMeasureDoc.h
# End Source File
# Begin Source File

SOURCE=.\MdiMeasureView.h
# End Source File
# Begin Source File

SOURCE=.\MdiSpectrumDoc.h
# End Source File
# Begin Source File

SOURCE=.\MdiSpectrumView.h
# End Source File
# Begin Source File

SOURCE=.\Memdc.h
# End Source File
# Begin Source File

SOURCE=.\MyTextButton.h
# End Source File
# Begin Source File

SOURCE=.\NewAxis.h
# End Source File
# Begin Source File

SOURCE=.\OneLayerDefine.h
# End Source File
# Begin Source File

SOURCE=.\OneResult.h
# End Source File
# Begin Source File

SOURCE=.\OneTask.h
# End Source File
# Begin Source File

SOURCE=.\PeriodicTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelCollimatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\SetCrossParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetMeasHeadParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetXyzParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpectrumAnalyzeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpectrumChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpectrumDisplay.h
# End Source File
# Begin Source File

SOURCE=.\SpectrumInfo.h
# End Source File
# Begin Source File

SOURCE=.\StaticChildWnd.h
# End Source File
# Begin Source File

SOURCE=.\StbTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdSample.h
# End Source File
# Begin Source File

SOURCE=.\StdSampleDlg.h
# End Source File
# Begin Source File

SOURCE=.\TimeTickDlg.h
# End Source File
# Begin Source File

SOURCE=.\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\TITLETIP.H
# End Source File
# Begin Source File

SOURCE=.\TMeter.h
# End Source File
# Begin Source File

SOURCE=.\TMeterDoc.h
# End Source File
# Begin Source File

SOURCE=.\TMeterLog.h
# End Source File
# Begin Source File

SOURCE=.\TMeterString.h
# End Source File
# Begin Source File

SOURCE=.\TMeterTypeDefine.h
# End Source File
# Begin Source File

SOURCE=.\TMeterView.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# Begin Source File

SOURCE=.\WorkZoneDataSet.h
# End Source File
# Begin Source File

SOURCE=.\WorkZoneDef.h
# End Source File
# Begin Source File

SOURCE=.\WorkZoneListDlg.h
# End Source File
# Begin Source File

SOURCE=.\XrfInstrument.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\TMeter.ico
# End Source File
# Begin Source File

SOURCE=.\res\TMeter.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TMeterDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section TMeter : {188192C8-98CC-4BA9-B468-D63F0D75C0D9}
# 	2:5:Class:C_ComboBox
# 	2:10:HeaderFile:_combobox.h
# 	2:8:ImplFile:_combobox.cpp
# End Section
# Section TMeter : {87088D20-19B3-4CEF-8DCF-528BEE37DE28}
# 	2:5:Class:C_PageBreak
# 	2:10:HeaderFile:_pagebreak.h
# 	2:8:ImplFile:_pagebreak.cpp
# End Section
# Section TMeter : {256DC007-642C-4147-A608-42B7B922E913}
# 	2:5:Class:C_ReportTitle
# 	2:10:HeaderFile:_reporttitle.h
# 	2:8:ImplFile:_reporttitle.cpp
# End Section
# Section TMeter : {B78D1D71-7EE9-4910-BA78-CAC70A095B33}
# 	2:5:Class:C_HPageBreaks
# 	2:10:HeaderFile:_hpagebreaks.h
# 	2:8:ImplFile:_hpagebreaks.cpp
# End Section
# Section TMeter : {E5BD4EDE-C4FE-4FA9-91F1-1606C5E9BA06}
# 	2:5:Class:C_Image
# 	2:10:HeaderFile:_image.h
# 	2:8:ImplFile:_image.cpp
# End Section
# Section TMeter : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section TMeter : {4FFEC2FE-8694-4E25-9A9D-B931787BD1A0}
# 	2:5:Class:C_Cell
# 	2:10:HeaderFile:_cell.h
# 	2:8:ImplFile:_cell.cpp
# End Section
# Section TMeter : {719911F9-0E2D-453B-8EAF-606CE0ECC374}
# 	2:5:Class:C_PageSetup
# 	2:10:HeaderFile:_pagesetup.h
# 	2:8:ImplFile:_pagesetup.cpp
# End Section
# Section TMeter : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section TMeter : {B72C55ED-9AB0-433E-99D1-B66A93AE2F07}
# 	2:21:DefaultSinkHeaderFile:_grid.h
# 	2:16:DefaultSinkClass:C_Grid
# End Section
# Section TMeter : {21792826-A6AA-4C90-A9E6-5AF9C49E9CE8}
# 	2:5:Class:C_Column
# 	2:10:HeaderFile:_column.h
# 	2:8:ImplFile:_column.cpp
# End Section
# Section TMeter : {14076117-2CB6-4823-BE5D-7505FC383A7C}
# 	2:5:Class:C_Images
# 	2:10:HeaderFile:_images.h
# 	2:8:ImplFile:_images.cpp
# End Section
# Section TMeter : {99EDD5F9-314D-4E54-A200-8C0ADAA5F59F}
# 	2:5:Class:C_Selection
# 	2:10:HeaderFile:_selection.h
# 	2:8:ImplFile:_selection.cpp
# End Section
# Section TMeter : {DB192B8D-73C1-48D5-8AD6-82C6F35E305D}
# 	2:5:Class:C_ReportTitles
# 	2:10:HeaderFile:_reporttitles.h
# 	2:8:ImplFile:_reporttitles.cpp
# End Section
# Section TMeter : {8F4F884B-6349-4C8A-B47C-DE01A0A87F5E}
# 	2:5:Class:C_Range
# 	2:10:HeaderFile:_range.h
# 	2:8:ImplFile:_range.cpp
# End Section
# Section TMeter : {45433E60-9E23-430F-9A33-0F6B61D99A4E}
# 	2:5:Class:C_VPageBreaks
# 	2:10:HeaderFile:_vpagebreaks.h
# 	2:8:ImplFile:_vpagebreaks.cpp
# End Section
# Section TMeter : {C2AB17C0-7B38-48DE-8CAE-742E053FCB54}
# 	2:5:Class:C_Grid
# 	2:10:HeaderFile:_grid.h
# 	2:8:ImplFile:_grid.cpp
# End Section
# Section TMeter : {D3DC5886-0030-4ACC-9423-772E9FDBF7E2}
# 	2:5:Class:C_Chart
# 	2:10:HeaderFile:_chart.h
# 	2:8:ImplFile:_chart.cpp
# End Section
# Section TMeter : {160623C6-29E2-4734-A3C1-4B4C3DE3DCE6}
# 	2:5:Class:C_Calendar
# 	2:10:HeaderFile:_calendar.h
# 	2:8:ImplFile:_calendar.cpp
# End Section
