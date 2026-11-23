@echo off
setlocal
where g++ >nul 2>nul
if errorlevel 1 (
    if exist "D:\msys64\ucrt64\bin\g++.exe" (
        set "PATH=D:\msys64\ucrt64\bin;%PATH%"
    ) else if exist "C:\msys64\ucrt64\bin\g++.exe" (
        set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    )
)

echo [1/2] Compiling resource.rc...
windres resource.rc -O coff -o resource.res
if errorlevel 1 exit /b 1

echo [2/2] Building standalone Monitor.exe...
g++ -O2 -s -Wall -Wextra -Wno-unused-parameter -mwindows Monitor.cpp resource.res -o Monitor.exe ^
    -static -ld2d1 -ldwrite -lgdi32 -luser32 -lshell32 -ladvapi32 -lcomdlg32 -liphlpapi -lws2_32 -lole32
if errorlevel 1 exit /b 1

echo Build succeeded: Monitor.exe
dir Monitor.exe | findstr Monitor.exe
