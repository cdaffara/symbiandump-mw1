@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem
@echo off
setlocal

set OPT_CT=
if /i "%2"=="changetracking" set OPT_CT=1
if /i "%1"=="uninstall" goto :uninstall
if /i "%1"=="install" goto :install
goto :done

rem *** START STANDARD FILE-REPLACEMENT FUNCTIONS ***
:replace
	set DST=%1
	set SRC=%2
	echo INFO: Replacing %DST% with %SRC%
	copy /y %DST% %DST%.original >NUL
	if defined SRC copy /y %SRC% %DST% >NUL
	goto :EOF
:restore
	echo INFO: Restoring %1 
	if not exist %1.original echo ERROR: %1.original does not exist
	copy /y %1.original %1 >NUL
	del %1.original >NUL
	goto :EOF
:replace_em
	if defined EMULATOR_DATA_DIR (
		call :replace %EMULATOR_DATA_DIR%\%1 %2
	) else (
		call :replace %EPOCROOT%epoc32\release\winscw\udeb\%1 %2
		call :replace %EPOCROOT%epoc32\release\winscw\urel\%1 %2
	)
	goto :EOF
:replace_em2
	if defined EMULATOR_DATA_DIR (
		call :replace %EMULATOR_DATA_DIR%\%1 %EMULATOR_DATA_DIR%\%2
	) else (
		call :replace %EPOCROOT%epoc32\release\winscw\udeb\%1 %EPOCROOT%epoc32\release\winscw\udeb\%2
		call :replace %EPOCROOT%epoc32\release\winscw\urel\%1 %EPOCROOT%epoc32\release\winscw\urel\%2
	)
	goto :EOF
:restore_em
	if defined EMULATOR_DATA_DIR (
		call :restore %EMULATOR_DATA_DIR%\%1
	) else (
		call :restore %EPOCROOT%epoc32\release\winscw\udeb\%1
		call :restore %EPOCROOT%epoc32\release\winscw\urel\%1
	)
	goto :EOF
rem *** END STANDARD FILE-REPLACEMENT FUNCTIONS ***


:install
	echo Replacing files with test versions
	set WSINI_TEST=z\appfwk\test\mulscreens.ini
	if defined OPT_CT set WSINI_TEST=z\appfwk\test\mulscreens_changetracking.ini
	call :replace_em2 z\system\data\wsini.ini   %WSINI_TEST%
	call :replace_em2 z\resource\data\wsini.ini %WSINI_TEST%
	call :replace %EPOC_INI%
	echo. >> %EPOC_INI%
	echo _NewScreen_ >> %EPOC_INI%
	echo ScreenWidth 640 >> %EPOC_INI%
	echo ScreenHeight 240 >> %EPOC_INI%
	goto :done


:uninstall
	call :restore_em z\system\data\wsini.ini
	call :restore_em z\resource\data\wsini.ini
	call :restore    %EPOC_INI%

:done
endlocal

