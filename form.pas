{

             Machine Study

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

  Sheet Formation Analysis Subprogram V1.1

}

unit Form;

{$R FORM.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs, ASEdit, PrObj;

const
  NumCylTypes = 2;
  NumBoardTypes = 1;

  SNumWidth = 12;

  NumWPrec = 8;
  NumDPrec = 5;

  HelpID = 998;

  NumVatsID = 1000;
  CaliperID = 1100;
  DensityID = 1101;
  BasisWeightID = 1102;
  SheetWidthID = 1103;
  FormConstID = 1104;
  CylTypeBaseID = 1111;
  BoardTypeBaseID = 1121;
  OtherID = 1131;

  TPDID = 1200;
  ConsID = 1201;
  GPMID = 1300;
  GPMPVID = 1301;
  MSID = 1400;
  EffID = 1500;
  CalcItID = 2000;
  TPDLinkID = 2001;

  PipeSizeID = 2002;
  AreaSizeID = 2003;
  ProdCalcID = 2004;
  HeadBoxID = 2005;

  CalcNumVatsID = 2006;
  GradeCalcID = 2007;
  ModeToggleID = 2200;

  PseudoXferID = 4001;
  PseudoRestoreID = 4002;

  CTypeConst : array[0..NumCylTypes] of Real =
             (0.852, 1.0, 1.126);

  CTypeNames : array[0..NumCylTypes, 0..40] of Char =
             ('36 in. Cylinder', '42 in. Cylinder.', '48 in. Cylinder');

  BTypeConst : array[0..NumBoardTypes] of Real =
             (0.217, 0.192);

  BTypeNames : array[0..NumBoardTypes, 0..40] of Char =
             ('Folding Box Board', 'Tube Board');

  ProdSheetWidthID = 1101;
  ProdMSID = 1102;
  ProdEffID = 1103;
  ProdCaliperID = 1150;
  ProdDensityID = 1151;
  ProdBWID = 1152;
  ProdTPDID = 1201;
  ProdCalcItID = 1300;
  ProdTransferID = 1700;
  ProdGradeCalcID = 1400;

  GradeCaliperCalcID = 1001;
  GradeDensityCalcID = 1002;
  GradeBWCalcID = 1003;
  GradeCaliperID = 2001;
  GradeDensityID = 2002;
  GradeBWID = 2003;
  GradeTransferID = 1400;

  HBoxTPDID = 1101;
  HBoxPConsID = 1102;
  HBoxNConsID = 1103;
  HBoxWaterID = 1201;
  HBoxCalcID = 1300;

  FormTPDID = 1101;
  FormNumVatsID = 1102;
  FormSWID = 1103;
  FormFormConstID = 1201;
  FormCalcID = 1300;
  FormTransferID = 1400;

type
  PValEdit = ^TValEdit;
  TValEdit = object(TASEdit)
    procedure WMKillFocus(var Msg: TMessage);
      virtual wm_First + wm_KillFocus;
  end;

  PFormData = ^TFormData;
  TFormData = record
    NumVats: array[0..SNumWidth] of Char;
    Caliper: array[0..SNumWidth] of Char;
    Density: array[0..SNumWidth] of Char;
    BasisWeight: array[0..SNumWidth] of Char;
    SheetWidth: array[0..SNumWidth] of Char;
    FormConst: array[0..SNumWidth] of Char;
    CylType: array[0..NumCylTypes] of Integer;
    BoardType: array[0..NumBoardTypes] of Integer;
    OtherBox: Integer;
    TPDCalcBox: Integer;
    TPD: array[0..SNumWidth] of Char;
    Cons: array[0..SNumWidth] of Char;
    GPM: array[0..SNumWidth] of Char;
    GPMV: array[0..SNumWidth] of Char;
    MS: array[0..SNumWidth] of Char;
    MachEff: array[0..SNumWidth] of Char;
  end;

  PProdData = ^TProdData;
  TProdData = record
    SheetWidth: array[0..SNumWidth] of Char;
    MS: array[0..SNumWidth] of Char;
    MachEff: array[0..SNumWidth] of Char;
    Caliper: array[0..SNumWidth] of Char;
    Density: array[0..SNumWidth] of Char;
    BasisWeight: array[0..SNumWidth] of Char;
    TPD: array[0..SNumWidth] of Char;
  end;

  PGradeData = ^TGradeData;
  TGradeData = record
    Caliper: array[0..SNumWidth] of Char;
    Density: array[0..SNumWidth] of Char;
    BasisWeight: array[0..SNumWidth] of Char;
  end;

  THeadBoxData = record
    TPD: array[0..SNumWidth] of Char;
    PCons: array[0..SNumWidth] of Char;
    NCons: array[0..SNumWidth] of Char;
    Water: array[0..SNumWidth] of Char;
  end;

  PFormCalcData = ^TFormCalcData;
  TFormCalcData = record
    TPD: array[0..SNumWidth] of Char;
    NumVats: array[0..SNumWidth] of Char;
    SheetWidth: array[0..SNumWidth] of Char;
    FormConst: array[0..SNumWidth] of Char;
  end;

  GetTPDRecord = record
    TPD: array[0..SNumWidth] of Char;
  end;

  PFormDialog = ^TFormDialog;
  TFormDialog = object(TPrDialog)
    Metric: Boolean;
    NumVatsEdit: PValEdit;
    CaliperEdit: PValEdit;
    DensityEdit: PValEdit;
    BasisWeightEdit: PStatic;
    SheetWidthEdit: PValEdit;
    FormConstEdit: PValEdit;
    CylType: array[0..NumCylTypes] of PRadioButton;
    BoardType: array[0..NumBoardTypes] of PRadioButton;
    OtherBox: PCheckBox;
    TPDCalcBox: PCheckBox;

    TPDEdit: PValEdit;
    ConsEdit: PValEdit;
    GPMEdit: PStatic;
    GPMPVEdit: PStatic;
    MSEdit: PStatic;
    EffEdit: PStatic;

    PseudoSave: TProdData;

    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure SetupWindow; virtual;
    function GetFormationConst: real; virtual;
    procedure CalcBW; virtual;
    procedure CalcTPD; virtual;
    procedure CalcMS; virtual;
    procedure CalcGPM; virtual;

    procedure CalcNumVats(TPD: Real); virtual;

    procedure CylTypeChange1(var Msg: TMessage);
      virtual Id_First + CylTypeBaseID;
    procedure CylTypeChange2(var Msg: TMessage);
      virtual Id_First + CylTypeBaseID+1;
    procedure CylTypeChange3(var Msg: TMessage);
      virtual Id_First + CylTypeBaseID+2;

    procedure BoardTypeChange1(var Msg: TMessage);
      virtual Id_First + BoardTypeBaseID;
    procedure BoardTypeChange2(var Msg: TMessage);
      virtual Id_First + BoardTypeBaseID+1;

    procedure OtherToggle(var Msg: TMessage);
      virtual Id_First + OtherID;
    procedure OtherTurnOn(Automatic: Boolean); virtual;

    procedure TPDLink(var Msg: TMessage);
      virtual Id_First + TPDLinkID;

    procedure DoPipeSize(var Msg: TMessage);
      virtual Id_First + PipeSizeID;
    procedure DoAreaSize(var Msg: TMessage);
      virtual Id_First + AreaSizeID;
    procedure DoProdCalc(var Msg: TMessage);
      virtual Id_First + ProdCalcID;
    procedure DoGradeCalc(var Msg: TMessage);
      virtual Id_First + GradeCalcID;
    procedure DoHBox(var Msg: TMessage);
      virtual Id_First + HeadBoxID;
    procedure DoCalcNumVats(var Msg: TMessage);
      virtual Id_First + CalcNumVatsID;
    procedure CalcIt(var Msg: TMessage);
      virtual Id_First + CalcItID;
    procedure CalcIt2(var Msg: TMessage);
      virtual wm_First + CalcItID;

    procedure DoModeToggle(var Msg: TMessage);
      virtual Id_First + ModeToggleID;
    procedure Cancel(var Msg: TMessage);
      virtual Id_First + id_Cancel;

    procedure PseudoXfer(var Msg: TMessage);
      virtual wm_First + PseudoXferID;
    procedure PseudoRestore(var Msg: TMessage);
      virtual wm_First + PseudoRestoreID;
    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PProdDialog = ^TProdDialog;
  TProdDialog = object(TPrDialog)
    Metric: Boolean;
    SheetWidthEdit: PValEdit;
    MSEdit: PValEdit;
    MachEffEdit: PValEdit;
    CaliperEdit: PValEdit;
    DensityEdit: PValEdit;
    BasisWeightEdit: PStatic;

    TPDEdit: PStatic;

    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure SetupWindow; virtual;
    procedure DoGradeCalc(var Msg: TMessage);
      virtual Id_First + ProdGradeCalcID;
    procedure DoCalc(var Msg: TMessage);
      virtual Id_First + ProdCalcItID;
    procedure CalcIt(var Msg: TMessage);
      virtual wm_First + CalcItID;
    procedure DoTransfer(var Msg: TMessage);
      virtual Id_First + ProdTransferID;
    procedure DoTransfer2(var Msg: TMessage);
      virtual wm_First + ProdTransferID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PGradeDialog = ^TGradeDialog;
  TGradeDialog = object(TPrDialog)
    Metric: Boolean;
    CaliperEdit: PASEdit;
    DensityEdit: PASEdit;
    BasisWeightEdit: PASEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure CalcCaliper(var Msg: TMessage);
      virtual Id_First + GradeCaliperCalcID;
    procedure CalcDensity(var Msg: TMessage);
      virtual Id_First + GradeDensityCalcID;
    procedure CalcBW(var Msg: TMessage);
      virtual Id_First + GradeBWCalcID;
    procedure DoTransfer(var Msg: TMessage);
      virtual Id_First + GradeTransferID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PHeadBoxDialog = ^THeadBoxDialog;
  THeadBoxDialog = object(TPrDialog)
    Metric: Boolean;
    TPDEdit: PValEdit;
    PConsEdit: PValEdit;
    NConsEdit: PValEdit;
    WaterEdit: PStatic;

    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure DoCalc(var Msg: TMessage);
      virtual Id_First + HBoxCalcID;
    procedure CalcIt(var Msg: TMessage);
      virtual wm_First + CalcItID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PFormCalcDialog = ^TFormCalcDialog;
  TFormCalcDialog = object(TPrDialog)
    Metric: Boolean;
    TPDEdit: PValEdit;
    NumVatsEdit: PValEdit;
    SheetWidthEdit: PValEdit;
    FormConstEdit: PStatic;

    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure SetupWindow; virtual;
    procedure DoCalc(var Msg: TMessage);
      virtual Id_First + FormCalcID;
    procedure CalcIt(var Msg: TMessage);
      virtual wm_First + CalcItID;
    procedure DoTransfer(var Msg: TMessage);
      virtual Id_First + FormTransferID;
    procedure DoTransfer2(var Msg: TMessage);
      virtual wm_First + FormTransferID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PGetTPDDialog = ^TGetTPDDialog;
  TGetTPDDialog = object(TDialog)
    constructor Init(AParent: PWindowsObject; AName: PChar);

    procedure DoHelp(var Msg: TMessage);
      virtual Id_First + HelpID;
  end;

function DoForm(AParent: PWindowsObject; MetricFlag: Boolean;
                          var FormData: TFormData): Boolean;

function DoProd(AParent: PWindowsObject; MetricFlag: Boolean;
               var ProdData: TProdData): Real;

function DoGrade(AParent: PWindowsObject; MetricFlag: Boolean;
               var GradeData: TGradeData): Real;

procedure DoHeadBox(AParent: PWindowsObject; MetricFlag: Boolean;
                    var HeadBoxData: THeadBoxData);

function DoFormationCalc(AParent: PWindowsObject; MetricFlag: Boolean;
                         var FormCalcData: TFormCalcData;
                         Automatic: Boolean): Real;

implementation
uses WinProcs, Validize, Strings, Flow;

{$I CONV.INC}

const
  PrintItID = 107;
  CRLF = ^M+^J;

  GetTPDID = 2001;

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

{ TValEdit Methods ------------------------------------------------------- }
procedure TValEdit.WMKillFocus(var Msg: TMessage);
begin
  inherited WMKillFocus(Msg);
  SendMessage(Parent^.HWindow, CalcItID, 0, 0);
end;

{ TFormDialog Methods ---------------------------------------------------- }
constructor TFormDialog.Init(AParent: PWindowsObject; AName: PChar;
                                      MetricFlag: Boolean);
var
  i: integer;
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  NumVatsEdit:=New(PValEdit, InitResource(@Self, NumVatsID, SNumWidth+1));
  NumVatsEdit^.SetValidator(New(PPosNZIntValidator, Init));
  CaliperEdit:=New(PValEdit, InitResource(@Self, CaliperID, SNumWidth+1));
  CaliperEdit^.SetValidator(New(PPosRealValidator, Init));
  DensityEdit:=New(PValEdit, InitResource(@Self, DensityID, SNumWidth+1));
  DensityEdit^.SetValidator(New(PPosRealValidator, Init));
  BasisWeightEdit:=New(PStatic, InitResource(@Self, BasisWeightID, SNumWidth+1));
  SheetWidthEdit:=New(PValEdit, InitResource(@Self, SheetWidthID, SNumWidth+1));
  SheetWidthEdit^.SetValidator(New(PPosRealValidator, Init));
  FormConstEdit:=New(PValEdit, InitResource(@Self, FormConstID, SNumWidth+1));
  FormConstEdit^.SetValidator(New(PPosRealValidator, Init));
  for i:=0 to NumCylTypes do
    CylType[i]:=New(PRadioButton, InitResource(@Self, CylTypeBaseID+i));
  for i:=0 to NumBoardTypes do
    BoardType[i]:=New(PRadioButton, InitResource(@Self, BoardTypeBaseID+i));
  OtherBox:=New(PCheckBox, InitResource(@Self, OtherID));

  TPDCalcBox:=New(PCheckBox, InitResource(@Self, TPDLinkID));

  TPDEdit:=New(PValEdit, InitResource(@Self, TPDID, SNumWidth+1));
  TPDEdit^.SetValidator(New(PPosRealValidator, Init));
  ConsEdit:=New(PValEdit, InitResource(@Self, ConsID, SNumWidth+1));
  ConsEdit^.SetValidator(New(PPosRealValidator, Init));
  GPMEdit:=New(PStatic, InitResource(@Self, GPMID, SNumWidth+1));
  GPMPVEdit:=New(PStatic, InitResource(@Self, GPMPVID, SNumWidth+1));
  MSEdit:=New(PStatic, InitResource(@Self, MSID, SNumWidth+1));
  EffEdit:=New(PStatic, InitResource(@Self, EffID, SNumWidth+1));
end;

procedure TFormDialog.SetupWindow;
var
  TempStr: array[0..SNumWidth+1] of Char;
  i: Integer;
begin
  inherited SetupWindow;
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  if OtherBox^.GetCheck=bf_UnChecked then
  begin
    for i:=0 to NumCylTypes do
      EnableWindow(CylType[i]^.HWindow, True);
    for i:=0 to NumBoardTypes do
      EnableWindow(BoardType[i]^.HWindow, True);
    EnableWindow(FormConstEdit^.HWindow, False)
  end else
  begin
    for i:=0 to NumCylTypes do
      EnableWindow(CylType[i]^.HWindow, False);
    for i:=0 to NumBoardTypes do
      EnableWindow(BoardType[i]^.HWindow, False);
    EnableWindow(FormConstEdit^.HWindow, True);
  end;
end;

procedure TFormDialog.OtherToggle(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
  i: integer;
begin
  if OtherBox^.GetCheck=bf_UnChecked then
    OtherTurnOn(False)
  else
  begin
    TPDCalcBox^.Check;
    OtherBox^.UnCheck;
    for i:=0 to NumCylTypes do
      EnableWindow(CylType[i]^.HWindow, True);
    for i:=0 to NumBoardTypes do
      EnableWindow(BoardType[i]^.HWindow, True);
    StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
    FormConstEdit^.SetText(TempStr);
    EnableWindow(FormConstEdit^.HWindow, False);
  end;

  CalcIt2(Msg);
end;

procedure TFormDialog.OtherTurnOn(Automatic: Boolean);
var
  FormCalcData: TFormCalcData;
  TPD,
  MachEff: Real;
  FormConst: Real;
  ErrorPos: integer;
  i: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  EffEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MachEff, ErrorPos);

  if MachEff<>0 then TPD:=TPD/(MachEff/100);
  StrPCopy(FormCalcData.TPD, Real2String(TPD, NumWPrec, NumDPrec));

  SheetWidthEdit^.GetText(FormCalcData.SheetWidth, SNumWidth);
  NumVatsEdit^.GetText(FormCalcData.NumVats, SNumWidth);
  StrCopy(FormCalcData.FormConst, '');
  FormConst:=DoFormationCalc(@Self, Metric, FormCalcData, Automatic);

  if FormConst<>-1 then
  begin
    TPDCalcBox^.Check;
    FormConstEdit^.SetText(FormCalcData.FormConst);
    OtherBox^.Check;
    for i:=0 to NumCylTypes do
      EnableWindow(CylType[i]^.HWindow, False);
    for i:=0 to NumBoardTypes do
      EnableWindow(BoardType[i]^.HWindow, False);
    EnableWindow(FormConstEdit^.HWindow, True);
  end;
end;

procedure TFormDialog.TPDLink(var Msg: TMessage);
begin
  if TPDCalcBox^.GetCheck=bf_Checked then TPDCalcBox^.UnCheck
    else TPDCalcBox^.Check;
  CalcIt2(Msg);
end;

function TFormDialog.GetFormationConst: Real;
var
  FormationConst: Real;
  CConst, BConst: Real;
  TempStr: array[0..SNumWidth+1] of Char;
  i: integer;
  ErrorPos: integer;
begin
  if OtherBox^.GetCheck=bf_Checked then
  begin
    FormConstEdit^.GetText(TempStr, SNumWidth);
    Val(TempStr, FormationConst, ErrorPos);
  end else
  begin
    for i:=0 to NumCylTypes do
      if CylType[i]^.GetCheck=bf_Checked then CConst:=CTypeConst[i];
    for i:=0 to NumBoardTypes do
      if BoardType[i]^.GetCheck=bf_Checked then BConst:=BTypeConst[i];
    FormationConst:=CConst*BConst;
    if Metric then FormationConst:=FormationConst*FormConv;
  end;
  GetFormationConst:=FormationConst;
end;

procedure TFormDialog.CalcBW;
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;

  BW:=Caliper*Density;
  if Metric then BW:=BW*BWConv;
  StrPCopy(TempStr, Real2String(BW, NumWPrec, NumDPrec));
  BasisWeightEdit^.SetText(TempStr);
end;

procedure TFormDialog.CalcTPD;
var
  SheetWidth,
  NumOfVats,
  FormationConst,
  MachEff,
  TPD: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  NumVatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, NumOfVats, ErrorPos);
  SheetWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, SheetWidth, ErrorPos);
  if Metric then SheetWidth:=SheetWidth/SWConv;
  FormationConst:=GetFormationConst;
  if Metric then FormationConst:=FormationConst/FormConv;
  EffEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MachEff, ErrorPos);

  TPD:=(SheetWidth*NumOfVats)*FormationConst*(MachEff/100);
  if Metric then TPD:=TPD*TPDConv;
  StrPCopy(TempStr, Real2String(TPD, NumWPrec, NumDPrec));
  TPDEdit^.SetText(TempStr);
