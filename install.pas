{

      Multipli Machinery Installation Program V1.0

              (C)1994 Multipli Machinery

              written by Donald Whisnant

}

program Install;

uses WinProcs, WinTypes, WinDos, Objects, OWindows, ODialogs, BWCC, Win31,
     Strings, ASEdit, UserInfo, LZExpand, Ver, ShellAPI;

{$R INSTALL.RES}
{$F+}

{ General Constants: }
const

{ Menu bar constants }
  MenuID              = 100; { Resource ID of the menu }
  InstallID           = 101; { File->Install ID }
  QuitID              = 109; { File->Quit ID }

  AboutID             = 999; { Help->About ID }

  InstallFileName     = 'INSTALL.INI';
  GroupRunFileName    = 'GROUPS.EXE';
  GroupDataFileName   = 'GROUPS.M$$';

  StrSize             = 80;

  MaxFiles            = 20;
  FileNameSize        = 80;
  SymBufferSize       = 100;

  CRLF                = ^M^J;

type
{ TSrcDialog Definitions: }
  PSrcDialog = ^TSrcDialog;
  TSrcDialog = object(TDialog)
    SrcDriveEdit: PASEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar);
  end;

  PUserData = ^TUserData;
  TUserData = record
    UserName: array[0..StrSize] of Char;
    CoName: array[0..StrSize] of Char;
    SerNum: array[0..StrSize] of Char;
    OkButton: Word;
  end;

{ TUserInfoDialog Definitions: }
  PUserInfoDialog = ^TUserInfoDialog;
  TUserInfoDialog = object(TDialog)
    UserNameEdit: PASEdit;
    CoNameEdit: PASEdit;
    SerNumEdit: PASEdit;
    OkButton: PButton;
    constructor Init(AParent: PWindowsObject; AName: PChar);
    procedure SetupWindow; virtual;
    procedure WMTimer(var Msg: TMessage);
      virtual wm_First + wm_Timer;
    procedure WMDestroy(var Msg: TMessage);
      virtual wm_First + wm_Destroy;
  end;

{ TDestDialog Definitions: }
  PDestDialog = ^TDestDialog;
  TDestDialog = object(TDialog)
    DestDriveEdit: PASEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar);
  end;

{ TInstallWindow Definitions: }
  PInstallWindow = ^TInstallWindow;
  TInstallWindow = object(TWindow)
    WinDir: array[0..fsPathName] of Char;
    SysDir: array[0..fsPathName] of Char;
    SrcDir: array[0..fsPathName] of Char;
    DestDir: array[0..fsPathName] of Char;
    CurDir: array[0..fsPathName] of Char;

    InstallFile: array[0..fsPathName] of Char;
    InstallFileHandle: Integer;
    InstallFileStruct: TOFStruct;

    UserInfoFile: array[0..fsPathName] of Char;
    UserInfoFileHandle: Integer;
    UserInfoFileStruct: TOFStruct;

    SrcName: array[0..fsPathName] of Char;
    SrcHandle: Integer;
    SrcStruct: TOFStruct;

    DestName: array[0..fsPathName] of Char;
    DestHandle: Integer;
    DestStruct: TOFStruct;

    UserInfo: TUserData;

    constructor Init(AParent: PWindowsObject; ATitle: PChar);

    procedure SetupWindow; virtual;

    procedure Quit(var Msg: TMessage);
      virtual cm_First + QuitID;
    procedure AboutBox(var Msg: TMessage);
      virtual cm_First + AboutID;
    procedure CMInstall(var Msg: TMessage);
      virtual cm_First + InstallID;

    procedure DoInstall; virtual;

    procedure GetWindowClass(var AWndClass: TWndClass); virtual;
    function GetClassName: PChar; virtual;
  end;


{ TInstallApp Definitions: }
  TInstallApp = object(TApplication)
    procedure InitMainWindow; virtual;
    procedure InitInstance; virtual;
  end;

{ ------------------------------------------------------------------------ }

{ Miscellaneous Functions }

{ Encode encodes a string by adding 128 to all values }
procedure Encode(AString: PChar);
var
  i: integer;
begin
  for i:=0 to StrLen(AString)-1 do
    Char(AString[I]):=Chr(Byte(AString[I])+128);
end;

{ Decode decodes a string by subtracting 128 from all values }
procedure Decode(AString: PChar);
var
  i: integer;
