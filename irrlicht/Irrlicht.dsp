# Microsoft Developer Studio Project File - Name="Irrlicht" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Irrlicht - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "Irrlicht.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Irrlicht.mak" CFG="Irrlicht - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "Irrlicht - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Irrlicht - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Irrlicht - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IRRLICHT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IRRLICHT_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Irrlicht - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IRRLICHT_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IRRLICHT_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Irrlicht - Win32 Release"
# Name "Irrlicht - Win32 Debug"
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\Color.h
# End Source File
# Begin Source File

SOURCE=.\include\EDriverTypes.h
# End Source File
# Begin Source File

SOURCE=.\include\ITexture.h
# End Source File
# Begin Source File

SOURCE=.\include\IVideoDriver.h
# End Source File
# Begin Source File

SOURCE=.\include\S3DVertex.h
# End Source File
# Begin Source File

SOURCE=.\include\SLight.h
# End Source File
# Begin Source File

SOURCE=.\include\SMaterial.h
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\aabbox3d.h
# End Source File
# Begin Source File

SOURCE=.\include\array.h
# End Source File
# Begin Source File

SOURCE=.\include\dimension2d.h
# End Source File
# Begin Source File

SOURCE=.\include\heapsort.h
# End Source File
# Begin Source File

SOURCE=.\include\irrmath.h
# End Source File
# Begin Source File

SOURCE=.\include\irrstring.h
# End Source File
# Begin Source File

SOURCE=.\include\line3d.h
# End Source File
# Begin Source File

SOURCE=.\include\list.h
# End Source File
# Begin Source File

SOURCE=.\include\matrix4.h
# End Source File
# Begin Source File

SOURCE=.\include\plane3d.h
# End Source File
# Begin Source File

SOURCE=.\include\plane3dex.h
# End Source File
# Begin Source File

SOURCE=.\include\position2d.h
# End Source File
# Begin Source File

SOURCE=.\include\rect.h
# End Source File
# Begin Source File

SOURCE=.\include\vector2d.h
# End Source File
# Begin Source File

SOURCE=.\include\vector3d.h
# End Source File
# End Group
# Begin Group "io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\IFileList.h
# End Source File
# Begin Source File

SOURCE=.\include\IFileSystem.h
# End Source File
# Begin Source File

SOURCE=.\include\IReadFile.H
# End Source File
# End Group
# Begin Group "scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\IAnimatedMesh.h
# End Source File
# Begin Source File

SOURCE=.\include\IAnimatedMeshSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\IBillboardSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\IBspTreeSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\ICameraSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\ILightSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\IMesh.h
# End Source File
# Begin Source File

SOURCE=.\include\IMeshBuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\IQ3LevelMesh.h
# End Source File
# Begin Source File

SOURCE=.\include\ISceneManager.h
# End Source File
# Begin Source File

SOURCE=.\include\ISceneNode.h
# End Source File
# Begin Source File

SOURCE=.\include\ISceneNodeAnimator.h
# End Source File
# Begin Source File

SOURCE=.\include\SMesh.h
# End Source File
# Begin Source File

SOURCE=.\include\SMeshBuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\SMeshBufferLightMap.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\IGUIButton.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUICheckbox.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIElement.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIFileOpenDialog.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIFont.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIImage.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIListBox.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIMeshViewer.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUISkin.h
# End Source File
# Begin Source File

SOURCE=.\include\IGUIWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\include\IEventReceiver.h
# End Source File
# Begin Source File

SOURCE=.\include\Irrlicht.h
# End Source File
# Begin Source File

SOURCE=.\include\IrrlichtDevice.h
# End Source File
# Begin Source File

SOURCE=.\include\irrTypes.h
# End Source File
# Begin Source File

SOURCE=.\include\IUnknown.h
# End Source File
# Begin Source File

SOURCE=.\include\Keycodes.h
# End Source File
# End Group
# Begin Group "gui impl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BuildInFont.h
# End Source File
# Begin Source File

SOURCE=.\CGUIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIButton.h
# End Source File
# Begin Source File

SOURCE=.\CGUICheckbox.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUICheckbox.h
# End Source File
# Begin Source File

SOURCE=.\CGUIEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\CGUIFileOpenDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIFileOpenDialog.h
# End Source File
# Begin Source File

SOURCE=.\CGUIFont.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIFont.h
# End Source File
# Begin Source File

SOURCE=.\CGUIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIImage.H
# End Source File
# Begin Source File

SOURCE=.\CGUIListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIListBox.h
# End Source File
# Begin Source File

SOURCE=.\CGUIMeshViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIMeshViewer.h
# End Source File
# Begin Source File

SOURCE=.\CGUIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\CGUISkin.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUISkin.h
# End Source File
# Begin Source File

SOURCE=.\CGUIStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIStaticText.h
# End Source File
# Begin Source File

SOURCE=.\CGUIWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CGUIWindow.h
# End Source File
# Begin Source File

SOURCE=.\GUIIcons.h
# End Source File
# End Group
# Begin Group "video impl"

# PROP Default_Filter ""
# Begin Group "Software"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSoftwareTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\CSoftwareTexture.h
# End Source File
# Begin Source File

SOURCE=.\CTRFlat.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRFlatWire.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRGouraud.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRGouraudWire.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRTextureFlat.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRTextureFlatWire.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRTextureGouraud.cpp
# End Source File
# Begin Source File

