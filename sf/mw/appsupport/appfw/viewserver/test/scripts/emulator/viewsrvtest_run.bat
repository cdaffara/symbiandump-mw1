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
:: ..\hardware\viewsrvtest_run.bat
:: *******************************************************************

set VIEWSRVTEST_SUMMARY=\epoc32\winscw\c\logs\testexecute\viewsrvtest_summary.txt
if exist %VIEWSRVTEST_SUMMARY% del %VIEWSRVTEST_SUMMARY% >NUL

call :test Capability1
call :test Capability2
call :test tvwa

goto :skipFunctions

:test
	set TEST_CASE=%1
	set TEST_RSC=%2
	set EPOCWIND_COPY=\epoc32\winscw\c\logs\testexecute\viewsrvtest_%TEST_CASE%_epocwind.out
	set EPOCWIND_ORIG=%TEMP%\epocwind.out

	if exist %EPOCWIND_COPY% del %EPOCWIND_COPY%

	echo testexecute.exe z:\viewsrvtest\viewsrvtest_%TEST_CASE%.script
	testexecute.exe z:\viewsrvtest\viewsrvtest_%TEST_CASE%.script

	copy %EPOCWIND_ORIG% %EPOCWIND_COPY% >NUL
	call :updateTestSummary
goto :EOF

:checkLog
	:: checkLog SEARCH_FILE SEARCH_STRING EXPECTED_NUMBER_OF_OCCURENCES
	set SEARCH_FILE=%1
	set SEARCH_STRING=%2
	set TEMP_FILE=sysstart_temp.txt
	set EXPECTED_COUNT=%3
	set LOG_COUNT=0
	type %SEARCH_FILE% | find /c %SEARCH_STRING%>%TEMP_FILE%
	for /f "delims=\" %%i in ('type %TEMP_FILE%') do set LOG_COUNT=%%i
	del %TEMP_FILE%
	if "%EXPECTED_COUNT%" ==  "%LOG_COUNT%" goto :EOF
	if "%EXPECTED_COUNT%" NEQ "%LOG_COUNT%" set TEST_RESULT_FLAG=FAIL
goto :EOF

:updateTestSummary
	set TEST_RESULT_FLAG=PASS
	call :checkLog \epoc32\winscw\c\logs\testexecute\viewsrvtest_%TEST_CASE%.htm "%TEST_CASE% ***Result = PASS" 1
	echo %TEST_CASE% - %TEST_RESULT_FLAG% >> %VIEWSRVTEST_SUMMARY% 
goto :EOF

:skipFunctions

endlocal