begin
  for i:=0 to StrLen(AString)-1 do
    Char(AString[I]):=Chr(Byte(AString[I])-128);
end;

{ The following replaces are strings in SrcStr matching '$' followed
    by a string in double-terminated string list SymBuffer with the
    corresponding string in the double-terminated string list SymRepBuffer
    and returns a Pascal String type }
function ReplaceSymbols(SrcStr, SymBuffer, SymRepBuffer: PChar): string;
var
  APointer, APointer2: PChar;
  SrcPointer: PChar;
  NewStr: string;
  FoundFlag: Boolean;
begin
  NewStr:='';
  SrcPointer:=SrcStr;
  while Byte(SrcPointer^)<>0 do
  begin
    if Char(SrcPointer^)='$' then
    begin
      APointer:=SymBuffer;
      APointer2:=SymRepBuffer;
      LongInt(SrcPointer):=LongInt(SrcPointer)+1;
      FoundFlag:=False;
      while ((Byte(APointer^)<>0) and (FoundFlag=False)) do
      begin
        if StrLComp(SrcPointer, APointer, StrLen(APointer))=0 then
        begin
          LongInt(SrcPointer):=LongInt(SrcPointer)+StrLen(APointer);
          while Byte(APointer2^)<>0 do
          begin
            NewStr:=NewStr+Char(APointer2^);
            LongInt(APointer2):=LongInt(APointer2)+1;
          end;
          LongInt(APointer2):=LongInt(APointer2)+1;       { Skip 00 }
          FoundFlag:=True;
        end else
          LongInt(APointer2):=LongInt(APointer2)+StrLen(APointer2)+1;

        LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
      end;
    end else
    begin
      NewStr:=NewStr+Char(SrcPointer^);
      LongInt(SrcPointer):=LongInt(SrcPointer)+1;
    end;
  end;

  ReplaceSymbols:=NewStr;
end;

{ The following checks the string SrcStr to be equivalent to ValidStr for
    all UPPERCASE characters and symbols.  Lower case control characters in
    ValidStr are used as "wildcard" replacements as follows:

             n - accept any integer number

   Returns back a boolean True/False representing acceptance }
function CheckValid(SrcStr, ValidStr: PChar): Boolean;
var
  SrcPointer: PChar;
  APointer: PChar;
  ValidFlag: Boolean;
begin
  ValidFlag:=True;                      { Assume valid until otherwise }
  SrcPointer:=SrcStr;
  APointer:=ValidStr;

  while ((Byte(SrcPointer^)<>0) and (Byte(APointer^)<>0) and ValidFlag) do
  begin
    if (IsCharUpper(Char(APointer^)) or
        (IsCharAlphaNumeric(Char(APointer^)) and
         not IsCharAlpha(Char(APointer^))) or
       (not IsCharAlphaNumeric(Char(APointer^)))) then
    begin
      if Byte(SrcPointer^)<>Byte(APointer^) then
        ValidFlag:=False
      else
      begin
        LongInt(SrcPointer):=LongInt(SrcPointer)+1;
        LongInt(APointer):=LongInt(APointer)+1;
      end;
    end else
    begin
      case Char(APointer^) of
        'n': while ((Byte(SrcPointer^)<>0) and
                    (IsCharAlphaNumeric(Char(SrcPointer^)) and
                     not IsCharAlpha(Char(SrcPointer^)))) do
               LongInt(SrcPointer):=LongInt(SrcPointer)+1;
      end;
      LongInt(APointer):=LongInt(APointer)+1;
    end;
  end;

  if ((Byte(SrcPointer^)<>0) or (Byte(APointer^)<>0)) then ValidFlag:=False;

  CheckValid:=ValidFlag;
end;

{ ------------------------------------------------------------------------ }

{ TSrcDialog Methods ----------------------------------------------------- }
constructor TSrcDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  SrcDriveEdit:=New(PASEdit, InitResource(@Self, 101, fsPathName+1));
end;

{ TDestDialog Methods ---------------------------------------------------- }
constructor TDestDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  DestDriveEdit:=New(PASEdit, InitResource(@Self, 101, fsPathName+1));
end;

{ TUserInfoDialog Methods ------------------------------------------------ }
constructor TUserInfoDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  UserNameEdit:=New(PASEdit, InitResource(@Self, 101, StrSize+1));
  CoNameEdit:=New(PASEdit, InitResource(@Self, 102, StrSize+1));
  SerNumEdit:=New(PASEdit, InitResource(@Self, 103, StrSize+1));
  OkButton:=New(PButton, InitResource(@Self, 1));
