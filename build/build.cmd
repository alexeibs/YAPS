@echo off
setlocal

if not defined QT_DIR goto qt_dir_error

set BUILD_DIR=%~dp0
set CRYPTO_DIR=cryptopp\release
set KBHOOK_DIR=kbhook\release
set YAPS_DIR=yaps\release
set YAPS_SRC_DIR=..\yaps

cd /d %BUILD_DIR%

echo -- Running qmake...
qmake ..\yaps-all.pro
if ERRORLEVEL 1 goto qmake_failed

echo -- Update translations...
call lrelease %YAPS_SRC_DIR%\yaps.pro
if ERRORLEVEL 1 goto tr_failed

echo -- Running make...
mingw32-make all -j8
if ERRORLEVEL 1 goto make_failed

if exist distr rmdir /S /Q distr
mkdir distr\platforms
mkdir distr\sqldrivers"

xcopy /Y /R "%QT_DIR%\bin\icudt51.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\icuin51.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\icuuc51.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\libgcc_s_dw2-1.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\libstdc++-6.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\libwinpthread-1.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Core.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Gui.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Widgets.dll" "distr\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Sql.dll" "distr\"
xcopy /Y /R "%QT_DIR%\plugins\platforms\qwindows.dll" "distr\platforms\"
xcopy /Y /R "%QT_DIR%\plugins\sqldrivers\qsqlite.dll" "distr\sqldrivers\"
xcopy /Y /R "%CRYPTO_DIR%\cryptopp.dll" "distr\"
xcopy /Y /R "%KBHOOK_DIR%\winkbhook.dll" "distr\"
xcopy /Y /R "%YAPS_DIR%\yaps.exe" "distr\"
xcopy /Y /R "%YAPS_SRC_DIR%\yaps.json" "distr\"

exit /b 0

:qt_dir_error
echo QT_DIR variable is not defined.
exit /b 1

:qmake_failed
echo qmake failed.
exit /b 1

:tr_failed
echo Unable to update translations.
exit /b 1

:make_failed
echo make failed.
exit /b 1
