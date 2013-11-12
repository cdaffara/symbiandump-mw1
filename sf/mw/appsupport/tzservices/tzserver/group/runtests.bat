@echo off

rem Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

call abld reallyclean
call bldmake bldfiles
call abld freeze %1
call abld build %1

cd ..\test\unit\group
call abld test reallyclean
call bldmake bldfiles
call abld test build %1

cd ..\..\integration\group
call abld test reallyclean
call bldmake bldfiles
call abld test build %1

if %1 == wins call \epoc32\release\wins\udeb\TESTEXECUTE.EXE c:\dstscripts\unittest.script
if %1 == wins call \epoc32\release\wins\udeb\TESTEXECUTE.EXE c:\dstscripts\conversiontest.script
if %1 == wins call \epoc32\release\wins\udeb\TESTEXECUTE.EXE c:\dstscripts\vcaltest.script

if %1 == winscw call \epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\dstscripts\unittest.script
if %1 == winscw call \epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\dstscripts\conversiontest.script
if %1 == winscw call \epoc32\release\winscw\udeb\TESTEXECUTE.EXE c:\dstscripts\vcaltest.script

cd ..\..\..\group
