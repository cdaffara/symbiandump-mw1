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
REM
REM This batch file is to allow the TZ Localization integration tests to be run
REM on hardware without having to use TestDriver.  The test script and data
REM files are included in the ROM by the TzLocalTestServer.iby file.
REM

COPY z:\tzlocintegtesthw.script c:\tzlocintegtesthw.script
COPY z:\tzlocaltestdata.ini c:\tzlocaltestdata.ini

testexecute.exe c:\tzlocintegtesthw.script
