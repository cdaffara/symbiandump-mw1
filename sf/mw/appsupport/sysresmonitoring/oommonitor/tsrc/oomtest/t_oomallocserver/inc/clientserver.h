/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/



#include <e32base.h>


// server name

_LIT(KAllocServerName,"t_oomAllocServer");
_LIT(KAllocServerImg,"t_oomAllocServer");
const TUid KServerUid3={0x10286A3E};

// A version must be specifyed when creating a session with the server

const TUint KCountServMajorVersionNumber=0;
const TUint KCountServMinorVersionNumber=1;
const TUint KCountServBuildVersionNumber=1;

IMPORT_C TInt StartThread(RThread& aServerThread);


// Function codes (opcodes) used in message passing between client and server
enum TAllocServRqst
    {
    EAllocServReset,
    EAllocServConfig,
    EAllocServStart,
    EAllocServStop,
    EAllocServMemoryLow,
    EAllocServMemoryGood
    };

enum TAllocServLeave
{
    ENonNumericString = 99
};
