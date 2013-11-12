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
* Description:  CSensorSourcePlugIn class implementation.
*
*/


#include <cfcontextinterface.h>
#include <cfcontextindication.h>
#include <cfcontextsubscription.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>
#include <sensrvchanneldatatypes.h>
#include <cfcontextsourcecommand.h>

#include "sensorsourceplugin.h"
#include "sensorsourcecontextdef.h"
#include "sensorchannelbase.h"
#include "sensorchanneldoubletap.h"
#include "sensorchannelorientation.h"
#include "sensorchannelproximity.h"
#include "sensortrace.h"
#include "sensorsourcecommand.h"
#include "sensorutil.h"

// CONSTANTS

#ifdef _DEBUG

// Panic category
_LIT( KPanicCat, "SensorSrc" );

// Panic codes
enum TPanicCode
    {
    EInvalidChannel,
    EInvalidChannelIndex
    };

// Local panic function
LOCAL_C void Panic( TPanicCode aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

_LIT( KChannel, "Channel" );
_LIT( KChannelTypeId, "ChannelTypeId" );
_LIT( KLocation, "Location" );
_LIT( KVendor, "Vendor" );

// MEMBER FUNCTIONS

CSensorSourcePlugIn* CSensorSourcePlugIn::NewL(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CSensorSourcePlugIn* self = CSensorSourcePlugIn::NewLC( aParams );
    CleanupStack::Pop( self );

    return self;
    }

CSensorSourcePlugIn* CSensorSourcePlugIn::NewLC(
TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CSensorSourcePlugIn* self =
        new( ELeave ) CSensorSourcePlugIn( aParams );
    CleanupStack::PushL( self );

    return self;
    }

// Destructor
CSensorSourcePlugIn::~CSensorSourcePlugIn()
    {
    FUNC_LOG;

    // Cleanup channels
    iChannels.ResetAndDestroy();

    // Cleanup subscriptions
    iCF.UnsubscribeContexts( *this );
    iSubscriptions.Close();

    // Cleanup settings
    delete iSettings;

    // Cleanup feature id
    delete iFeatureId;
    }

CSensorSourcePlugIn::CSensorSourcePlugIn(
    TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::HandleSettingL
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* aSettingList )
	{
    FUNC_LOG;

	// Take ownership from settings
    iSettings = aSettingList;
	}

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::DefineContextsL
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::DefineContextsL()
    {
    FUNC_LOG;

    iCF.DefineContext( KSensorSource,
        KSensorSourceEvent,
        KSensorSourceEventSec );

    // Temporary solution to make sure that channels are not kept open
    // unnecessarily - This would have been fixed in dev branch but of course
    // we cannot include the implementation since it will surely break something.
    RThread thread;
    CCFContextObject* co = NULL;

    // Define context
    iCF.DefineContext( KSensorSource,
        KSensorSourceEventAutoRotate,
        KSensorSourceEventAutoRotateSec );

    // Publish context
    co = CCFContextObject::NewLC( KSensorSource,
        KSensorSourceEventAutoRotate,
        TPtrC( KSensorSourceEventAutoRotateVal[EAutoRotateDisabled] ) );
    iCF.PublishContext( *co, thread );
    CleanupStack::PopAndDestroy( co );

    // Define context
    iCF.DefineContext( KSensorSource,
        KSensorSourceEventAutoRotateAndDoubleTap,
        KSensorSourceEventAutoRotateAndDoubleTapSec );

    // Publish context
    co = CCFContextObject::NewLC( KSensorSource,
        KSensorSourceEventAutoRotateAndDoubleTap,
        TPtrC( KSensorSourceEventAutoRotateAndDoubleTapVal[EAutoRotateAndDoubleTapDisabled] ) );
    iCF.PublishContext( *co, thread );
    CleanupStack::PopAndDestroy( co );

    // Clean up
    thread.Close();
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::InitializeL
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::InitializeL()
    {
    FUNC_LOG;

    TInt count = 0;
    if( iSettings )
        {
        count = iSettings->Count();
        TUint32 channelId = 0;
        TUint32 channelTypeId = 0;
        TPtrC value( KNullDesC );
        TPtrC key( KNullDesC );
        HBufC8* location = NULL;
        HBufC8* vendor = NULL;
        for( TInt i = 0; i < count; i++ )
            {
            // Check attributes
            const RKeyValueArray& attributes = iSettings->Item( i ).Attributes();
            TInt attrCount = attributes.Count();

            // Check channel
            for( TInt j = 0; j < attrCount; j++ )
                {
                // Currently only interested in "Channel" attribute
                if( attributes[j]->Key().CompareF( KChannel ) == KErrNone )
                    {
                    // Found channel, panic if invalid channel
                    TInt err = SensorUtil::ConvertToHex( attributes[j]->Value(), channelId );
                    __ASSERT_DEBUG( channelId < CSensorChannelBase::EChannelCount &&
                        err == KErrNone,
                        Panic( EInvalidChannel ) );
                    INFO_1( "Found channel: %x", channelId );
                    break;
                    }
                }

            // Check parameters
            const RSettingParameterArray& parameters = iSettings->Item( i ).Parameters();
            TInt paramCount = parameters.Count();
            for( TInt j = 0; j < paramCount; j++ )
                {
                const RKeyValueArray& paramAttr = parameters[j]->Attributes();
                TInt paramAttrCount = paramAttr.Count();
                for( TInt k = 0; k < paramAttrCount; k++ )
                    {
                    key.Set( paramAttr[k]->Key() );
                    value.Set( paramAttr[k]->Value() );

                    // Check channel type id
                    if( key.CompareF( KChannelTypeId ) == KErrNone )
                        {
                        // Found channel type id
                        TInt err = SensorUtil::ConvertToHex( value, channelTypeId );
                        if( err != KErrNone )
                            {
                            channelTypeId = 0x00;
                            }
                        INFO_1( "Found channel type id: %x", channelTypeId );
                        }
                    // Check location
                    else if( key.CompareF( KLocation ) == KErrNone )
                        {
                        // Found location
                        location = HBufC8::NewLC( value.Length() );
                        TPtr8 locationPtr = location->Des();
                        locationPtr.Copy( value );
                        INFO_1( "Found location: %S", &locationPtr );
                        }

                    // Check vendor id
                    else if( key.CompareF( KVendor ) == KErrNone )
                        {
                        // Found location
                        vendor = HBufC8::NewLC( value.Length() );
                        TPtr8 vendorPtr = vendor->Des();
                        vendorPtr.Copy( value );
                        INFO_1( "Found vendor: %S", &vendorPtr );
                        }
                    }
                }
            // Construct channel from parameters
            CSensorChannelBase* sensorChannel = NULL;
            TRAPD( err, sensorChannel = CreateChannelL(
                static_cast<CSensorChannelBase::TSensorChannelId>( channelId ),
                channelTypeId,
                location ? *location : KNullDesC8(),
                vendor ? *vendor : KNullDesC8() ) );
            ERROR_1( err, "Failed to create channel: [%x]", channelId );
            if( sensorChannel && err == KErrNone )
                {
                CleanupStack::PushL( sensorChannel );
                iChannels.AppendL( sensorChannel );
                CleanupStack::Pop( sensorChannel );
                }

            // Cleanup location and vendor buffers
            if( location )
                {
                CleanupStack::PopAndDestroy(); // location
                }
            if( vendor )
                {
                CleanupStack::PopAndDestroy(); // vendor
                }
            }

        // Free memory allocated for settings since we don't need them anymore
        delete iSettings;
        iSettings = NULL;
        }

    count = iChannels.Count();
    if( !count )
        {
        // Invalid settings, we don't have any channels so leave
        User::Leave( KErrArgument );
        }

    // Define channel specific contexts
    for( TInt i = 0; i < count; i++ )
        {
        iChannels[i]->DefineContextsL();
        }

    // Subscribe for activation and deactivation sensor contexts
    SubscribeL();

    // Create feature id descriptor
    iFeatureId = HBufC::NewL( KMaxFileName );
    }

//------------------------------------------------------------------------------
// CSensorSourcePlugIn::Extension
//------------------------------------------------------------------------------
//
TAny* CSensorSourcePlugIn::Extension( const TUid& aExtensionUid ) const
    {
    FUNC_LOG;

    TAny* extension = NULL;
    if( aExtensionUid == KCFContextSourceInterfaceUid )
        {
        MCFContextSource* interface = (MCFContextSource*)this;
        extension = (TAny*)interface;
        }

    return extension;
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::ContextIndicationL
//
// NOTE:
//  Context inddication should not be used to activate / deactivate sensors!
//  Use context source command instead.
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::ContextIndicationL(
    CCFContextIndication* aIndication )
    {
    FUNC_LOG;

    const CCFContextObject& co = aIndication->Context();
    INFO_3( "Received context indication: [%S: %S: %S]",
        &co.Source(),
        &co.Type(),
        &co.Value() );

    // Collect sensor source command paramters
    TSecureId sid = RThread().SecureId();
    TInt scriptId = KErrNotFound;
    TUint32 channelId = 0x0;
    TPtrC commandId( KNullDesC );
    TPtr featureId( iFeatureId->Des() );
    featureId.Zero();

    // Check the channel id and command id
    if( co.Source() == KSensorSource &&
        co.Type() == KSensorSourceEvent )
        {
        if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventDoubleTapActivate] ) )
            {
            // Activate double tap channel
            channelId = CSensorChannelBase::EChannelDoubleTap;
            commandId.Set( KSensorCommandIdActivateName );
            iActiveDoubleTap++;
            featureId.Format( KSensorFeatureGeneralName, iActiveDoubleTap );
            }
        else if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventDoubleTapDeactivate] ) )
            {
            // Deactivate double tap channel
            channelId = CSensorChannelBase::EChannelDoubleTap;
            commandId.Set( KSensorCommandIdDeactivateName );
            featureId.Format( KSensorFeatureGeneralName, iActiveDoubleTap );
            iActiveDoubleTap--;
            if( iActiveDoubleTap < 0 )
                {
                iActiveDoubleTap = 0;
                }
            }
        else if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventOrientationActivate] ) )
            {
            // Activate orientation channel
            channelId = CSensorChannelBase::EChannelOrientation;
            commandId.Set( KSensorCommandIdActivateName );
            iActiveOrientation++;
            featureId.Format( KSensorFeatureGeneralName, iActiveOrientation );
            }
        else if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventOrientationDeactivate] ) )
            {
            // Deactivate orientation channel
            channelId = CSensorChannelBase::EChannelOrientation;
            commandId.Set( KSensorCommandIdDeactivateName );
            featureId.Format( KSensorFeatureGeneralName, iActiveOrientation );
            iActiveOrientation--;
            if( iActiveOrientation < 0 )
                {
                iActiveOrientation = 0;
                }
            }
       else if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventProximityActivate] ) )
            {
            // Activate proximity channel
            //ActivateSensorChannelL(
            //    CSensorChannelBase::EChannelProximity );

            channelId = CSensorChannelBase::EChannelProximity;
            commandId.Set( KSensorCommandIdActivateName );
            iActiveProximity++;
            featureId.Format( KSensorFeatureGeneralName, iActiveProximity );

            }
       else if( co.Value() == TPtrC(
            KSensorSourceEventValues[EEventProximityDeactivate] ) )
            {
            // Deactivate proximity channel
            //DeactivateSensorChannel(
            //    CSensorChannelBase::EChannelProximity );

            channelId = CSensorChannelBase::EChannelProximity;
            commandId.Set( KSensorCommandIdDeactivateName );
            featureId.Format( KSensorFeatureGeneralName, iActiveProximity );
            iActiveProximity--;
            if( iActiveProximity < 0 )
                {
                iActiveProximity = 0;
                }

            }
        }

    // Convert context indication in to sensor source command
    CSensorSourceCommand* command = CSensorSourceCommand::NewLC(
        sid, scriptId, featureId, commandId, channelId );

    // Search for the channel
    TInt count = iChannels.Count();
    CSensorChannelBase* channel = NULL;
    for( TInt i = 0 ; i < count; i++ )
        {
        channel = iChannels[i];
        if( channel->SensorChannelId() == command->ChannelId() )
            {
            channel->HandleSensorCommandL( *command );
            break;
            }
        channel = NULL;
        }
    if( !channel )
        {
        ERROR_1( KErrNotFound, "Could not find channel id: 0%x", command->ChannelId() );
        }

    // Clean up
    CleanupStack::PopAndDestroy( command );
    delete aIndication;
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::Client
//-----------------------------------------------------------------------------
//
TInt CSensorSourcePlugIn::Client( RThread& /*aThread*/ ) const
    {
    FUNC_LOG;

    // We are running in cfserver process main thread
    // nothing must be done
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::HandleContextSubscriptionError
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    FUNC_LOG;

    // nothing must be done
    }

