@echo off
DEL /S /Q "*.exe"
DEL /S /Q "*.o"
DEL /S /Q "*.opt-record.json.gz"
FOR /F "delims=" %%d IN ('DIR /S /B /AD ^| SORT /R') DO (
    RD "%%d" 2>NUL
)
pause