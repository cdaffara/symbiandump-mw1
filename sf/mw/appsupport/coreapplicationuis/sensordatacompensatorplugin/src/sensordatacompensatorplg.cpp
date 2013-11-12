/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Compensation item and compensation controller
*
*/


// INCLUDE FILES
#include <sensordatacompensatorplugintypes.h>
#include "sensordatacompensatorplg.h"
#include "trace.h"

#include <sensrvaccelerometersensor.h>
#include <sensrvmagnetometersensor.h>
#include <sensrvmagneticnorthsensor.h>
#include <sensrvtappingsensor.h>

#include <cfkeyvaluepair.h>
#include <cfactionindication.h>
#include <cfactionsubscription.h>
#include <cfcontextobject.h>

// ======== MEMBER FUNCTIONS ========

_LIT( KEventTypeText, "Type" );
_LIT( KSensorDataCompensation, "SensorDataCompensation" );
_LIT( KSource, "Sensor" );
_LIT( KType, "Event.Compensation" );
_LIT( KValue, "NewClient" );

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CSensorDataCompensatorPlg::CSensorDataCompensatorPlg(
    TSensrvChannelDataTypeId aDataType,
    TSensorCompensationType aType,
    MSensorDataCompensatorObserver& aObserver ) :
    iObserver( aObserver ),
    iDataType( aDataType ),
    iType( aType )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSensorDataCompensatorPlg::~CSensorDataCompensatorPlg()
    {
    FUNC_LOG;

    delete iCfThread;
    iArrayOfItems.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// 2-phase constructor
// ---------------------------------------------------------------------------
//
CSensorDataCompensatorPlg* CSensorDataCompensatorPlg::NewL(
    TSensrvChannelDataTypeId aDataType,
    TSensorCompensationType aType,
    MSensorDataCompensatorObserver& aObserver )
    {
    FUNC_LOG;

    CSensorDataCompensatorPlg* self =
        new (ELeave) CSensorDataCompensatorPlg( aDataType, aType, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 2nd phase of construction
// ---------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::ConstructL()
    {
    FUNC_LOG;

    // Creates and starts the cf listener thread.
    iCfThread = CSensorDataCompensatorCfTread::NewL( *this );
    iAllowMultiples = ( TSensrvAccelerometerAxisData::KDataTypeId == iDataType ||
                        TSensrvMagnetometerAxisData::KDataTypeId  == iDataType ||
                        TSensrvMagneticNorthData::KDataTypeId     == iDataType );
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::NotifyControllerL
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::NotifyControllerL( const RKeyValueArray& aKeyValueArray)
    {
    FUNC_LOG;
    
    SetItemL( aKeyValueArray );
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::SetItemL
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::SetItemL( const RKeyValueArray& aKeyValueArray )
    {
    FUNC_LOG;

    TBool changed( EFalse );
    TBool skip( EFalse );
    CSensorDataCompensatorItem* item = NULL;
    TInt paramCount = aKeyValueArray.Count();

    for( TInt i = 0; i < paramCount; i++ )
        {
        // Process parameters
        const CCFKeyValuePair* parameter = aKeyValueArray[ i ];
        TPtrC key( parameter->Key() );
        TPtrC value( parameter->Value() );

        if( !key.Compare( KEventTypeText ) )
            {
            INFO_2( "CSensorDataCompensatorPlg::SetItemL() - New type event: [%S, %S]", &key, &value );

            // Check if this compensation was requested
            TInt type = CSensorDataCompensatorItem::GetCompensationType( value );
            if (   ( iType == ESensorCompensateDeviceOrientation && type == EDeviceCompensationItem )  // Only non-UI. -orientation types compensated
                || ( iType == ESensorCompensateDeviceAndUIOrientation && ( type == EDeviceCompensationItem || type == EUICompensationItem ) )
                || ( iType == ESensorCompensateUIOrientation && type == EUICompensationItem ) )     // Only UI. -orientation types compensated
                {
                skip = EFalse;
                if ( iAllowMultiples ) // Use superposition for different sources
                    {
                    item = CSensorDataCompensatorItem::Find( value, iArrayOfItems );
                    if ( !item )
                        {
                        item = CSensorDataCompensatorItem::InsertL( value, iArrayOfItems );
                        }
                    }
                else // New values always override previous values, even if source differs
                    {
                    if ( iArrayOfItems.Count() > 0 )
                        {
                        item = iArrayOfItems[ 0 ];
                        }
                    else
                        {
                        // Only single instance allowed
                        item = CSensorDataCompensatorItem::InsertL( value, iArrayOfItems );
                        }
                    }
                }
            else
                {
                INFO( "CSensorDataCompensatorPlg::SetItemL() - Requested compensation type and initialised SensorCompensationType do not match" );
                skip = ETrue; // Skip until next type arrives
                }
            }
        else if ( item && !skip )
            {
            INFO_2( "CSensorDataCompensatorPlg::SetItemL() - New value event: [%S, %S]", &key, &value  );
            if ( item->Parse( key, value ) )
                {
                changed = ETrue;
                }
            }
        }

    CleanItems();

    if ( changed )
        {
#ifdef _DEBUG
        TInt count( iArrayOfItems.Count() );
        for( TInt i( 0 ); i < count; ++i )
            {
            INFO_1("CSensorDataCompensatorPlg::SetItemL() ItemIndex %d", i );
            iArrayOfItems[ i ]->DebugPrint();
            }
#endif // _DEBUG

        INFO( "CSensorDataCompensatorPlg::SetItemL() - Compensation values updated" );
        iObserver.CompensationValuesUpdated();
        }
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::CleanItems
// Cleanup empty items.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::CleanItems()
    {
    FUNC_LOG;

    CSensorDataCompensatorItem* item = NULL;
    TInt i = iArrayOfItems.Count();

    while ( --i >= 0 )
        {
        if ( iArrayOfItems[ i ]->IsEmpty( iDataType ) )
            {
            item = iArrayOfItems[ i ];
            delete item;
            iArrayOfItems.Remove( i );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::ThreadInitL
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::ThreadInitL()
    {
    FUNC_LOG;

    iInitializer = CSensorDataCompensatorInitializer::NewL( *this );
    }
    
// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::ThreadRelease
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::ThreadRelease()
    {
    FUNC_LOG;

    delete iInitializer;
    iInitializer = NULL;
    delete iCFClient;
    iCFClient = NULL;
    }
    
// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::ContextIndicationL
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::ContextIndicationL(
    const CCFContextIndication&/* aChangedContext*/ )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::ActionIndicationL
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::ActionIndicationL(
    const CCFActionIndication& aActionToExecute )
    {
    FUNC_LOG;

    if( aActionToExecute.Identifier() == KSensorDataCompensation )
        {
        iCfThread->StoreValuesAndNotify( aActionToExecute );
        }
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::HandleContextFrameworkError
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::HandleContextFrameworkError(
    TCFError /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::Extension
// Run in listener thread context
// -----------------------------------------------------------------------------
//
TAny* CSensorDataCompensatorPlg::Extension( const TUid& /*aExtensionUid*/ ) const
    {
    FUNC_LOG;

    return NULL;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorPlg::InitCfwL
// Run in listener thread context
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorPlg::InitCfwL()
    {
    FUNC_LOG;

    iCFClient = CCFClient::NewL( *this );
    delete iInitializer;
    iInitializer = NULL;
    iCFClient->DefineAction( KSensorDataCompensation, TSecurityPolicy::EAlwaysPass );

    // Subscribe compensation values
    CCFActionSubscription* action = CCFActionSubscription::NewLC();
    action->SetActionIdentifierL( KSensorDataCompensation );
    iCFClient->SubscribeAction( *action );
    CleanupStack::PopAndDestroy( action );

    // Publish 'new client' to get the initial values
    iCFClient->DefineContext( KSource, KType, TSecurityPolicy::EAlwaysPass );
    CCFContextObject* obj = CCFContextObject::NewL( KSource, KType, KValue );
    iCFClient->PublishContext( *obj );
    delete obj;
    }

// ---------------------------------------------------------------------------
// CSensorDataCompensatorPlg::GetCompensationItemCount
// ---------------------------------------------------------------------------
//
TInt CSensorDataCompensatorPlg::GetCompensationItemCount()
    {
    FUNC_LOG;

    TInt ret( KErrNotReady );

    if ( iCFClient )
        {
        ret = iArrayOfItems.Count();
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CSensorDataCompensatorPlg::GetCompensationItem
// ---------------------------------------------------------------------------
//
TPtrC8 CSensorDataCompensatorPlg::GetCompensationItem( TInt aIndex )
    {
    FUNC_LOG;

    if ( aIndex >= 0 && aIndex < iArrayOfItems.Count() )
        {
        CSensorDataCompensatorItem& item = *( iArrayOfItems[ aIndex ] );
        switch( iDataType )
            {
            case TSensrvAccelerometerAxisData::KDataTypeId:
            case TSensrvMagnetometerAxisData::KDataTypeId: // Fall through
                {
                item.GetCompensationTypeAxisData( iCompensationTypeAxisDataBuf() );
                return iCompensationTypeAxisDataBuf;
                }
            case TSensrvMagneticNorthData::KDataTypeId:
                {
                item.GetCompensationTypeAngleData( iCompensationTypeAngleDataBuf() );
                return iCompensationTypeAngleDataBuf;
                }

            case TSensrvTappingData::KDataTypeId:
                {
                item.GetCompensationTypeDirectionData( iCompensationTypeDirectionDataBuf() );
                return iCompensationTypeDirectionDataBuf;
            }
            default:
                {
                break;
                }
            }
        }
    return KNullDesC8();
    }
