;****************************************************************************
;**
;** Multipli Machine Study Application
;** Copyright (C) 1994, 2018-2022 Multipli Machinery, Corp.
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
#expr VersionInstaller(".\bin\MachineStudy.exe")
AppCopyright=Copyright (C) 1994, 2018-2022 Multipli Machinery Corp.
AppPublisher=Dewtronics/Multipli
AppPublisherURL=http://www.dewtronics.com/
AppContact=Multipli Machinery Corp.
AppSupportURL=http://www.multiplimachine.com/
AppSupportPhone=makepaper@multiplimachine.com
AppComments=Multipli Cylinder Paper Machine Study Software
DefaultDirName={commonpf}\MultipliMachineStudy
DefaultGroupName=Multipli Machine Study
ShowLanguageDialog=auto
LicenseFile=.\bin\gpl-3.0.txt
Compression=lzma
ChangesAssociations=no
PrivilegesRequired=admin

[CustomMessages]
InstallingVCredist=Installing VCRedist...

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons};

[Files]
; Main app and Qt files:
Source: ".\bin\*"; Excludes: "vc_redist.x64.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs;
; Runtime:
Source: ".\bin\vc_redist.x64.exe"; DestDir: "{tmp}"; Flags: ignoreversion;

[Icons]
Name: "{group}\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe";
Name: "{group}\{cm:UninstallProgram,{#SetupSetting("AppName")}}"; Filename: "{uninstallexe}";
Name: "{group}\Machine Study Documentation"; Filename: "{app}\MachineStudy.pdf";
Name: "{group}\Machine Study Examples"; Filename: "{app}\MachineStudy-Examples.pdf";
Name: "{commondesktop}\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe"; Tasks: desktopicon;
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#SetupSetting("AppName")}"; Filename: "{app}\MachineStudy.exe"; Tasks: quicklaunchicon;

[Run]
Filename: "{tmp}\vc_redist.x64.exe"; Description: "{cm:InstallingVCredist}"; StatusMsg: "{cm:InstallingVCredist}"; Parameters: "/quiet"; Flags: waituntilterminated
Filename: "{app}\MachineStudy.exe"; Description: {cm:LaunchProgram,{#SetupSetting("AppName")}}; Flags: nowait postinstall skipifsilent
