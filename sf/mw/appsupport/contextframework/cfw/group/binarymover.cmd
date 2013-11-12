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


rem Preconditions
rem ----------------------------------------------------------------------------

IF "%1" == "FINAL" (
    IF "%2" == "WINSCW" (
        IF "%3" == "UDEB" goto DoCopy
    )
)

IF "%1" == "CLEAN" (
    IF "%2" == "WINSCW" (
        IF "%3" == "UDEB" goto DoDelete
        IF "%3" == "" goto DoDelete
    )
)

goto End


rem Copying
rem ----------------------------------------------------------------------------

:DoCopy
@echo on
call copy /y %epocroot%epoc32\release\winscw\udeb\basicoperationsplugin.dll %epocroot%epoc32\release\winscw\udeb\z\sys\bin
@echo off
goto End


rem Deleting
rem ----------------------------------------------------------------------------

:DoDelete
@echo on
call del %epocroot%epoc32\release\winscw\udeb\z\sys\bin\basicoperationsplugin.dll /f
@echo off
goto End


rem Ending
rem ----------------------------------------------------------------------------

:End
