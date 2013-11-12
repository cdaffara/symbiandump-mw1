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

:: Initialise local variables
set COMMAND_CHOICE=%1
set TEST_CASE=%2
set TEST_BOOTMODE=%3
set TESTCASE_NUMBER=%4

set FALLBACK_RSC_LOCATION=Z\private\10205C44
set FALLBACK_TEST_RSC=%FALLBACK_RSC_LOCATION%\tssaac_emulator.RSC
set FALLBACK_TEST_EXE_RSC=%FALLBACK_RSC_LOCATION%\tssaac_tapp_emulator.RSC
set FALLBACK_SM0_RSC=%FALLBACK_RSC_LOCATION%\SSCForStartupMode0.RSC

set SSMA_RSC_LOCATION=Z\private\2000d75b\startup\0
set SSMA_RSC_NAME=0
set SSMA_RSC_LOCATION_ORIGINAL=Z\private\2000d75b\startup\0.original
set SSMA_RSC_NAME_ORIGINAL=0.original
set SSMA_SM0_RSC=%SSMA_RSC_LOCATION%\gsastartupcmdlist0.rsc
set SSMA_RSC_TEST_LOCATION=Z\private\2000d75b\startup\test\tssaac_emulator_ssma.RSC
set SSMA_RSC_TEST_EXE_LOCATION=Z\private\2000d75b\startup\test\tssaac_tapp_emulator_ssma.RSC
set SSMA_SM0_RSC_LOCATION=Z\private\2000d75b\startup\test\gsastartupcmdlist0.rsc


set TESTCASE_CONFIG=z\apparctest\TestCaseConfig.txt
set EPOCWIND=%TEMP%\epocwind.out
set TESTNONPANIC_RESULT_FILE=\epoc32\winscw\c\SysStartApparc_TestNonPanic.txt

set CHOICE=%COMMAND_CHOICE%%TEST_BOOTMODE%
goto :%CHOICE%

:instfb
	if exist "%SSMA_RSC_LOCATION%" ren %SSMA_RSC_LOCATION% %SSMA_RSC_NAME_ORIGINAL%
	if not exist "%FALLBACK_TEST_RSC%" echo SysStart Test: ERROR - test resource file does not exist & goto :EOF
	if not exist "%FALLBACK_TEST_EXE_RSC%" echo SysStart Test: ERROR - test resource file does not exist & goto :EOF

	if not exist %FALLBACK_SM0_RSC%.original copy %FALLBACK_SM0_RSC% %FALLBACK_SM0_RSC%.original > NUL
	
	if /i "%TEST_CASE%" NEQ "T_TestLocalisedCaptionL" copy %FALLBACK_TEST_RSC% %FALLBACK_SM0_RSC% > NUL
	if /i "%TEST_CASE%" == "T_TestLocalisedCaptionL" copy %FALLBACK_TEST_EXE_RSC% %FALLBACK_SM0_RSC% > NUL
	
	if exist %TESTCASE_CONFIG% del %TESTCASE_CONFIG%
	if exist %TESTNONPANIC_RESULT_FILE% del %TESTNONPANIC_RESULT_FILE%

	echo Test Case %TEST_CASE%=%TESTCASE_NUMBER% > %TESTCASE_CONFIG%

goto :EOF

:uninstfb
	if exist %SSMA_RSC_LOCATION_ORIGINAL% ren %SSMA_RSC_LOCATION_ORIGINAL% %SSMA_RSC_NAME% > NUL
	if exist %FALLBACK_SM0_RSC%.original copy %FALLBACK_SM0_RSC%.original %FALLBACK_SM0_RSC% > NUL
	if exist %FALLBACK_SM0_RSC%.original del %FALLBACK_SM0_RSC%.original

	:: TestStartApp1L and TestLocalisedCaptionL are the only non-paniccing test cases
	if /i %TEST_CASE% == "T_TestStartApp1L" goto :bypass
	if /i %TEST_CASE% == "T_TestLocalisedCaptionL" goto :bypass

	call :test
	:bypass
	if exist %EPOCROOT%epoc32\winscw\c\T_SSAAC_PanicResultFile.txt del %EPOCROOT%epoc32\winscw\c\T_SSAAC_PanicResultFile.txt