end;

procedure TFormDialog.CalcMS;
var
  SheetWidth,
  BasisWeight,
  TPD,
  MachEff,
  MS: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  SheetWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, SheetWidth, ErrorPos);
  if Metric then SheetWidth:=SheetWidth/SWConv;
  BasisWeightEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BasisWeight, ErrorPos);
  if Metric then BasisWeight:=BasisWeight/BWConv;
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  if Metric then TPD:=TPD/TPDConv;
  EffEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MachEff, ErrorPos);

  if BasisWeight<=0 then
    MSEdit^.SetText('')
  else
  if SheetWidth<=0 then
    MSEdit^.SetText('')
  else
  if MachEff<=0 then
    MSEdit^.SetText('')
  else
  begin
    MS:=((TPD/24)/60)*2000;     { Find lbs per min }
    MS:=(MS/BasisWeight)*(1000/(SheetWidth/12));   { Find ft per min }
    MS:=MS/(MachEff/100);
    if Metric then MS:=MS*MSConv;
    StrPCopy(TempStr, Real2String(MS, NumWPrec, NumDPrec));
    MSEdit^.SetText(TempStr);
  end;
end;

procedure TFormDialog.CalcGPM;
var
  NumOfVats,
  TPD,
  Cons,
  GPM,
  GPMPV: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  NumVatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, NumOfVats, ErrorPos);
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  if Metric then TPD:=TPD/TPDConv;
  ConsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Cons, ErrorPos);

  if Cons<=0 then
  begin
    GPMEdit^.SetText('');
    GPMPVEdit^.SetText('');
  end else
  begin
    GPM:=((TPD*2000)/1440)/((Cons*((5/6)*100))/1000);
    if Metric then GPM:=GPM*GPMConv;
    StrPCopy(TempStr, Real2String(GPM, NumWPrec, NumDPrec));
    GPMEdit^.SetText(TempStr);
    if NumOfVats=0 then GPMPV:=GPM else GPMPV:=GPM/NumOfVats;
    StrPCopy(TempStr, Real2String(GPMPV, NumWPrec, NumDPrec));
    GPMPVEdit^.SetText(TempStr);
  end;
