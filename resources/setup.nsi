!include "MUI2.nsh"
!include "logiclib.nsh"

!define NAME "Book Tracker"
!define APPFILE "book-tracker.exe"
!define VERSION ""
!define SLUG "${NAME} v${VERSION}"
!define MUI_ICON "main.ico"

Name "${NAME}"
OutFile "${NAME} Setup.exe"
InstallDir "$PROGRAMFILES\${NAME}"
InstallDirRegKey HKCU "Software\${NAME}" ""
RequestExecutionLevel admin

  ; Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set UI language
!insertmacro MUI_LANGUAGE "English"

; Installer stuff
Section
SectionIn RO
SetOutPath "$INSTDIR"
SetOverwrite on
CreateDirectory "$SMPROGRAMS\BookTracker"
File "build\book-tracker"
File "main.ico"
CreateShortCut "$SMPROGRAMS\BookTracker\${NAME}.lnk" "$INSTDIR\${APPFILE}" "" "$INSTDIR\main.ico"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" \
                 "DisplayName" "${NAME}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
WriteRegStr HKCU "Software\${NAME}" "" $INSTDIR
WriteUninstaller "$INSTDIR\Uninstall.exe"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoModify" 1
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "NoRepair" 1
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayVersion" "${VERSION}"
SectionEnd

; Uninstaller stuff
Section "Uninstall"
DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
Delete "$SMPROGRAMS\BookTracker\${NAME}.lnk"
Delete $INSTDIR\Uninstall.exe
RMDir $INSTDIR
SectionEnd
