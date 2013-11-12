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

IF "%1" == "compile" (
	@echo ERROR: Missing upgrade version
	goto End
)

IF "%1" == "" (
	@echo ERROR: Missing upgrade version
	goto End
)

IF "%2" == "compile" (
    pushd ..\group
    call bldmake bldfiles -k
    IF EXIST contextsource_%1.mmp (
	    call abld test build winscw udeb contextsource_%1
	    call abld test build armv5 urel contextsource_%1
    )
    IF EXIST action_%1.mmp (
	    call abld test build winscw udeb action_%1
	    call abld test build armv5 urel action_%1
    )
    IF EXIST operation_%1.mmp (
	    call abld test build winscw udeb operation_%1
	    call abld test build armv5 urel operation_%1
    )
    popd
)

rem Create hw sis
rem ----------------------------------------------------------------------------

IF EXIST contextsource_%1.mmp (
	IF EXIST contextsource_%1.sis (
	    call del /f contextsource_%1.sis
	)
	
	IF EXIST contextsource_%1.sisx (
	    call del /f contextsource_%1.sisx
	)
	
	call makesis contextsource_%1.pkg
	call signsis contextsource_%1.sis contextsource_%1.sisx rd.cer rd-key.pem
	call del contextsource_%1.sis
	call copy /y contextsource_%1.sisx \epoc32\data\z\data\cftestdata\testsis\contextsource_%1\contextsource.sisx
)

IF EXIST action_%1.mmp (
	IF EXIST action_%1.sis (
	    call del /f action_%1.sis
	)
	
	IF EXIST action_%1.sisx (
	    call del /f action_%1.sisx
	)
	
	call makesis action_%1.pkg
	call signsis action_%1.sis action_%1.sisx rd.cer rd-key.pem
	call del action_%1.sis
	call copy /y action_%1.sisx \epoc32\data\z\data\cftestdata\testsis\action_%1\action.sisx
)

IF EXIST operation_%1.mmp (
	IF EXIST operation_%1.sis (
	    call del /f operation_%1.sis
	)
	
	IF EXIST operation_%1.sisx (
	    call del /f operation_%1.sisx
	)
	
	call makesis operation_%1.pkg
	call signsis operation_%1.sis operation_%1.sisx rd.cer rd-key.pem
	call del operation_%1.sis
	call copy /y operation_%1.sisx \epoc32\data\z\data\cftestdata\testsis\action_%1\operation.sisx
)

rem Create wins sis
rem ----------------------------------------------------------------------------

IF EXIST contextsource_%1.mmp (
	IF EXIST contextsource_%1_wins.sis (
	    call del /f contextsource_%1_wins.sis
	)
	
	IF EXIST contextsource_%1_wins.sisx (
	    call del /f contextsource_%1_wins.sisx
	)
	
	call makesis contextsource_%1_wins.pkg
	call signsis contextsource_%1_wins.sis contextsource_%1_wins.sisx rd.cer rd-key.pem
	call del contextsource_%1_wins.sis
	call copy /y contextsource_%1_wins.sisx \epoc32\release\winscw\udeb\z\data\cftestdata\testsis\contextsource_%1\contextsource.sisx
)

IF EXIST action_%1.mmp (
	IF EXIST action_%1_wins.sis (
	    call del /f action_%1_wins.sis
	)
	
	IF EXIST action_%1_wins.sisx (
	    call del /f action_%1_wins.sisx
	)
	
	call makesis action_%1_wins.pkg
	call signsis action_%1_wins.sis action_%1_wins.sisx rd.cer rd-key.pem
	call del action_%1_wins.sis
	call copy /y action_%1_wins.sisx \epoc32\release\winscw\udeb\z\data\cftestdata\testsis\action_%1\action.sisx
)

IF EXIST operation_%1.mmp (
	IF EXIST operation_%1_wins.sis (
	    call del /f operation_%1_wins.sis
	)
	
	IF EXIST operation_%1_wins.sisx (
	    call del /f operation_%1_wins.sisx
	)
	
	call makesis operation_%1_wins.pkg
	call signsis operation_%1_wins.sis operation_%1_wins.sisx rd.cer rd-key.pem
	call del operation_%1_wins.sis
	call copy /y operation_%1_wins.sisx \epoc32\release\winscw\udeb\z\data\cftestdata\testsis\operation_%1\operation.sisx
)

goto End

rem Ending
rem ----------------------------------------------------------------------------

:End

pushd ..\group
call bldmake bldfiles -k
IF EXIST contextsource_%1.mmp (
	call abld test clean contextsource_%1
)
IF EXIST action_%1.mmp (
	call abld test clean action_%1
)
IF EXIST operation_%1.mmp (
	call abld test clean operation_%1
)
popd

IF EXIST \epoc32\release\winscw\udeb\z\sys\bin\contextsource_%1.dll (
	call del \epoc32\release\winscw\udeb\z\sys\bin\contextsource_%1.dll
)

IF EXIST \epoc32\release\winscw\udeb\z\sys\bin\action_%1.dll (
	call del \epoc32\release\winscw\udeb\z\sys\bin\action_%1.dll
)

IF EXIST \epoc32\release\winscw\udeb\z\sys\bin\operation_%1.dll (
	call del \epoc32\release\winscw\udeb\z\sys\bin\operation_%1.dll
)