end;

procedure TFormDialog.CalcNumVats(TPD: Real);
var
  SheetWidth,
  NumOfVats,
  MachEff,
  FormationConst: Real;
  i: integer;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  SheetWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, SheetWidth, ErrorPos);
  if Metric then SheetWidth:=SheetWidth/SWConv;
  FormationConst:=GetFormationConst;
  if Metric then FormationConst:=FormationConst/FormConv;
  EffEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MachEff, ErrorPos);

  if ((SheetWidth<=0) or (FormationConst<=0) or (MachEff<=0)) then
    NumVatsEdit^.SetText('')
  else
  begin
    NumOfVats:=((TPD/(MachEff/100))/FormationConst)/SheetWidth;
    if NumOfVats<>Trunc(NumOfVats) then NumOfVats:=Trunc(NumOfVats)+1;
    StrPCopy(TempStr, Real2String(NumOfVats, NumWPrec, 0));
    NumVatsEdit^.SetText(TempStr);
  end;

  SendMessage(HWindow, CalcItID, 0, 0);
end;

procedure TFormDialog.DoPipeSize(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  GPMPVEdit^.GetText(TempStr, SNumWidth);
  DoFlowPipe(@Self, Metric, TempStr);
end;

procedure TFormDialog.DoAreaSize(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  GPMPVEdit^.GetText(TempStr, SNumWidth);
  DoFlowArea(@Self, Metric, TempStr);
end;

procedure TFormDialog.DoProdCalc(var Msg: TMessage);
var
  TPD: Real;
  ProdData: TProdData;
  TempCheck: Integer;
begin
  SheetWidthEdit^.GetText(ProdData.SheetWidth, SNumWidth);
  MSEdit^.GetText(ProdData.MS, SNumWidth);
  EffEdit^.GetText(ProdData.MachEff, SNumWidth);
  CaliperEdit^.GetText(ProdData.Caliper, SNumWidth);
  DensityEdit^.GetText(ProdData.Density, SNumWidth);
  BasisWeightEdit^.GetText(ProdData.BasisWeight, SNumWidth);
  StrCopy(ProdData.TPD, '');

  TempCheck:=TPDCalcBox^.GetCheck;
  TPDCalcBox^.UnCheck;                  { Simulate transfer }
  TPD:=DoProd(@Self, Metric, ProdData);
  TPDCalcBox^.SetCheck(TempCheck);

  if TPD<>-1 then
  begin
    SheetWidthEdit^.SetText(ProdData.SheetWidth);
    EffEdit^.SetText(ProdData.MachEff);
    CaliperEdit^.SetText(ProdData.Caliper);
    DensityEdit^.SetText(ProdData.Density);
    TPDEdit^.SetText(ProdData.TPD);
    TPDCalcBox^.UnCheck;
    if MessageBox(HWindow, 'Do you wish to recalc the forming rate?',
                        'Recalculation Request',
                         mb_yesno+mb_iconquestion)=idYes then
      OtherTurnOn(True);
  end;

  CalcIt2(Msg);
end;

procedure TFormDialog.DoGradeCalc(var Msg: TMessage);
var
  GradeData: TGradeData;
begin
  CaliperEdit^.GetText(GradeData.Caliper, SNumWidth);
  DensityEdit^.GetText(GradeData.Density, SNumWidth);
  BasisWeightEdit^.GetText(GradeData.BasisWeight, SNumWidth);

  if DoGrade(@Self, Metric, GradeData)<>-1 then
  begin
    CaliperEdit^.SetText(GradeData.Caliper);
    DensityEdit^.SetText(GradeData.Density);
    BasisWeightEdit^.SetText(GradeData.BasisWeight);
  end;

  CalcIt2(Msg);
end;

procedure TFormDialog.DoHBox(var Msg: TMessage);
var
  HeadBoxData: THeadBoxData;
begin
  TPDEdit^.GetText(HeadBoxData.TPD, SNumWidth);
  ConsEdit^.GetText(HeadBoxData.NCons, SNumWidth);
  StrCopy(HeadBoxData.PCons, '');
  StrCopy(HeadBoxData.Water, '');
  DoHeadBox(@Self, Metric, HeadBoxData);
end;

procedure TFormDialog.DoCalcNumVats(var Msg: TMessage);
var
  GetTPDDlg: PGetTPDDialog;
  GetTPDData: GetTPDRecord;
  SheetWidth,
  NumOfVats,
  FormationConst,
  TPD: Real;
  i: integer;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  if Metric then
    GetTPDDlg:=New(PGetTPDDialog, Init(@Self, 'GETTPD_METRIC'))
  else
    GetTPDDlg:=New(PGetTPDDialog, Init(@Self, 'GETTPD_ENGLISH'));

  TPDEdit^.GetText(GetTPDData.TPD, SNumWidth);
  GetTPDDlg^.TransferBuffer:=@GetTPDData;
  if Application^.ExecDialog(GetTPDDlg)<>id_Cancel then
  begin
    Val(GetTPDData.TPD, TPD, ErrorPos);
    if Metric then TPD:=TPD/TPDConv;
    SheetWidthEdit^.GetText(TempStr, SNumWidth);
    Val(TempStr, SheetWidth, ErrorPos);
    if Metric then SheetWidth:=SheetWidth/SWConv;
    FormationConst:=GetFormationConst;

    if (SheetWidth=0) then
      MessageBox(HWindow, 'Sheet Width cannot be equal to zero!',
                          'Calculation Error', mb_ok+mb_IconExclamation)
    else
    if (FormationConst=0) then
      MessageBox(HWindow, 'Sheet Formation Constant cannot be equal to zero!',
                          'Calculation Error', mb_ok+mb_IconExclamation)
    else
      CalcNumVats(TPD);
  end;
end;

procedure TFormDialog.CalcIt(var Msg: TMessage);
begin
  SetFocus(SetFocus(HWindow));
  CalcIt2(Msg);
end;

procedure TFormDialog.CalcIt2(var Msg: TMessage);
begin
  CalcBW;
  if TPDCalcBox^.GetCheck=bf_Checked then CalcTPD;
  CalcMS;
  CalcGPM;
end;

procedure TFormDialog.CylTypeChange1(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  CalcIt2(Msg);
end;

procedure TFormDialog.CylTypeChange2(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  CalcIt2(Msg);
end;

procedure TFormDialog.CylTypeChange3(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  CalcIt2(Msg);
end;

procedure TFormDialog.BoardTypeChange1(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  CalcIt2(Msg);
end;

procedure TFormDialog.BoardTypeChange2(var Msg: TMessage);
var
  TempStr: array[0..SNumWidth+1] of Char;
begin
  StrPCopy(TempStr, Real2String(GetFormationConst, NumWPrec, NumDPrec));
  FormConstEdit^.SetText(TempStr);
  CalcIt2(Msg);
end;

procedure TFormDialog.DoPrint(AEdit: PEdit);
var
  Temp: array[0..255] of Char;
  i: integer;
begin
  inherited DoPrint(AEdit);

  SendMessage(HWindow, CalcItID, 0, 0);

  with AEdit^ do
  begin
    Insert('Sheet Formation Calculation Window:'+CRLF);
    Insert('-----------------------------------'+CRLF+CRLF);

    Insert('Number of Vats: ');
    NumVatsEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF+CRLF);
    Insert(Temp);

    if TPDCalcBox^.GetCheck=bf_Checked then
    begin
      Insert('Caliper (points): ');
      CaliperEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      if Metric then
        Insert('Sheet Density (Grams/point): ')
      else
        Insert('Sheet Density (lbs/point): ');
      DensityEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      if Metric then
        Insert('Basis Weight (Grams/Meter Sq.): ')
      else
        Insert('Basis Weight (lbs/1K Sq. Ft): ');
      BasisWeightEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      if Metric then
        Insert('Formation Constant (Kilograms/mm of Cyl): ')
      else
        Insert('Formation Constant (Tons/in of Cyl): ');
      FormConstEdit^.GetText(Temp, SizeOf(Temp));
      Insert(Temp);
      if OtherBox^.GetCheck=bf_UnChecked then
      begin
        Insert(CRLF);
        Insert('   Based On:'+CRLF);
        Insert('       Cylinder: ');
        for i:=0 to NumCylTypes do
          if CylType[i]^.GetCheck=bf_Checked then
            Insert(CTypeNames[i]);
        Insert(CRLF);
        Insert('       Board Type: ');
        for i:=0 to NumBoardTypes do
          if BoardType[i]^.GetCheck=bf_Checked then
            Insert(BTypeNames[i]);
      end;

      Insert(CRLF);

      if Metric then
        Insert('Sheet Trim Width (mm): ')
      else
        Insert('Sheet Trim Width (in.): ');
      SheetWidthEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      Insert(CRLF);
      if Metric then
        Insert('Maximum Kilograms Per Day: ')
      else
        Insert('Maximum Tons Per Day: ');
      TPDEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      Insert(CRLF);
      if Metric then
        Insert('Required Machine Speed (Meters/Min): ')
      else
        Insert('Required Machine Speed (Ft/Min): ');
      MSEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);
    end else
    begin
      if Metric then
        Insert('Kilograms Per Day: ')
      else
        Insert('Tons Per Day: ');
      TPDEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);

      Insert(CRLF);
      if Metric then
        Insert('Machine Speed (Meters/Min): ')
      else
        Insert('Machine Speed (Ft/Min): ');
      MSEdit^.GetText(Temp, SizeOf(Temp));
      StrCat(Temp, CRLF);
      Insert(Temp);
    end;

    Insert('Machine Efficiency (%): ');
    EffEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF+CRLF);
    Insert(Temp);

    Insert('Consistency (%): ');
    ConsEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Liters Per Minute (total): ')
    else
      Insert('Gallons Per Minute (total): ');
    GPMEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Liters Per Minute (per vat): ')
    else
      Insert('Gallons Per Minute (per vat): ');
    GPMPVEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF);
  end;
end;

procedure TFormDialog.PseudoXfer(var Msg: TMessage);
var
  XData: PProdData;
begin
  XData:=PProdData(Msg.lParam);
  with PseudoSave do
  begin
    CaliperEdit^.GetText(Caliper, SNumWidth);
    DensityEdit^.GetText(Density, SNumWidth);
    BasisWeightEdit^.GetText(BasisWeight, SNumWidth);
    SheetWidthEdit^.GetText(SheetWidth, SNumWidth);
    TPDEdit^.GetText(TPD, SNumWidth);
    EffEdit^.GetText(MachEff, SNumWidth);
  end;

  with XData^ do
  begin
    CaliperEdit^.SetText(Caliper);
    DensityEdit^.SetText(Density);
    BasisWeightEdit^.SetText(BasisWeight);
    SheetWidthEdit^.SetText(SheetWidth);
    TPDEdit^.SetText(TPD);
    EffEdit^.SetText(MachEff);
  end;
end;

procedure TFormDialog.PseudoRestore(var Msg: TMessage);
begin
  with PseudoSave do
  begin
    CaliperEdit^.SetText(Caliper);
    DensityEdit^.SetText(Density);
    BasisWeightEdit^.SetText(BasisWeight);
    SheetWidthEdit^.SetText(SheetWidth);
    TPDEdit^.SetText(TPD);
    EffEdit^.SetText(MachEff);
  end;
end;

procedure TFormDialog.DoModeToggle(var Msg: TMessage);
begin
  TransferData(tf_GetData);
  EndDlg(ModeToggleID);
end;

procedure TFormDialog.Cancel(var Msg: TMessage);
begin
  TransferData(tf_GetData);
  inherited Cancel(Msg);
end;

{ TProdDialog Methods ---------------------------------------------------- }
constructor TProdDialog.Init(AParent: PWindowsObject; AName: PChar;
                                      MetricFlag: Boolean);
var
  i: integer;
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  SheetWidthEdit:=New(PValEdit, InitResource(@Self, ProdSheetWidthID, SNumWidth+1));
  SheetWidthEdit^.SetValidator(New(PPosRealValidator, Init));
  MSEdit:=New(PValEdit, InitResource(@Self, ProdMSID, SNumWidth+1));
  MSEdit^.SetValidator(New(PPosRealValidator, Init));
  MachEffEdit:=New(PValEdit, InitResource(@Self, ProdEffID, SNumWidth+1));
  MachEffEdit^.SetValidator(New(PPosRealValidator, Init));

  CaliperEdit:=New(PValEdit, InitResource(@Self, ProdCaliperID, SNumWidth+1));
  CaliperEdit^.SetValidator(New(PPosRealValidator, Init));
  DensityEdit:=New(PValEdit, InitResource(@Self, ProdDensityID, SNumWidth+1));
  DensityEdit^.SetValidator(New(PPosRealValidator, Init));
  BasisWeightEdit:=New(PStatic, InitResource(@Self, ProdBWID, SNumWidth+1));

  TPDEdit:=New(PStatic, InitResource(@Self, ProdTPDID, SNumWidth+1));
end;

procedure TProdDialog.SetupWindow;
begin
  inherited SetupWindow;
  SendMessage(HWindow, CalcItID, 0, 0);
end;

procedure TProdDialog.DoGradeCalc(var Msg: TMessage);
var
  GradeData: TGradeData;
begin
  CaliperEdit^.GetText(GradeData.Caliper, SNumWidth);
  DensityEdit^.GetText(GradeData.Density, SNumWidth);
  BasisWeightEdit^.GetText(GradeData.BasisWeight, SNumWidth);

  if DoGrade(@Self, Metric, GradeData)<>-1 then
  begin
    CaliperEdit^.SetText(GradeData.Caliper);
    DensityEdit^.SetText(GradeData.Density);
    BasisWeightEdit^.SetText(GradeData.BasisWeight);
  end;

  CalcIt(Msg);
end;

procedure TProdDialog.CalcIt(var Msg: TMessage);
var
  SheetWidth,
  MS,
  Density,
  BasisWeight,
  MachEff,
  Caliper,
  TPD: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;

  SheetWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, SheetWidth, ErrorPos);
  if Metric then SheetWidth:=SheetWidth/SWConv;
  MSEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MS, ErrorPos);
  if Metric then MS:=MS/MSConv;
  MachEffEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, MachEff, ErrorPos);

  BasisWeight:=Caliper*Density;
  if Metric then BasisWeight:=BasisWeight*BWConv;
  StrPCopy(TempStr, Real2String(BasisWeight, NumWPrec, NumDPrec));
  BasisWeightEdit^.SetText(TempStr);
  if Metric then BasisWeight:=BasisWeight/BWConv;       { Conv Back for Calc }

  if (SheetWidth<=0) then
  begin
    TPDEdit^.SetText('');
  end else
  begin
    TPD:=(((MS/(1000/(SheetWidth/12)))*BasisWeight*60)/2000)*
         (MachEff/100)*24;
    if Metric then TPD:=TPD*TPDConv;
    StrPCopy(TempStr, Real2String(TPD, NumWPrec, NumDPrec));
    TPDEdit^.SetText(TempStr);
  end;
