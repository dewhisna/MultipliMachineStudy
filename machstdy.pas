{

         Machine Study Program V1.1

    Copyright(c)1994 Multipli Machinery

         Written by Donald Whisnant

}


program MachStudy;

uses WinProcs, WinTypes, Objects, OWindows, ODialogs, WinDos,
     BWCC, OPrinter, Win31, Strings, PrObj, Form, Flow, UserInfo;

{$R MACHSTDY.RES}
{$F+}

{$I CONV.INC}

{ General Constants: }
const
  SNumWidth = 12;

  NumWPrec = 8;
  NumDPrec = 5;

{ Menu bar constants }
  MenuID              = 100; { Resource ID of the menu }

  FPrintID            = 105; { File->Print ID }
  FPrintSetupID       = 106; { File->Print Setup ID }
  QuitID              = 109; { File->Quit ID }

  HelpID              = 998; { Help->Help ID }
  AboutID             = 999; { Help->About ID }

  MetricID            = 101; { Setup->Metric ID }

  FlowPipeID          = 201; { Flow->Pipe ID }
  FlowAreaID          = 202; { Flow->Area ID }

  FormID              = 301; { Formation->Formation ID }
  ProdID              = 302; { Formation->Production ID }
  GradeID             = 303; { Formation->Grade ID }
  HeadBoxID           = 304; { Formation->HeadBox ID }
  FormConstID         = 305; { Formation->Formation Const ID }

  NipWidthID          = 401; { Deflection->Nip Width ID }

  UserInfoFileName    = 'MACHSTDY.USR';
  HelpFileName        = 'MACHSTDY.HLP';

type

{ TMachStdyWindow Definitions: }
  PMachStdyWindow = ^TMachStdyWindow;
  TMachStdyWindow = object(TPrWindow)
    Metric: Boolean;

    FormData: TFormData;

    constructor Init(AParent: PWindowsObject; ATitle: PChar);

    procedure SetupWindow; virtual;

    procedure DoFPrint(var Msg: TMessage);
      virtual cm_First + FPrintID;
    procedure DoFPrintSetup(var Msg: TMessage);
      virtual cm_First + FPrintSetupID;
    procedure Quit(var Msg: TMessage);
      virtual cm_First + QuitID;
    procedure AboutBox(var Msg: TMessage);
      virtual cm_First + AboutID;

    procedure DoHelp(var Msg: TMessage);
      virtual cm_First + HelpID;

    procedure ToggleMetric(var Msg: TMessage);
      virtual cm_First + MetricID;

    procedure FlowPipe(var Msg: TMessage);
      virtual cm_First + FlowPipeID;
    procedure FlowArea(var Msg: TMessage);
      virtual cm_First + FlowAreaID;

    procedure Form(var Msg: TMessage);
      virtual cm_First + FormID;
    procedure Prod(var Msg: TMessage);
      virtual cm_First + ProdID;
    procedure HeadBox(var Msg: TMessage);
      virtual cm_First + HeadBoxID;
    procedure FormConst(var Msg: TMessage);
      virtual cm_First + FormConstID;
    procedure Grade(var Msg: TMessage);
      virtual cm_First + GradeID;

{
    procedure NipWidth(var Msg: TMessage);
      virtual cm_First + NipWidthID;
}

{    procedure WMGetMinMaxInfo(var Msg: TMessage);
      virtual wm_First+ wm_GetMinMaxInfo;
    procedure WMTimer(var Msg: TMessage);
      virtual wm_First + wm_Timer;
    procedure WMDestroy(var Msg: TMessage);
      virtual wm_First + wm_Destroy;
}
    procedure GetWindowClass(var AWndClass: TWndClass); virtual;
    function GetClassName: PChar; virtual;

  end;

{ TMachStdy Definitions: }
  TMachStdyApp = object(TApplication)
    procedure InitMainWindow; virtual;
    procedure InitInstance; virtual;
  end;

{ Miscellaneous Procedures/Functions ------------------------------------- }

function Real2String(aReal: Real; NumWidth, NumPrec: Integer): string;
var
  TempStr1, TempStr2: string;
  i: integer;
begin
  Str(aReal:NumWidth:NumPrec, TempStr1);
  TempStr2:='';
  for i:=1 to Length(TempStr1) do
    if TempStr1[i]<>' ' then TempStr2:=TempStr2+TempStr1[i];
  Real2String:=TempStr2;
end;

procedure ConvertEtoM(AStr: PChar; ConvFactor: Real);
var
  TempVal: Real;
  ErrorPos: Integer;
begin
  Val(AStr, TempVal, ErrorPos);
  TempVal:=TempVal*ConvFactor;
  StrPCopy(AStr, Real2String(TempVal, NumWPrec, NumDPrec));
end;

procedure ConvertMtoE(AStr: PChar; ConvFactor: Real);
var
  TempVal: Real;
  ErrorPos: Integer;
begin
  Val(AStr, TempVal, ErrorPos);
  TempVal:=TempVal/ConvFactor;
  StrPCopy(AStr, Real2String(TempVal, NumWPrec, NumDPrec));