goto :EOF

:instss
	if not exist "%SSMA_RSC_TEST_LOCATION%" echo Ssma Test: ERROR - test resource file does not exist %SSMA_RSC_TEST_LOCATION% & goto :EOF
	if not exist "%SSMA_RSC_TEST_EXE_LOCATION%" echo Ssma Test: ERROR - test resource file does not exist %SSMA_RSC_TEST_EXE_LOCATION% & goto :EOF

	if exist %SSMA_SM0_RSC% copy %SSMA_SM0_RSC% %SSMA_SM0_RSC_LOCATION% > NUL
	if exist %SSMA_SM0_RSC% del %SSMA_SM0_RSC%

	
	if /i "%TEST_CASE%" NEQ "T_TestLocalisedCaptionL" copy %SSMA_RSC_TEST_LOCATION% %SSMA_SM0_RSC% > NUL
	if /i "%TEST_CASE%" == "T_TestLocalisedCaptionL" copy %SSMA_RSC_TEST_EXE_LOCATION% %SSMA_SM0_RSC% > NUL
	
	if exist %TESTCASE_CONFIG% del %TESTCASE_CONFIG%
	if exist %TESTNONPANIC_RESULT_FILE% del %TESTNONPANIC_RESULT_FILE%

	echo Test Case %TEST_CASE%=%TESTCASE_NUMBER% > %TESTCASE_CONFIG%

goto :EOF

:uninstss
	if exist %SSMA_SM0_RSC_LOCATION% copy %SSMA_SM0_RSC_LOCATION% %SSMA_SM0_RSC% > NUL

	if exist %SSMA_SM0_RSC_LOCATION% del %SSMA_SM0_RSC_LOCATION%

	:: TestStartApp1L and TestLocalisedCaptionL are the only non-paniccing test cases
	if /i "%TEST_CASE%" == "T_TestStartApp1L" goto :bypass
	if /i "%TEST_CASE%" == "T_TestLocalisedCaptionL" goto :bypass
	call :test
	:bypass
	if exist %EPOCROOT%epoc32\winscw\c\T_SSAAC_PanicResultFile.txt del %EPOCROOT%epoc32\winscw\c\T_SSAAC_PanicResultFile.txt
goto :EOF

:execTimeOut
	set TIMEOUT=300
	\ExecTimeOut.exe "testexecute.exe z:\apparctest\apparctest_%TEST_CASE%.script" %TIMEOUT%
goto :EOF

:testExecute
	testexecute.exe z:\apparctest\apparctest_%TEST_CASE%.script
goto :EOF

:test
	call z\apparctest\sysstart_apparc_checkEpocWind.bat %TEST_CASE%

	if exist %TEMP%\epocwind.out copy %TEMP%\epocwind.out %TEMP%\epocwind.out.temp > NUL

	:: If using SYMBIAN overnight DABS test scripts, then utilise ExecTimeOut to ensure
	:: execution does not exceed expected execution time.
	if exist \ExecTimeOut.exe call :execTimeOut

	:: If simply executing manually, then call testexecute directly.
	if not exist \ExecTimeOut.exe call :testExecute

	if exist %TEMP%\epocwind.out.temp copy %TEMP%\epocwind.out.temp %TEMP%\epocwind.out > NUL
	if exist %TEMP%\epocwind.out.temp del %TEMP%\epocwind.out.temp
goto :EOF

:restore
	if exist %SM0_RSC%.original copy %SM0_RSC%.original %SM0_RSC% > NUL
	if exist %SM0_RSC%.original del %SM0_RSC%.original
goto :EOF

endlocal

