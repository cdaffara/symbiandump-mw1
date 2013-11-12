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
* Description:  The application gets instantiated starting from this class.
*
*/



// INCLUDE FILES
#include    <e32svr.h>
#include    <e32base.h>
#include    <e32std.h>
#include    "CProfileChangeEvent.h"
#include    "ProfilesDebug.h"

// CONSTANTS
_LIT( KProfileScheduleEvent, "ProfileScheduleEvent" );

// ============================= LOCAL FUNCTIONS ===============================

LOCAL_D TInt Execute( TFileName& /* aFileName */ )
    {
    __UHEAP_MARK;
    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        User::Panic( KProfileScheduleEvent, KErrNoMemory );
        }

    CActiveScheduler* scheduler = new CActiveScheduler; // CSI: 62 # scheduler tested for NULL below
    if (!scheduler)
        {
        PRODEBUG( "ERROR: couldn't create active scheduler!" );
        User::Panic( KProfileScheduleEvent, KErrNoMemory );
        }
    CActiveScheduler::Install(scheduler);
    
	PRODEBUG( " ProfileSchedEvent:Execute before ExecuteL " );
    TRAPD( err, CProfileChangeEvent::ExecuteL( ) );
	PRODEBUG( " ProfileSchedEvent:Execute after ExecuteL " );

    CActiveScheduler::Install( NULL );
    delete scheduler;

    delete tc;
    __UHEAP_MARKEND;
    return err;
    }

GLDEF_C TInt E32Main()
    {
	PRODEBUG( "ProfileSchedEvent:E32Main()" );
    TBuf< 0x100 > cmd;
    User::CommandLine( cmd );
    return Execute( cmd );
    }

//  End of File

