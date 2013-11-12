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


@echo Cleaning advanced tsp test stub...
pushd ..\..\stub\group
call bldmake bldfiles
call abld test reallyclean
popd
@echo Cleaning advanced tsp test stub... Finished

@echo Restoring remcon...
pushd \sf\os\devicesrv\accessoryservices\remotecontrolfw\server\group
call bldmake bldfiles
call abld cleanexport
call abld export
popd
pushd \sf\os\devicesrv\accessoryservices\remotecontrolfw\targetselectorplugin\group
call bldmake bldfiles
call abld reallyclean
call abld build
popd
@echo Restoring remcon... Finished

@echo Restoring advanced tsp
pushd ..\..\..\group
call bldmake bldfiles
call abld reallyclean
call abld build
popd
@echo Restoring advanced tsp... Finished
