@echo off
setlocal

echo Building with MinGW...

echo Cleaning release directory...
if exist release rmdir /s /q release
mkdir release

echo.
echo Running qmake...
"C:\Qt\5.15.2\mingw81_64\bin\qmake.exe" -spec win32-g++ -r mindvision_qobject.pro
if errorlevel 1 (
    echo qmake failed.
    goto error
)

echo.
echo Running mingw32-make...
C:\msys64\mingw64\bin\mingw32-make.exe
if errorlevel 1 (
    echo mingw32-make failed.
    goto error
)

echo.
echo Build successful!
endlocal
goto :eof

:error
echo Build failed.
endlocal
exit /b 1