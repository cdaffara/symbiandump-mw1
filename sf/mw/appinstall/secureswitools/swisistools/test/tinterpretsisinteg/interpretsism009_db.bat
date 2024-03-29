@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of the License "Eclipse Public License v1.0"
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

:: This batch file creates sis file/files
@ECHO OFF
REM testcaseM009
REM -r option used with -z
call interpretsis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\interpretsism003.sis -t .\romdrive\system\install\ -z .\romdrive -c .\cdrive -r \epoc32\winscw\c\tswi\tinterpretsisinteg\testdatam003\rombld.log,\epoc32\winscw\c\tswi\tinterpretsisinteg\testdatam003\rombld.rofs.log -w info -l /epoc32/winscw/c/interpretsis_test_harness_db.txt > NUL
IF NOT %errorlevel%==-7 GOTO LAST
ECHO ***ERRORCODE*** %errorlevel% PASS>>/epoc32/winscw/c/interpretsis_test_harness_db.txt
GOTO END
:LAST
ECHO ***ERRORCODE*** %errorlevel% FAIL>>/epoc32/winscw/c/interpretsis_test_harness_db.txt
:END

