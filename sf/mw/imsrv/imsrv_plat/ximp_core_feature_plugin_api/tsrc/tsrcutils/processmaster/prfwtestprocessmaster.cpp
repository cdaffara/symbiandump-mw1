/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "prfwtestprocessmaster.h"


// ==============================================================
// ============ DoKillProcessL()                    =============
// ==============================================================
void DoKillProcessL( const TDesC& aProcessName )
    {
    TFullName psName;
    psName.Append( _L("*") );
    psName.Append( aProcessName );
    psName.Append( _L("*") );

    TFindProcess psFinder;
    psFinder.Find( psName );

    TInt killCount( 0 );
    while( psFinder.Next( psName ) != KErrNotFound )
        {
        RProcess ps;
        User::LeaveIfError( ps.Open( psFinder ) );
        ps.Kill( -666 );
        ps.Close();
        killCount++;
        }

    User::Leave( killCount );
    }


// ==============================================================
// ============ E32MainL()                          =============
// ==============================================================
void E32MainL()
    {
    TInt theCommand;
    User::LeaveIfError( User::GetTIntParameter( PrfwTestProcessMaster::EMsgSlot_Command, theCommand ) );

    TBuf< PrfwTestProcessMaster::EMaxMsgDataLength > theDesC16;
    User::LeaveIfError( User::GetDesParameter( PrfwTestProcessMaster::EMsgSlot_Desc16Data, theDesC16 ) );

    switch( theCommand )
        {
        case PrfwTestProcessMaster::EKillProcess:
            {
            DoKillProcessL( theDesC16 );
            break;
            }

        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    }


// ==============================================================
// ============ E32Main()                           =============
// ==============================================================
GLDEF_C TInt E32Main()
    {
    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        return KErrNoMemory;
        }

    __UHEAP_MARK;
    TRAPD( err, E32MainL() );
    __UHEAP_MARKEND;

    delete tc;

    RProcess::Rendezvous( err );
    return err;
    }


//End of file