end;

{ TMachStdyWindow Definitions: ------------------------------------------- }
constructor TMachStdyWindow.Init(AParent: PWindowsObject; ATitle: PChar);
var
  i: integer;
begin
  inherited Init(AParent, ATitle);
  Attr.Menu:=LoadMenu(HInstance, MakeIntResource(MenuID));

  Metric:=False;
  CheckMenuItem(GetMenu(HWindow), MetricID, mf_ByCommand+mf_UnChecked);
{  StreamRegistration;  }

  with FormData do
  begin
    StrCopy(NumVats, '1');
    StrCopy(Caliper, '');
    StrCopy(Density, '');
    StrCopy(BasisWeight, '');
    StrCopy(SheetWidth, '');
    StrCopy(FormConst, '');
    for i:=0 to NumCylTypes do CylType[i]:=bf_UnChecked;
    CylType[1]:=bf_Checked;
    for i:=0 to NumBoardTypes do BoardType[i]:=bf_UnChecked;
    BoardType[0]:=bf_Checked;
    OtherBox:=bf_UnChecked;
    TPDCalcBox:=bf_Checked;
    StrCopy(TPD, '');
    StrCopy(Cons, '');
    StrCopy(GPM, '');
    StrCopy(GPMV, '');
    StrCopy(MS, '');
    StrCopy(MachEff, '100');
  end;
end;

procedure TMachStdyWindow.SetupWindow;
var
  Result: Integer;
  Msg: TMessage;
begin
  Show(sw_ShowMaximized);
  inherited SetupWindow;

{  Result := IDRetry;
  while (SetTimer(hWIndow, 0, TickRate, nil) = 0) and (Result = IDRetry) do
    Result := MessageBox(GetFocus,'Could not Create Timer', 'Machine Study',
      mb_RetryCancel);
  if Result = IDCancel then PostQuitMessage(0);
}

  if ShowUserInfo(@Self, 'Cylinder Machine Study Program V1.1',
  		         UserInfoFileName, True)<>0 then
    PostQuitMessage(-1);

  Form(Msg);
end;

procedure TMachStdyWindow.DoFPrintSetup(var Msg: TMessage);
begin
  Printer^.Setup(@Self);
end;

procedure TMachStdyWindow.DoFPrint(var Msg: TMessage);
var
  ThePrint: PEditPrintOut;
begin
  ThePrint:=New(PEditPrintOut, Init(Editor, 'Machine Study Printout'));
  Printer^.Print(@Self, ThePrint);
  Dispose(ThePrint, Done);
end;

procedure TMachStdyWindow.Quit(var Msg: TMessage);
begin
  CloseWindow;
end;

procedure TMachStdyWindow.AboutBox(var Msg: TMessage);
begin
  Application^.ExecDialog(New(PDialog, Init(@Self, 'ABOUTBOX')));
end;

procedure TMachStdyWindow.DoHelp(var Msg: TMessage);
var
  HFileNameLen: Integer;
  HFileName: array[0..fsPathName+1] of Char;
  I: Integer;