SOURCE=.\CTRTextureGouraud.h
# End Source File
# Begin Source File

SOURCE=.\CTRTextureGouraudWire.cpp
# End Source File
# Begin Source File

SOURCE=.\CVideoSoftware.cpp
# End Source File
# Begin Source File

SOURCE=.\CVideoSoftware.h
# End Source File
# Begin Source File

SOURCE=.\CZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\IK3DTriangleRenderer.h
# End Source File
# Begin Source File

SOURCE=.\IZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\S2DVertex.h
# End Source File
# End Group
# Begin Group "OpenGL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\COpenGLTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\COpenGLTexture.h
# End Source File
# Begin Source File

SOURCE=.\CVideoOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\CVideoOpenGL.h
# End Source File
# End Group
# Begin Group "DirectX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDirectX8Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\CDirectX8Texture.h
# End Source File
# Begin Source File

SOURCE=.\CVideoDirectX8.cpp
# End Source File
# Begin Source File

SOURCE=.\CVideoDirectX8.h
# End Source File
# End Group
# Begin Group "Null"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CColorConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\CColorConverter.h
# End Source File
# Begin Source File

SOURCE=.\CFPSCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\CFPSCounter.h
# End Source File
# Begin Source File

SOURCE=.\CSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\CSurface.h
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderBmp.cpp
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderBmp.h
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderJPG.cpp
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderJPG.h
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderPSD.cpp
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderPSD.h
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\CSurfaceLoaderTGA.h
# End Source File
# Begin Source File

SOURCE=.\CVideoNull.cpp
# End Source File
# Begin Source File

SOURCE=.\CVideoNull.h
# End Source File
# Begin Source File

SOURCE=.\ISurface.h
# End Source File
# Begin Source File

SOURCE=.\ISurfaceLoader.h
# End Source File
# Begin Source File

SOURCE=.\ISurfacePresenter.h
# End Source File
# End Group
# End Group
# Begin Group "scene impl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAnimatedMeshMD2.cpp
# End Source File
# Begin Source File

SOURCE=.\CAnimatedMeshMD2.h
# End Source File
# Begin Source File

SOURCE=.\CAnimatedMeshMS3D.cpp
# End Source File
# Begin Source File

SOURCE=.\CAnimatedMeshMS3D.h
# End Source File
# Begin Source File

SOURCE=.\CAnimatedMeshSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CAnimatedMeshSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CBillboardSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CBillboardSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CBspTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CBspTree.h
# End Source File
# Begin Source File

SOURCE=.\CBspTreeSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CBspTreeSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CCameraMayaSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CCameraMayaSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CCameraSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CCameraSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CLightSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CLightSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CMeshSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CMeshSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\COctTreeSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\COctTreeSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\CQ3LevelMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\CQ3LevelMesh.h
# End Source File
# Begin Source File

SOURCE=.\CSceneManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSceneManager.h
# End Source File
# Begin Source File

SOURCE=.\CSceneNodeAnimatorFlyCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\CSceneNodeAnimatorFlyCircle.h
# End Source File
# Begin Source File

SOURCE=.\CSceneNodeAnimatorRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\CSceneNodeAnimatorRotation.h
# End Source File
# Begin Source File

SOURCE=.\CStaticMeshOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\CStaticMeshOBJ.h
# End Source File
# Begin Source File

SOURCE=.\CTestSceneNode.cpp
# End Source File
# Begin Source File

SOURCE=.\CTestSceneNode.h
# End Source File
# Begin Source File

SOURCE=.\OctTree.h
# End Source File
# End Group
# Begin Group "io impl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileList.h
# End Source File
# Begin Source File

SOURCE=.\CFileSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileSystem.h
# End Source File
# Begin Source File

SOURCE=.\CLimitReadFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CLimitReadFile.h
# End Source File
# Begin Source File

SOURCE=.\CMemoryReadFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CMemoryReadFile.h
# End Source File
# Begin Source File

SOURCE=.\CReadFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CReadFile.h
# End Source File
# Begin Source File

SOURCE=.\CZipReader.cpp
# End Source File
# Begin Source File

SOURCE=.\CZipReader.h
# End Source File
# End Group
# Begin Group "other impl"

# PROP Default_Filter ""
# Begin Group "extern"

# PROP Default_Filter ""
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "jpeglib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jpeglib\cderror.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\cdjpeg.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdct.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jerror.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jerror.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jutils.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\jversion.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdbmp.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdcolmap.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdgif.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdppm.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdrle.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdswitch.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\rdtarga.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\transupp.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\transupp.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib\wrbmp.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\wrgif.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\wrppm.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\wrrle.c
# End Source File
# Begin Source File

SOURCE=.\jpeglib\wrtarga.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\fast_atof.h
# End Source File
# Begin Source File

SOURCE=.\os.cpp
# End Source File
# Begin Source File

SOURCE=.\os.h
# End Source File
# End Group
# Begin Group "doc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Todo.txt
# End Source File
# End Group
# Begin Source File

SOURCE=.\CIrrDeviceWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\CIrrDeviceWin32.h
# End Source File
# Begin Source File

SOURCE=.\Irrlicht.cpp
# End Source File
# End Target
# End Project
