/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorChannelProximity class implementation.
*
*/


#include <sensrvchannelinfo.h>
#include <sensrvsensorchannels.h>
#include <sensrvproximitysensor.h>
#include <sensrvchanneldatatypes.h>
#include <sensrvchannel.h>
#include <cfcontextinterface.h>

#include "sensorchannelproximity.h"
#include "sensorsourcecontextdef.h"
#include "sensortrace.h"

// MEMBER FUNCTIONS

CSensorChannelProximity* CSensorChannelProximity::NewL(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CSensorChannelProximity* self = CSensorChannelProximity::NewLC(
        aLocation,
        aVendor,
        aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

CSensorChannelProximity* CSensorChannelProximity::NewLC(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CSensorChannelProximity* self =
        new( ELeave ) CSensorChannelProximity( aCF );
    CleanupStack::PushL( self );
    self->ConstructL( aLocation, aVendor );
    
    return self;
    }

CSensorChannelProximity::~CSensorChannelProximity()
    {
    FUNC_LOG;
    }

CSensorChannelProximity::CSensorChannelProximity( MCFContextInterface& aCF ):
    CSensorChannelBase( aCF, CSensorChannelBase::EChannelProximity )
    {
    FUNC_LOG;
    }
    
void CSensorChannelProximity::ConstructL( const TDesC8& aLocation,
    const TDesC8& aVendor )
    {
    FUNC_LOG;
    
    // Configure channel
    if( !FindChannelL( KSensrvChannelTypeIdProximityMonitor,
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
// CSensorChannelProximity::DefineContextsL
//-----------------------------------------------------------------------------
//
void CSensorChannelProximity::DefineContextsL()
    {
    FUNC_LOG;
    
    iCF.DefineContext( KSensorSource,
        KSensorSourceEventProximity,
        KDeviceActivitySec );
    }

//-----------------------------------------------------------------------------
// CSensorChannelProximity::HandleDataReceivedL
//-----------------------------------------------------------------------------
//
void CSensorChannelProximity::HandleDataReceivedL( CSensrvChannel& aChannel, 
    TInt aCount, 
    TInt /*aDataLost*/ )
    {
    FUNC_LOG;
    
    // Ensure that we actually have data
    if( aCount )
        {
        
        TSensrvProximityData data;
        TPckg<TSensrvProximityData> dataPckg( data );
        TInt getErr = aChannel.GetData( dataPckg );
        if( getErr == KErrNone )
            {
            
            iContext->SetSourceL( KSensorSource );
            iContext->SetTypeL( KSensorSourceEventProximity );
            switch( data.iProximityState )
                {
                // Proximity discernible
                case TSensrvProximityData::EProximityDiscernible:
                    {
                    iContext->SetValueL( TPtrC(
                        KSensorSourceEventProximityValues[EDiscernible] ) );
                    break;
                    }

                // Proximity indiscernible
                case TSensrvProximityData::EProximityIndiscernible:
                    {
                    iContext->SetValueL( TPtrC(
                        KSensorSourceEventProximityValues[EIndiscernible] ) );
                    break;
                    }
 
                // Proximity data is not reliable, e.g.
                // case TSensrvProximityData::EProximityUndefined:
                // Default
                default:
                    {
                    iContext->SetValueL( KNullDesC );
                    break;
                    }
                }

            // Check if we really need to publish context
            if( iContext->Value().Length() )
                {
                RThread thread;
                iCF.PublishContext( *iContext, thread );
                thread.Close();
                }
            }
        }
    }
