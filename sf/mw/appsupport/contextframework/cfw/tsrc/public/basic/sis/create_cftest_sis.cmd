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


IF "%1" == "compile" (
    pushd ..\group
    call bldmake bldfiles
    call abld test build armv5 urel
    popd
)

IF EXIST cftest.sis (
    call del /f cftest.sis
)

IF EXIST cftest.sisx (
    call del /f cftest.sisx
)

call makesis cftest.pkg
call signsis cftest.sis cftest.sisx rd.cer rd-key.pem
call del cftest.sis
