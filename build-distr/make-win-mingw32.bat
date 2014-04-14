set QT_DIR=c:\Qt\Qt5.2.1\5.2.1\mingw48_32
set BUILD_DIR=..\build-yaps-Desktop_Qt_5_2_1_MinGW_32bit-Release\release
set CRYPTO_DIR=..\cryptopp562\release
set KBHOOK_DIR=..\kbhook\release
set YAPS_DIR=..\yaps

rmdir /S /Q distr
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
xcopy /Y /R "%CRYPTO_DIR%\cryptopp562.dll" "distr\"
xcopy /Y /R "%KBHOOK_DIR%\winkbhook.dll" "distr\"
xcopy /Y /R "%BUILD_DIR%\yaps.exe" "distr\"
xcopy /Y /R "%YAPS_DIR%\yaps.json" "distr\"