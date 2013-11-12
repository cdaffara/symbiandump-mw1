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
* Description:  CSensorChannelBase class implementation.
*
*/


#include <sensrvchannel.h>
#include <sensrvchannelfinder.h>

#include "sensorchannelbase.h"
#include "sensortrace.h"
#include "sensorsourcecontextdef.h"
#include "sensorsourceclient.h"
#include "sensorsourcecommand.h"

// CONSTANTS

#ifdef _DEBUG

// Converts channel id in to descriptor format
_LIT( KChannelNameDoubleTap, "DoubleTap" );
_LIT( KChannelNameOrientation, "Orientation" );

LOCAL_C const TDesC& ChannelName( CSensorChannelBase::TSensorChannelId aId )
    {
    switch( aId )
        {
        case CSensorChannelBase::EChannelDoubleTap:
            {
            return KChannelNameDoubleTap;
            }
        case CSensorChannelBase::EChannelOrientation:
            {
            return KChannelNameOrientation;
            }
        default:
            {
            return KNullDesC;
            }
        }
    }

// Converts state id in to descriptor format
_LIT( KChannelStateIdle, "Idle" );
_LIT( KChannelStateInit, "Initializing" );
_LIT( KChannelStateActive, "Active" );

LOCAL_C const TDesC& ChannelStateName(
    CSensorChannelBase::TSensorChannelState aState )
    {
    switch( aState )
        {
        case CSensorChannelBase::EChannelStateIdle:
            {
            return KChannelStateIdle;
            }
        case CSensorChannelBase::EChannelStateInitializing:
            {
            return KChannelStateInit;
            }
        case CSensorChannelBase::EChannelStateActive:
            {
            return KChannelStateActive;
            }
        default:
            {
            return KNullDesC;
            }
        }
    }

#endif

// Default sample rates
const TInt KMinSampleRate = 1;
const TInt KMaxSampleRate = 1;
const TInt KBufferRate = 0;

// Destructor
CSensorChannelBase::~CSensorChannelBase()
    {
    FUNC_LOG;

    iClients.ResetAndDestroy();
    delete iContext;
    if( iSensorChannel )
        {
        iSensorChannel->CloseChannel();
        delete iSensorChannel;
        }
    }

// Constructor
CSensorChannelBase::CSensorChannelBase(
    MCFContextInterface& aCF,
    TSensorChannelId aSensorChannel ):
    iChannelId( aSensorChannel ),
    iCF( aCF )
    {
    FUNC_LOG;
    }

// 2nd phase constructor
void CSensorChannelBase::BaseConstructL( const TSensrvChannelInfo& aChannelInfo )
    {
    FUNC_LOG;

    iSensorChannel = CSensrvChannel::NewL( aChannelInfo );
    iContext = CCFContextObject::NewL();

    INFO_1( "Created sensor channel: [%S]", &ChannelName( iChannelId ) );

    ChangeState( EChannelStateIdle );
    }

// METHODS