//------------------------------------------------------------------------------
// CSensorSourcePlugIn::Subscribers
//------------------------------------------------------------------------------
//
void CSensorSourcePlugIn::Subscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/ )
    {
    FUNC_LOG;

    // Nothing to do.
    }

//------------------------------------------------------------------------------
// CSensorSourcePlugIn::NoSubscribers
//------------------------------------------------------------------------------
//
void CSensorSourcePlugIn::NoSubscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType */)
    {
    FUNC_LOG;

    // Nothing to do.
    }

//------------------------------------------------------------------------------
// CSensorSourcePlugIn::HandleCommand
//------------------------------------------------------------------------------
//
void CSensorSourcePlugIn::HandleCommand(
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    TRAPD( err, HandleCommandL( aCommand ) );
    if( err != KErrNone )
        {
        ERROR_2( err, "Failed to handle command from Sid: [0x%x], Script ID: [%d]",
            aCommand.Sender().iSender.iId, aCommand.Sender().iScriptId );
        }
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::SubscribeL
//-----------------------------------------------------------------------------
//
void CSensorSourcePlugIn::SubscribeL()
    {
    FUNC_LOG;

    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    subscription->SetContextSourceL( KSensorSource );
    subscription->SetContextTypeL( KSensorSourceEvent );
    subscription->SetOnlyChangesIndicated( EFalse );
    iSubscriptions.AppendL( subscription );
    CleanupStack::Pop( subscription );

    TInt count = iSubscriptions.Count();
    for( TInt i = 0; i < count; i++ )
        {
        RThread thread;
        iCF.SubscribeContext( iSubscriptions[i], this, thread );
        thread.Close();
        }
    }

//-----------------------------------------------------------------------------
// CSensorSourcePlugIn::CreateChannelL
//-----------------------------------------------------------------------------
//
CSensorChannelBase* CSensorSourcePlugIn::CreateChannelL(
    CSensorChannelBase::TSensorChannelId aChannel,
    TUint /*aChannelTypeId*/,
    const TDesC8& aLocation,
    const TDesC8& aVendor )
    {
    FUNC_LOG;

    // First check that we do not have already this channel created
    TInt count = iChannels.Count();
    for( TInt i = 0 ; i < count; i++ )
        {
        if( iChannels[i]->SensorChannelId() == aChannel )
            {
            // channle already found, leave
            User::Leave( KErrAlreadyExists );
            }
        }

    CSensorChannelBase* channel = NULL;
    switch( aChannel )
        {
        case CSensorChannelBase::EChannelDoubleTap:
            {
            channel = CSensorChannelDoubleTap::NewL(
                aLocation,
                aVendor,
                iCF );
            INFO( "Succesully created double tap channel" );
            break;
            }
        case CSensorChannelBase::EChannelOrientation:
            {
            channel = CSensorChannelOrientation::NewL(
                aLocation,
                aVendor,
                iCF );
            INFO( "Succesully created orientation channel" );
            break;
            }
        case CSensorChannelBase::EChannelProximity:
            {
            channel = CSensorChannelProximity::NewL(
                aLocation,
                aVendor,
                iCF );
            INFO( "Succesully created proximity channel" );
            break;
            }

        default:
            {
            break;
            }
        }

    return channel;
    }

//------------------------------------------------------------------------------
// CSensorSourcePlugIn::HandleCommandL
//------------------------------------------------------------------------------
//
void CSensorSourcePlugIn::HandleCommandL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    // Create sensor command
    CSensorSourceCommand* command = CSensorSourceCommand::NewLC( aCommand );

    // Search the sensor channel
    TInt count = iChannels.Count();
    CSensorChannelBase* channel = NULL;
    for( TInt i = 0 ; i < count; i++ )
        {
        channel = iChannels[i];
        if( channel->SensorChannelId() == command->ChannelId() )
            {
            channel->HandleSensorCommandL( *command );
            break;
            }
        channel = NULL;
        }
    if( !channel )
        {
        ERROR_1( KErrNotFound, "Could not find channel id: 0%x", command->ChannelId() );
        }

    CleanupStack::PopAndDestroy( command );
    }
