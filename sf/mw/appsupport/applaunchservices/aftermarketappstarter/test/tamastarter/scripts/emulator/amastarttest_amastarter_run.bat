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

:: *******************************************************************
:: Any ADDITIONS or DELETIONS to the test cases in this script MUST
:: be copied to the partner script that is used on hardware:
:: ..\hardware\amastarttest_amastarter_run.bat
:: *******************************************************************


:: AmaStart AmaStarter tests
call :test cancel
call :test invalidsid
call :test ssc fallback tamastarter_amaadder.rsc 
call :test ssc ssma tamastarter_amaadder_ssma.rsc
call :test standalone
call :test startl
call :test startlasync
call :test startlpending

goto :skipFunctions

:test
	set TEST_CASE=%1
	set TEST_BOOTMODE=%2
	set TEST_RSC=%3
	if "%TEST_RSC%" == "" call :normal %TEST_CASE%
	if "%TEST_RSC%" NEQ "" call :setup %TEST_CASE% %TEST_BOOTMODE% %TEST_RSC% 
goto :EOF

:normal
	set TEST_CASE=%1
	echo testexecute.exe z:\amastarttest\amastarttest_amastarter_%TEST_CASE%.script
	testexecute.exe z:\amastarttest\amastarttest_amastarter_%TEST_CASE%.script
goto :EOF

:setup
	set TEST_CASE=%1
	set TEST_BOOTMODE=%2
	set TEST_RSC=%3
	call z\amastarttest\amastarttest_amastarter_setup.bat install %TEST_BOOTMODE% %TEST_RSC%
	echo testexecute.exe z:\amastarttest\amastarttest_amastarter_%TEST_CASE%.script
	testexecute.exe z:\amastarttest\amastarttest_amastarter_%TEST_CASE%.script
	call z\amastarttest\amastarttest_amastarter_setup.bat uninstall %TEST_BOOTMODE% %TEST_RSC% 
	call :renameHtmFile %TEST_CASE% %TEST_BOOTMODE%
goto :EOF

:renameHtmFile
	set TEST_CASE=%1
	set TEST_BOOTMODE=%2
	if exist \epoc32\winscw\c\logs\testexecute\amastarttest_amastarter_%TEST_CASE%_%TEST_BOOTMODE%.htm del \epoc32\winscw\c\logs\testexecute\amastarttest_amastarter_%TEST_CASE%_%TEST_BOOTMODE%.htm
	ren \epoc32\winscw\c\logs\testexecute\amastarttest_amastarter_%TEST_CASE%.htm amastarttest_amastarter_%TEST_CASE%_%TEST_BOOTMODE%.htm > NUL 2>&1
goto :EOF

:skipFunctions 

endlocal
