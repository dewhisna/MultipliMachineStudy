{

              Machine Study

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

      Flow Analysis Subprogram V1.1

}

unit Flow;

{$R FLOW.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs, ASEdit, PrObj;

const
  VelocityCalcID = 1001;
  GPMCalcID = 1002;
  PipeCalcID = 1003;
  AreaCalcID = 1003;
  VelocityEditID = 2001;
  GPMEditID = 2002;
  PipeEditID = 2003;
  AreaEditID = 2003;

const
  SNumWidth = 12;

  NumWPrec = 8;
  NumDPrec = 5;

type
  PFlowPipeDialog = ^TFlowPipeDialog;
  TFlowPipeDialog = object(TPrDialog)
    Metric: Boolean;
    TempGPM: array[0..SNumWidth+1] of Char;
    VelocityEdit: PASEdit;
    GPMEdit: PASEdit;
    PipeEdit: PASEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean; InitGPM: PChar);
    procedure SetupWindow; virtual;
    procedure CalcVelocity(var Msg: TMessage);
      virtual Id_First + VelocityCalcID;
    procedure CalcGPM(var Msg: TMessage);
      virtual Id_First + GPMCalcID;
    procedure CalcPipe(var Msg: TMessage);
      virtual Id_First + PipeCalcID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

  PFlowAreaDialog = ^TFlowAreaDialog;
  TFlowAreaDialog = object(TPrDialog)
    Metric: Boolean;
    TempGPM: array[0..SNumWidth+1] of Char;
    VelocityEdit: PASEdit;
    GPMEdit: PASEdit;
    AreaEdit: PASEdit;
    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean; InitGPM: PChar);
    procedure SetupWindow; virtual;
    procedure CalcVelocity(var Msg: TMessage);
      virtual Id_First + VelocityCalcID;
    procedure CalcGPM(var Msg: TMessage);
      virtual Id_First + GPMCalcID;
    procedure CalcArea(var Msg: TMessage);
      virtual Id_First + AreaCalcID;

    procedure DoPrint(AEdit: PEdit); virtual;
  end;

procedure DoFlowPipe(AParent: PWindowsObject; MetricFlag: Boolean;
                              InitGPM: PChar);
procedure DoFlowArea(AParent: PWindowsObject; MetricFlag: Boolean;
                              InitGPM: PChar);

implementation
uses WinProcs, Validize, Strings;

{$I CONV.INC}

const
  CRLF = ^M+^J;

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

{ TFlowPipeDialog Methods ------------------------------------------------ }
constructor TFlowPipeDialog.Init(AParent: PWindowsObject; AName: PChar;
                                 MetricFlag: Boolean; InitGPM: PChar);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  VelocityEdit:=New(PASEdit, InitResource(@Self, VelocityEditID, SNumWidth+1));
  VelocityEdit^.SetValidator(New(PPosRealValidator, Init));
  GPMEdit:=New(PASEdit, InitResource(@Self, GPMEditID, SNumWidth+1));
  GPMEdit^.SetValidator(New(PPosRealValidator, Init));
  PipeEdit:=New(PASEdit, InitResource(@Self, PipeEditID, SNumWidth+1));
  PipeEdit^.SetValidator(New(PPosRealValidator, Init));
  StrCopy(TempGPM, InitGPM);
end;

procedure TFlowPipeDialog.SetupWindow;
begin
  inherited SetupWindow;
  GPMEdit^.SetText(TempGPM);
end;

procedure TFlowPipeDialog.CalcVelocity(var Msg: TMessage);
var
  Velocity,
  GPM,
  PipeSize: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;
  PipeEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, PipeSize, ErrorPos);
  if Metric then PipeSize:=PipeSize/PipeConv;
  if PipeSize<=0 then
    MessageBox(HWindow, 'Pipe size must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Velocity:=((GPM/7.48)/
               ((Sqr(PipeSize/2)*pi)/144))/60;
    if Metric then Velocity:=Velocity*VelocityConv;
    StrPCopy(TempStr, Real2String(Velocity, NumWPrec, NumDPrec));
    VelocityEdit^.SetText(TempStr);
  end;
