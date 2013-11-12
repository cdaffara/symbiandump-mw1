/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    Including console menu, constants, and log file names.
**
*/




#ifndef __RTPAPICONSOLE_DEF_H
#define __RTPAPICONSOLE_DEF_H


// Log File Names
#define KLogFileTest _L("TestLog1.txt")	
#define KLogFileRtcp _L("TestLog2.txt")	
#define KLogFileRtp  _L("TestLog3.txt")	
#define KLogFileStat _L("TestLog4.txt")

// Default Log File Path
#define KDefaultPath _L("c:\\system\\data\\")

const TInt KMaxLogLineLength =	512;

_LIT(KTxtLineBreak, "\n");
_LIT(KTxtRunTestTxt1, "Enter a 40 digit code: ");
_LIT(KTxtRunTestTxt2, "You entered: ");

#endif __RTPAPICONSOLE_DEF_H
