@rem
@rem Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
:: This batch file is used to run tests on 9.3 and higher versions.
::
:: Any ADDITIONS or DELETIONS to the test cases in this script MUST
:: be copied to the partner script that is used on hardware:
:: ..\hardware\apparctest_run.bat
:: *******************************************************************

set APPARCTEST_SUMMARY=\epoc32\winscw\c\logs\testexecute\apparctest_summary.txt
if exist %APPARCTEST_SUMMARY% del %APPARCTEST_SUMMARY% >NUL

call :test T_AppList 
call :test T_AppListFile_Boot1
call :test T_AppListFile_Boot2
call :test T_AppListFile_Boot3
call :test T_AppListFileUpdate
call :test T_ApsScan
call :test T_AutoMMCReaderOpen 
call :test T_Backup 
call :test T_Capability1 
call :test T_Capability2 
call :test T_DataTypeMappingWithSid
call :test T_Caption 
call :test T_Cmdln 
call :test T_ControlPanelTest 
call :test T_DataMappingPersistenceA 
call :test T_DataMappingPersistenceB 
call :test T_DataMappingPersistenceC 
call :test T_DriveNotification
call :test T_EndTask
call :test T_Exe 
call :test T_File2 
call :test T_File3 
call :test T_Foreground 
call :test T_GroupName 
call :test T_GroupName_ver1 
call :test T_GroupName_ver2 
call :test T_IntegritySupport
call :test T_IntegritySupportReboot1
call :test T_IntegritySupportReboot2
call :test T_LargeStack
call :test T_Locale 
call :test T_Mdr 
call :test T_MimeContentPolicy
call :test T_Mru 
call :test T_NonNativeApps 
call :test T_Notif 
call :test T_OOM 
call :test T_Pro 
call :test T_Proc 
call :test T_RApaLsSession 
call :test T_RuleBasedLaunching 
call :test T_Serv2 
call :test T_Serv3 
call :test T_ServiceBase 
call :test T_ServiceRegistry 
call :test T_Services 
call :test T_StartApp 
call :test T_StartDoc 
call :test T_Wgnam 
call :test T_WindowChaining 
::call :test T_RecUpgrade  This test can not run on emulator as it requires loading, unloading and deletion of a plug-in and Windows OS don’t allow deletion of loaded binaries 
call :test T_UpdateAppList
call :test T_ForceReg
call :test T_ClientNotif
call :test T_NonNativeTest

call sysstart_apparc_run.bat
type \epoc32\winscw\c\logs\testexecute\sysstart_apparctest_summary.txt >> %APPARCTEST_SUMMARY%

goto :skipFunctions

:test
	set TEST_CASE=%1
	set TEST_RSC=%2
	set EPOCWIND_COPY=\epoc32\winscw\c\logs\testexecute\apparctest_%TEST_CASE%_epocwind.out
	set EPOCWIND_ORIG=%TEMP%\epocwind.out

	if exist %EPOCWIND_COPY% del %EPOCWIND_COPY%

	echo testexecute.exe z:\apparctest\apparctest_%TEST_CASE%.script
	testexecute.exe z:\apparctest\apparctest_%TEST_CASE%.script

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
	call :checkLog \epoc32\winscw\c\logs\testexecute\apparctest_%TEST_CASE%.htm "%TEST_CASE% ***Result = PASS" 1
	echo %TEST_CASE% - %TEST_RESULT_FLAG% >> %APPARCTEST_SUMMARY% 
goto :EOF

:skipFunctions


endlocal
