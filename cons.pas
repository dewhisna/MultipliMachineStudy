{

              Machine Study

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

    Consistency Analysis Subprogram V1.0

}

unit Cons;

{$R CONS.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs;

const
  ConsCalcID = 1001;
  TPDCalcID = 1002;
  VatsCalcID = 1003;
  GPMCalcID = 1004;
  ConsEditID = 2001;
  TPDEditID = 2002;
  VatsEditID = 2003;
  GPMEditID = 2004;
  GPMPVEditID = 2005;

type
  PConsDialog = ^TConsDialog;
  TConsDialog = object(TDialog)
    ConsEdit: PEdit;
    TPDEdit: PEdit;
    VatsEdit: PEdit;
    GPMEdit: PEdit;
    GPMPVEdit: PEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar);
    procedure CalcCons(var Msg: TMessage);
      virtual Id_First + ConsCalcID;
    procedure CalcTPD(var Msg: TMessage);
      virtual Id_First + TPDCalcID;
    procedure CalcVats(var Msg: TMessage);
      virtual Id_First + VatsCalcID;
    procedure CalcGPM(var Msg: TMessage);
      virtual Id_First + GPMCalcID;
  end;

procedure DoCons(AParent: PWindowsObject; MetricFlag: Boolean);

implementation
uses WinProcs, Validize, Strings;

const
  SNumWidth = 12;

  NumWPrec = 8;
  NumDPrec = 5;

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

{ TConsDialog Methods ---------------------------------------------------- }
constructor TConsDialog.Init(AParent: PWindowsObject; AName: PChar);
begin
  inherited Init(AParent, AName);
  ConsEdit:=New(PEdit, InitResource(@Self, ConsEditID, SNumWidth+1));
  ConsEdit^.SetValidator(New(PPosRealValidator, Init));
  TPDEdit:=New(PEdit, InitResource(@Self, TPDEditID, SNumWidth+1));
  TPDEdit^.SetValidator(New(PPosRealValidator, Init));
  VatsEdit:=New(PEdit, InitResource(@Self, VatsEditID, SNumWidth+1));
  VatsEdit^.SetValidator(New(PPosIntValidator, Init));
  GPMEdit:=New(PEdit, InitResource(@Self, GPMEditID, SNumWidth+1));
  GPMEdit^.SetValidator(New(PPosRealValidator, Init));
  GPMPVEdit:=New(PEdit, InitResource(@Self, GPMPVEditID, SNumWidth+1));
  GPMPVEdit^.SetValidator(New(PPosRealValidator, Init));
end;

procedure TConsDialog.CalcCons(var Msg: TMessage);
var
  Cons,
  TPD,
  GPM: Real;
  Vats: Integer;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  VatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Vats, ErrorPos);
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
{  if PipeSize<=0 then
    MessageBox(GetFocus, 'Pipe size must be greater than ZERO!!',
                         'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Velocity:=((GPM/7.48)/
               ((Sqr(PipeSize/2)*pi)/144))/60;
    StrPCopy(TempStr, Real2String(Velocity, NumWPrec, NumDPrec));
    VelocityEdit^.SetText(TempStr);
  end;
}
end;

procedure TConsDialog.CalcTPD(var Msg: TMessage);
var
  Cons,
  TPD,
  GPM: Real;
  Vats: Integer;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  ConsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Cons, ErrorPos);
  VatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Vats, ErrorPos);
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
{
  GPM:=(Velocity*60)*((Sqr(PipeSize/2)*pi)/144)*7.48;
  StrPCopy(TempStr, Real2String(GPM, NumWPrec, NumDPrec));
  GPMEdit^.SetText(TempStr);
}
end;

procedure TConsDialog.CalcGPM(var Msg: TMessage);
var
  Cons,
  TPD,
  GPM: Real;
  Vats: Integer;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  ConsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Cons, ErrorPos);
  TPDEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, TPD, ErrorPos);
  VatsEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Vats, ErrorPos);

  if Cons<=0 then
    MessageBox(GetFocus, 'Consistency must be greater than ZERO!!',
                         'Calculation Error', mb_OK+mb_IconExclamation)
  else
  if Vats<=0 then
    MessageBox(GetFocus, 'Number of Vats must be greater than ZERO!!',
                         'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    GPM:=((TPD*2000)/1440)/((Cons*83.4)/1000);
    StrPCopy(TempStr, Real2String(GPM, NumWPrec, NumDPrec));
    GPMEdit^.SetText(TempStr);
  end;
end;

{ ------------------------------------------------------------------------ }

procedure DoCons(AParent: PWindowsObject; MetricFlag: Boolean);
begin
  if not MetricFlag then
    Application^.ExecDialog(New(PConsDialog,
        Init(AParent, 'CONS_ENGLISH')))
  else
    Application^.ExecDialog(New(PConsDialog,
        Init(AParent, 'CONS_METRIC')));
end;

end.
