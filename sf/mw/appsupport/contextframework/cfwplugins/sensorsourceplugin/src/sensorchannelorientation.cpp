/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorChannelOrientation class implementation.
*
*/


#include <sensrvchannelinfo.h>
#include <sensrvsensorchannels.h>
#include <sensrvchanneldatatypes.h>
#include <sensrvchannel.h>
#include <cfcontextinterface.h>

#include "sensorchannelorientation.h"
#include "sensorsourcecontextdef.h"
#include "sensortrace.h"

// MEMBER FUNCTIONS

CSensorChannelOrientation* CSensorChannelOrientation::NewL(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CSensorChannelOrientation* self = CSensorChannelOrientation::NewLC(
        aLocation,
        aVendor,
        aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

CSensorChannelOrientation* CSensorChannelOrientation::NewLC(
    const TDesC8& aLocation,
    const TDesC8& aVendor,
    MCFContextInterface& aCF )
    {
    FUNC_LOG;
    
    CSensorChannelOrientation* self =
        new( ELeave ) CSensorChannelOrientation( aCF );
    CleanupStack::PushL( self );
    self->ConstructL( aLocation, aVendor );
    
    return self;
    }

CSensorChannelOrientation::~CSensorChannelOrientation()
    {
    FUNC_LOG;
    }

CSensorChannelOrientation::CSensorChannelOrientation( MCFContextInterface& aCF ):
    CSensorChannelBase( aCF, CSensorChannelBase::EChannelOrientation )
    {
    FUNC_LOG;
    }
    
void CSensorChannelOrientation::ConstructL( const TDesC8& aLocation,
    const TDesC8& aVendor )
    {
    FUNC_LOG;
    
    // Configure channel
    if( !FindChannelL( KSensrvChannelTypeIdOrientationData,
        aLocation,
        aVendor,
        iChannelInfo ) )
        {
        User::Leave( KErrNotFound );
        }

    // Finalize construction
    BaseConstructL( iChannelInfo );
    
    // Get initial value
    FetchInitialValueL();
    }
    
// METHODS

//-----------------------------------------------------------------------------
// CSensorChannelOrientation::DefineContextsL
//-----------------------------------------------------------------------------
//
void CSensorChannelOrientation::DefineContextsL()
    {
    FUNC_LOG;
    
    iCF.DefineContext( KSensorSource,
        KSensorSourceEventOrientation,
        KSensorSourceEventOrientationSec );
    
    iCF.DefineContext( KSensorSource,
        KSensorSourceEventOrientationPrevious,
        KSensorSourceEventOrientationSec );
    
    // Initialize context object
    iContext->SetSourceL( KSensorSource );
    iContext->SetTypeL( KSensorSourceEventOrientation );
    }

//-----------------------------------------------------------------------------
// CSensorChannelOrientation::HandleDataReceivedL
//-----------------------------------------------------------------------------
//
void CSensorChannelOrientation::HandleDataReceivedL( CSensrvChannel& aChannel, 
    TInt aCount, 
    TInt /*aDataLost*/ )
    {
    FUNC_LOG;
    
    // Ensure that we actually have data
    if( aCount )
        {
        TSensrvOrientationData data;
        TPckg<TSensrvOrientationData> dataPckg( data );
        if( aChannel.GetData( dataPckg ) == KErrNone )
            {
            // Store previous orientation
            iPreviousOrientation = iCurrentOrientation;
            switch( data.iDeviceOrientation )
                {
                // Display up
                case TSensrvOrientationData::EOrientationDisplayUp:
                    {
                    iCurrentOrientation = EDisplayUp;
                    break;
                    }

                // Display down
                case TSensrvOrientationData::EOrientationDisplayDown:
                    {
                    iCurrentOrientation = EDisplayDown;
                    break;
                    }

                // Display left up
                case TSensrvOrientationData::EOrientationDisplayLeftUp:
                    {
                    iCurrentOrientation = EDisplayLeftUp;
                    break;
                    }

                // Display right up
                case TSensrvOrientationData::EOrientationDisplayRightUp:
                    {
                    iCurrentOrientation = EDisplayRightUp;
                    break;
                    }

                // Display upwards
                case TSensrvOrientationData::EOrientationDisplayUpwards:
                    {
                    iCurrentOrientation = EDisplayUpwards;
                    break;
                    }

                // Display downwards
                case TSensrvOrientationData::EOrientationDisplayDownwards:
                    {
                    iCurrentOrientation = EDisplayDownwards;
                    break;
                    }

                // Default
                default:
                    {
                    iCurrentOrientation = EUndefined;
                    break;
                    }
                }
            
            // Publish the current orientation context
            INFO_1( "Received orientation data: Current orientation: [%d]",
                data.iDeviceOrientation );
            PublishOrientationL();
            }
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelOrientation::StopListening
//------------------------------------------------------------------------------
//
void CSensorChannelOrientation::StopListening()
    {
    FUNC_LOG;

    // Store previous state
    TSensorChannelState state = iState;
    
    // Call base class
    CSensorChannelBase::StopListening();
    
    // Check if the channel was closed, update orientation context
    if ( iState == CSensorChannelBase::EChannelStateIdle )
        {
        // Double check that state really has changed
        if ( state != iState )
            {
            iPreviousOrientation = iCurrentOrientation;
            iCurrentOrientation = EUndefined;
        
            // Update orientation context
            PublishOrientation();
            }
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelOrientation::PublishOrientation
//------------------------------------------------------------------------------
//
void CSensorChannelOrientation::PublishOrientation()
    {
    FUNC_LOG;
    
    TRAP_IGNORE( PublishOrientationL() );
    }

//------------------------------------------------------------------------------
// CSensorChannelOrientation::PublishOrientationL
//------------------------------------------------------------------------------
//
void CSensorChannelOrientation::PublishOrientationL()
    {
    FUNC_LOG;
    
    RThread thread;

    // Previous orientation
    iContext->SetTypeL( KSensorSourceEventOrientationPrevious );
    iContext->SetValueL( DESC(
        KSensorSourceEventOrientationValues, iPreviousOrientation ) );
    iCF.PublishContext( *iContext, thread );

    // Current orientation
    iContext->SetSourceL( KSensorSource );
    iContext->SetTypeL( KSensorSourceEventOrientation );
    iContext->SetValueL( DESC(
        KSensorSourceEventOrientationValues, iCurrentOrientation ) );
    iCF.PublishContext( *iContext, thread );

    thread.Close();
    }

// End of file

