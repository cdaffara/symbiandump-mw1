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
REM Run security tests on hardware
REM
REM This should run the same tests as test_spec_cedar, minus OOM and
REM cancellation testing.

REM Attempt to stop the execution in case it's accidentally started on a PC
ECHO About to run Security Team Hardware Tests
pause

REM create testresults directory
md c:\testresults
md e:\testresults
copy z:\buildid.txt e:\
copy z:\buildid.txt e:\testresults

REM copy hosts file to redirect cam-ocspserver request to ban-ocspserver
md c:\private
md c:\private\10000882
copy z:\hosts c:\private\10000882\hosts

REM install automated commdb
copy z:\auto.cfg c:\auto.cfg
ced -i c:\auto.cfg

sntpclient -savings cbtime01.symbian.intra bantime01.symbian.intra

runusiftests.bat

runcaftests.bat

runswiromtests.bat

REM Enable the following test when DEF076875(or relevant CR) is done.
REM runtlsclientauth.bat

runupstests.bat


REM Temporary solution for ROM overflow issues (DEF109635)
runpkcs12tests.bat

runcommonutilstests.bat
