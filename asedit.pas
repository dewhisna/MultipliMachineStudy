{

                Machine Study

     Copyright(c)1994 Multipli Machinery

        Written by Donald Whisnant

        Auto Select Edit Object V1.0
}

unit ASEdit;

interface
uses WinTypes, Objects, OWindows, ODialogs;

type
  PASEdit = ^TASEdit;
  TASEdit = object(TEdit)
    procedure WMSetFocus(var Msg: TMessage);
      virtual wm_First + wm_SetFocus;
    procedure WMLButtonDown(var Msg: TMessage);
      virtual wm_First + wm_LButtonDown;
  end;

{ ------------------------------------------------------------------------ }

implementation
uses WinProcs;

{ TASEdit Methods ------------------------------------------------------- }
procedure TASEdit.WMLButtonDown(var Msg: TMessage);
begin
  inherited WMLButtonDown(Msg);
  SetSelection(0, GetTextLen);
end;

procedure TASEdit.WMSetFocus(var Msg: TMessage);
begin
  DefWndProc(Msg);
  SetSelection(0, GetTextLen);
end;

end.