end;

procedure TUserInfoDialog.SetupWindow;
var
  Result: Integer;
begin
  inherited SetupWindow;
  EnableWindow(OkButton^.HWindow, False);

  Result := IDRetry;
  while (SetTimer(HWindow, 0, 1000, nil) = 0) and (Result = IDRetry) do
    Result := MessageBox(GetFocus,'Could not Create Timer', 'Installation',
      mb_RetryCancel);
  if Result = IDCancel then PostQuitMessage(0);

end;

procedure TUserInfoDialog.WMTimer(var Msg: TMessage);
var
  TempTxt: array[0..StrSize] of Char;
begin
  if ((UserNameEdit^.GetText(TempTxt, StrSize)<>0) and
      (CoNameEdit^.GetText(TempTxt, StrSize)<>0) and
      (SerNumEdit^.GetText(TempTxt, StrSize)<>0)) then
    EnableWindow(OkButton^.HWindow, True)
  else
    EnableWindow(OkButton^.HWindow, False);
end;

procedure TUserInfoDialog.WMDestroy(var Msg: TMessage);
begin
  KillTimer(HWindow, 0);
  inherited WMDestroy(Msg);
end;

{ TInstallWindow Definitions: ------------------------------------------- }
constructor TInstallWindow.Init(AParent: PWindowsObject; ATitle: PChar);
begin
  inherited Init(AParent, ATitle);
  Attr.Menu:=LoadMenu(HInstance, MakeIntResource(MenuID));
  StrCopy(SrcDir, 'A:');
  StrCopy(DestDir, 'C:\PROGRAM');
  StrCopy(UserInfo.UserName, '');
  StrCopy(UserInfo.CoName, '');
  StrCopy(UserInfo.SerNum, '');
  UserInfo.OkButton:=0;
end;

procedure TInstallWindow.SetupWindow;
var
  Result: Integer;
begin
  Show(sw_ShowMaximized);
  inherited SetupWindow;
  DoInstall;
end;

procedure TInstallWindow.Quit(var Msg: TMessage);
begin
  CloseWindow;
end;

procedure TInstallWindow.AboutBox(var Msg: TMessage);
begin
  Application^.ExecDialog(New(PDialog, Init(@Self, 'ABOUTBOX')));
end;

procedure TInstallWindow.CMInstall(var Msg: TMessage);
begin
  DoInstall;
end;

procedure TInstallWindow.GetWindowClass(var AWndClass: TWndClass);
begin
  inherited GetWindowClass(AWndClass);
  AWndClass.hIcon := LoadIcon(HInstance, 'LOGO');
end;

function TInstallWindow.GetClassName: PChar;
begin
  GetClassName:='Multipli Installation Program';
end;

procedure TInstallWindow.DoInstall;
var
  TempFileName: array[0..fsPathName] of Char;
  TempFileHandle: Integer;
  TempFileStruct: TOFStruct;
  CurFileName: array[0..fsPathName] of Char;
  TempInsFile: array[0..255] of Char;
  TempInsStruct: TOFStruct;
  TempInsFileLen: Word;
  TempFile2: array[0..fsPathName] of Char;
  TempFile2Handle: Integer;
  TempFile2Struct: TOFStruct;
  SrcDlg: PSrcDialog;
  DestDlg: PDestDialog;
  UserInfoDlg: PUserInfoDialog;
  DlgFlag: Boolean;
  UserInfoFlag: Boolean;
  SNValid: Boolean;
  ValidSN: array[0..80] of Char;
  TempStr: array[0..80] of Char;
  FileNameBuffer: array[0..(MaxFiles*FileNameSize)] of Char;
  SymBuffer: array[0..SymBufferSize] of Char;
  SymRepBuffer: array[0..SymBufferSize] of Char;
  APointer: PChar;
  APointer2: PChar;
  IsWorkingDlg: PDialog;
  BufSize: LongInt;
  I, FileNameLen: Integer;