end;

procedure TFlowPipeDialog.CalcGPM(var Msg: TMessage);
var
  Velocity,
  GPM,
  PipeSize: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  PipeEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, PipeSize, ErrorPos);
  if Metric then PipeSize:=PipeSize/PipeConv;

  GPM:=(Velocity*60)*((Sqr(PipeSize/2)*pi)/144)*7.48;
  if Metric then GPM:=GPM*GPMConv;
  StrPCopy(TempStr, Real2String(GPM, NumWPrec, NumDPrec));
  GPMEdit^.SetText(TempStr);
end;

procedure TFlowPipeDialog.CalcPipe(var Msg: TMessage);
var
  Velocity,
  GPM,
  PipeSize: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;
  if Velocity<=0 then
    MessageBox(HWindow, 'Velocity must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    PipeSize:=Sqrt((((GPM/7.48)/(Velocity*60))*144)/pi)*2;
    if Metric then PipeSize:=PipeSize*PipeConv;
    StrPCopy(TempStr, Real2String(PipeSize, NumWPrec, NumDPrec));
    PipeEdit^.SetText(TempStr);
  end;
end;

procedure TFlowPipeDialog.DoPrint(AEdit: PEdit);
var
  Velocity,
  GPM,
  PipeSize: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
  Temp: array[0..255] of Char;
begin
  inherited DoPrint(AEdit);

  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  PipeEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, PipeSize, ErrorPos);
  if Metric then PipeSize:=PipeSize/PipeConv;
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;

  if round(GPM+0.3)<>round(((Velocity*60)*((Sqr(PipeSize/2)*pi)/144)*7.48)+0.3) then
    MessageBox(GetLastActivePopUp(Application^.MainWindow^.HWindow),
            'Cannot print pipe sizing information until you run calculate.',
            'Print Calculation Error', mb_OK+mb_IconExclamation)
  else
  with AEdit^ do
  begin
    Insert('Pipe Sizing Calculation Window:'+CRLF);
    Insert('-------------------------------'+CRLF+CRLF);

    if Metric then
      Insert('Velocity (Meters/Sec): ')
    else
      Insert('Velocity (ft/sec): ');
    VelocityEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Liters Per Minute: ')
    else
      Insert('Gallons Per Minute: ');
    GPMEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Pipe Size (mm): ')
    else
      Insert('Pipe Size (in.): ');
    PipeEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;
end;

{ TFlowAreaDialog Methods ------------------------------------------------ }
constructor TFlowAreaDialog.Init(AParent: PWindowsObject; AName: PChar;
                                 MetricFlag: Boolean; InitGPM: PChar);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  VelocityEdit:=New(PASEdit, InitResource(@Self, VelocityEditID, SNumWidth+1));
  VelocityEdit^.SetValidator(New(PPosRealValidator, Init));
  GPMEdit:=New(PASEdit, InitResource(@Self, GPMEditID, SNumWidth+1));
  GPMEdit^.SetValidator(New(PPosRealValidator, Init));
  AreaEdit:=New(PASEdit, InitResource(@Self, AreaEditID, SNumWidth+1));
  AreaEdit^.SetValidator(New(PPosRealValidator, Init));
  StrCopy(TempGPM, InitGPM);
end;

procedure TFlowAreaDialog.SetupWindow;
begin
  inherited SetupWindow;
  GPMEdit^.SetText(TempGPM);
end;