end;

procedure TProdDialog.DoCalc(var Msg: TMessage);
begin
  SetFocus(SetFocus(HWindow));
  CalcIt(Msg);
end;

procedure TProdDialog.DoTransfer(var Msg: TMessage);
begin
  Ok(Msg);
end;

procedure TProdDialog.DoTransfer2(var Msg: TMessage);
begin
  DoTransfer(Msg);
end;

procedure TProdDialog.DoPrint(AEdit: PEdit);
var
  Temp: array[0..255] of Char;
  TempData: TProdData;
begin
  SendMessage(HWindow, CalcItID, 0, 0);

  with TempData do
  begin
    CaliperEdit^.GetText(Caliper, SNumWidth);
    DensityEdit^.GetText(Density, SNumWidth);
    BasisWeightEdit^.GetText(BasisWeight, SNumWidth);
    SheetWidthEdit^.GetText(SheetWidth, SNumWidth);
    MSEdit^.GetText(MS, SNumWidth);
    MachEffEdit^.GetText(MachEff, SNumWidth);
    TPDEdit^.GetText(TPD, SNumWidth);
  end;

  SendMessage(Parent^.HWindow, PseudoXferID, 0, LongInt(@TempData));
  SendMessage(Parent^.HWindow, CalcItID, 0, 0);

  with AEdit^ do
  begin
    Insert('Production Calculation Window:'+CRLF);
    Insert('------------------------------'+CRLF+CRLF);

    if Metric then
      Insert('Sheet Trim Width (mm): ')
    else
      Insert('Sheet Trim Width (in.): ');
    SheetWidthEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Machine Speed (Meters/Min): ')
    else
      Insert('Machine Speed (Ft/Min): ');
    MSEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('Machine Efficiency (%): ');
    MachEffEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF);
    if Metric then
      Insert('Caliper (points): ')
    else
      Insert('Caliper (points): ');
    CaliperEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Sheet Density (grams/point): ')
    else
      Insert('Sheet Density (lbs/point): ');
    DensityEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Basis Weight (Grams/Meter Sq.): ')
    else
      Insert('Basis Weight (lbs/1K Sq. Ft): ');
    BasisWeightEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF);
    if Metric then
      Insert('Kilograms Per Day: ')
    else
      Insert('Tons Per Day: ');
    TPDEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;

  inherited DoPrint(AEdit);

  SendMessage(Parent^.HWindow, PseudoRestoreID, 0, 0);
