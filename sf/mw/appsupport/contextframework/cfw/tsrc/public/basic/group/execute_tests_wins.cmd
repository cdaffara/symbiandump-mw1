@echo off
REM Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
REM All rights reserved.
REM This component and the accompanying materials are made available
REM under the terms of "Eclipse Public License v1.0"
REM which accompanies this distribution, and is available
REM at the URL "http://www.eclipse.org/legal/epl-v10.html".
REM
REM Initial Contributors:
REM Nokia Corporation - initial contribution.
REM
REM Contributors:
REM
REM Description:  ?Description
REM
REM
@echo on


rem ----------------------------------------------------------------------------

SET ALL_NO_SISUPGRADES=
SET ALL=
SET COMPILE=
SET EXECUTE=

SET ALL_NO_SISUPGRADES=mt_cfservices.dll mt_cfcontextsourcemanager.dll mt_cfcontextsourcesettingsmanager.dll mt_cfclient.dll mt_cfscriptengine.dll mt_basicoperationsplugin.dll mt_cfoperationpluginmanager.dll ut_cfcontextsourcemanager.dll ut_ccfcontextmanager.dll ut_ccfengine.dll ut_cfcontextsourcesettingsmanager.dll ut_cfactivatorengine.dll ut_cfoperationpluginmanager.dll mt_cfactionplugin.dll
SET ALL=%ALL_NO_SISUPGRADES% mt_cfsisupgrade.dll

IF "%1" == "help" (
	@echo Usage:
	@echo   Compile and execute test cases: execute_tests_wins compile
	@echo   Compile test cases:             execute_tests_wins compile_only
	@echo   Execute test cases:             execute_tests_wins
	goto end
) ELSE IF "%1" == "compile" (
    SET COMPILE=yes
    SET EXECUTE=yes
) ELSE IF "%1" == "compile_only" (
    SET COMPILE=yes
    SET EXECUTE=no
) ELSE (
    SET COMPILE=no
    SET EXECUTE=yes
)

rem START ----------------------------------------------------------------------

IF "%COMPILE%" == "yes" (
	goto compile
)

IF "%EXECUTE%" == "yes" (
	goto execute
)

goto end:

rem ----------------------------------------------------------------------------
:compile
rem ----------------------------------------------------------------------------

@echo Compiling CFW tests...

call bldmake bldfiles -k
call abld test build
call pushd ..\sis
call create_sisupgrades.cmd v1 compile
call create_sisupgrades.cmd v2 compile
call create_sisupgrades.cmd v3 compile
call popd

@echo Compiling CFW tests... Finished.

IF "%EXECUTE%" == "yes" (
	goto execute
) ELSE (
	goto end
)

rem ----------------------------------------------------------------------------
:execute
rem ----------------------------------------------------------------------------

@echo Executing CFW tests...

rem call \epoc32\release\winscw\udeb\eunitexerunner.exe /w 60 /l xml /e s60appenv /t 60 %ALL%
call \epoc32\release\winscw\udeb\eunitexerunner.exe /w 60 /l xml /e s60appenv /t 60 %ALL_NO_SISUPGRADES%
call copy /y \epoc32\winscw\c\shared\eunit\logs\eunit_log.xml .
rem call eunit_log.xml

@echo Executing CFW tests... Finished.

goto end

rem ----------------------------------------------------------------------------
:end
rem ----------------------------------------------------------------------------

SET ALL_NO_SISUPGRADES=
SET ALL=
SET COMPILE=
SET EXECUTE=

@echo Finished.
