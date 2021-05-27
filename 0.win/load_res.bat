@echo off
echo ===========================================
echo *** Clearing .\bin\res folder...
rmdir /s /q "%CD%\bin\windows"
mkdir "%CD%\bin\windows"
mkdir "%CD%\bin\windows\res\"
echo *** done!
echo ===========================================
echo.
echo ===========================================
echo *** Copying resources...
Xcopy "%CD%\res" "%CD%\bin\windows\res\" /E /H /C /I
Xcopy "%CD%\xtra" "%CD%\bin\windows\" /E /H /C /I
Xcopy "%CD%\dll" "%CD%\bin\windows\" /E /H /C /I
echo *** done!
echo ===========================================
echo.
echo ===========================================
echo *** Creating icon...
windres 0.win\icon.rc -O coff -o 0.win\icon.res
echo *** done!
echo ===========================================
echo.