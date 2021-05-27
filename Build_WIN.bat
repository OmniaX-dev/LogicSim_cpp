@echo off

set /p TARGET= <0.win\build.target
if "%TARGET%" == "DEBUG" goto DBG_SET
if "%TARGET%" == "RELEASE" goto RLS_SET
	echo Unknown build target.
	echo.
	echo %TARGET%
	pause
	exit
:DBG_SET
	set /p ARGS= <0.win\.gcc_args_d.args
	set /p LIBS= <0.win\.libs_d.args
	set /p DEFS= <0.win\.defs_d.args
	goto END_SET
:RLS_SET
	set /p ARGS= <0.win\.gcc_args.args
	set /p LIBS= <0.win\.libs.args
	set /p DEFS= <0.win\.defs.args
:END_SET
	 
set /p SRC_FLDR= <0.win\.src.args
set /p INCL= <0.win\.incl.args
set /p APP_NAME= <0.win\.app_name.anm
set /p BUILD_NR= <build.nr
set /p LIBS_F= <0.win\.libsf.args

call 0.win\load_res.bat
echo ===========================================
echo *** Building Project... [ Build_target: %TARGET% ]
@echo on
g++ -O3 %SRC_FLDR% -o bin/windows/%APP_NAME%.exe 0.win/icon.res %ARGS% %INCL% %LIBS_F% %LIBS% %DEFS% -D __BUILD_NUMBER__=%BUILD_NR% -D __APP_NAME__=\"%APP_NAME%\"
@echo off
echo.
echo *** done!
echo ===========================================
echo.
pause
:end