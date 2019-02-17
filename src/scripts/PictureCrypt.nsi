############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006
############################################################################################

!define APP_NAME "PictureCrypt"
!define COMP_NAME "Alex Kovrigin"
!define WEB_SITE "https://alexkovrigin.me"
!define VERSION "0.1.4.1"
!define COPYRIGHT "Alex Kovrigin � 2019"
!define DESCRIPTION "An image-steganography project"
!define LICENSE_TXT "C:\projects\picturecrypt\LICENSE"
!define INSTALLER_NAME "C:\projects\picturecrypt\app\PictureCrypt-setup.exe"
!define MAIN_APP_EXE "PictureCrypt.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\PictureCrypt"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "PictureCrypt"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\projects\picturecrypt\app\deploy\release\D3Dcompiler_47.dll"
File "C:\projects\picturecrypt\app\deploy\release\libEGL.dll"
File "C:\projects\picturecrypt\app\deploy\release\libgcc_s_dw2-1.dll"
File "C:\projects\picturecrypt\app\deploy\release\libGLESV2.dll"
File "C:\projects\picturecrypt\app\deploy\release\libstdc++-6.dll"
File "C:\projects\picturecrypt\app\deploy\release\libwinpthread-1.dll"
File "C:\projects\picturecrypt\app\deploy\release\opengl32sw.dll"
File "C:\projects\picturecrypt\app\deploy\release\PictureCrypt.exe"
File "C:\projects\picturecrypt\app\deploy\release\Qt5Core.dll"
File "C:\projects\picturecrypt\app\deploy\release\Qt5Gui.dll"
File "C:\projects\picturecrypt\app\deploy\release\Qt5Svg.dll"
File "C:\projects\picturecrypt\app\deploy\release\Qt5Widgets.dll"
SetOutPath "$INSTDIR\translations"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_ar.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_bg.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_ca.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_cs.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_da.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_de.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_en.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_es.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_fi.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_fr.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_gd.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_he.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_hu.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_it.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_ja.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_ko.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_lv.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_pl.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_ru.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_sk.qm"
File "C:\projects\picturecrypt\app\deploy\release\translations\qt_uk.qm"
SetOutPath "$INSTDIR\styles"
File "C:\projects\picturecrypt\app\deploy\release\styles\qwindowsvistastyle.dll"
SetOutPath "$INSTDIR\platforms"
File "C:\projects\picturecrypt\app\deploy\release\platforms\qwindows.dll"
SetOutPath "$INSTDIR\imageformats"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qgif.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qicns.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qico.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qjpeg.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qsvg.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qtga.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qtiff.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qwbmp.dll"
File "C:\projects\picturecrypt\app\deploy\release\imageformats\qwebp.dll"
SetOutPath "$INSTDIR\iconengines"
File "C:\projects\picturecrypt\app\deploy\release\iconengines\qsvgicon.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\PictureCrypt"
CreateShortCut "$SMPROGRAMS\PictureCrypt\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\PictureCrypt\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\PictureCrypt\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\D3Dcompiler_47.dll"
Delete "$INSTDIR\libEGL.dll"
Delete "$INSTDIR\libgcc_s_dw2-1.dll"
Delete "$INSTDIR\libGLESV2.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\opengl32sw.dll"
Delete "$INSTDIR\PictureCrypt.exe"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Svg.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\translations\qt_ar.qm"
Delete "$INSTDIR\translations\qt_bg.qm"
Delete "$INSTDIR\translations\qt_ca.qm"
Delete "$INSTDIR\translations\qt_cs.qm"
Delete "$INSTDIR\translations\qt_da.qm"
Delete "$INSTDIR\translations\qt_de.qm"
Delete "$INSTDIR\translations\qt_en.qm"
Delete "$INSTDIR\translations\qt_es.qm"
Delete "$INSTDIR\translations\qt_fi.qm"
Delete "$INSTDIR\translations\qt_fr.qm"
Delete "$INSTDIR\translations\qt_gd.qm"
Delete "$INSTDIR\translations\qt_he.qm"
Delete "$INSTDIR\translations\qt_hu.qm"
Delete "$INSTDIR\translations\qt_it.qm"
Delete "$INSTDIR\translations\qt_ja.qm"
Delete "$INSTDIR\translations\qt_ko.qm"
Delete "$INSTDIR\translations\qt_lv.qm"
Delete "$INSTDIR\translations\qt_pl.qm"
Delete "$INSTDIR\translations\qt_ru.qm"
Delete "$INSTDIR\translations\qt_sk.qm"
Delete "$INSTDIR\translations\qt_uk.qm"
Delete "$INSTDIR\styles\qwindowsvistastyle.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qicns.dll"
Delete "$INSTDIR\imageformats\qico.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtga.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\imageformats\qwbmp.dll"
Delete "$INSTDIR\imageformats\qwebp.dll"
Delete "$INSTDIR\iconengines\qsvgicon.dll"

RmDir "$INSTDIR\iconengines"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\platforms"
RmDir "$INSTDIR\styles"
RmDir "$INSTDIR\translations"

Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\PictureCrypt\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\PictureCrypt\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\PictureCrypt\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\PictureCrypt"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################
