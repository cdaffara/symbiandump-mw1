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

testexecute.exe z:\viewsrvtest\viewsrvtest_Capability1.script
testexecute.exe z:\viewsrvtest\viewsrvtest_Capability2.script
testexecute.exe z:\viewsrvtest\viewsrvtest_tvwa.script

:: *******************************************************************
:: Any ADDITIONS or DELETIONS to the test cases in this script MUST
:: be copied to the partner script that is used on emulator:
:: ..\emulator\viewsrvtest_run.bat
:: *******************************************************************

:: Logs are copied to MMC which is assumed to be drive E:
md e:\logs
md e:\logs\testexecute
copy c:\logs\testexecute\viewsrvtest*.htm e:\logs\testexecute\
