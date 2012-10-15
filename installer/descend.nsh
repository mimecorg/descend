/**************************************************************************
* This file is part of the Descend program
* Copyright (C) 2012 Michał Męciński
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

!define VERSION "0.2"
!define BUILDVERSION "0.2.0.4671"

!define SRCDIR ".."
!define BUILDDIR "..\release"

!define UNINST_KEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Descend"

!ifndef SIGN
    !verbose 2

    !system "$\"${NSISDIR}\makensis$\" /V2 /DSIGN ${SCRIPTNAME}" = 0

    !system "..\..\sign.bat descend-${VERSION}-${ARCHITECTURE}.exe" = 0

    SetCompress off

    OutFile "$%TEMP%\signinst.exe"

    Section
    SectionEnd
!else

!include "MUI2.nsh"

!include "languages\descend_en.nsh"

!ifdef INNER
    SetCompress off

    OutFile "$%TEMP%\innerinst.exe"
!else
    !verbose 4

    !system "$\"${NSISDIR}\makensis$\" /V2 /DSIGN /DINNER ${SCRIPTNAME}" = 0

    !system "$%TEMP%\innerinst.exe" = 2

    !system "..\..\sign.bat $%TEMP%\uninstall.exe" = 0

    SetCompressor /SOLID lzma
    SetCompressorDictSize 32

    OutFile "descend-${VERSION}-${ARCHITECTURE}.exe"
!endif

!define MULTIUSER_EXECUTIONLEVEL "Highest"
!define MULTIUSER_MUI
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "${UNINST_KEY}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME "UninstallString"
!define MULTIUSER_INSTALLMODE_INSTDIR "Descend"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "${UNINST_KEY}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "InstallLocation"
!if ${ARCHITECTURE} == "win_x64"
    !define MULTIUSER_USE_PROGRAMFILES64
!endif
!ifndef INNER
    !define MULTIUSER_NOUNINSTALL
!endif
!include "include\multiuser64.nsh"

Name "$(NAME)"

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install-blue.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall-blue.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "images\wizard.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "images\wizard.bmp"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "images\header.bmp"
!define MUI_HEADERIMAGE_RIGHT

!define MUI_WELCOMEPAGE_TITLE "$(TITLE)"
!define MUI_WELCOMEPAGE_TEXT "$(WELCOME_TEXT)"
!insertmacro MUI_PAGE_WELCOME

!define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "${SRCDIR}\COPYING"

!insertmacro MULTIUSER_PAGE_INSTALLMODE

!insertmacro MUI_PAGE_DIRECTORY

ShowInstDetails nevershow
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_TITLE "$(TITLE)"
!insertmacro MUI_PAGE_FINISH
  
!ifdef INNER  
    !define MUI_WELCOMEPAGE_TITLE "$(TITLE)"
    !insertmacro MUI_UNPAGE_WELCOME

    !insertmacro MUI_UNPAGE_CONFIRM

    ShowUninstDetails nevershow
    !insertmacro MUI_UNPAGE_INSTFILES

    !define MUI_FINISHPAGE_TITLE "$(TITLE)"
    !insertmacro MUI_UNPAGE_FINISH
!endif

!insertmacro MUI_LANGUAGE "English"

VIProductVersion "${BUILDVERSION}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Michał Męciński"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Descend Setup"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright (C) 2011-2012 Michał Męciński"
VIAddVersionKey /LANG=${LANG_ENGLISH} "OriginalFilename" "descend-${VERSION}-${ARCHITECTURE}.exe"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Descend"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "${VERSION}"

Function .onInit

!if ${ARCHITECTURE} == "win_x64"
    SetRegView 64
!endif

!ifdef INNER
    WriteUninstaller "$%TEMP%\uninstall.exe"
    Quit
!endif

    !insertmacro MULTIUSER_INIT

FunctionEnd

Section

    SetOutPath "$INSTDIR"

    File "${SRCDIR}\ChangeLog"
    File "${SRCDIR}\COPYING"
    File "${SRCDIR}\README"

    SetOutPath "$INSTDIR\bin"

    Delete "$INSTDIR\bin\*.*"

    File "${BUILDDIR}\descend.exe"

    SetOutPath "$INSTDIR\templates"

    Delete "$INSTDIR\templates\*.*"

    File "${SRCDIR}\templates\templates.ini"

    File "${SRCDIR}\templates\*.dscn"

    SetOutPath "$INSTDIR\bin"

    CreateShortCut "$SMPROGRAMS\Descend.lnk" "$INSTDIR\bin\descend.exe"
    CreateShortCut "$DESKTOP\Descend.lnk" "$INSTDIR\bin\descend.exe"

    WriteRegStr SHCTX "SOFTWARE\Classes\.dscn" "" "Descend.Project"
    WriteRegStr SHCTX "SOFTWARE\Classes\Descend.Project" "" "Descend Project"
    WriteRegStr SHCTX "SOFTWARE\Classes\Descend.Project\DefaultIcon" "" "$INSTDIR\bin\descend.exe,0"
    WriteRegStr SHCTX "SOFTWARE\Classes\Descend.Project\shell\open\command" "" '"$INSTDIR\bin\descend.exe" "%1"'

    WriteRegStr SHCTX "${UNINST_KEY}" "DisplayIcon" '"$INSTDIR\bin\descend.exe"'
    WriteRegStr SHCTX "${UNINST_KEY}" "DisplayName" "Descend ${VERSION}${SUFFIX}"
    WriteRegStr SHCTX "${UNINST_KEY}" "DisplayVersion" "${VERSION}"
    WriteRegStr SHCTX "${UNINST_KEY}" "UninstallString" '"$INSTDIR\uninstall.exe" /$MultiUser.InstallMode'
    WriteRegStr SHCTX "${UNINST_KEY}" "InstallLocation" "$INSTDIR"
    WriteRegStr SHCTX "${UNINST_KEY}" "Publisher" "Michał Męciński"
    WriteRegStr SHCTX "${UNINST_KEY}" "HelpLink" "http://descend.mimec.org"
    WriteRegStr SHCTX "${UNINST_KEY}" "URLInfoAbout" "http://descend.mimec.org"
    WriteRegStr SHCTX "${UNINST_KEY}" "URLUpdateInfo" "http://descend.mimec.org/downloads"
    WriteRegDWORD SHCTX "${UNINST_KEY}" "NoModify" 1
    WriteRegDWORD SHCTX "${UNINST_KEY}" "NoRepair" 1

!ifndef INNER
    SetOutPath "$INSTDIR"
    File "$%TEMP%\uninstall.exe"
!endif

SectionEnd

!ifdef INNER

Function un.onInit

!if ${ARCHITECTURE} == "win_x64"
    SetRegView 64
!endif

    !insertmacro MULTIUSER_UNINIT

FunctionEnd

Section "Uninstall"

    DeleteRegKey SHCTX "${UNINST_KEY}"
    
    DeleteRegKey SHCTX "SOFTWARE\Classes\.dscn"
    DeleteRegKey SHCTX "SOFTWARE\Classes\Descend.Project"

    Delete "$SMPROGRAMS\Descend.lnk"
    Delete "$DESKTOP\Descend.lnk"

    Delete "$INSTDIR\ChangeLog"
    Delete "$INSTDIR\COPYING"
    Delete "$INSTDIR\README"
    RMDir /r "$INSTDIR\bin"
    RMDir /r "$INSTDIR\templates"
    Delete "$INSTDIR\uninstall.exe"
    RMDir "$INSTDIR"

SectionEnd

!endif
!endif
