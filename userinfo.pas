{

       User Info Display Unit V1.1

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

}

unit UserInfo;

{$R USERINFO.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs;

function ShowUserInfo(AParent: PWindowsObject; ProgramName: PChar;
                                               AFileName: PChar;
                                               Prompt: Boolean): Integer;

implementation
uses WinProcs, WinDos, Strings;

const
  StrSize = 80;

type
  PUserData = ^TUserData;
  TUserData = record
    ProgramName: array[0..StrSize] of Char;
    UserName: array[0..StrSize] of Char;
    CoName: array[0..StrSize] of Char;
    SerNum: array[0..StrSize] of Char;
  end;

  PUserInfoDialog = ^TUserInfoDialog;
  TUserInfoDialog = object(TDialog)
    ProgramNameEdit: PStatic;
    UserNameEdit: PStatic;
    CoNameEdit: PStatic;
    SerNumEdit: PStatic;
    constructor Init(AParent: PWindowsObject; AName: PChar);
  end;

{ Miscellaneous Procedures/Functions ------------------------------------- }

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

{ TUserInfoDialog Methods ------------------------------------------------ }

constructor TUserInfoDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  ProgramNameEdit:=New(PStatic, InitResource(@Self, 101, StrSize+1));
  UserNameEdit:=New(PStatic, InitResource(@Self, 102, StrSize+1));
  CoNameEdit:=New(PStatic, InitResource(@Self, 103, StrSize+1));
  SerNumEdit:=New(PStatic, InitResource(@Self, 104, StrSize+1));
end;

{ ------------------------------------------------------------------------ }

function ShowUserInfo(AParent: PWindowsObject; ProgramName: PChar;
                                               AFileName: PChar;
                                               Prompt: Boolean): Integer;
var
  UserInfoDlg: PUserInfoDialog;
  UserData: TUserData;
  UserInfoFileHandle: Integer;
  UserInfoFile: array[0..fsPathName+1] of Char;
  UserInfoFileStruct: TOFStruct;
  ReadBuffer: array[0..3*StrSize+2] of Char;
  APointer: PChar;
  FileNameLen: Integer;
  I: Integer;
  OKFlag: Boolean;
begin
  { Construct FileName from Module Name }
  FileNameLen := GetModuleFileName(HInstance, UserInfoFile, fsPathName);
  I := FileNameLen - 1;
  while (I <> 0) and ((UserInfoFile[I] <> '\') and (UserInfoFile[I] <> ':')) do
    Dec(I);
  Inc(I);
  if I + 13 <= fsPathName then
    StrCopy(@UserInfoFile[I], AFileName)
  else
    StrCopy(@UserInfoFile[I], '?');

  If Prompt then
    UserInfoFileHandle:=OpenFile(UserInfoFile, UserInfoFileStruct,
                                                     OF_Prompt+OF_Cancel+
                                                     OF_Read)
  else
  begin
    UserInfoFileHandle:=OpenFile(UserInfoFile, UserInfoFileStruct,
                                                     OF_Exist);
    if UserInfoFileHandle>=0 then
      UserInfoFileHandle:=OpenFile(UserInfoFile, UserInfoFileStruct,
                                                       OF_Prompt+OF_Cancel+
                                                       OF_Read);
  end;

  OKFlag:=True;
  if UserInfoFileHandle>=0 then
  begin
    Byte(ReadBuffer[_lread(UserInfoFileHandle, ReadBuffer, SizeOf(ReadBuffer))]):=0;

    if ((ReadBuffer[0]<>chr(170)) or (ReadBuffer[1]<>chr(85))) then
      OKFlag:=False;
    APointer:=ReadBuffer;

    StrCopy(UserData.ProgramName, ProgramName);

    LongInt(APointer):=LongInt(APointer)+2;
    StrCopy(UserData.UserName, APointer);

    LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
    StrCopy(UserData.CoName, APointer);

    LongInt(APointer):=LongInt(APointer)+StrLen(APointer)+1;
    StrCopy(UserData.SerNum, APointer);

    Decode(UserData.UserName);
    Decode(UserData.CoName);
    Decode(UserData.SerNum);

    _lclose(UserInfoFileHandle);

    if OKFlag then
    begin
      UserInfoDlg:=New(PUserInfoDialog, Init(AParent, 'USERINFO'));
      UserInfoDlg^.TransferBuffer:=@UserData;
      Application^.ExecDialog(UserInfoDlg);

      ShowUserInfo:=0;
    end else
    ShowUserInfo:=-1;
  end else
    ShowUserInfo:=-1;
end;

end.
