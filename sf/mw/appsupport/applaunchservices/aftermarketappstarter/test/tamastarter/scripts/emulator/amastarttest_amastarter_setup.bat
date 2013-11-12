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
set TEST_BOOTMODE=%2
set TEST_RSC=%3

set SSMA_RSC_LOCATION=Z\private\2000d75b\startup\0
set SSMA_RSC_NAME=0
set SSMA_RSC_LOCATION_ORIGINAL=Z\private\2000d75b\startup\0.original
set SSMA_RSC_NAME_ORIGINAL=0.original
set SSMA_RSC_FILE=Z\private\2000d75b\startup\0\gsastartupcmdlist0.rsc
set SSMA_RSC_FILE_ORIGINAL=Z\private\2000d75b\startup\test\gsastartupcmdlist0.rsc.original

set SSMA_RSC_TEST_FILE=Z\private\2000d75b\startup\test\%TEST_RSC%


set FALLBACK_RSC_LOCATION=Z\private\10205C44
set FALLBACK_RSC_LOCATION_ORIGINAL=Z\private\10205C44.original
set FALLBACK_RSC_FILE=Z\private\10205C44\SSCForStartupMode0.rsc
set FALLBACK_RSC_FILE_ORIGINAL=Z\private\10205C44\SSCForStartupMode0.rsc.original

set FALLBACK_RSC_TEST_FILE=Z\private\10205C44\test\%TEST_RSC%



set CHOICE=%COMMAND_CHOICE%%TEST_BOOTMODE%
goto :%CHOICE%

:installfallback
	if exist "%SSMA_RSC_LOCATION%" ren %SSMA_RSC_LOCATION% %SSMA_RSC_NAME_ORIGINAL%
	if not exist "%FALLBACK_RSC_FILE_ORIGINAL%" copy %FALLBACK_RSC_FILE% %FALLBACK_RSC_FILE_ORIGINAL% > NUL
	if not exist "%FALLBACK_RSC_TEST_FILE%" echo AmaStart AmaStarter Using SysStart Test: ERROR - test resource file does not exist & goto :EOF
	copy %FALLBACK_RSC_TEST_FILE% %FALLBACK_RSC_FILE% > NUL

goto :EOF

:uninstallfallback
	if exist %FALLBACK_RSC_FILE_ORIGINAL% copy %FALLBACK_RSC_FILE_ORIGINAL% %FALLBACK_RSC_FILE% > NUL
	if exist %FALLBACK_RSC_FILE_ORIGINAL% del %FALLBACK_RSC_FILE_ORIGINAL%
	if exist %SSMA_RSC_LOCATION_ORIGINAL% ren %SSMA_RSC_LOCATION_ORIGINAL% %SSMA_RSC_NAME% > NUL
goto :EOF



:installssma
	if not exist %SSMA_RSC_FILE_ORIGINAL% copy %SSMA_RSC_FILE% %SSMA_RSC_FILE_ORIGINAL%
	if not exist "%SSMA_RSC_TEST_FILE%" echo AmaStart AmaStarter Using SSMA Test: ERROR - test resource file does not exist & goto :EOF
	copy %SSMA_RSC_TEST_FILE% %SSMA_RSC_FILE% > NUL

goto :EOF

:uninstallssma
	if exist %SSMA_RSC_FILE_ORIGINAL% copy %SSMA_RSC_FILE_ORIGINAL% %SSMA_RSC_FILE% > NUL
	if exist %SSMA_RSC_FILE_ORIGINAL% del %SSMA_RSC_FILE_ORIGINAL%
goto :EOF


endlocal
