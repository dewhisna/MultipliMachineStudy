{

        for Machine Study Program V2.0

    Copyright(c)1994 Multipli Machinery

         Written by Donald Whisnant

   Deflection Calculations SubProgram V1.0

}

unit Defl;

{$R DEFL.RES}

interface
uses WinTypes, Objects, OWindows, ODialogs, ASEdit, PrObj;

const
  CRLF = ^M+^J;

  SNumWidth = 12;

  NumWPrec = 8;
  NumDPrec = 5;

  CenterWidthID = 1101;
  EndWidthID = 1102;
  CouchRollSizeID = 1103;
  BottomRollSizeID = 1104;
  CrownID = 1105;

  CalcItID = 2000;

type
  PValEdit = ^TValEdit;
  TValEdit = object(TASEdit)
    procedure WMKillFocus(var Msg: TMessage);
      virtual wm_First + wm_KillFocus;
  end;

  PNipWidthDialog = ^TNipWidthDialog;
  TNipWidthDialog = object(TPrDialog)
    Metric: Boolean;
    CenterWidthEdit: PValEdit;
    EndWidthEdit: PValEdit;
    CouchRollSizeEdit: PValEdit;
    BottomRollSizeEdit: PValEdit;
    CrownEdit: PStatic;
    constructor Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
    procedure CalcIt(var Msg: TMessage);
      virtual ID_First + CalcItID;
    procedure CalcIt2(var Msg: TMessage);
      virtual wm_First + CalcItID;
    procedure DoPrint(AEdit: PEdit); virtual;

    procedure CalcCrown; virtual;
  end;

procedure DoNipWidth(AParent: PWindowsObject; MetricFlag: Boolean);

implementation
uses WinProcs, Validize, Strings, Inst;

const
  SourceCredit = 'Nip Equations by Beloit->Bulletin No. BMI 115 12/87-6M';

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

{ TNipWidthDialog Methods ------------------------------------------------ }
constructor TNipWidthDialog.Init(AParent: PWindowsObject; AName: PChar;
                              MetricFlag: Boolean);
begin
  inherited Init(AParent, AName);
  Metric:=MetricFlag;
  CenterWidthEdit:=New(PValEdit,
             InitResource(@Self, CenterWidthID, SNumWidth+1));
  CenterWidthEdit^.SetValidator(New(PPosRealValidator, Init));
  EndWidthEdit:=New(PValEdit,
             InitResource(@Self, EndWidthID, SNumWidth+1));
  EndWidthEdit^.SetValidator(New(PPosRealValidator, Init));
  CouchRollSizeEdit:=New(PValEdit,
             InitResource(@Self, CouchRollSizeID, SNumWidth+1));
  CouchRollSizeEdit^.SetValidator(New(PPosRealValidator, Init));
  BottomRollSizeEdit:=New(PValEdit,
             InitResource(@Self, BottomRollSizeID, SNumWidth+1));
  BottomRollSizeEdit^.SetValidator(New(PPosRealValidator, Init));
  CrownEdit:=New(PStatic, InitResource(@Self, CrownID, SNumWidth+1));
end;

procedure TNipWidthDialog.CalcIt(var Msg: TMessage);
begin
  SetFocus(SetFocus(HWindow));
  CalcIt2(Msg);
end;

procedure TNipWidthDialog.CalcIt2(var Msg: TMessage);
begin
  CalcCrown;
end;

procedure TNipWidthDialog.CalcCrown;
var
  CenterWidth,
  EndWidth,
  CouchRollSize,
  BottomRollSize,
  Crown: Real;
  ErrorPos: integer;
  TempStr,
  TempStr2: array[0..SNumWidth+1] of Char;
