@echo off
setlocal

REM ==========================
REM Compile 1.cpp
REM ==========================
g++ -o 1.exe 1.cpp
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    exit /b 1
)

REM ==========================
REM Run program on input.txt
REM ==========================
if exist output.txt del output.txt
for /f "usebackq delims=" %%L in ("input.txt") do (
    echo %%L | 1.exe >> output.txt
)

REM ==========================
REM Compare output vs expected_output
REM ==========================
fc /W output.txt expected_output.txt > nul
if %ERRORLEVEL% EQU 0 (
    echo All test cases PASSED!
) else (
    echo FAIL - output.txt differs from expected_output.txt
    echo --- Differences ---
    fc output.txt expected_output.txt
)

endlocal