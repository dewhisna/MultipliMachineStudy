{

  Miscellaneous Validators Unit V1.0

  Written by Donald Whisnant

}

unit Validize;

interface
uses Objects, Validate;

type
{ Validator Definitions: }
  PPosRealValidator = ^TPosRealValidator;
  TPosRealValidator = object(TValidator)
    function IsValidInput(var S: string; SuppressFill: Boolean): Boolean; virtual;
  end;

  PPosIntValidator = ^TPosIntValidator;
  TPosIntValidator = object(TValidator)
    function IsValidInput(var S: string; SuppressFill: Boolean): Boolean; virtual;
  end;

  PPosNZIntValidator = ^TPosNZIntValidator;
  TPosNZIntValidator = object(TValidator)
    function IsValidInput(var S: string; SuppressFill: Boolean): Boolean; virtual;
  end;

implementation
{ TEdit Validators: ------------------------------------------------------ }

function TPosRealValidator.IsValidInput(var S: string;
                                  SuppressFill: Boolean): Boolean;
var
  i: integer;
  OkFlag: Boolean;
  n: integer;
begin
  OkFlag:=True;
  n:=0;
  for i:=1 to length(S) do
    begin
      if (((S[i]<'0') or (S[i]>'9')) and (S[i]<>'.')) then OkFlag:=False;
      if (S[i]='.') then n:=n+1;
    end;
  if n>1 then OkFlag:=False;
  IsValidInput:=OkFlag;
end;

function TPosIntValidator.IsValidInput(var S: string;
                                 SuppressFill: Boolean): Boolean;
var
  i: integer;
  OkFlag: Boolean;
begin
  OkFlag:=True;
  for i:=1 to Length(S) do
    if ((S[i]<'0') or (S[i]>'9')) then OkFlag:=False;
  IsValidInput:=OkFlag;
end;

function TPosNZIntValidator.IsValidInput(var S: string;
                                 SuppressFill: Boolean): Boolean;
var
  i: integer;
  OkFlag: Boolean;
  TempVal: LongInt;
  ErrorPos: Integer;
begin
  OkFlag:=True;
  for i:=1 to Length(S) do
    if ((S[i]<'0') or (S[i]>'9')) then OkFlag:=False;
  Val(S, TempVal, ErrorPos);
  if ((TempVal=0) or (ErrorPos<>0)) then OkFlag:=False;
  IsValidInput:=OkFlag;
end;

end.
