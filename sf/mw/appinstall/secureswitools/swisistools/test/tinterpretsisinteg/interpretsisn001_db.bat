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
REM testcaseN001
call interpretsis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\interpretsisn001.sis -t .\romdrive\system\install\ -c .\cdrive -r \epoc32\winscw\c\tswi\tinterpretsisinteg\testdatan001\rombld.log -w info -l /epoc32/winscw/c/interpretsis_test_harness_db.txt
IF NOT %errorlevel%==0 GOTO LAST
ECHO ***ERRORCODE*** %errorlevel% PASS>>/epoc32/winscw/c/interpretsis_test_harness_db.txt
GOTO END
:LAST
ECHO ***ERRORCODE*** %errorlevel% FAIL>>/epoc32/winscw/c/interpretsis_test_harness_db.txt
:END

