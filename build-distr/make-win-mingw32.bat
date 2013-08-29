set QT_DIR=c:\Qt\Qt5.1.0\5.1.0\mingw48_32
set BUILD_DIR=..\build-yaps-Desktop_Qt_5_1_0_MinGW_32bit-Release\release
set CRYPTO_DIR=..\cryptopp562\release
set KBHOOK_DIR=..\kbhook\release

del /Q /F /S *.exe
del /Q /F /S *.dll
mkdir "%CD%\sqldrivers"
del /Q /F /S "%CD%\sqldrivers\*.*"

xcopy /Y /R "%QT_DIR%\bin\icudt51.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\icuin51.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\icuuc51.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\libgcc_s_dw2-1.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\libstdc++-6.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\libwinpthread-1.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Core.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Gui.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Widgets.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\bin\Qt5Sql.dll" "%CD%\"
xcopy /Y /R "%QT_DIR%\plugins\sqldrivers\qsqlite.dll" "%CD%\sqldrivers\"
xcopy /Y /R "%CRYPTO_DIR%\cryptopp562.dll" "%CD%\"
xcopy /Y /R "%KBHOOK_DIR%\winkbhook.dll" "%CD%\"
xcopy /Y /R "%BUILD_DIR%\yaps.exe" "%CD%\"