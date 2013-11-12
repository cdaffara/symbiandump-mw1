// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "TzCachingTestStep.h"
#include <tzconverter.h>
#include "DstStep.h"
#include "DstUnitStep.h"
#include "DstUnitServer.h"

CTzCachingTestStep::~CTzCachingTestStep()
	{
	}

CTzCachingTestStep::CTzCachingTestStep( CDstUnitServer &aServer ) :
    CDstUnitStep  (aServer)
	{
	SetTestStepName( KTimezoneCacheTestName );
	}

/**
* Tests the new design from 9.3 where caching is done in RTz. This function
* does little verification other than exercising the API.
*/
TInt CTzCachingTestStep::CreateAndRunTestUnitL()
    {
    // Connect and do some conversions using non-cached method.
    User::LeaveIfError( iTzServer.Connect() );

    TTime cachedLocalTime;
    User::LeaveIfError( iTzServer.ConvertToLocalTime( cachedLocalTime ) );
    
    TTime cachedUniTime;
    User::LeaveIfError( iTzServer.ConvertToUniversalTime( cachedUniTime ) );
    
    // Get the time zone.
    CTzId* tzIdCached = iTzServer.GetTimeZoneIdL();
    CleanupStack::PushL( tzIdCached );
    TUint tzId1 = tzIdCached->TimeZoneNumericID();

    // Close connection to server and re-connect.
    iTzServer.Close();
    User::LeaveIfError( iTzServer.Connect() );
    
    TTime cachedLocalTime2;
    User::LeaveIfError( iTzServer.ConvertToLocalTime( cachedLocalTime2 ) );

    // Close connection to server. 
    iTzServer.Close();

    // Re-connect.
    User::LeaveIfError( iTzServer.Connect() );
    
    // Now do another conversion.
    TTime cachedUniTime2;
    User::LeaveIfError( iTzServer.ConvertToUniversalTime( cachedUniTime2 ) );

    // Set time zone.
    iTzServer.SetTimeZoneL( *tzIdCached );
    CleanupStack::PopAndDestroy( tzIdCached );

    // Get the time zone again.
    CTzId* tzIdCached2 = iTzServer.GetTimeZoneIdL();
    TUint tzId2 = tzIdCached2->TimeZoneNumericID();
    delete tzIdCached2;
    
    if (tzId1 != tzId2)
        {
        return KErrGeneral;
        }
    
    iTzServer.Close();

    // ---- Test converter ----

    // Create converter with unconnected RTz. This should leave.
    TRAPD( err, CTzConverter* conv = CTzConverter::NewL( iTzServer ));
    if (err != KErrNotReady)
        {
        return KErrGeneral;
        }

    // Create converter with connected RTz. Should be fine.
    User::LeaveIfError( iTzServer.Connect() );
    CTzConverter* conv = NULL;
    TRAP( err, conv = CTzConverter::NewL( iTzServer ));
    if (err != KErrNone)
        {
        return KErrGeneral;
        }
    CleanupStack::PushL( conv );
    TUint16 tz = conv->CurrentTzId();
    
    // Close session and re-open. Caching rules should be cleared and
    // then caching should be enabled again once re-connected.
    iTzServer.Close();
    User::LeaveIfError( iTzServer.Connect() );
    
    // Get the time again using cached method.
    TTime cachedLocalTime3;
    cachedLocalTime3.UniversalTime();
    User::LeaveIfError( conv->ConvertToLocalTime( cachedLocalTime3 ) );

    TTime cachedUniTime3;
    cachedUniTime3.HomeTime();
    User::LeaveIfError( conv->ConvertToUniversalTime( cachedUniTime3 ) );

    // The timezone should be the same as before.    
    TUint16 tzidConv = conv->CurrentTzId();
    if (tzidConv != tzId2)
        {
        return KErrGeneral;
        }
        
    // Close the session and try doing a conversion, it should leave with
    // KErrNotReady because the session is closed. Even though caching is
    // done without making requests to the server, a connection is still 
    // required for tz change notifications so that the client can ensure the 
    // cache is valid.
    iTzServer.Close();
    TTime cachedUniTime4;
    err = conv->ConvertToUniversalTime( cachedUniTime4 );
    if (err != KErrNotReady)
        {
        return KErrGeneral;
        }
    
    // Reconnect to the server and do a conversion. Caching is currently
    // disabled but should be enabled when the conversion call is made, it 
    // should be successful.
    User::LeaveIfError( iTzServer.Connect() );
    TTime cachedUniTime5;
    cachedUniTime5.HomeTime();
    err = conv->ConvertToUniversalTime( cachedUniTime5 );
    if (err != KErrNone)
        {
        return KErrGeneral;
        }

    CleanupStack::PopAndDestroy( conv );
    
    return KErrNone;
    }

void CTzCachingTestStep::DestroyTestUnit()
    {
    }