//-----------------------------------------------------------------------------
// CSensorChannelBase::StartListeningL
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::StartListeningL()
    {
    FUNC_LOG;

    // If we do not have active connections and the channel is currently in
    // idle mode
    if( iState == EChannelStateIdle )
        {
        DoStartL( EChannelStateActive );
        }
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::StopListening
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::StopListening()
    {
    FUNC_LOG;

    // Check if the channel needs to be closed
    if( iState == EChannelStateActive )
        {
        DoStop();
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::FetchInitialValueL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::FetchInitialValueL()
    {
    FUNC_LOG;

    DoStartL( EChannelStateInitializing );
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::HandleSensorCommandL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::HandleSensorCommandL(
    const CSensorSourceCommand& aCommand )
    {
    FUNC_LOG;

    TPtrC commandId( aCommand.CommandId() );

    // activate
    if( commandId.CompareF( KSensorCommandIdActivateName )
        == KErrNone )
        {
        HandleActivateCommandL( aCommand );
        INFO_1( "Handled sensor source command: [%S]", &(const TDesC&)aCommand.CommandId() );
        }
    // deactivate
    else if( commandId.CompareF( KSensorCommandIdDeactivateName )
        == KErrNone )
        {
        HandleDeactivateCommandL( aCommand );
        INFO_1( "Handled sensor source command: [%S]", &(const TDesC&)aCommand.CommandId() );
        }
    // unknown command
    else
        {
        ERROR_1( KErrNotSupported, "Unknown sensor source command: [%S]",
            &commandId );
        }
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::SensorChannelId
//-----------------------------------------------------------------------------
//
CSensorChannelBase::TSensorChannelId CSensorChannelBase::SensorChannelId() const
    {
    FUNC_LOG;

    return iChannelId;
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::SensorActive
//------------------------------------------------------------------------------
//
TBool CSensorChannelBase::SensorActive() const
    {
    FUNC_LOG;

    INFO_1( "SensorActive(): active clients [%d]", iClients.Count() );

    return ( iClients.Count() > 0 );
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::DataReceived
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::DataReceived( CSensrvChannel& aChannel,
    TInt aCount,
    TInt aDataLost )
    {
    FUNC_LOG;

    TRAP_IGNORE( HandleDataReceivedL( aChannel, aCount, aDataLost ) );

    // If we are fetching initial value, make sure that the channel is closed
    // properly if there are no active connections
    if( !SensorActive() && iState == EChannelStateInitializing )
        {
        // Stop data listening and close up channel
        DoStop();
        }
    else
        {
        // Active connections received, change state
        ChangeState( EChannelStateActive );
        }
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::DataError
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::DataError( CSensrvChannel& /*aChannel*/,
    TSensrvErrorSeverity aError )
    {
    FUNC_LOG;

    ChangeState( EChannelStateIdle );
    if( aError == ESensrvErrorSeverityFatal )
        {
        DoReopenChannel();
        }
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::GetDataListenerInterfaceL
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::GetDataListenerInterfaceL( TUid /*aInterfaceUid*/,
        TAny*& aInterface )
    {
    aInterface = NULL;
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::FindChannelL
//-----------------------------------------------------------------------------
//
TBool CSensorChannelBase::FindChannelL(
    const TSensrvChannelTypeId& aChannelTypeId,
    const TDesC8& aLocation,
    const TDesC8& aVendorId,
    TSensrvChannelInfo& aChannelInfo )
    {
    FUNC_LOG;

    TBool found = EFalse;

    // Configure channel
    CSensrvChannelFinder* channelFinder = CSensrvChannelFinder::NewLC();
    RSensrvChannelInfoList channelInfoList;
    CleanupClosePushL( channelInfoList );

    // Find double tap channel
    aChannelInfo.iChannelId = 0;
    aChannelInfo.iContextType = ESensrvContextTypeNotDefined;
    aChannelInfo.iQuantity = ESensrvQuantityNotdefined;
    aChannelInfo.iChannelType = aChannelTypeId;
    aChannelInfo.iLocation = aLocation;
    aChannelInfo.iVendorId = aVendorId;
    aChannelInfo.iDataItemSize = 0;
    channelFinder->FindChannelsL( channelInfoList, aChannelInfo );
    if( channelInfoList.Count() )
        {
        // Take first match and assume it to be best one
        aChannelInfo = channelInfoList[0];
        found = ETrue;
        INFO_3( "Found sensor channel with parameters: ChannelTypeId: [%x], Location: [%S], Vendor: [%S]",
             aChannelTypeId, &aLocation, &aVendorId );
        }
    else
        {
        ERROR_3( KErrNotFound, "Did not found sensor channel: ChannelTypeId: [%x], Location: [%S], Vendor: [%S]",
             aChannelTypeId,
             &aLocation,
             &aVendorId );
        }

    // Cleanup
    CleanupStack::PopAndDestroy( 2, channelFinder );

    return found;
    }

//-----------------------------------------------------------------------------
// CSensorChannelBase::DoReopenChannel
//-----------------------------------------------------------------------------
//
void CSensorChannelBase::DoReopenChannel()
    {
    FUNC_LOG;

    delete iSensorChannel;
    iSensorChannel = NULL;
    TRAP_IGNORE( StartListeningL() );
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::ChangeState
//------------------------------------------------------------------------------
//
void CSensorChannelBase::ChangeState( TSensorChannelState aState )
    {
    FUNC_LOG;

    if( aState != iState )
        {
        INFO_3( "Sensor channel [%S] changed state: [%S] -> [%S]",
            &ChannelName( iChannelId ),
            &ChannelStateName( iState ),
            &ChannelStateName( aState ) );

        iState = aState;
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::SearchCommandByFeatureId
//------------------------------------------------------------------------------
//
TInt CSensorChannelBase::SearchCommandByFeatureId( const TPtrC& aFeatureId ) const
    {
    FUNC_LOG;

    TInt ret = KErrNotFound;
    for( TInt i = 0; i < iClients.Count(); i++ )
        {
        CSensorSourceClient* client = iClients[i];
        if( client->iFeatureId.CompareF( aFeatureId ) == KErrNone )
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::SearchCommandBySid
//------------------------------------------------------------------------------
//
TInt CSensorChannelBase::SearchCommandBySid( const TSecureId& aSid ) const
    {
    FUNC_LOG;

    TInt ret = KErrNotFound;
    for( TInt i = 0; i < iClients.Count(); i++ )
        {
        CSensorSourceClient* client = iClients[i];
        if( client->iSid == aSid )
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::SearchCommandByScriptId
//------------------------------------------------------------------------------
//
TInt CSensorChannelBase::SearchCommandByScriptId( TInt aScriptId ) const
    {
    FUNC_LOG;

    TInt ret = KErrNotFound;
    for( TInt i = 0; i < iClients.Count(); i++ )
        {
        CSensorSourceClient* client = iClients[i];
        if( client->iScriptId == aScriptId )
            {
            ret = i;
            break;
            }
        }
    return ret;
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::DoStartL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::DoStartL( TSensorChannelState aState )
    {
    FUNC_LOG;

    // If we have not connected to channel, connect
    if( !iSensorChannel )
        {
        iSensorChannel = CSensrvChannel::NewL( iChannelInfo );
        }

    iSensorChannel->OpenChannelL();
    INFO_1( "Opened sensor channel: [%S]", &ChannelName( iChannelId ) );

    iSensorChannel->StartDataListeningL( this,
        KMinSampleRate, KMaxSampleRate, KBufferRate );
    INFO_1( "Started sensor data listening: [%S]", &ChannelName( iChannelId ) );

    ChangeState( aState );
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::DoStop
//------------------------------------------------------------------------------
//
void CSensorChannelBase::DoStop()
    {
    FUNC_LOG;

    if( iSensorChannel )
        {
        iSensorChannel->StopDataListening();
        INFO_1( "Stopped sensor data listening: [%S]", &ChannelName( iChannelId ) );

        iSensorChannel->CloseChannel();
        INFO_1( "Closed sensor channel: [%S]", &ChannelName( iChannelId ) );

        ChangeState( EChannelStateIdle );
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::HandleActivateCommandL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::HandleActivateCommandL(
    const CSensorSourceCommand& aCommand )
    {
    FUNC_LOG;

    // First check the feature id
    if( aCommand.FeatureId().Length() )
        {
        // Check if the feature already has activated the sensors
        if( SearchCommandByFeatureId( aCommand.FeatureId() ) == KErrNotFound )
            {
            // Activate sensor channel for this feature
            CSensorSourceClient* client = CSensorSourceClient::NewLC(
                aCommand.SecureId(), aCommand.ScriptId(), aCommand.FeatureId() );
            AddClientL( client );
            CleanupStack::Pop( client );
            INFO_2( "Sensor channel: [%S] activated by feature: [%S]",
                &ChannelName( iChannelId ), &(const TDesC&)aCommand.FeatureId() );
            }
        else
            {
            INFO_2( "Sensor channel: [%S] already activated by feature: [%S]",
                &ChannelName( iChannelId ), &(const TDesC&)aCommand.FeatureId() );
            }
        }
    // Check the script id
    else if( aCommand.ScriptId() != KErrNotFound )
        {
        // Check if the script id already has activated the sensors
        if( SearchCommandByScriptId( aCommand.ScriptId() ) == KErrNotFound )
            {
            // Activate sensor for this script
            CSensorSourceClient* client = CSensorSourceClient::NewLC(
                aCommand.SecureId(), aCommand.ScriptId(), aCommand.FeatureId() );
            AddClientL( client );
            CleanupStack::Pop( client );
            INFO_2( "Sensor channel: [%S] activated by script id: [%d]",
                &ChannelName( iChannelId ), aCommand.ScriptId() );
            }
        else
            {
            INFO_2( "Sensor channel: [%S] already activated by script id: [%d]",
                &ChannelName( iChannelId ), aCommand.ScriptId() );
            }
        }
    // Check the SID
    else
        {
        // Check if the script id already has activated the sensors
        if( SearchCommandBySid( aCommand.SecureId() ) == KErrNotFound )
            {
            // Activate sensor for this Sid
            CSensorSourceClient* client = CSensorSourceClient::NewLC(
                aCommand.SecureId(), aCommand.ScriptId(), aCommand.FeatureId() );
            AddClientL( client );
            CleanupStack::Pop( client );
            INFO_2( "Sensor channel: [%S] activated by Sid: [0x%x]",
                &ChannelName( iChannelId ), aCommand.SecureId().iId );
            }
        else
            {
            INFO_2( "Sensor channel: [%S] already activated by Sid: [0x%x]",
                &ChannelName( iChannelId ), aCommand.SecureId().iId );
            }
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::HandleDeactivateCommandL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::HandleDeactivateCommandL(
    const CSensorSourceCommand& aCommand )
    {
    FUNC_LOG;

    // First check the feature id
    if( aCommand.FeatureId().Length() )
        {
        // Check if the feature already has activated the sensors
        TInt index = SearchCommandByFeatureId( aCommand.FeatureId() );
        if( index != KErrNotFound )
            {
            RemoveClient( index );
            INFO_2( "Sensor channel: [%S] deactivated by feature id: [%S]",
                &ChannelName( iChannelId ), &(const TDesC&)aCommand.FeatureId() );
            }
        }
    // Check the script id
    else if( aCommand.ScriptId() != KErrNotFound )
        {
        // Check if the script id already has activated the sensors
        TInt index = SearchCommandByScriptId( aCommand.ScriptId() );
        if( index != KErrNotFound )
            {
            RemoveClient( index );
            INFO_2( "Sensor channel: [%S] deactivated by script id: [%d]",
                &ChannelName( iChannelId ), aCommand.ScriptId() );
            }
        }
    // Check the SID
    else
        {
        // Check if the script id already has activated the sensors
        TInt index = SearchCommandBySid( aCommand.SecureId() );
        if( index != KErrNotFound )
            {
            RemoveClient( index );
            INFO_2( "Sensor channel: [%S] deactivated by Sid: [0x%x]",
                &ChannelName( iChannelId ), aCommand.SecureId().iId );
            }
        }
    }

//------------------------------------------------------------------------------
// CSensorChannelBase::AddClientL
//------------------------------------------------------------------------------
//
void CSensorChannelBase::AddClientL( CSensorSourceClient* aClient )
    {
    FUNC_LOG;

    // Get previous client count
    TInt oldCount = iClients.Count();

    if( iState == EChannelStateIdle && oldCount == 0 )
        {
        // Activate sensor channel
        StartListeningL();
        }
    // Add command in to list
    iClients.AppendL( aClient );

	INFO_2( "AddClientL(): active clients [%d], old client count [%d]", iClients.Count(), oldCount );

    }

//------------------------------------------------------------------------------
// CSensorChannelBase::RemoveClient
//------------------------------------------------------------------------------
//
void CSensorChannelBase::RemoveClient( TInt aIndex )
    {
    FUNC_LOG;

    if( aIndex >= 0 && aIndex < iClients.Count() )
        {
        CSensorSourceClient* client = iClients[aIndex];
        iClients.Remove( aIndex );
        if( iClients.Count() == 0 )
            {
            // There are no clients anymore, stop sensor channel
            StopListening();
            }
        delete client;
        client = NULL;
        }
    }

// End of file
