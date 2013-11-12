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


set WSINI= %EPOCROOT%epoc32\data\z\system\data\wsini.ini
set WSINI_BAK= %EPOCROOT%epoc32\data\z\system\data\wsini.bak
set WSINI_TEST= %EPOCROOT%epoc32\data\z\appfwk\test\mulscreens.ini
set WSINI_TEST_CHANGETRACKING= %EPOCROOT%epoc32\data\z\appfwk\test\mulscreens_changetracking.ini

if /i "%1"=="uninstall" goto :uninstall

if /i "%1"=="install" if /i "%2"=="changetracking" set CHANGETRACKING=on
if defined CHANGETRACKING goto :install_changetracking
if /i "%1"=="install" if not defined CHANGETRACKING goto :install
if /i "%1"=="help" goto :help

:help
	echo.
	echo Description:
	echo This script substitutes in the wsini ini file to run multiple screens on the H4
	echo reference platform. 
	echo.It takes as a first argument "install or "uninstall"
	echo.It can can also take "changetracking" as a second argument in order to use a wsini ini file 
	echo that enables CHANGETRACKING
goto :EOF

:install
echo Replacing WSINI.INI files with test versions.
	if not exist %WSINI_BAK% if exist %WSINI% copy %WSINI% %WSINI_BAK% >NUL
	if exist %WSINI% xcopy /r /y %WSINI_TEST% %WSINI% >NUL 
goto :EOF

:install_changetracking
echo Replacing WSINI.INI files with test versions htat enable CHANGETRACKING.
	if not exist %WSINI_BAK% if exist %WSINI% copy %WSINI% %WSINI_BAK% >NUL
	if exist %WSINI% xcopy /r /y %WSINI_TEST_CHANGETRACKING% %WSINI% >NUL 
goto :EOF

:uninstall
echo Restoring WSINI.INI files.
	if exist %WSINI_BAK% copy %WSINI_BAK% %WSINI% >NUL
	if exist %WSINI_BAK% del %WSINI_BAK% >NUL
goto :EOF