begin
  HFileNameLen:=GetModuleFileName(HInstance, HFileName, fsPathName);
  I:=HFileNameLen-1;
  while (I<>0) and ((HFileName[I]<>'\') and (HFileName[I]<>':')) do
    Dec(I);
  Inc(I);
  if I+13<=fsPathName then
    StrCopy(@HFileName[I], HelpFileName)
  else
    StrCopy(@HFileName[I], '?');

  WinHelp(HWindow, HFileName, Help_Index, 0);
end;

procedure TMachStdyWindow.ToggleMetric(var Msg: TMessage);
var
  TempVal: Real;
  ErrorPos: Integer;
begin
  Metric:=not Metric;
  if not Metric then
  begin
    CheckMenuItem(GetMenu(HWindow), MetricID, mf_ByCommand+mf_UnChecked);
    with FormData do
    begin
      ConvertMtoE(Caliper, CaliperConv);
      ConvertMtoE(Density, DensityConv);
      ConvertMtoE(BasisWeight, BWConv);
      ConvertMtoE(SheetWidth, SWConv);
      ConvertMtoE(FormConst, FormConv);
      ConvertMtoE(TPD, TPDConv);
      ConvertMtoE(GPM, GPMConv);
      ConvertMtoE(GPMV, GPMConv);
      ConvertMtoE(MS, MSConv);
    end;
  end else
  begin
    CheckMenuItem(GetMenu(HWindow), MetricID, mf_ByCommand+mf_Checked);
    with FormData do
    begin
      ConvertEtoM(Caliper, CaliperConv);
      ConvertEtoM(Density, DensityConv);
      ConvertEtoM(BasisWeight, BWConv);
      ConvertEtoM(SheetWidth, SWConv);
      ConvertEtoM(FormConst, FormConv);
      ConvertEtoM(TPD, TPDConv);
      ConvertEtoM(GPM, GPMConv);
      ConvertEtoM(GPMV, GPMConv);
      ConvertEtoM(MS, MSConv);
    end;
  end;
end;

procedure TMachStdyWindow.FlowPipe(var Msg: TMessage);
begin
  DoFlowPipe(@Self, Metric, '');
end;

procedure TMachStdyWindow.FlowArea(var Msg: TMessage);
begin
  DoFlowArea(@Self, Metric, '');
end;

procedure TMachStdyWindow.Form(var Msg: TMessage);
begin
  while DoForm(@Self, Metric, FormData)=True do
  begin
    ToggleMetric(Msg);
  end;
end;

procedure TMachStdyWindow.Prod(var Msg: TMessage);
var
  ProdData: TProdData;
begin
  StrCopy(ProdData.Caliper, '');
  StrCopy(ProdData.Density, '');
  StrCopy(ProdData.BasisWeight, '');
  StrCopy(ProdData.SheetWidth, '');
  StrCopy(ProdData.MS, '');
  StrCopy(ProdData.MachEff, '');
  StrCopy(ProdData.TPD, '');

  DoProd(@Self, Metric, ProdData);
end;

procedure TMachStdyWindow.Grade(var Msg: TMessage);
var
  GradeData: TGradeData;
begin
  StrCopy(GradeData.Caliper, '');
  StrCopy(GradeData.Density, '');
  StrCopy(GradeData.BasisWeight, '');

  DoGrade(@Self, Metric, GradeData);
end;

procedure TMachStdyWindow.HeadBox(var Msg: TMessage);
var
  HeadBoxData: THeadBoxData;
begin
  StrCopy(HeadBoxData.TPD, '');
  StrCopy(HeadBoxData.PCons, '');
  StrCopy(HeadBoxData.NCons, '');
  StrCopy(HeadBoxData.Water, '');
  DoHeadBox(@Self, Metric, HeadBoxData);
end;

procedure TMachStdyWindow.FormConst(var Msg: TMessage);
var
  FormCalcData: TFormCalcData;
begin
  StrCopy(FormCalcData.TPD, '');
  StrCopy(FormCalcData.NumVats, '');
  StrCopy(FormCalcData.SheetWidth, '');
  StrCopy(FormCalcData.FormConst, '');

  DoFormationCalc(@Self, Metric, FormCalcData, False);
end;

{
procedure TMachStdyWindow.NipWidth(var Msg: TMessage);
begin
  DoNipWidth(@Self, Metric);
end;
}


{ Specify limits for max and min of window size }
{
procedure TLaserWindow.WMGetMinMaxInfo(var Msg: TMessage);
var
  APoint: TPoint;
begin
  APoint.x:=MinScannerWindowX+16;
  APoint.y:=MinScannerWindowY+56;
  PMinMaxInfo(Msg.LParam)^.ptMinTrackSize:=APoint;
  DefWndProc(Msg);
end;

}
{ In response to WMTimer messages, each MDI child window's TimerTick
  Method is called. }
{
procedure TLaserWindow.WMTimer(var Msg: TMessage);

  procedure ChildTimers(PChildWindow: PBaseWindow); far;
  begin
    PChildWindow^.TimerTick;
  end;

begin
  ForEach(@ChildTimers);
end;
}

{
procedure TLaserWindow.WMDestroy(var Msg: TMessage);
begin
  KillTimer(HWindow, 0);
  inherited WMDestroy(Msg);
end;
}

procedure TMachStdyWindow.GetWindowClass(var AWndClass: TWndClass);
begin
  inherited GetWindowClass(AWndClass);
  AWndClass.hIcon := LoadIcon(HInstance, 'LOGO');
end;

function TMachStdyWindow.GetClassName: PChar;
begin
  GetClassName:='Multipli Machine Study';
end;


{ TMachStdyApp Methods: -------------------------------------------------- }

procedure TMachStdyApp.InitMainWindow;
begin
  { Create a main window of type TMachStdyWindow. }
  MainWindow := New(PMachStdyWindow, Init(nil, 'Multipli Machine Study'));
end;

procedure TMachStdyApp.InitInstance;
begin
  inherited InitInstance;
{  HAccTable:=LoadAccelerators(HInstance, 'HOTKEYS');  }
end;

var
  MachStdyApp: TMachStdyApp;
  ExitSave: Pointer;

procedure GoodExit; far;
begin
  ExitProc:=ExitSave;
  if ((ErrorAddr<>nil) and (ExitCode<>0)) then
    MessageBox(GetLastActivePopUp(MachStdyApp.MainWindow^.HWindow),
                         'WARNING! A Run-Time Error has occured... contact '+
                         'Multipli Machinery with the error code and error '+
                         'address that you are about to be given.',
                         'Run-Time Error', mb_OK + mb_IconExclamation);
end;

begin
  ExitSave:=ExitProc;
  ExitProc:=@GoodExit;
  MachStdyApp.Init('MachStdyApp');
  MachStdyApp.Run;
  MachStdyApp.Done;
end.
