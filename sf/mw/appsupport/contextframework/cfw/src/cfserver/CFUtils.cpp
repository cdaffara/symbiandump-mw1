/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFUtils methods.
*
*/


#include "cfutils.h"
#include "cftrace.h"

//-----------------------------------------------------------------------------
// CFUtils::RequestFromSameProcess
//-----------------------------------------------------------------------------
//
TBool CFUtils::RequestFromSameProcess( const RThread& aClientThread )
    {
    FUNC_LOG;
    
    TBool sameProcess = EFalse;
    RThread cfMainThread;
    RProcess cfProcess;
    TInt err = cfMainThread.Process( cfProcess );
    if( err == KErrNone )
        {
        RProcess clientProcess;
        err = aClientThread.Process( clientProcess );
        if( err == KErrNone )
            {
            // Client runs in cf process -> secure
            if( cfProcess.Id() == clientProcess.Id() )
                {
                sameProcess = ETrue;
                }
            }
        clientProcess.Close();
        }
    cfMainThread.Close();
    cfProcess.Close();
    return sameProcess;
    }