procedure TFlowAreaDialog.CalcVelocity(var Msg: TMessage);
var
  Velocity,
  GPM,
  Area: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;
  AreaEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Area, ErrorPos);
  if Metric then Area:=Area/AreaConv;
  if Area<=0 then
    MessageBox(HWindow, 'Area must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Velocity:=((GPM/7.48)/(Area/144))/60;
    if Metric then Velocity:=Velocity*VelocityConv;
    StrPCopy(TempStr, Real2String(Velocity, NumWPrec, NumDPrec));
    VelocityEdit^.SetText(TempStr);
  end;
end;

procedure TFlowAreaDialog.CalcGPM(var Msg: TMessage);
var
  Velocity,
  GPM,
  Area: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  AreaEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Area, ErrorPos);
  if Metric then Area:=Area/AreaConv;

  GPM:=(Velocity*60)*(Area/144)*7.48;
  if Metric then GPM:=GPM*GPMConv;
  StrPCopy(TempStr, Real2String(GPM, NumWPrec, NumDPrec));
  GPMEdit^.SetText(TempStr);
end;

procedure TFlowAreaDialog.CalcArea(var Msg: TMessage);
var
  Velocity,
  GPM,
  Area: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
begin
  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;
  if Velocity<=0 then
    MessageBox(HWindow, 'Velocity must be greater than ZERO!!',
                        'Calculation Error', mb_OK+mb_IconExclamation)
  else
  begin
    Area:=((GPM/7.48)/(Velocity*60))*144;
    if Metric then Area:=Area*AreaConv;
    StrPCopy(TempStr, Real2String(Area, NumWPrec, NumDPrec));
    AreaEdit^.SetText(TempStr);
  end;
end;

procedure TFlowAreaDialog.DoPrint(AEdit: PEdit);
var
  Velocity,
  GPM,
  Area: Real;
  ErrorPos: Integer;
  TempStr: array[0..SNumWidth+1] of Char;
  Temp: array[0..255] of Char;
begin
  inherited DoPrint(AEdit);

  VelocityEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Velocity, ErrorPos);
  if Metric then Velocity:=Velocity/VelocityConv;
  AreaEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, Area, ErrorPos);
  if Metric then Area:=Area/AreaConv;
  GPMEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, GPM, ErrorPos);
  if Metric then GPM:=GPM/GPMConv;

  if round(GPM+0.3)<>round(((Velocity*60)*(Area/144)*7.48)+0.3) then
    MessageBox(GetLastActivePopUp(Application^.MainWindow^.HWindow),
            'Cannot print area sizing information until you run calculate.',
            'Print Calculation Error', mb_OK+mb_IconExclamation)
  else
  with AEdit^ do
  begin
    Insert('Area Sizing Calculation Window:'+CRLF);
    Insert('-------------------------------'+CRLF+CRLF);

    if Metric then
      Insert('Velocity (Meters/Sec): ')
    else
      Insert('Velocity (ft/sec): ');
    VelocityEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Liters Per Minute: ')
    else
      Insert('Gallons Per Minute: ');
    GPMEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    if Metric then
      Insert('Cross Sectional Area (Sq. mm): ')
    else
      Insert('Cross Sectional Area (sq in.): ');
    AreaEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;
end;

{ ------------------------------------------------------------------------ }

procedure DoFlowPipe(AParent: PWindowsObject; MetricFlag: Boolean;
                              InitGPM: PChar);
begin
  if not MetricFlag then
    Application^.ExecDialog(New(PFlowPipeDialog,
        Init(AParent, 'FLOWPIPE_ENGLISH', MetricFlag, InitGPM)))
  else
    Application^.ExecDialog(New(PFlowPipeDialog,
        Init(AParent, 'FLOWPIPE_METRIC', MetricFlag, InitGPM)));
end;

procedure DoFlowArea(AParent: PWindowsObject; MetricFlag: Boolean;
                              InitGPM: PChar);
begin
  if not MetricFlag then
    Application^.ExecDialog(New(PFlowAreaDialog,
        Init(AParent, 'FLOWAREA_ENGLISH', MetricFlag, InitGPM)))
  else
    Application^.ExecDialog(New(PFlowAreaDialog,
        Init(AParent, 'FLOWAREA_METRIC', MetricFlag, InitGPM)));
end;

end.
