@ECHO OFF
ECHO This batch file will create a distribution diskette
ECHO for the Multipli Machine Study Program...
ECHO Insert a blank disk in drive A:
PAUSE
COPY MACHSTDY.EXE A:
COPY MACHSTDY.HLP A:
COPY INSTALL.EXE A:
COPY DIST\*.* A:
ECHO DONE...