end;

{ TGradeDialog Methods --------------------------------------------------- }
constructor TGradeDialog.Init(AParent: PWindowsObject; AName: PChar;
                                       MetricFlag: Boolean);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  CaliperEdit:=New(PASEdit, InitResource(@Self, GradeCaliperID, SNumWidth+1));
  CaliperEdit^.SetValidator(New(PPosRealValidator, Init));
  DensityEdit:=New(PASEdit, InitResource(@Self, GradeDensityID, SNumWidth+1));
  DensityEdit^.SetValidator(New(PPosRealValidator, Init));
  BasisWeightEdit:=New(PASEdit, InitResource(@Self, GradeBWID, SNumWidth+1));
  BasisWeightEdit^.SetValidator(New(PPosRealValidator, Init));
end;

procedure TGradeDialog.CalcCaliper(var Msg: TMessage);
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;
  BasisWeightEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BW, ErrorPos);
  if Metric then BW:=BW/BWConv;

  if Density<=0 then
    MessageBox(HWindow, 'Density must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Caliper:=BW/Density;
    if Metric then Caliper:=Caliper*CaliperConv;
    StrPCopy(TempStr, Real2String(Caliper, NumWPrec, NumDPrec));
    CaliperEdit^.SetText(TempStr);
  end;
end;

procedure TGradeDialog.CalcDensity(var Msg: TMessage);
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  BasisWeightEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BW, ErrorPos);
  if Metric then BW:=BW/BWConv;

  if Caliper<=0 then
    MessageBox(HWindow, 'Caliper must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Density:=BW/Caliper;
    if Metric then Density:=Density*DensityConv;
    StrPCopy(TempStr, Real2String(Density, NumWPrec, NumDPrec));
    DensityEdit^.SetText(TempStr);
  end;
end;

procedure TGradeDialog.CalcBW(var Msg: TMessage);
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;

  BW:=Caliper*Density;
  if Metric then BW:=BW*BWConv;
  StrPCopy(TempStr, Real2String(BW, NumWPrec, NumDPrec));
  BasisWeightEdit^.SetText(TempStr);
end;

procedure TGradeDialog.DoPrint(AEdit: PEdit);
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
  Temp: array[0..255] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;
  BasisWeightEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BW, ErrorPos);
  if Metric then BW:=BW/BWConv;

  if round(BW+0.3)<>round((Caliper*Density)+0.3) then
    MessageBox(GetLastActivePopUp(Application^.MainWindow^.HWindow),
            'Cannot print grade information until you run calculate.',
            'Print Calculation Error', mb_OK+mb_IconExclamation)
  else
  with AEdit^ do
  begin
    Insert('Grade Calculation Window:'+CRLF);
    Insert('-------------------------'+CRLF+CRLF);

    Insert('Caliper (points): ');
    CaliperEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Sheet Density (grams/point): ')
    else
      Insert('Sheet Density (lbs/point): ');
    DensityEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Basis Weight (Grams/M Sq): ')
    else
      Insert('Basis Weight (lbs/1K Sq. Ft): ');
    BasisWeightEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;