begin
  GetWindowsDirectory(WinDir, SizeOf(WinDir));
  GetSystemDirectory(SysDir, SizeOf(SysDir));

  DlgFlag:=False;
  FileNameLen := GetModuleFileName(HInstance, SrcDir, fsPathName);
  I := FileNameLen - 1;
  while (I <> 0) and ((SrcDir[I] <> '\') and (SrcDir[I] <> ':')) do
    Dec(I);
  If SrcDir[I]<>'\' then Inc(I);
  SrcDir[I]:=chr(0);

  GetCurDir(CurDir, 0);
  SetCurDir(SrcDir);
  InstallFileHandle:=OpenFile(InstallFileName, InstallFileStruct,
                                               OF_Prompt+OF_Exist);
  if InstallFileHandle<0 then
    SetCurDir(CurDir)
  else
    DlgFlag:=True;

  while (InstallFileHandle=-1) do
  begin
    SrcDlg:=New(PSrcDialog, Init(@Self, 'SOURCE_DRIVE'));
    SrcDlg^.TransferBuffer:=@SrcDir;
    DlgFlag:=False;
    if Application^.ExecDialog(SrcDlg)=id_ok then
    begin
      GetCurDir(CurDir, 0);
      SetCurDir(SrcDir);
      InstallFileHandle:=OpenFile(InstallFileName, InstallFileStruct,
                                                   OF_Prompt+OF_Exist);
      if InstallFileHandle<0 then SetCurDir(CurDir);
      DlgFlag:=True;
    end else InstallFileHandle:=0;
  end;

  if DlgFlag=True then
  begin
    FileExpand(InstallFile, InstallFileName);
    GetPrivateProfileString('DEFDIR', 'directory', 'C:\PROGRAM',
                            DestDir, SizeOf(DestDir), InstallFile);
    GetPrivateProfileString('INFO', 'infofile', 'PROGRAM.USR',
                            TempFileName, SizeOf(TempFileName),
                            InstallFile);

    GetPrivateProfileString('SYMBOLS', nil, chr(0)+chr(0),
                         SymBuffer, SizeOf(SymBuffer),
                         InstallFile);
    APointer:=SymBuffer;
    APointer2:=SymRepBuffer;
    while (Byte(APointer^)<>0) do
    begin
      GetPrivateProfileString('SYMBOLS', APointer, chr(0)+chr(0), TempStr,
                                         SizeOf(TempStr), InstallFile);
      StrCopy(APointer2, TempStr);
      LongInt(APointer2):=LongInt(APointer2)+StrLen(TempStr)+1;
      LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
    end;
    Byte(APointer2^):=0;

    GetPrivateProfileString('INFO', 'validsn', '', ValidSN,
                                    SizeOf(ValidSN), InstallFile);

    StrPCopy(ValidSN, ReplaceSymbols(ValidSN, SymBuffer, SymRepBuffer));

    FileExpand(UserInfoFile, TempFileName);

    UserInfoFileHandle:=ShowUserInfo(@Self, 'Installation Program',
                                            TempFileName, False);
    SNValid:=True;

    DestDlg:=New(PDestDialog, Init(@Self, 'DIR_NAME'));
    DestDlg^.TransferBuffer:=@DestDir;
    if Application^.ExecDialog(DestDlg)=id_ok then
    begin

      if UserInfoFileHandle<0 then
        UserInfoFlag:=False
      else
        UserInfoFlag:=True;

      while ((UserInfoFileHandle<0) or (SNValid=False)) do
      begin

        UserInfoDlg:=New(PUserInfoDialog, Init(@Self, 'USER_INFO'));
        UserInfoDlg^.TransferBuffer:=@UserInfo;
        if Application^.ExecDialog(UserInfoDlg)=id_ok then
        begin
          SNValid:=CheckValid(UserInfo.SerNum, ValidSN);

          if SNValid then
          begin
            UserInfoFileHandle:=OpenFile(UserInfoFile, UserInfoFileStruct,
                                                     OF_Create);

            if UserInfoFileHandle>=0 then
            begin
              _lwrite(UserInfoFileHandle, chr(170)+chr(85), 2);
              Encode(UserInfo.UserName);
              _lwrite(UserInfoFileHandle, UserInfo.UserName,
                                          StrLen(UserInfo.UserName)+1);
              Encode(UserInfo.CoName);
              _lwrite(UserInfoFileHandle, UserInfo.CoName,
                                          StrLen(UserInfo.CoName)+1);
              Encode(UserInfo.SerNum);
              _lwrite(UserInfoFileHandle, UserInfo.SerNum,
                                          StrLen(UserInfo.SerNum)+1);

              _lclose(UserInfoFileHandle);
              UserInfoFlag:=True;
            end else
            begin
              MessageBox(HWindow, 'Error writing configuration file',
                                  'Installation Error', mb_Ok+mb_IconExclamation);
              UserInfoFileHandle:=0;
            end;
          end else
          begin
            MessageBox(HWindow, 'Serial Number is invalid.  Please Re-enter it.',
                                'Configuration Error', mb_ok+mb_IconExclamation);
          end;

        end else
        begin
          UserInfoFileHandle:=0;
          SNValid:=True;
        end;
      end;

      if UserInfoFlag=True then
      begin
        IsWorkingDlg:=New(PDialog, Init(@Self, 'WORKING'));
        IsWorkingDlg^.EnableAutoCreate;
        Application^.MakeWindow(IsWorkingDlg);

        CreateDir(DestDir);
        StrCopy(FileNameBuffer, chr(0)+chr(0));
        GetPrivateProfileString('PROGRAM', nil, chr(0)+chr(0),
                            FileNameBuffer, SizeOf(FileNameBuffer),
                            InstallFile);
        APointer:=FileNameBuffer;
        while (Byte(APointer^)<>0) do
        begin
          SetCurDir(SrcDir);
          FileExpand(SrcName, APointer);
          SetCurDir(DestDir);
          FileExpand(DestName, APointer);
          SrcHandle:=LZOpenFile(SrcName, SrcStruct, OF_Prompt+OF_Read);
          if SrcHandle>=0 then
          begin
            DestHandle:=OpenFile(DestName, DestStruct, OF_Create);
            if DestHandle>=0 then
            begin
              if LZCopy(SrcHandle, DestHandle)<0 then
              begin
                MessageBox(HWindow, 'Error Copying Files', 'Installation',
                                    mb_Ok + mb_IconExclamation);
                DestroyWindow(IsWorkingDlg^.HWindow);
                Halt(1);
              end;
              _lclose(DestHandle);
            end;
            LZClose(SrcHandle);
          end else
          begin
            DestroyWindow(IsWorkingDlg^.HWindow);
            Halt(1);
          end;

          LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
        end;

        SetCurDir(DestDir);
        StrCopy(FileNameBuffer, chr(0)+chr(0));
        GetPrivateProfileString('SUBDIR', nil, chr(0)+chr(0),
                            FileNameBuffer, SizeOf(FileNameBuffer),
                            InstallFile);
        APointer:=FileNameBuffer;
        while (Byte(APointer^)<>0) do
        begin
          FileExpand(TempFileName, APointer);
          CreateDir(TempFileName);
          LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
        end;

        StrCopy(FileNameBuffer, chr(0)+chr(0));
        GetPrivateProfileString('SYSTEM', nil, chr(0)+chr(0),
                            FileNameBuffer, SizeOf(FileNameBuffer),
                            InstallFile);
        APointer:=FileNameBuffer;
        while (Byte(APointer^)<>0) do
        begin
          StrCopy(CurFileName, SysDir);
          StrCopy(TempInsFile, '');
          TempInsFileLen:=SizeOf(TempInsFile);
          if VerInstallFile(VIFF_DontDeleteOld, APointer, APointer,
                           SrcDir, SysDir, CurFileName,
                           TempInsFile, TempInsFileLen)<>VIF_TempFile then
            VerInstallFile(VIFF_ForceInstall, TempInsFile, APointer,
                           SrcDir, SysDir, CurFileName,
                           TempInsFile, TempInsFileLen)
          else
          begin
            MessageBox(HWindow, 'Error Installing System File', 'Installation',
                                mb_Ok + mb_IconExclamation);
            OpenFile(TempInsFile, TempInsStruct, OF_Delete);
            DestroyWindow(IsWorkingDlg^.HWindow);
            Halt(1);
          end;
          LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
        end;

        StrCopy(FileNameBuffer, chr(0)+chr(0));
        GetPrivateProfileString('WINDOWS', nil, chr(0)+chr(0),
                            FileNameBuffer, SizeOf(FileNameBuffer),
                            InstallFile);
        APointer:=FileNameBuffer;
        while (Byte(APointer^)<>0) do
        begin
          StrCopy(CurFileName, WinDir);
          StrCopy(TempInsFile, '');
          TempInsFileLen:=SizeOf(TempInsFile);
          if VerInstallFile(VIFF_DontDeleteOld, APointer, APointer,
                           SrcDir, WinDir, CurFileName,
                           TempInsFile, TempInsFileLen)<>VIF_TempFile then
            VerInstallFile(VIFF_ForceInstall, TempInsFile, APointer,
                           SrcDir, WinDir, CurFileName,
                           TempInsFile, TempInsFileLen)
          else
          begin
            MessageBox(HWindow, 'Error Installing System File', 'Installation',
                                mb_Ok + mb_IconExclamation);
            OpenFile(TempInsFile, TempInsStruct, OF_Delete);
            DestroyWindow(IsWorkingDlg^.HWindow);
            Halt(1);
          end;
          LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
        end;

        SetCurDir(WinDir);
        FileExpand(TempFileName, GroupDataFileName);
        TempFileHandle:=OpenFile(TempFileName, TempFileStruct,
                                               OF_Create);
        if TempFileHandle>=0 then
        begin
          _lwrite(TempFileHandle, '[group1]'+CRLF, 10);
          _lwrite(TempFileHandle, 'caption=', 8);
          GetPrivateProfileString('GROUP', 'caption', '',
                              TempInsFile, SizeOf(TempInsFile),
                              InstallFile);
          _lwrite(TempFileHandle, TempInsFile, StrLen(TempInsFile));
          _lwrite(TempFileHandle, CRLF, 2);
          _lwrite(TempFileHandle, 'file=', 5);
          GetPrivateProfileString('GROUP', 'file', '',
                              TempInsFile, SizeOf(TempInsFile),
                              InstallFile);
          _lwrite(TempFileHandle, TempInsFile, StrLen(TempInsFile));
          _lwrite(TempFileHandle, CRLF, 2);
          _lwrite(TempFileHandle, '[icons1]'+CRLF, 10);

          GetPrivateProfileString('GROUP', 'def', 'INSTALL.DEF',
                              CurFileName, SizeOf(CurFileName),
                              InstallFile);
          SetCurDir(SrcDir);
          FileExpand(TempFile2, CurFileName);

          TempFile2Handle:=OpenFile(TempFile2, TempFile2Struct, OF_Read);
          if TempFile2Handle>=0 then
          begin
            BufSize:=_lread(TempFile2Handle, FileNameBuffer,
                                      SizeOf(FileNameBuffer));
            Byte(FileNameBuffer[BufSize]):=0;
            _lclose(TempFile2Handle);
            APointer:=FileNameBuffer;
            while StrPos(APointer, '$HOME')<>nil do
            begin
              _lwrite(TempFileHandle, APointer,
                  LongInt(StrPos(APointer, '$HOME'))-
                  LongInt(APointer));
              APointer:=StrPos(APointer, '$HOME')+5;
              _lwrite(TempFileHandle, DestDir, StrLen(DestDir));
            end;
            _lwrite(TempFileHandle, APointer, StrLen(APointer));
          end;

          _lclose(TempFileHandle);

          FileExpand(TempFileName, GroupRunFileName);
          MessageBox(HWindow, 'Installation Complete!', 'Installation',
                              mb_Ok);
          ShellExecute(0, 'open', TempFileName, GroupDataFileName,
                                  WinDir, sw_ShowNormal);

        end else
          MessageBox(HWindow, 'Error Creating Group Control File', 'Installation',
                              mb_Ok + mb_IconExclamation);

        PostQuitMessage(0);
      end;
    end;
  end;

end;

{ TInstallApp Methods: --------------------------------------------------- }

procedure TInstallApp.InitMainWindow;
begin
  MainWindow := New(PInstallWindow, Init(nil, 'Multipli Installation'));
end;

procedure TInstallApp.InitInstance;
begin
  inherited InitInstance;
{  HAccTable:=LoadAccelerators(HInstance, 'HOTKEYS');  }
end;

var
  InstallApp: TInstallApp;
  ExitSave: Pointer;

procedure GoodExit; far;
begin
  ExitProc:=ExitSave;
  if ((ErrorAddr<>nil) and (ExitCode<>0)) then
    MessageBox(GetLastActivePopUp(InstallApp.MainWindow^.HWindow),
                         'WARNING! A Run-Time Error has occured... contact '+
                         'Multipli Machinery with the error code and error '+
                         'address that you are about to be given.',
                         'Run-Time Error', mb_OK + mb_IconExclamation);
end;

begin
  ExitSave:=ExitProc;
  ExitProc:=@GoodExit;
  InstallApp.Init('InstallApp');
  InstallApp.Run;
  InstallApp.Done;
end.
