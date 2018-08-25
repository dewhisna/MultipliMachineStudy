;****************************************************************************
;**
;** Multipli Machine Study Application
;** Copyright (C) 2018 Multipli Machinery, Corp.
;** Contact: http://multiplimachine.com/
;**
;** This file is part of the Multipli Machine Study Application,
;** written and developed for Multipli Machinery by Donald Whisnant.
;**
;** GNU General Public License Usage
;** This file may be used under the terms of the GNU General Public License
;** version 3.0 as published by the Free Software Foundation and appearing
;** in the file gpl-3.0.txt included in the packaging of this file. Please
;** review the following information to ensure the GNU General Public License
;** version 3.0 requirements will be met:
;** http://www.gnu.org/copyleft/gpl.html.
;**
;** Other Usage
;** Alternatively, this file may be used in accordance with the terms and
;** conditions contained in a signed written agreement between you and
;** Multipli Machinery Corp.
;**
;****************************************************************************

;#define EXTRAVERSION "Preview1"
#include "version.iss"

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"

[Setup]
AppName=Multipli Machine Study
AppId=MultipliMachineStudy
AppMutex=MachineStudyMutex
#expr VersionInstaller(".\MachineStudy.exe")
AppCopyright=Copyright (C) 2018 Multipli Machinery Corp.
AppPublisher=Dewtronics/Multipli
AppPublisherURL=http://www.dewtronics.com/
AppContact=Multipli Machinery Corp.
AppSupportURL=http://www.multiplimachine.com/
AppSupportPhone=makepaper@multiplimachine.com
AppComments=Multipli Cylinder Paper Machine Study Software
DefaultDirName={pf}\MultipliMachineStudy
DefaultGroupName=Multipli Machine Study
ShowLanguageDialog=auto
LicenseFile=gpl-3.0.txt
Compression=lzma
ChangesAssociations=no
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons};

[Files]
; Main app:
Source: ".\MachineStudy.exe"; DestDir: "{app}"; Flags: ignoreversion;

; Qt Libraries:
Source: ".\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\Qt5Help.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion;

; Qt plugins/platforms
Source: ".\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion;
Source: ".\platforms\qminimal.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion;

; Qt plugins/platformthemes
Source: ".\platformthemes\qflatpak.dll"; DestDir: "{app}\platformthemes"; Flags: ignoreversion;

; Qt plugins/generic
Source: ".\generic\qtuiotouchplugin.dll"; DestDir: "{app}\generic"; Flags: ignoreversion;

; Qt plugins/imageformats
Source: ".\imageformats\qgif.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion;
Source: ".\imageformats\qicns.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion;
Source: ".\imageformats\qico.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion;
Source: ".\imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion;
Source: ".\imageformats\qwbmp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion;

; Qt plugins/styles
Source: ".\styles\qwindowsvistastyle.dll"; DestDir: "{app}\styles"; Flags: ignoreversion;

; Qt plugins/sqldrivers
Source: ".\sqldrivers\qsqlite.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion;

; Qt plugins/printsupport
Source: ".\printsupport\windowsprintersupport.dll"; DestDir: "{app}\printsupport"; Flags: ignoreversion;

; MSVCRT:
Source: ".\msvcp140.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\concrt140.dll"; DestDir: "{app}"; Flags: ignoreversion;

; HelpFiles:
Source: ".\machstdy.qch"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\machstdy.qhc"; DestDir: "{app}"; Flags: ignoreversion;

; license
Source: ".\LICENSE"; DestDir: "{app}"; Flags: ignoreversion;
Source: ".\gpl-3.0.txt"; DestDir: "{app}"; Flags: ignoreversion;

[Icons]
Name: "{group}\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe";
Name: "{group}\{cm:UninstallProgram,{#SetupSetting("AppName")}}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe"; Tasks: desktopicon;
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe"; Tasks: quicklaunchicon;

[Run]
Filename: {app}\MachineStudy.exe; Description: {cm:LaunchProgram,{#SetupSetting("AppName")}}; Flags: nowait postinstall skipifsilent