end;

procedure TGradeDialog.DoTransfer(var Msg: TMessage);
var
  Caliper,
  Density,
  BW: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
  Temp: array[0..255] of Char;
begin
  CaliperEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Caliper, ErrorPos);
  if Metric then Caliper:=Caliper/CaliperConv;
  DensityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Density, ErrorPos);
  if Metric then Density:=Density/DensityConv;
  BasisWeightEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BW, ErrorPos);
  if Metric then BW:=BW/BWConv;

  if round(BW+0.3)<>round((Caliper*Density)+0.3) then
    MessageBox(GetLastActivePopUp(Application^.MainWindow^.HWindow),
            'Cannot transfer grade information until you run calculate.',
            'Transfer Calculation Error', mb_OK+mb_IconExclamation)
  else Ok(msg);
end;

{ THeadBoxDialog Methods ------------------------------------------------- }
constructor THeadBoxDialog.Init(AParent: PWindowsObject; AName: PChar;
                                         MetricFlag: Boolean);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  TPDEdit:=New(PValEdit, InitResource(@Self, HBoxTPDID, SNumWidth+1));
  TPDEdit^.SetValidator(New(PPosRealValidator, Init));
  PConsEdit:=New(PValEdit, InitResource(@Self, HBoxPConsID, SNumWidth+1));
  PConsEdit^.SetValidator(New(PPosRealValidator, Init));
  NConsEdit:=New(PValEdit, InitResource(@Self, HBoxNConsID, SNumWidth+1));
  NConsEdit^.SetValidator(New(PPosRealValidator, Init));

  WaterEdit:=New(PStatic, InitResource(@Self, HBoxWaterID, SNumWidth+1));
