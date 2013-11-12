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
* Description:  CSensorChannelDoubleTap class implementation.
*
*/


#include <sensrvchannelinfo.h>
#include <sensrvsensorchannels.h>
#include <sensrvchanneldatatypes.h>
#include <sensrvchannel.h>
#include <cfcontextinterface.h>

#include "sensorchanneldoubletap.h"
#include "sensorsourcecontextdef.h"
#include "sensortrace.h"


// MEMBER FUNCTIONS

CSensorChannelDoubleTap* CSensorChannelDoubleTap::NewL(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CSensorChannelDoubleTap* self = CSensorChannelDoubleTap::NewLC(
        aLocation,
        aVendor,
        aCF );
    CleanupStack::Pop( self );

    return self;
    }

CSensorChannelDoubleTap* CSensorChannelDoubleTap::NewLC(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CSensorChannelDoubleTap* self =
        new( ELeave ) CSensorChannelDoubleTap( aCF );
    CleanupStack::PushL( self );
    self->ConstructL( aLocation, aVendor );

    return self;
    }

CSensorChannelDoubleTap::~CSensorChannelDoubleTap()
    {
    FUNC_LOG;
    }

CSensorChannelDoubleTap::CSensorChannelDoubleTap( MCFContextInterface& aCF ):
    CSensorChannelBase( aCF, CSensorChannelBase::EChannelDoubleTap )
    {
    FUNC_LOG;
    }

void CSensorChannelDoubleTap::ConstructL( const TDesC8& aLocation,
    const TDesC8& aVendor )
    {
    FUNC_LOG;

    // Configure channel
    if( !FindChannelL( KSensrvChannelTypeIdAccelerometerDoubleTappingData,
        aLocation,
        aVendor,
        iChannelInfo ) )
        {
        User::Leave( KErrNotFound );
        }

    // Finalize construction
    BaseConstructL( iChannelInfo );
    }

// METHODS

//-----------------------------------------------------------------------------
// CSensorChannelDoubleTap::DefineContextsL
//-----------------------------------------------------------------------------
//
void CSensorChannelDoubleTap::DefineContextsL()
    {
    FUNC_LOG;

    iCF.DefineContext( KSensorSource,
        KSensorSourceEventDoubleTap,
        KSensorSourceEventDoubleTapSec );
    }

//-----------------------------------------------------------------------------
// CSensorChannelDoubleTap::HandleDataReceivedL
//-----------------------------------------------------------------------------
//
void CSensorChannelDoubleTap::HandleDataReceivedL( CSensrvChannel& aChannel,
    TInt aCount,
    TInt /*aDataLost*/ )
    {
    FUNC_LOG;
    INFO_1( "Received double tapping data. Count: %d", aCount );

    // Ensure that we actually have data
    if( aCount )
        {
        TSensrvTappingData data;
        TPckg<TSensrvTappingData> dataBuf( data );
        if( aChannel.GetData( dataBuf ) == KErrNone )
            {
            // Define context
            iContext->SetSourceL( KSensorSource );
            iContext->SetTypeL( KSensorSourceEventDoubleTap );
            if( data.iDirection == (
                KSensrvAccelerometerDirectionXplus |
                KSensrvAccelerometerDirectionXminus ) )
                {
                // Tapping from X axel
                iContext->SetValueL( TPtrC(
                    KSensorSourceEventDoubleTapValues[EDoubleTapX] ) );
                }
            else if( data.iDirection == (
                KSensrvAccelerometerDirectionYplus |
                KSensrvAccelerometerDirectionYminus ) )
                {
                // Tapping from Y axel
                iContext->SetValueL( TPtrC(
                    KSensorSourceEventDoubleTapValues[EDoubleTapY] ) );
                }
            else
                {
                // Tapping from Z axel
                iContext->SetValueL( TPtrC(
                    KSensorSourceEventDoubleTapValues[EDoubleTapZ] ) );
                }

            INFO_1( "Double tap data from sensor. Direction: %x",
                data.iDirection );

            // Publish context - use change detection
            RThread thread;
            iCF.PublishContext( *iContext, thread );
            thread.Close();
            }
        }
    }
