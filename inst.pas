{

            Machine Study Program

    Copyright(c)1994 Multipli Machinery

         Written by Donald Whisnant

        Instruction Dialog Unit V1.0

}

unit Inst;

{$R INST.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs;

const
  TitleID = 1000;
  InstID = 1001;
  PrevID = 1101;
  NextID = 1102;
  PageNumID = 1201;
  NumOfPagesID = 1202;
  PageBuffer=4096;
  TitleBuffer=255;

type
  PInstDialog = ^TInstDialog;
  TInstDialog = object(TDialog)
    InstTitle: array[0..TitleBuffer] of Char;
    Pages: PCollection;
    TitleEdit: PStatic;
    InstEdit: PStatic;
    PageNumEdit: PStatic;
    NumOfPagesEdit: PStatic;
    PrevButton,
    NextButton: PButton;
    CurrentPage: Integer;
    constructor Init(AParent: PWindowsObject; AName: PChar);
    destructor Done; virtual;
    procedure SetupWindow; virtual;
    procedure SetInstTitle(ATitle: PChar); virtual;
    procedure InsertPage; virtual;
    procedure InsertTextOnPage(PageNumber: Integer; AText: PChar); virtual;
    procedure DoNextButton(var Msg: TMessage);
      virtual Id_First + NextID;
    procedure DoPrevButton(var Msg: TMessage);
      virtual Id_First + PrevID;
  end;

  function MakeInstDialog(AParent: PWindowsObject): PInstDialog;

implementation
uses WinProcs, Strings;

type
  PInstString = ^TInstString;
  TInstString = object(TObject)
    InstText: array[0..PageBuffer] of Char;
  end;

{ TInstDialog Methods ---------------------------------------------------- }
constructor TInstDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  TitleEdit:=New(PStatic, InitResource(@Self, TitleID, 0));
  InstEdit:=New(PStatic, InitResource(@Self, InstId, 0));
  PrevButton:=New(PButton, InitResource(@Self, PrevID));
  NextButton:=New(PButton, InitResource(@Self, NextID));
  PageNumEdit:=New(PStatic, InitResource(@Self, PageNumID, 0));
  NumOfPagesEdit:=New(PStatic, InitResource(@Self, NumOfPagesID, 0));
  Pages:=New(PCollection, Init(10, 5));
  CurrentPage:=0;
end;

destructor TInstDialog.Done;
begin
  Dispose(Pages, Done);
  inherited Done;
end;

procedure TInstDialog.SetupWindow;
var
  Temp: array[0..5] of Char;
begin
  inherited SetupWindow;
  TitleEdit^.SetText(InstTitle);
  if Pages^.Count>0 then CurrentPage:=1;
  if Pages^.Count<=1 then
    EnableWindow(NextButton^.HWindow, False);
  EnableWindow(PrevButton^.HWindow, False);
  Str(CurrentPage, Temp);
  PageNumEdit^.SetText(Temp);
  Str(Pages^.Count, Temp);
  NumOfPagesEdit^.SetText(Temp);
  if CurrentPage=1 then
    InstEdit^.SetText(PInstString(Pages^.At(CurrentPage-1))^.InstText);
end;

procedure TInstDialog.SetInstTitle(ATitle: PChar);
begin
  StrCopy(InstTitle, ATitle);
end;

procedure TInstDialog.InsertPage;
var
  NewStr: PInstString;
begin
  NewStr:=New(PInstString, Init);
  Pages^.Insert(NewStr);
  StrCopy(NewStr^.InstText, '');
end;

procedure TInstDialog.InsertTextOnPage(PageNumber: Integer; AText: PChar);
begin
  if ((PageNumber<=Pages^.Count) and (PageNumber>0)) then
    with PInstString(Pages^.At(PageNumber-1))^ do
      StrCat(InstText, AText);
end;

procedure TInstDialog.DoNextButton(var Msg: TMessage);
var
  Temp: array[0..5] of Char;
begin
  if Pages^.Count>CurrentPage then
  begin
    Inc(CurrentPage);
    InstEdit^.SetText(PInstString(Pages^.At(CurrentPage-1))^.InstText);
    if CurrentPage=Pages^.Count then
      EnableWindow(NextButton^.HWindow, False);
    if CurrentPage>1 then
      EnableWindow(PrevButton^.HWindow, True);
    Str(CurrentPage, Temp);
    PageNumEdit^.SetText(Temp);
  end;
end;

procedure TInstDialog.DoPrevButton(var Msg: TMessage);
var
  Temp: array[0..5] of Char;
begin
  if CurrentPage>1 then
  begin
    Dec(CurrentPage);
    InstEdit^.SetText(PInstString(Pages^.At(CurrentPage-1))^.InstText);
    if CurrentPage=1 then
      EnableWindow(PrevButton^.HWindow, False);
    if CurrentPage<Pages^.Count then
      EnableWindow(NextButton^.HWindow, True);
    Str(CurrentPage, Temp);
    PageNumEdit^.SetText(Temp);
  end;
end;

{ ------------------------------------------------------------------------ }

function MakeInstDialog(AParent: PWindowsObject): PInstDialog;
begin
  if BWCCClassNames then
    MakeInstDialog:=New(PInstDialog, Init(AParent, 'INSTRUCTIONSB'))
  else
    MakeInstDialog:=New(PInstDialog, Init(AParent, 'INSTRUCTIONS'));
end;

end.
