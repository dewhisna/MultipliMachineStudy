{

            Machine Study V1.1

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

  Printable Dialog Chain Definitions V1.0

}

unit PrObj;

{$R PROBJ.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs, OPrinter, OStdWnds;

const
  PrintID = 107;
  HelpID = 998;
  PrintEditID = 3001;

type
  PPrDialog = ^TPrDialog;
  TPrDialog = object(TDialog)
    procedure DoPrint(AEdit: PEdit); virtual;
    procedure PrintIt(var Msg: TMessage);
      virtual Id_First + PrintID;
    procedure HelpIt(var Msg: TMessage);
      virtual Id_First + HelpID;
  end;

  PPrWindow = ^TPrWindow;
  TPrWindow = object(TFileWindow)
    Printer: PPrinter;                  { Printer Object }
    PrintTitle:  array[0..255] of Char;
    constructor Init(AParent: PWindowsObject; ATitle: PChar);
    destructor Done; virtual;
    procedure SetupWindow; virtual;
    procedure DoPrint(AEdit: PEdit); virtual;
    procedure PrintIt(var Msg: TMessage);
      virtual wm_First + PrintID;
  end;

implementation
uses WinProcs, WinDos, Strings;

const
  PrintTitleID = 2001;

type
  TPrintTitleRecord = record
    Title: array[0..255] of Char;
  end;

  PPrintTitleDialog = ^TPrintTitleDialog;
  TPrintTitleDialog = object(TDialog)
    TitleEdit: PEdit;
    constructor Init(AParent: PWindowsObject; ATitle: PChar);
  end;

{ TPrintTitleDialogMethods ----------------------------------------------- }
constructor TPrintTitleDialog.Init(AParent: PWindowsObject; ATitle: PChar);
begin
  inherited Init(AParent, ATitle);
  TitleEdit:=New(PEdit, InitResource(@Self, PrintTitleID, 255));
end;

{ TPrDialog Methods ------------------------------------------------------ }

{ DoPrint is an abstract null procedure that is overriden by children
    objects to perform printing of that object }
procedure TPrDialog.DoPrint(AEdit: PEdit);
begin
  if Parent<>Application^.MainWindow then PPrDialog(Parent)^.DoPrint(AEdit);
end;

procedure TPrDialog.PrintIt(var Msg: TMessage);
var
  ThePrint: PPrintOut;
begin
  SendMessage(Application^.MainWindow^.HWindow, PrintID, 0, LongInt(@Self));
end;

procedure TPrDialog.HelpIt(var Msg: TMessage);
var
  HFileNameLen: Integer;
  HFileName: array[0..fsPathName+1] of Char;
  I: Integer;
begin
  HFileNameLen:=GetModuleFileName(HInstance, HFileName, fsPathName);
  I:=HFileNameLen-1;

  while (I<>0) and (HFileName[I]<>'.') do
    Dec(I);
  Inc(I);
  if I+13<=fsPathName then
    StrCopy(@HFileName[I], 'HLP')
  else
    StrCopy(@HFileName[I], '?');

  WinHelp(HWindow, HFileName, Help_Index, 0);
end;

{ TPrWindow Methods ------------------------------------------------------ }

constructor TPrWindow.Init(AParent: PWindowsObject; ATitle: PChar);
begin
  inherited Init(AParent, ATitle, '');
  Printer:=New(PPrinter, Init);
  StrCopy(PrintTitle, '');
end;

destructor TPrWindow.Done;
begin
  Dispose(Printer, Done);
  inherited Done;
end;

procedure TPrWindow.SetupWindow;
var
  WindowSize: TRect;
begin
  inherited SetupWindow;
  GetClientRect(HWindow, WindowSize);
  with WindowSize do
    MoveWindow(Editor^.HWindow, 0, 0, Right-Left, Bottom-Top, True);
end;

{ DoPrint is an abstract null procedure that is overriden by children
    objects to perform printing of that object }
procedure TPrWindow.DoPrint(AEdit: PEdit);
begin
end;

procedure TPrWindow.PrintIt(var Msg: TMessage);
var
  ThePrint: PEditPrintOut;
  PrintTitleDlg: PPrintTitleDialog;
  PrintTitleData: TPrintTitleRecord;
  AppendIt: Integer;
  AppendFlag: Boolean;
begin
  AppendFlag:=False;
  if Editor^.GetTextLen>0 then
  begin
    AppendIt:=MessageBox(PPrDialog(Msg.lParam)^.HWindow,
                         'Append to Print File?',
                         'Print', mb_yesno);
    if AppendIt=idno then
      Editor^.Clear
    else
    begin
      Editor^.SetSelection(Editor^.GetTextLen, Editor^.GetTextLen);
      AppendFlag:=True;
    end;
  end;

  if BWCCClassNames then
    PrintTitleDlg:=New(PPrintTitleDialog, Init(PPrDialog(Msg.lParam), 'PRINT_TITLEB'))
  else
    PrintTitleDlg:=New(PPrintTitleDialog, Init(PPrDialog(Msg.lParam), 'PRINT_TITLE'));

  StrCopy(PrintTitleData.Title, PrintTitle);
  PrintTitleDlg^.TransferBuffer:=@PrintTitleData;
  if Application^.ExecDialog(PrintTitleDlg)<>id_Cancel then
    StrCopy(PrintTitle, PrintTitleData.Title);

  if not AppendFlag then
  begin
    Editor^.Insert('Machine Study Data Printout'+^M+^J+^M+^J);
    Editor^.Insert('Machine Study Software v1.1'+^M+^J);
    Editor^.Insert('Copyright(c)1994 Multipli Machinery'+^M+^J+^M+^J);
  end;

    Editor^.Insert('Customer: ');
    Editor^.Insert(PrintTitle);
    Editor^.Insert(^M+^J+^M+^J);

  PPrDialog(Msg.lParam)^.DoPrint(Editor);

  ThePrint:=New(PEditPrintOut, Init(Editor, 'Machine Study Printout'));
  Printer^.Print(PPrDialog(Msg.lParam), ThePrint);
  Dispose(ThePrint, Done);
end;

end.
