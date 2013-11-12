/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:file for generating XImLogger
*
*/



// INCLUDE FILES
#include "imlogutils.h"

// CONSTANTS
const TInt KLogBufferLength = 256;
_LIT(KImLogDir, "ximpim");
_LIT(KImLogFile, "im.txt");

void XImLogger::Log(TRefByValue<const TDesC> aFmt,...)
    {
#ifdef _DEBUG
    VA_LIST list;
    VA_START(list, aFmt);
    // Print to log file
    TBuf<KLogBufferLength> buf;
    buf.FormatList(aFmt, list);
    RFileLogger::Write(KImLogDir, KImLogFile, EFileLoggingModeAppend, buf);
#endif
    }

//  End of File  
