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
REM testcaseB006a
call /epoc32/winscw/c/tswi/tinterpretsisinteg/makesisB006.bat
call interpretsis -z .\romdrive -c .\cdrive -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ai1.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\bi1.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ci1.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\di1.sis -w info -l  /epoc32/winscw/c/interpretsis_test_harness.txt
ECHO ***ERRORCODE*** %errorlevel% >>/epoc32/winscw/c/interpretsis_test_harness.txt
call interpretsis -z .\romdrive -c .\cdrive -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ai2.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\bi2.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ci2.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\di2.sis -w info -l /epoc32/winscw/c/interpretsis_test_harness.txt
ECHO ***ERRORCODE*** %errorlevel% >>/epoc32/winscw/c/interpretsis_test_harness.txt
call interpretsis -z .\romdrive -c .\cdrive -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ai3.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\bi3.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ci3.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\di3.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\ei3.sis -s \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\fi3.sis -w info -l  /epoc32/winscw/c/interpretsis_test_harness.txt
ECHO ***ERRORCODE*** %errorlevel% >>/epoc32/winscw/c/interpretsis_test_harness.txt
:END