begin
  CenterWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, CenterWidth, ErrorPos);
  EndWidthEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, EndWidth, ErrorPos);
  CouchRollSizeEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, CouchRollSize, ErrorPos);
  BottomRollSizeEdit^.GetText(TempStr, SNumWidth);
  Val(TempStr, BottomRollSize, ErrorPos);

  if ((CouchRollSize=0) or (BottomRollSize=0)) then
  begin
    CrownEdit^.SetText('');
  end else
  begin
    Crown:=((Sqr(EndWidth)-Sqr(CenterWidth))*
           (CouchRollSize+BottomRollSize))/(2*CouchRollSize*BottomRollSize);
    StrPCopy(TempStr, Real2String(Crown, NumWPrec, NumDPrec));
    if Crown<0 then
    begin
      StrCopy(TempStr2,'[-]');
      StrCopy(TempStr, PChar(@TempStr)+1);
    end else
      StrCopy(TempStr2,'[+]');
    StrCat(TempStr2, TempStr);
    CrownEdit^.SetText(TempStr2);
  end;
end;

procedure TNipWidthDialog.DoPrint(AEdit: PEdit);
var
  Temp: array[0..255] of Char;
begin
  inherited DoPrint(AEdit);

  StrCopy(Temp, SourceCredit);

  SendMessage(HWindow, CalcItID, 0, 0);
  with AEdit^ do
  begin
    Insert('Nip Width to Crown Calculation Window:'+CRLF);
    Insert('--------------------------------------'+CRLF);

    Insert('Nip Width at Center (inches): ');
    CenterWidthEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('Nip Width at 2" from End (inches): ');
    EndWidthEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('Couch Roll Diameter (inches): ');
    CouchRollSizeEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert('Bottom Roll Diameter (inches): ');
    BottomRollSizeEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF);
    Insert('Recommended Crown: ');
    CrownEdit^.GetText(Temp, SizeOf(Temp));
    StrCat(Temp, CRLF);
    Insert(Temp);

    Insert(CRLF+CRLF);
  end;
end;

{ ------------------------------------------------------------------------ }

procedure DoNipWidth(AParent: PWindowsObject; MetricFlag: Boolean);
var
  ADialog: PInstDialog;
begin
  ADialog:=MakeInstDialog(AParent);

  with ADialog^ do
  begin
    SetInstTitle('Nip Width to Crown Calculation Instructions');
    InsertPage;
    InsertTextOnPage(1,'This program will yield a close number for a required ');
    InsertTextOnPage(1,'crown for a given'+CRLF);
    InsertTextOnPage(1,'    "Nip Impression"'+CRLF);
    InsertTextOnPage(1,CRLF);
    InsertTextOnPage(1,'1) First, Set the desired load <EQUAL FRONT TO BACK> and take ');
    InsertTextOnPage(1,'Nip Impression.'+CRLF+CRLF);
    InsertTextOnPage(1,'2) After taking the Nip Impression, make two(2) readings ');
    InsertTextOnPage(1,'from it:'+CRLF);
    InsertTextOnPage(1,'       A) Measure the Nip Width EXACTLY at the CENTER'+CRLF);
    InsertTextOnPage(1,'       B) Measure the Nip Width 2" in from EACH END'+CRLF+CRLF);
    InsertTextOnPage(1,'   NOTE: If the Nip Width is not EXACTLY the same 2" in from ');
    InsertTextOnPage(1,'both ends, the roll is'+CRLF);
    InsertTextOnPage(1,'           unevenly loaded front to back and we will not get ');
    InsertTextOnPage(1,'a "good" number.'+CRLF+CRLF);

    InsertPage;
    InsertTextOnPage(2,'After we have calculated a new crown number, it is important to know');
    InsertTextOnPage(2,' what to do with it:'+CRLF+CRLF);
    InsertTextOnPage(2,'    1) If the calculated value is Positive (+), it must be ADDED ');
    InsertTextOnPage(2,'to the existing crown'+CRLF+CRLF);
    InsertTextOnPage(2,'    2) If the calculated value is Negative (-), it must be SUBTRACTED ');
    InsertTextOnPage(2,'from the existing'+CRLF);
    InsertTextOnPage(2,'           crown'+CRLF+CRLF);
  end;

  Application^.ExecDialog(ADialog);

  if BWCCClassNames then
    Application^.ExecDialog(New(PNipWidthDialog,
          Init(AParent, 'NIPWIDTHB', MetricFlag)))
  else
    Application^.ExecDialog(New(PNipWidthDialog,
          Init(AParent, 'NIPWIDTH', MetricFlag)));
end;

end.