end;

procedure THeadBoxDialog.CalcIt(var Msg: TMessage);
var
  TPD,
  PCons,
  NCons,
  Water: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  if Metric then TPD:=TPD/TPDConv;
  PConsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, PCons, ErrorPos);
  NConsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, NCons, ErrorPos);

  if ((PCons<=0) or (NCons<=0)) then
  begin
    WaterEdit^.SetText('');
  end else
  begin
    Water:=((TPD*((1/6)*100))/NCons) - ((TPD*((1/6)*100))/PCons);
    if Metric then Water:=Water*GPMConv;
    StrPCopy(TempStr, Real2String(Water, NumWPrec, NumDPrec));
    WaterEdit^.SetText(TempStr);
  end;
end;

procedure THeadBoxDialog.DoCalc(var Msg: TMessage);
begin
  SetFocus(SetFocus(HWindow));
  CalcIt(Msg);
end;

procedure THeadBoxDialog.DoPrint(AEdit: PEdit);
var
  Temp: array[0..255] of Char;
begin
  inherited DoPrint(AEdit);

  SendMessage(HWindow, CalcItID, 0, 0);
  with AEdit^ do
  begin
    Insert('HeadBox Calculation Window:'+CRLF);
    Insert('---------------------------'+CRLF);

    if Metric then
      Insert('Kilograms Per Day: ')
    else
      Insert('Tons Per Day: ');
    TPDEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('Present Consistency (%): ');
    PConsEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('New Consistency (%): ');
    NConsEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Water Needed (LPM): ')
    else
      Insert('Water Needed (GPM): ');
    WaterEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;
end;

{ TFormCalcDialog Methods ------------------------------------------------ }

constructor TFormCalcDialog.Init(AParent: PWindowsObject; AName: PChar;
                                          MetricFlag: Boolean);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  TPDEdit:=New(PValEdit, InitResource(@Self, FormTPDID, SNumWidth+1));
  TPDEdit^.SetValidator(New(PPosRealValidator, Init));
  NumVatsEdit:=New(PValEdit, InitResource(@Self, FormNumVatsID, SNumWidth+1));
  NumVatsEdit^.SetValidator(New(PPosIntValidator, Init));
  SheetWidthEdit:=New(PValEdit, InitResource(@Self, FormSWID, SNumWidth+1));
  SheetWidthEdit^.SetValidator(New(PPosRealValidator, Init));

  FormConstEdit:=New(PStatic, InitResource(@Self, FormFormConstID, SNumWidth+1));
end;

procedure TFormCalcDialog.SetupWindow;
begin
  inherited SetupWindow;
  SendMessage(HWindow, CalcItID, 0, 0);
end;

procedure TFormCalcDialog.CalcIt(var Msg: TMessage);
var
  TPD,
  NumVats,
  SheetWidth,
  FormConst: Real;
  ErrorPos: integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  if Metric then TPD:=TPD/TPDConv;
  NumVatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, NumVats, ErrorPos);
  SheetWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, SheetWidth, ErrorPos);
  if Metric then SheetWidth:=SheetWidth/SWConv;

  if ((NumVats<=0) or (SheetWidth<=0)) then
  begin
    FormConstEdit^.SetText('');
  end else
  begin
    FormConst:=(TPD/NumVats)/SheetWidth;
    if Metric then FormConst:=FormConst*FormConv;
    StrPCopy(TempStr, Real2String(FormConst, NumWPrec, NumDPrec));
    FormConstEdit^.SetText(TempStr);
  end;
end;

procedure TFormCalcDialog.DoCalc(var Msg: TMessage);
begin
  SetFocus(SetFocus(HWindow));
  CalcIt(Msg);
end;

procedure TFormCalcDialog.DoTransfer(var Msg: TMessage);
begin
  Ok(Msg);
end;

procedure TFormCalcDialog.DoTransfer2(var Msg: TMessage);
begin
  DoTransfer(Msg);
end;

procedure TFormCalcDialog.DoPrint(AEdit: PEdit);
var
  Temp: array[0..255] of Char;
begin
  inherited DoPrint(AEdit);
end;

{ TGetTPDDialog Methods -------------------------------------------------- }
constructor TGetTPDDialog.Init(AParent: PWindowsObject; AName: PChar);
var
  AEdit: PValEdit;
begin
  inherited Init(AParent, AName);
  AEdit:=New(PValEdit, InitResource(@Self, GetTPDID, SNumWidth+1));
  AEdit^.SetValidator(New(PPosRealValidator, Init));
end;

procedure TGetTPDDialog.DoHelp(var Msg: TMessage);
begin
  SendMessage(Application^.MainWindow^.HWindow, HelpID, 0, 0);
end;

{ ------------------------------------------------------------------------ }

function DoForm(AParent: PWindowsObject; MetricFlag: Boolean;
                          var FormData: TFormData): Boolean;
var
  FormDialog: PFormDialog;
begin
  if not MetricFlag then
    FormDialog:=New(PFormDialog, Init(AParent, 'FORM_ENGLISH', MetricFlag))
  else
    FormDialog:=New(PFormDialog, Init(AParent, 'FORM_METRIC', MetricFlag));

  FormDialog^.TransferBuffer:=@FormData;
  if Application^.ExecDialog(FormDialog)=ModeToggleID then
    DoForm:=True
  else
    DoForm:=False;
end;

function DoProd(AParent: PWindowsObject; MetricFlag: Boolean;
               var ProdData: TProdData): Real;
var
  ProdDialog: PProdDialog;
  TempVal: Real;
  ErrorPos: Integer;
begin
  if not MetricFlag then
    ProdDialog:=New(PProdDialog, Init(AParent, 'PROD_ENGLISH', MetricFlag))
  else
    ProdDialog:=New(PProdDialog, Init(AParent, 'PROD_METRIC', MetricFlag));

  ProdDialog^.TransferBuffer:=@ProdData;
  if Application^.ExecDialog(ProdDialog)<>id_Cancel then
    Val(ProdData.TPD, TempVal, ErrorPos)
  else
    TempVal:=-1;

  DoProd:=TempVal;
end;

function DoGrade(AParent: PWindowsObject; MetricFlag: Boolean;
                          var GradeData: TGradeData): Real;
var
  GradeDialog: PGradeDialog;
  TempVal: Real;
begin
  if not MetricFlag then
    GradeDialog:=New(PGradeDialog, Init(AParent, 'GRADE_ENGLISH', MetricFlag))
  else
    GradeDialog:=New(PGradeDialog, Init(AParent, 'GRADE_METRIC', MetricFlag));

  GradeDialog^.TransferBuffer:=@GradeData;
  if Application^.ExecDialog(GradeDialog)<>id_Cancel then
    TempVal:=0
  else
    TempVal:=-1;

  DoGrade:=TempVal;
end;

procedure DoHeadBox(AParent: PWindowsObject; MetricFlag: Boolean;
                   var HeadBoxData: THeadBoxData);
var
  HeadBoxDialog: PHeadBoxDialog;
begin
  if not MetricFlag then
    HeadBoxDialog:=New(PHeadBoxDialog, Init(AParent, 'HEADBOX_ENGLISH', MetricFlag))
  else
    HeadBoxDialog:=New(PHeadBoxDialog, Init(AParent, 'HEADBOX_METRIC', MetricFlag));
  HeadBoxDialog^.TransferBuffer:=@HeadBoxData;
  Application^.ExecDialog(HeadBoxDialog);
end;

function DoFormationCalc(AParent: PWindowsObject; MetricFlag: Boolean;
               var FormCalcData:TFormCalcData;
               Automatic: Boolean): Real;
var
  FormCalcDialog: PFormCalcDialog;
  TempVal: Real;
  ErrorPos: Integer;
begin
  if not MetricFlag then
    FormCalcDialog:=New(PFormCalcDialog, Init(AParent, 'FORMCALC_ENGLISH', MetricFlag))
  else
    FormCalcDialog:=New(PFormCalcDialog, Init(AParent, 'FORMCALC_METRIC', MetricFlag));

  FormCalcDialog^.TransferBuffer:=@FormCalcData;
  if Automatic=False then
  begin
    if Application^.ExecDialog(FormCalcDialog)<>id_Cancel then
      Val(FormCalcData.FormConst, TempVal, ErrorPos)
    else
      TempVal:=-1;
  end else
  begin
    FormCalcDialog^.Create;
    FormCalcDialog^.TransferData(tf_GetData);
    DestroyWindow(FormCalcDialog^.HWindow);
    Val(FormCalcData.FormConst, TempVal, ErrorPos);
  end;

  DoFormationCalc:=TempVal;
end;

end.
