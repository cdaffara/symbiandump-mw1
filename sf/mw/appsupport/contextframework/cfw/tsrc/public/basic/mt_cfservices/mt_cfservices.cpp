/*
* Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


// CLASS HEADER
#include "MT_CFServices.h"

// EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <S32MEM.H>

// INTERNAL INCLUDES
#include "CFActionIndicationImpl.h"
#include "CFActionSubscriptionImpl.h"
#include "CFContextIndicationImpl.h"
#include "cfcontextobjectimpl.h"
#include "CFContextQueryImpl.h"
#include "CFContextSubscriptionImpl.h"
#include "CFKeyValuePair.h"
#include "cfserviceutils.h"
#include "cfcontextdataproxy.h"

// CONSTANTS
_LIT( KKey, "Key_%d" );
_LIT( KValue, "Value_%d" );
_LIT( KActionId, "ActionID" );
_LIT( KShortActionId, "ActID" );
_LIT( KLongActionId, "LongActionIdentifier" );
static const TInt KParamCount = 10;

_LIT( KContextSource, "ContextSource" );
_LIT( KContextSourceLong, "ContextSourceQuiteABitLonger" );
_LIT( KContextType, "ContextType" );
_LIT( KContextTypeLong, "ContextTypeQuiteABitLonger" );
_LIT( KContextValue, "ContextValue" );
_LIT( KContextValueLong, "ContextValueQuiteABitLonger" );

// CONSTRUCTION
MT_CFServices* MT_CFServices::NewL()
    {
    MT_CFServices* self = MT_CFServices::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFServices* MT_CFServices::NewLC()
    {
    MT_CFServices* self = new( ELeave ) MT_CFServices();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFServices::~MT_CFServices()
    {
    Teardown();
    }

// Default constructor
MT_CFServices::MT_CFServices()
    {
    }

// Second phase construct
void MT_CFServices::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// METHODS

void MT_CFServices::ActionIndicationL( CCFActionIndication* /*aIndication*/ )
    {
    
    }

void MT_CFServices::ContextIndicationL( CCFContextIndication* aIndication )
    {
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    
    iIndicatedContext = CCFContextObject::NewL();
    iIndicatedContext->CopyL( aIndication->Context() );
    
    delete aIndication;
    }

TInt MT_CFServices::Client( RThread& /*aThread*/ ) const
    {
    return KErrNone;
    }

void MT_CFServices::HandleContextSubscriptionError( TInt /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType*/ )
    {

    }

void MT_CFServices::SetupL(  )
    {

    }

void MT_CFServices::SetupActionIndicationL(  )
    {
    iActionIndication = CCFActionIndicationImpl::NewL();
    }

void MT_CFServices::Teardown(  )
    {
    delete iActionIndication;
    iActionIndication = NULL;
    
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    }
    
CCFActionIndicationImpl* MT_CFServices::ConfigureActionIndicationL(
    const TDesC& aActionId,
    TInt aNumOfParams )
    {
    TBuf<20> key( KNullDesC );
    TBuf<20> value( KNullDesC );

    CCFActionIndicationImpl* actionIndication = CCFActionIndicationImpl::NewLC();
    if( aActionId.Length() )
        {
        actionIndication->SetIdentifierL( aActionId );
        }
    for( TInt i = 0; i < aNumOfParams; i++ )
        {
        key.Format( KKey, i );
        value.Format( KValue, i );
        actionIndication->AddParameterL( key, value );
        }
    
    CleanupStack::Pop( actionIndication );
    return actionIndication;
    }

CCFActionSubscriptionImpl* MT_CFServices::ConfigureActionSubscriptionL(
    const TDesC& aActionId )
    {
    CCFActionSubscriptionImpl* actionSubscription =
        CCFActionSubscriptionImpl::NewLC();
    
    if( aActionId.Length() )
        {
        actionSubscription->SetActionIdentifierL( aActionId );
        }
    
    CleanupStack::Pop( actionSubscription );
    return actionSubscription;
    }
    
CCFContextObjectImpl* MT_CFServices::ConfigureContextObjectLC(
    const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue,
    TUint aConfidence,
    const TTime& aTime )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    if( aSource.Length() )
        {
        context->SetSourceL( aSource );
        }
    if( aType.Length() )
        {
        context->SetTypeL( aType );
        }
    if( aValue.Length() )
        {
        context->SetValueL( aValue );
        }
    context->SetConfidence( aConfidence );
    context->SetTimestamp( aTime );
    
    return context;
    }
    
CCFContextSubscriptionImpl* MT_CFServices::ConfigureContextSubscriptionLC(
    const TDesC& aSource,
    const TDesC& aType,
    TBool aSubTypeMatch,
    TBool aIndicateOnlyChanges )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    if( aSource.Length() )
        {
        subscription->SetContextSourceL( aSource );
        }
    if( aType.Length() )
        {
        subscription->SetContextTypeL( aType );
        }
    subscription->SetSubTypeMatch( aSubTypeMatch );
    subscription->SetOnlyChangesIndicated( aIndicateOnlyChanges );
    
    return subscription;
    }
    
//-----------------------------------------------------------------------------
// CCFActionIndication class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFActionIndication_NewLL(  )
    {
    CCFActionIndication* instance = CCFActionIndication::NewL();
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFActionIndication class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionIndication_NewLCL(  )
    {
    CCFActionIndication* instance = CCFActionIndication::NewLC();
    
    EUNIT_ASSERT_DESC( instance, "CCFActionIndication class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// CCFActionIndicationImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFActionIndicationImpl_NewLL(  )
    {
    CCFActionIndicationImpl* instance = CCFActionIndicationImpl::NewL();
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFActionIndicationImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionIndicationImpl_NewLCL(  )
    {
    CCFActionIndicationImpl* instance = CCFActionIndicationImpl::NewLC();
    
    EUNIT_ASSERT_DESC( instance, "CCFActionIndicationImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionIndicationImpl_SetIdentifierLL(  )
    {
    TPtrC id( KNullDesC );
    
    id.Set( iActionIndication->Identifier() );
    EUNIT_ASSERT_DESC( id == KNullDesC, "Invalid action ID!" );
    
    iActionIndication->SetIdentifierL( KActionId );
    id.Set( iActionIndication->Identifier() );
    EUNIT_ASSERT_DESC( id == KActionId, "Invalid action ID!" );

    iActionIndication->SetIdentifierL( KShortActionId );
    id.Set( iActionIndication->Identifier() );
    EUNIT_ASSERT_DESC( id == KShortActionId, "Invalid action ID!" );

    iActionIndication->SetIdentifierL( KLongActionId );
    id.Set( iActionIndication->Identifier() );
    EUNIT_ASSERT_DESC( id == KLongActionId, "Invalid action ID!" );
    }

void MT_CFServices::MT_CCFActionIndicationImpl_AddParameterLL(  )
    {
    TInt count = 0;
    TBuf<20> key( KNullDesC );
    TBuf<20> value( KNullDesC );
    
    count = iActionIndication->Parameters().Count();
    EUNIT_ASSERT_DESC( !count, "Invalid parameter count!" );
    
    delete iActionIndication;
    iActionIndication = NULL;
    iActionIndication = ConfigureActionIndicationL( KActionId, KParamCount );
    count = iActionIndication->Parameters().Count();
    EUNIT_ASSERT_DESC( count == KParamCount, "Invalid parameter count!" );
    
    const RKeyValueArray& params = iActionIndication->Parameters();
    for( TInt i = 0; i < KParamCount; i++ )
        {
        key.Format( KKey, i );
        EUNIT_ASSERT_DESC( key == params[i]->Key(), "Invalid key!" );
        
        value.Format( KValue, i );
        EUNIT_ASSERT_DESC( value == params[i]->Value(), "Invalid value!" );
        }
    }

void MT_CFServices::MT_CCFActionIndicationImpl_ExternalizeLL(  )
    {
    TInt size = 0;
    CCFActionIndicationImpl* actionIndication = NULL;
    
    static const TInt KCount = 2;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                // Create action indication
                actionIndication = ConfigureActionIndicationL( KActionId, KParamCount );
                CleanupStack::PushL( actionIndication );
                break;
                }
            case 1:
            default:
                {
                // Create action indication
                actionIndication = ConfigureActionIndicationL( KNullDesC, KParamCount * 2 );
                CleanupStack::PushL( actionIndication );
                break;
                }
            }
        
        // Externalize
        size = actionIndication->Size();
        HBufC8* buffer = HBufC8::NewLC( size );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        actionIndication->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        CCFActionIndicationImpl* newActionIndication = CCFActionIndicationImpl::NewLC();
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        newActionIndication->InternalizeL( readStream );

        // Clean up
        CleanupStack::PopAndDestroy( &readStream );
        
        // Check result
        EUNIT_ASSERT_DESC(
            actionIndication->Identifier() == newActionIndication->Identifier(),
                "Invalid identifier!" );
        EUNIT_ASSERT_DESC(
            actionIndication->Parameters().Count() == newActionIndication->Parameters().Count(),
                "Invalid parameter count!");
        TInt count = actionIndication->Parameters().Count();
        for( TInt i = 0; i < count; i++ )
            {
            EUNIT_ASSERT_DESC(
                actionIndication->Parameters()[i]->Key() == newActionIndication->Parameters()[i]->Key(),
                    "Invalid key!" );
            EUNIT_ASSERT_DESC(
                actionIndication->Parameters()[i]->Value() == newActionIndication->Parameters()[i]->Value(),
                    "Invalid value!" );
            }
        
        // Clean up
        CleanupStack::PopAndDestroy( newActionIndication );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( actionIndication );
        }
    }

//-----------------------------------------------------------------------------
// CCFActionSubscription class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFActionSubscription_NewLL(  )
    {
    CCFActionSubscription* instance = CCFActionSubscription::NewL();
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFActionSubscription class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionSubscription_NewLCL(  )
    {
    CCFActionSubscription* instance = CCFActionSubscription::NewLC();
    
    EUNIT_ASSERT_DESC( instance, "CCFActionSubscription class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// CCFActionSubscriptionImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFActionSubscriptionImpl_NewLL(  )
    {
    CCFActionSubscriptionImpl* instance = CCFActionSubscriptionImpl::NewL();
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFActionSubscriptionImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionSubscriptionImpl_NewLCL(  )
    {
    CCFActionSubscriptionImpl* instance = CCFActionSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( instance, "CCFActionSubscriptionImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionSubscriptionImpl_SetActionIdentifierLL(  )
    {
    CCFActionSubscriptionImpl* instance = CCFActionSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( instance->ActionIdentifier() == KNullDesC,
        "Incorrect action identifier!" );
        
    instance->SetActionIdentifierL( KActionId );
    EUNIT_ASSERT_DESC( instance->ActionIdentifier() == KActionId,
        "Incorrect action identifier!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFActionSubscriptionImpl_ExternalizeLL(  )
    {
    static const TInt KCount = 2;
    CCFActionSubscriptionImpl* instance = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                instance = ConfigureActionSubscriptionL( KActionId );
                CleanupStack::PushL( instance );
                break;
                }
            case 1:
            default:
                {
                instance = ConfigureActionSubscriptionL( KNullDesC );
                CleanupStack::PushL( instance );
                break;
                }
            }
            
        // Externalize
        HBufC8* buffer = HBufC8::NewLC( instance->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        instance->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        CCFActionSubscriptionImpl* newInstance = CCFActionSubscriptionImpl::NewLC();
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        newInstance->InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );
        
        // Check result
        EUNIT_ASSERT_DESC(
            instance->ActionIdentifier() == newInstance->ActionIdentifier(),
                "Incorrect action identifier!" );
        
        // Clean up
        CleanupStack::PopAndDestroy( newInstance );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( instance );
        }
    }

void MT_CFServices::MT_CCFActionSubscriptionImpl_SetListenerL(  )
    {
    CCFActionSubscriptionImpl* instance = CCFActionSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( !( &instance->Listener() ), "Invalid listener!" );
    
    instance->SetListener( this );
    EUNIT_ASSERT_DESC( ( &instance->Listener() ), "Invalid listener!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// CCFContextIndication class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextIndication_NewLL(  )
    {
    CCFContextIndication* instance = CCFContextIndication::NewL( KNullUid );
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFContextIndication class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFContextIndication_NewLCL(  )
    {
    CCFContextIndication* instance = CCFContextIndication::NewLC( KNullUid );
    
    EUNIT_ASSERT_DESC( instance, "CCFContextIndication class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// CCFContextIndicationImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextIndicationImpl_NewLL(  )
    {
    CCFContextIndicationImpl* instance = CCFContextIndicationImpl::NewL( KNullUid );
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFContextIndicationImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFContextIndicationImpl_NewLCL(  )
    {
    CCFContextIndicationImpl* instance = CCFContextIndicationImpl::NewLC( KNullUid );
    
    EUNIT_ASSERT_DESC( instance, "CCFContextIndicationImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFContextIndicationImpl_SetContextLL(  )
    {
    static const TUint KConfidence = 100;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
        
    CCFContextIndicationImpl* indication = CCFContextIndicationImpl::NewLC( KNullUid );
    indication->SetContextL( *context );
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::IsSame( *context, indication->Context() ),
            "Incorrect comparison result!" );
            
    context->SetSourceL( KContextSourceLong );
    EUNIT_ASSERT_DESC(
        !CCFContextObjectImpl::IsSame( *context, indication->Context() ),
            "Incorrect comparison result!" );
        
    // clean up
    CleanupStack::PopAndDestroy( indication );
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextIndicationImpl_ExternalizeLL(  )
    {
    static const TUint KConfidence = 100;
    TTime time;
    static const TInt KCount = 3;
    CCFContextObjectImpl* context = NULL;
    CCFContextIndicationImpl* indication = NULL;
    CCFContextIndicationImpl* copyIndication = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        time.HomeTime();
        switch( i )
            {
            case 0:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KNullDesC,
                    KNullDesC,
                    KConfidence,
                    time );
                break;
                }
            case 1:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KContextType,
                    KNullDesC,
                    KConfidence,
                    time );
                break;
                }
            case 2:
            default:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KContextType,
                    KContextValue,
                    KConfidence,
                    time );
                break;
                }
            }
        
        indication = CCFContextIndicationImpl::NewLC( KNullUid );
        indication->SetContextL( *context );
        
        // Externalize
        HBufC8* buffer = HBufC8::NewLC( indication->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        indication->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        copyIndication = CCFContextIndicationImpl::NewLC( readStream );
        
        // Check result
        EUNIT_ASSERT_DESC(
            CCFContextObjectImpl::IsSame( indication->Context(), copyIndication->Context() ),
                "Incorrect externalize result!" );
            
        // Clean up
        CleanupStack::PopAndDestroy( copyIndication );
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( indication );
        CleanupStack::PopAndDestroy( context );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextObject class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextObject_NewLL(  )
    {
    CCFContextObject* context = CCFContextObject::NewL();
    CleanupStack::PushL( context );
    EUNIT_ASSERT_DESC( context, "CCFContextObject class instance not created!" );
    CleanupStack::PopAndDestroy( context );

    context = CCFContextObject::NewL(
        _L("Source"), _L("Type"), _L("Value") );
    CleanupStack::PushL( context );
    EUNIT_ASSERT_DESC( context, "CCFContextObject class instance not created!" );
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObject_NewLCL(  )
    {
    CCFContextObject* context = CCFContextObject::NewLC();
    EUNIT_ASSERT_DESC( context, "CCFContextObject class instance not created!" );
    CleanupStack::PopAndDestroy( context );

    context = CCFContextObject::NewLC(
        _L("Source"), _L("Type"), _L("Value") );
    EUNIT_ASSERT_DESC( context, "CCFContextObject class instance not created!" );
    CleanupStack::PopAndDestroy( context );
    }

//-----------------------------------------------------------------------------
// CCFContextObjectImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextObjectImpl_NewLL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewL();
    CleanupStack::PushL( context );
    
    EUNIT_ASSERT_DESC( context, "CCFContextObjectImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_NewLCL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    EUNIT_ASSERT_DESC( context, "CCFContextObjectImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetTypeLL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    EUNIT_ASSERT_DESC( context->Type() == KNullDesC,
        "Invalid context type!" );
        
    context->SetTypeL( KContextType );
    EUNIT_ASSERT_DESC( context->Type() == KContextType,
        "Invalid context type!" );
    
    context->SetTypeL( KContextTypeLong );
    EUNIT_ASSERT_DESC( context->Type() == KContextTypeLong,
        "Invalid context type!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetValueLL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    EUNIT_ASSERT_DESC( context->Value() == KNullDesC,
        "Invalid context value!" );
        
    context->SetValueL( KContextValue );
    EUNIT_ASSERT_DESC( context->Value() == KContextValue,
        "Invalid context value!" );
    
    context->SetValueL( KContextValueLong );
    EUNIT_ASSERT_DESC( context->Value() == KContextValueLong,
        "Invalid context value!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetSourceLL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    EUNIT_ASSERT_DESC( context->Source() == KNullDesC,
        "Invalid context source!" );
        
    context->SetSourceL( KContextSource );
    EUNIT_ASSERT_DESC( context->Source() == KContextSource,
        "Invalid context source!" );

    context->SetSourceL( KContextSourceLong );
    EUNIT_ASSERT_DESC( context->Source() == KContextSourceLong,
        "Invalid context source!" );

    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetConfidenceL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    EUNIT_ASSERT_DESC( context->Confidence() == 100,
        "Invalid confidence!" );
        
    context->SetConfidence( 50 );
    EUNIT_ASSERT_DESC( context->Confidence() == 50,
        "Invalid confidence!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetTimestampToHomeTimeL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    
    TTime now;
    now.HomeTime();
    
    User::After( 1000 );
    context->SetTimestampToHomeTime();
    User::After( 1000 );
    
    TTime after;
    after.HomeTime();
    
    const TTime& time = context->Timestamp();
    EUNIT_ASSERT_DESC( time > now, "Invalid home timestamp!" );
    EUNIT_ASSERT_DESC( time < after, "Invalid home timestamp!" );

    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_SetTimestampL(  )
    {
    CCFContextObjectImpl* context = CCFContextObjectImpl::NewLC();
    TTime now;
    now.HomeTime();
    
    EUNIT_ASSERT_DESC( now != context->Timestamp(), "Invalid time stamp!" );
    
    context->SetTimestamp( now );
    EUNIT_ASSERT_DESC( now == context->Timestamp(), "Invalid time stamp!" );
    
    User::After( 1000 );
    now.HomeTime();

    context->SetTimestamp( now.Int64() );
    EUNIT_ASSERT_DESC( now == context->Timestamp(), "Invalid time stamp!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_CopyLL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time
        );

    CCFContextObjectImpl* copyContext = CCFContextObjectImpl::NewLC();
    copyContext->CopyL( *context );
    
    EUNIT_ASSERT_DESC( context->Source() == copyContext->Source(),
        "Invalid source copy!" );
    EUNIT_ASSERT_DESC( context->Type() == copyContext->Type(),
        "Invalid type copy!" );
    EUNIT_ASSERT_DESC( context->Value() == copyContext->Value(),
        "Invalid value copy!" );
    EUNIT_ASSERT_DESC( context->Confidence() == copyContext->Confidence(),
        "Invalid confidence copy!" );
    EUNIT_ASSERT_DESC( context->Timestamp() == copyContext->Timestamp(),
        "Invalid timestamp copy!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( copyContext );
    CleanupStack::PopAndDestroy( context );
    }

void MT_CFServices::MT_CCFContextObjectImpl_ExternalizeLL(  )
    {
    static const TInt KConfidence = 50;
    static const TInt KCount = 3;
    TTime time;
    CCFContextObjectImpl* context = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 1:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KNullDesC,
                    KContextValue,
                    KConfidence,
                    time );
                break;
                }
            case 2:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KNullDesC,
                    KNullDesC,
                    KConfidence,
                    time );
                break;
                }
            case 0:
            default:
                {
                context = ConfigureContextObjectLC(
                    KContextSource,
                    KContextType,
                    KContextValue,
                    KConfidence,
                    time );
                break;
                }
            }
        time.HomeTime();
        
        // Externalize
        HBufC8* buffer = HBufC8::NewLC( context->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        context->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        CCFContextObjectImpl* copyContext = CCFContextObjectImpl::NewLC();
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        copyContext->InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );
        
        // Check results
        EUNIT_ASSERT_DESC( context->Source() == copyContext->Source(),
            "Invalid source copy!" );
        EUNIT_ASSERT_DESC( context->Type() == copyContext->Type(),
            "Invalid type copy!" );
        EUNIT_ASSERT_DESC( context->Value() == copyContext->Value(),
            "Invalid value copy!" );
        EUNIT_ASSERT_DESC( context->Confidence() == copyContext->Confidence(),
            "Invalid confidence copy!" );
        EUNIT_ASSERT_DESC( context->Timestamp() == copyContext->Timestamp(),
            "Invalid timestamp copy!" );
        
        // Clean up
        CleanupStack::PopAndDestroy( copyContext );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( context );
        }
    }

void MT_CFServices::MT_CCFContextObjectImpl_CompareValueL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context1 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context2 = ConfigureContextObjectLC(
        KNullDesC,
        KNullDesC,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context3 = ConfigureContextObjectLC(
        KNullDesC,
        KNullDesC,
        KContextValueLong,
        KConfidence,
        time );
        
    EUNIT_ASSERT_DESC( context1->CompareValue( *context2 ) == KErrNone,
        "Incorrect comparison result!" );
    EUNIT_ASSERT_DESC( context1->CompareValue( *context3 ) != KErrNone,
        "Incorrect comparison result!" );
        
    CleanupStack::PopAndDestroy( context3 );
    CleanupStack::PopAndDestroy( context2 );
    CleanupStack::PopAndDestroy( context1 );
    }

void MT_CFServices::MT_CCFContextObjectImpl_CompareByTypeL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context1 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context2 = ConfigureContextObjectLC(
        KNullDesC,
        KContextType,
        KNullDesC,
        KConfidence,
        time );
    CCFContextObjectImpl* context3 = ConfigureContextObjectLC(
        KNullDesC,
        KContextTypeLong,
        KContextValueLong,
        KConfidence,
        time );
        
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByType( *context1, *context2 ) == KErrNone,
            "Incorrect comparison result!" );
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByType( *context1, *context3 ) != KErrNone,
            "Incorrect comparison result!" );
        
    CleanupStack::PopAndDestroy( context3 );
    CleanupStack::PopAndDestroy( context2 );
    CleanupStack::PopAndDestroy( context1 );
    }

void MT_CFServices::MT_CCFContextObjectImpl_CompareByTypeAndSourceL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context1 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context2 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KNullDesC,
        KConfidence,
        time );
    CCFContextObjectImpl* context3 = ConfigureContextObjectLC(
        KNullDesC,
        KContextTypeLong,
        KContextValueLong,
        KConfidence,
        time );
        
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByTypeAndSource( *context1, *context2 ) == KErrNone,
            "Incorrect comparison result!" );
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByTypeAndSource( *context1, *context3 ) != KErrNone,
            "Incorrect comparison result!" );
        
    CleanupStack::PopAndDestroy( context3 );
    CleanupStack::PopAndDestroy( context2 );
    CleanupStack::PopAndDestroy( context1 );
    }

void MT_CFServices::MT_CCFContextObjectImpl_CompareByTimeDescendingL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context1 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context2 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KNullDesC,
        KConfidence,
        time );
    User::After( 1000 );
    time.HomeTime();
    CCFContextObjectImpl* context3 = ConfigureContextObjectLC(
        KNullDesC,
        KContextTypeLong,
        KContextValueLong,
        KConfidence,
        time );
        
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByTimeDescending( *context1, *context2 ) == 0,
            "Incorrect comparison result!" );
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByTypeAndSource( *context1, *context3 ) < 0,
            "Incorrect comparison result!" );
    EUNIT_ASSERT_DESC(
        CCFContextObjectImpl::CompareByTypeAndSource( *context3, *context2 ) > 0,
            "Incorrect comparison result!" );
        
    CleanupStack::PopAndDestroy( context3 );
    CleanupStack::PopAndDestroy( context2 );
    CleanupStack::PopAndDestroy( context1 );
    }

void MT_CFServices::MT_CCFContextObjectImpl_IsSameL(  )
    {
    static const TInt KConfidence = 50;
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* context1 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context2 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        KConfidence,
        time );
    CCFContextObjectImpl* context3 = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KNullDesC,
        KConfidence,
        time );
    
    EUNIT_ASSERT_DESC( CCFContextObjectImpl::IsSame( *context1, *context2 ),
        "Contexts are not same!" );
    EUNIT_ASSERT_DESC( !CCFContextObjectImpl::IsSame( *context1, *context3 ),
        "Contexts are same!" );
        
    CleanupStack::PopAndDestroy( context3 );
    CleanupStack::PopAndDestroy( context2 );
    CleanupStack::PopAndDestroy( context1 );
    }

//-----------------------------------------------------------------------------
// CCFContextQuery class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextQuery_NewLL(  )
    {
    CCFContextQuery* query = CCFContextQuery::NewL();
    CleanupStack::PushL( query );
    EUNIT_ASSERT_DESC( query, "CCFContextQuery class instance not created!" );
    CleanupStack::PopAndDestroy( query );

    query = CCFContextQuery::NewL( KContextSource, KContextType );
    CleanupStack::PushL( query );
    EUNIT_ASSERT_DESC( query, "CCFContextQuery class instance not created!" );
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQuery_NewLCL(  )
    {
    CCFContextQuery* query = CCFContextQuery::NewLC();
    EUNIT_ASSERT_DESC( query, "CCFContextQuery class instance not created!" );
    CleanupStack::PopAndDestroy( query );

    query = CCFContextQuery::NewLC( KContextSource, KContextType );
    EUNIT_ASSERT_DESC( query, "CCFContextQuery class instance not created!" );
    CleanupStack::PopAndDestroy( query );
    }

//-----------------------------------------------------------------------------
// CCFContextQueryImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextQueryImpl_NewLL(  )
    {
    CCFContextQueryImpl* query = CCFContextQueryImpl::NewL( KNullDesC, KNullDesC );
    CleanupStack::PushL( query );
    
    EUNIT_ASSERT_DESC( query, "CCFContextQueryImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_NewLCL(  )
    {
    CCFContextQueryImpl* query = CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
    
    EUNIT_ASSERT_DESC( query, "CCFContextQueryImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_SetTypeLL(  )
    {
    CCFContextQueryImpl* query = CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
    
    EUNIT_ASSERT_DESC( query->Type() == KNullDesC,
        "Incorrect query type!" );
    
    query->SetTypeL( KContextType );
    EUNIT_ASSERT_DESC( query->Type() == KContextType,
        "Incorrect query type!" );

    query->SetTypeL( KContextTypeLong );
    EUNIT_ASSERT_DESC( query->Type() == KContextTypeLong,
        "Incorrect query type!" );
    
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_SetSourceLL(  )
    {
    CCFContextQueryImpl* query = CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
    
    EUNIT_ASSERT_DESC( query->Source() == KNullDesC,
        "Incorrect query source!" );
    
    query->SetSourceL( KContextSource );
    EUNIT_ASSERT_DESC( query->Source() == KContextSource,
        "Incorrect query source!" );
    
    query->SetSourceL( KContextSourceLong );
    EUNIT_ASSERT_DESC( query->Source() == KContextSourceLong,
        "Incorrect query source!" );
    
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_SetSubTypeMatchL(  )
    {
    CCFContextQueryImpl* query = CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
    
    EUNIT_ASSERT_DESC( !query->SubTypeMatch(),
        "Incorrect query sub type match!" );
        
    query->SetSubTypeMatch( ETrue );
    EUNIT_ASSERT_DESC( query->SubTypeMatch(),
        "Incorrect query sub type match!" );
    
    query->SetSubTypeMatch( EFalse );
    EUNIT_ASSERT_DESC( !query->SubTypeMatch(),
        "Incorrect query sub type match!" );
    
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_ExternalizeLL(  )
    {
    static const TInt KCount = 3;
    CCFContextQueryImpl* query = NULL;
    CCFContextQueryImpl* copyQuery = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                query = CCFContextQueryImpl::NewLC( KContextSource, KNullDesC );
                break;
                }
            case 1:
                {
                query = CCFContextQueryImpl::NewLC( KNullDesC, KContextType );
                break;
                }
            case 2:
            default:
                {
                query = CCFContextQueryImpl::NewLC( KContextSource, KContextType );
                break;
                }
            }
        
        // Externalize
        HBufC8* buffer = HBufC8::NewLC( query->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        query->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        copyQuery = CCFContextQueryImpl::NewLC( KNullDesC, KNullDesC );
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        copyQuery->InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );
        
        // Check results
        EUNIT_ASSERT_DESC( CCFContextQueryImpl::IsSame( *query, *copyQuery ),
            "Query externalization failed!" );
        
        // Clean up
        CleanupStack::PopAndDestroy( copyQuery );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( query );
        }
    }

void MT_CFServices::MT_CCFContextQueryImpl_MatchesL(  )
    {
    CCFContextQueryImpl* query =
        CCFContextQueryImpl::NewLC( KContextSource, KContextType );
        
    EUNIT_ASSERT_DESC( query->Matches( KContextType, KContextSource ),
        "Invalid query match!" );
    EUNIT_ASSERT_DESC( !query->Matches( KContextType, KNullDesC ),
        "Invalid query match!" );
    EUNIT_ASSERT_DESC( !query->Matches( KNullDesC, KContextSource ),
        "Invalid query match!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_MatchesQueryL(  )
    {
    CCFContextQueryImpl* query =
        CCFContextQueryImpl::NewLC( KContextSource, KContextType );
    query->SetSubTypeMatch( ETrue );
        
    EUNIT_ASSERT_DESC(
        CCFContextQueryImpl::MatchesQuery( *query, KContextType, KContextSource, EFalse ),
            "Invalid query match!" );
    EUNIT_ASSERT_DESC(
        CCFContextQueryImpl::MatchesQuery( *query, KContextTypeLong, KContextSource, ETrue ),
            "Invalid query match!" );
    EUNIT_ASSERT_DESC(
        !CCFContextQueryImpl::MatchesQuery( *query, KNullDesC, KContextSource, ETrue ),
            "Invalid query match!" );
    EUNIT_ASSERT_DESC(
        !CCFContextQueryImpl::MatchesQuery( *query, KContextType, KNullDesC, ETrue ),
            "Invalid query match!" );

    // Clean up
    CleanupStack::PopAndDestroy( query );
    }

void MT_CFServices::MT_CCFContextQueryImpl_IsSameL(  )
    {
    CCFContextQueryImpl* query1 =
        CCFContextQueryImpl::NewLC( KContextSource, KContextType );
    CCFContextQueryImpl* query2 =
        CCFContextQueryImpl::NewLC( KContextSource, KContextType );
    CCFContextQueryImpl* query3 =
        CCFContextQueryImpl::NewLC( KContextSource, KContextTypeLong );
        
    EUNIT_ASSERT_DESC(
        CCFContextQueryImpl::IsSame( *query1, *query2 ),
            "Invalid query match!" );
    EUNIT_ASSERT_DESC(
        !CCFContextQueryImpl::IsSame( *query1, *query3 ),
            "Invalid query match!" );
        
    // Clean up
    CleanupStack::PopAndDestroy( query3 );
    CleanupStack::PopAndDestroy( query2 );
    CleanupStack::PopAndDestroy( query1 );
    }

//-----------------------------------------------------------------------------
// CCFContextSubscription class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextSubscription_NewLL(  )
    {
    CCFContextSubscription* subscription = CCFContextSubscription::NewL();
    CleanupStack::PushL( subscription );
    
    EUNIT_ASSERT_DESC( subscription, "CCFContextSubscription class instance not created!" );
    
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscription_NewLCL(  )
    {
    CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
    
    EUNIT_ASSERT_DESC( subscription, "CCFContextSubscription class instance not created!" );
    
    CleanupStack::PopAndDestroy( subscription );
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextSubscriptionImpl_NewLL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewL();
    CleanupStack::PushL( subscription );
    
    EUNIT_ASSERT_DESC( subscription, "CCFContextSubscriptionImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_NewLCL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( subscription, "CCFContextSubscriptionImpl class instance not created!" );
    
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetContextTypeLL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( subscription->ContextType() == KNullDesC,
        "Invalid contex type in subscription!" );
        
    subscription->SetContextTypeL( KContextType );
    EUNIT_ASSERT_DESC( subscription->ContextType() == KContextType,
        "Invalid contex type in subscription!" );
    
    subscription->SetContextTypeL( KContextTypeLong );
    EUNIT_ASSERT_DESC( subscription->ContextType() == KContextTypeLong,
        "Invalid contex type in subscription!" );
    
    subscription->SetContextTypeL( KContextType );
    EUNIT_ASSERT_DESC( subscription->ContextType() == KContextType,
        "Invalid contex type in subscription!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetContextSourceLL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( subscription->ContextSource() == KNullDesC,
        "Invalid context source in subscription!" );
        
    subscription->SetContextSourceL( KContextSource );
    EUNIT_ASSERT_DESC( subscription->ContextSource() == KContextSource,
        "Invalid context source in subscription!" );

    subscription->SetContextSourceL( KContextSourceLong );
    EUNIT_ASSERT_DESC( subscription->ContextSource() == KContextSourceLong,
        "Invalid context source in subscription!" );

    subscription->SetContextSourceL( KContextSource );
    EUNIT_ASSERT_DESC( subscription->ContextSource() == KContextSource,
        "Invalid context source in subscription!" );

    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetSubTypeMatchL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( !subscription->SubTypeMatch(),
        "Invalid sub type match in subscription!" );
        
    subscription->SetSubTypeMatch( ETrue );
    EUNIT_ASSERT_DESC( subscription->SubTypeMatch(),
        "Invalid sub type match in subscription!" );
    
    subscription->SetSubTypeMatch( EFalse );
    EUNIT_ASSERT_DESC( !subscription->SubTypeMatch(),
        "Invalid sub type match in subscription!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetOnlyChangesIndicatedL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( !subscription->OnlyChangesIndicated(),
        "Invalid only changes indicated flag in subscription!" );
    
    subscription->SetOnlyChangesIndicated( ETrue );
    EUNIT_ASSERT_DESC( subscription->OnlyChangesIndicated(),
        "Invalid only changes indicated flag in subscription!" );
    
    subscription->SetOnlyChangesIndicated( EFalse );
    EUNIT_ASSERT_DESC( !subscription->OnlyChangesIndicated(),
        "Invalid only changes indicated flag in subscription!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetDataObjectEnabledL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( !subscription->DataObjectEnabled(),
        "Incorrect DataObjectEnabled value!" );
    
    subscription->SetDataObjectEnabled( ETrue );
    EUNIT_ASSERT_DESC( subscription->DataObjectEnabled(),
        "Incorrect DataObjectEnabled value!" );
    
    subscription->SetDataObjectEnabled( EFalse );
    EUNIT_ASSERT_DESC( !subscription->DataObjectEnabled(),
        "Incorrect DataObjectEnabled value!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_ExternalizeLL(  )
    {
    static const TInt KCount = 4;
    CCFContextSubscriptionImpl* subscription = NULL;
    CCFContextSubscriptionImpl* copySubscription = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                subscription = ConfigureContextSubscriptionLC(
                    KContextSource,
                    KContextType,
                    ETrue,
                    ETrue );
                subscription->SetDataObjectEnabled( ETrue );
                break;
                }
            case 1:
                {
                subscription = ConfigureContextSubscriptionLC(
                    KNullDesC,
                    KContextType,
                    EFalse,
                    ETrue );
                break;
                }
            case 2:
                {
                subscription = ConfigureContextSubscriptionLC(
                    KNullDesC,
                    KNullDesC,
                    EFalse,
                    ETrue );
                break;
                }
            case 3:
            default:
                {
                subscription = ConfigureContextSubscriptionLC(
                    KContextSource,
                    KNullDesC,
                    EFalse,
                    EFalse );
                break;
                }
            }
        // Externalize
        HBufC8* buffer = HBufC8::NewLC( subscription->Size() );
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        subscription->ExternalizeL( writeStream );
        CleanupStack::PopAndDestroy( &writeStream );
        
        // Internalize
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        copySubscription = CCFContextSubscriptionImpl::NewLC( readStream );
        
        // Check results
        EUNIT_ASSERT_DESC( subscription->ContextSource() == copySubscription->ContextSource(),
            "Invalid context source on subscription!" );
        EUNIT_ASSERT_DESC( subscription->ContextType() == copySubscription->ContextType(),
            "Invalid context type on subscription!" );
        EUNIT_ASSERT_DESC( subscription->SubTypeMatch() == copySubscription->SubTypeMatch(),
            "Invalid sub type match on subscription!" );
        EUNIT_ASSERT_DESC( subscription->OnlyChangesIndicated() == copySubscription->OnlyChangesIndicated(),
            "Invalid only changes indicated flag on subscription!" );
        EUNIT_ASSERT_DESC( subscription->DataObjectEnabled() == copySubscription->DataObjectEnabled(),
            "Invalid only changes indicated flag on subscription!" );
        
        // Clean up
        CleanupStack::PopAndDestroy( copySubscription );
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
        CleanupStack::PopAndDestroy( subscription );
        }
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_SetSubscriptionListenerL(  )
    {
    CCFContextSubscriptionImpl* subscription = CCFContextSubscriptionImpl::NewLC();
    
    EUNIT_ASSERT_DESC( !( &subscription->SubscriptionListener() ),
        "Invalid subscription listener!" );
        
    subscription->SetSubscriptionListener( *this );
    MCFContextSubscriptionListener* listener = this;
    EUNIT_ASSERT_DESC( &subscription->SubscriptionListener() == listener,
        "Invalid subscription listener!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_EvaluateContextChangeLL(  )
    {
    TTime time;
    time.HomeTime();
    CCFContextObjectImpl* newContext = ConfigureContextObjectLC(
        KContextSource,
        KContextType,
        KContextValue,
        100,
        time );
    CCFContextSubscriptionImpl* subscription = ConfigureContextSubscriptionLC(
        KContextSource,
        KContextType,
        EFalse,
        EFalse );
    subscription->SetSubscriptionListener( *this );
       
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    subscription->EvaluateContextChangeL( *newContext, *newContext, EFalse );
    EUNIT_ASSERT_DESC( iIndicatedContext, "Context change not indicated!" );
    EUNIT_ASSERT_DESC( iIndicatedContext->Source() == newContext->Source(),
        "Context source incorrect!" );
    EUNIT_ASSERT_DESC( iIndicatedContext->Type() == newContext->Type(),
        "Context type incorrect!" );
    EUNIT_ASSERT_DESC( iIndicatedContext->Value() == newContext->Value(),
        "Context value incorrect!" );
        
    subscription->SetOnlyChangesIndicated( ETrue );
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    subscription->EvaluateContextChangeL( *newContext, *newContext, EFalse );
    EUNIT_ASSERT_DESC( !iIndicatedContext, "Context change not indicated!" );
    subscription->EvaluateContextChangeL( *newContext, *newContext, ETrue );
    EUNIT_ASSERT_DESC( iIndicatedContext, "Context change not indicated!" );
    
    subscription->SetSubTypeMatch( ETrue );
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    subscription->EvaluateContextChangeL( *newContext, *newContext, ETrue );
    EUNIT_ASSERT_DESC( iIndicatedContext, "Context change not indicated!" );
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    newContext->SetTypeL( KContextTypeLong );
    subscription->EvaluateContextChangeL( *newContext, *newContext, ETrue );
    EUNIT_ASSERT_DESC( iIndicatedContext, "Context change not indicated!" );
    delete iIndicatedContext;
    iIndicatedContext = NULL;
    
    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    CleanupStack::PopAndDestroy( newContext );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_IsSameL(  )
    {
    CCFContextSubscriptionImpl* subscription1 = ConfigureContextSubscriptionLC(
        KContextSource,
        KContextType,
        EFalse,
        EFalse );
    CCFContextSubscriptionImpl* subscription2 = ConfigureContextSubscriptionLC(
        KContextSource,
        KContextType,
        EFalse,
        EFalse );
    CCFContextSubscriptionImpl* subscription3 = ConfigureContextSubscriptionLC(
        KContextSource,
        KContextTypeLong,
        EFalse,
        EFalse );
        
    EUNIT_ASSERT_DESC( subscription1->IsSame( *subscription2 ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( !subscription1->IsSame( *subscription3 ),
        "Invalid subscription match!" );
        
    // Clean up
    CleanupStack::PopAndDestroy( subscription3 );
    CleanupStack::PopAndDestroy( subscription2 );
    CleanupStack::PopAndDestroy( subscription1 );
    }

void MT_CFServices::MT_CCFContextSubscriptionImpl_MatchesL(  )
    {
    CCFContextSubscriptionImpl* subscription = ConfigureContextSubscriptionLC(
        KContextSource,
        KContextType,
        EFalse,
        EFalse );
        
    EUNIT_ASSERT_DESC( subscription->Matches( KContextType ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( !subscription->Matches( KContextTypeLong ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( !subscription->Matches( KNullDesC ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( subscription->Matches( KContextType, KContextSource ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( !subscription->Matches( KContextType, KContextSourceLong ),
        "Invalid subscription match!" );
    EUNIT_ASSERT_DESC( !subscription->Matches( KContextType, KNullDesC ),
        "Invalid subscription match!" );

    // Clean up
    CleanupStack::PopAndDestroy( subscription );
    }

//-----------------------------------------------------------------------------
// CCFKeyValuePair class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFKeyValuePair_NewLL(  )
    {
    CCFKeyValuePair* instance = CCFKeyValuePair::NewL( KKey, KValue );
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFKeyValuePair class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFKeyValuePair_NewLCL(  )
    {
    CCFKeyValuePair* instance = CCFKeyValuePair::NewLC( KKey, KValue );
    
    EUNIT_ASSERT_DESC( instance, "CCFKeyValuePair class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFKeyValuePair_NewL_1L(  )
    {
    HBufC* key = KKey().AllocLC();
    HBufC* value = KValue().AllocLC();
    CCFKeyValuePair* instance = CCFKeyValuePair::NewL( key, value );
    CleanupStack::PushL( instance );
    
    EUNIT_ASSERT_DESC( instance, "CCFKeyValuePair class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    CleanupStack::Pop( value );
    CleanupStack::Pop( key );
    }

void MT_CFServices::MT_CCFKeyValuePair_NewLC_1L(  )
    {
    HBufC* key = KKey().AllocLC();
    HBufC* value = KValue().AllocLC();
    CCFKeyValuePair* instance = CCFKeyValuePair::NewLC( key, value );
    
    EUNIT_ASSERT_DESC( instance, "CCFKeyValuePair class instance not created!" );
    
    CleanupStack::PopAndDestroy( instance );
    CleanupStack::Pop( value );
    CleanupStack::Pop( key );
    }

void MT_CFServices::MT_CCFKeyValuePair_KeyL(  )
    {
    CCFKeyValuePair* instance = CCFKeyValuePair::NewLC( KKey, KValue );
    
    EUNIT_ASSERT_DESC( instance->Key() == KKey, "Invalid key!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( instance );
    }

void MT_CFServices::MT_CCFKeyValuePair_ValueL(  )
    {
    CCFKeyValuePair* instance = CCFKeyValuePair::NewLC( KKey, KValue );
    
    EUNIT_ASSERT_DESC( instance->Value() == KValue, "Invalid value!" );
    
    // Clean up
    CleanupStack::PopAndDestroy( instance );
    }

//-----------------------------------------------------------------------------
// CFServiceMTils class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CFServiceMTils_UpdateBufferLL(  )
    {
    HBufC* buffer = KNullDesC().AllocL();
    
    CFServiceUtils::UpdateBufferL( buffer, KContextSource );
    EUNIT_ASSERT_DESC( buffer, "Buffer not created!" );
    EUNIT_ASSERT_DESC( *buffer == KContextSource, "Incorrect buffer value!" );

    CFServiceUtils::UpdateBufferL( buffer, KContextSourceLong );
    EUNIT_ASSERT_DESC( buffer, "Buffer not created!" );
    EUNIT_ASSERT_DESC( *buffer == KContextSourceLong, "Incorrect buffer value!" );

    CFServiceUtils::UpdateBufferL( buffer, KContextSource );
    EUNIT_ASSERT_DESC( buffer, "Buffer not created!" );
    EUNIT_ASSERT_DESC( *buffer == KContextSource, "Incorrect buffer value!" );
    
    // Clean up
    delete buffer;
    }

void MT_CFServices::MT_CFServiceMTils_WriteIntoStreamLL(  )
    {
    static const TInt KCount = 2;
    HBufC8* buffer = NULL;
    HBufC* desc = NULL;
    for( TInt i = 0; i < KCount; i++ )
        {
        switch( i )
            {
            case 0:
                {
                buffer = HBufC8::NewLC( KContextSource().Size() );
                desc = KContextSource().AllocLC();
                break;
                }
            case 1:
            default:
                {
                buffer = HBufC8::NewLC( KContextSource().Size() );
                break;
                }
            }
        // Write stream
        TPtr8 bufferPtr = buffer->Des();
        RDesWriteStream writeStream( bufferPtr );
        writeStream.PushL();
        CFServiceUtils::WriteIntoStreamL( desc, writeStream );
        writeStream.CommitL();
        CleanupStack::PopAndDestroy( &writeStream );
        if( desc )
            {
            CleanupStack::PopAndDestroy( desc );
            desc = NULL;
            }
        
        // Read stream
        RDesReadStream readStream( bufferPtr );
        readStream.PushL();
        CFServiceUtils::ReadFromStreamL( desc, readStream );
        CleanupStack::PopAndDestroy( &readStream );
        if( desc )
            {
            CleanupStack::PushL( desc );
            }
        
        // Check results
        switch( i )
            {
            case 0:
                {
                EUNIT_ASSERT_DESC( desc, "Buffer not created!" );
                EUNIT_ASSERT_DESC( *desc == KContextSource, "Invalid buffer value!" );
                break;
                }
            case 1:
            default:
                {
                EUNIT_ASSERT_DESC( !desc, "Buffer created!" );
                break;
                }
            }
        
        // Clean up
        if( desc )
            {
            CleanupStack::PopAndDestroy( desc );
            desc = NULL;
            }
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;
        }
    }

//-----------------------------------------------------------------------------
// CCFContextDataProxy class unit test cases
//-----------------------------------------------------------------------------
//
void MT_CFServices::MT_CCFContextDataProxy_NewLL(  )
    {
    CCFContextDataProxy* obj = CCFContextDataProxy::NewL();
    CleanupStack::PushL( obj );
    EUNIT_ASSERT_DESC( obj, "CCFContextDataProxy instance not created!" );
    CleanupStack::PopAndDestroy( obj );
    }

void MT_CFServices::MT_CCFContextDataProxy_NewLCL(  )
    {
    CCFContextDataProxy* obj = CCFContextDataProxy::NewLC();
    EUNIT_ASSERT_DESC( obj, "CCFContextDataProxy instance not created!" );
    CleanupStack::PopAndDestroy( obj );
    }

void MT_CFServices::MT_CCFContextDataProxy_SetContextDataUidL(  )
    {
    const TUid KUid1 = { 0x01234567 };
    const TUid KUid2 = { 0x98765432 };
    CCFContextDataProxy* obj = CCFContextDataProxy::NewLC();
    CCFContextDataObject* objBase = static_cast<CCFContextDataObject*>( obj );
       
    obj->SetContextDataUid( KUid1 );
    EUNIT_ASSERT_DESC( objBase->Uid() == KUid1, "Invalid context data object UID!" );

    obj->SetContextDataUid( KUid2 );
    EUNIT_ASSERT_DESC( objBase->Uid() == KUid2, "Invalid context data object UID!" );

    // Clean up
    CleanupStack::PopAndDestroy( obj );
    }

void MT_CFServices::MT_CCFContextDataProxy_SetContextDataL(  )
    {
    const TInt len1 = 128;
    const TInt len2 = 512;
    const TInt count = 2;
    CCFContextDataProxy* obj = CCFContextDataProxy::NewLC();
    CCFContextDataObject* objBase = static_cast<CCFContextDataObject*>( obj );

    TInt length = 0;
    for( TInt i = 0; i < count; i++ )
        {
        switch( i )
            {
            case 0:
                {
                length = len1;
                break;
                }
            case 1:
                {
                length = len2;
                break;
                }
            default:
                {
                break;
                }
            }
            
        HBufC8* data = HBufC8::NewLC( length );
        TPtr8 dataPtr( data->Des() );
        dataPtr.Fill( 'z', length );
        obj->SetContextData( data );
        EUNIT_ASSERT_DESC( objBase->Size() == data->Size(),
            "Invalid context object data!" );
        CleanupStack::Pop( data );
        }

    // Clean up
    CleanupStack::PopAndDestroy( obj );
    }

void MT_CFServices::MT_CCFContextDataProxy_ExternalizeLL(  )
    {
    _LIT8( KData, "Some_Data" );
    CCFContextDataProxy* obj = CCFContextDataProxy::NewLC();
    CCFContextDataObject* objBase = static_cast<CCFContextDataObject*>( obj );
    
    // First set data in data object
    HBufC8* data = KData().AllocLC();
    obj->SetContextData( data );
    CleanupStack::Pop( data );
    data = NULL;
    
    // Externalize object
    data = HBufC8::NewLC( objBase->Size() );
    TPtr8 dataPtr( data->Des() );
    RDesWriteStream writeStream( dataPtr );
    writeStream.PushL();
    objBase->ExternalizeL( writeStream );
    CleanupStack::PopAndDestroy( &writeStream );
    
    // Internalize object
    CCFContextDataProxy* newObj = CCFContextDataProxy::NewLC();
    CCFContextDataObject* newObjBase = static_cast<CCFContextDataObject*>( obj );
    RDesReadStream readStream( dataPtr );
    readStream.PushL();
    newObjBase->InternalizeL( readStream );
    CleanupStack::PopAndDestroy( &readStream );
    
    // Check
    EUNIT_ASSERT_DESC( newObjBase->Size() == objBase->Size(),
        "Invalid object size!" );

    // Clean up
    CleanupStack::PopAndDestroy( newObj );
    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( obj );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    MT_CFServices,
    "CFServices unit test.",
    "UNIT" )

// CCFActionIndication class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFActionIndication",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionIndication_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionIndication",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionIndication_NewLCL, Teardown)

// CCFActionIndicationImpl class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFActionIndicationImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionIndicationImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionIndicationImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionIndicationImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetIdentifierL - test ",
    "CCFActionIndicationImpl",
    "SetIdentifierL",
    "FUNCTIONALITY",
    SetupActionIndicationL, MT_CCFActionIndicationImpl_SetIdentifierLL, Teardown)

EUNIT_TEST(
    "AddParameterL - test ",
    "CCFActionIndicationImpl",
    "AddParameterL",
    "FUNCTIONALITY",
    SetupActionIndicationL, MT_CCFActionIndicationImpl_AddParameterLL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFActionIndicationImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupActionIndicationL, MT_CCFActionIndicationImpl_ExternalizeLL, Teardown)

// CCFActionSubscription class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFActionSubscription",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscription_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionSubscription",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscription_NewLCL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFActionSubscriptionImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscriptionImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFActionSubscriptionImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscriptionImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetActionIdentifierL - test ",
    "CCFActionSubscriptionImpl",
    "SetActionIdentifierL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscriptionImpl_SetActionIdentifierLL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFActionSubscriptionImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscriptionImpl_ExternalizeLL, Teardown)

EUNIT_TEST(
    "SetListener - test ",
    "CCFActionSubscriptionImpl",
    "SetListener",
    "FUNCTIONALITY",
    SetupL, MT_CCFActionSubscriptionImpl_SetListenerL, Teardown)

// CCFContextIndication class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextIndication",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndication_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextIndication",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndication_NewLCL, Teardown)

// CCFContextIndicationImpl class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextIndicationImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndicationImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextIndicationImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndicationImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetContextL - test ",
    "CCFContextIndicationImpl",
    "SetContextL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndicationImpl_SetContextLL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFContextIndicationImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextIndicationImpl_ExternalizeLL, Teardown)

// CCFContextObject class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextObject",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObject_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextObject",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObject_NewLCL, Teardown)

// CCFContextObjectImpl class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextObjectImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextObjectImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetTypeL - test ",
    "CCFContextObjectImpl",
    "SetTypeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetTypeLL, Teardown)

EUNIT_TEST(
    "SetValueL - test ",
    "CCFContextObjectImpl",
    "SetValueL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetValueLL, Teardown)

EUNIT_TEST(
    "SetSourceL - test ",
    "CCFContextObjectImpl",
    "SetSourceL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetSourceLL, Teardown)

EUNIT_TEST(
    "SetConfidence - test ",
    "CCFContextObjectImpl",
    "SetConfidence",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetConfidenceL, Teardown)

EUNIT_TEST(
    "SetTimestampToHomeTime - test ",
    "CCFContextObjectImpl",
    "SetTimestampToHomeTime",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetTimestampToHomeTimeL, Teardown)

EUNIT_TEST(
    "SetTimestamp - test ",
    "CCFContextObjectImpl",
    "SetTimestamp",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_SetTimestampL, Teardown)

EUNIT_TEST(
    "CopyL - test ",
    "CCFContextObjectImpl",
    "CopyL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_CopyLL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFContextObjectImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_ExternalizeLL, Teardown)

EUNIT_TEST(
    "CompareValue - test ",
    "CCFContextObjectImpl",
    "CompareValue",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_CompareValueL, Teardown)

EUNIT_TEST(
    "CompareByType - test ",
    "CCFContextObjectImpl",
    "CompareByType",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_CompareByTypeL, Teardown)

EUNIT_TEST(
    "CompareByTypeAndSource - test ",
    "CCFContextObjectImpl",
    "CompareByTypeAndSource",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_CompareByTypeAndSourceL, Teardown)

EUNIT_TEST(
    "CompareByTimeDescending - test ",
    "CCFContextObjectImpl",
    "CompareByTimeDescending",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_CompareByTimeDescendingL, Teardown)

EUNIT_TEST(
    "IsSame - test ",
    "CCFContextObjectImpl",
    "IsSame",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextObjectImpl_IsSameL, Teardown)

// CCFContextQuery class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextQuery",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQuery_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextQuery",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQuery_NewLCL, Teardown)

// CCFContextQueryImpl class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextQueryImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextQueryImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetTypeL - test ",
    "CCFContextQueryImpl",
    "SetTypeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_SetTypeLL, Teardown)

EUNIT_TEST(
    "SetSourceL - test ",
    "CCFContextQueryImpl",
    "SetSourceL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_SetSourceLL, Teardown)

EUNIT_TEST(
    "SetSubTypeMatch - test ",
    "CCFContextQueryImpl",
    "SetSubTypeMatch",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_SetSubTypeMatchL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFContextQueryImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_ExternalizeLL, Teardown)

EUNIT_TEST(
    "Matches - test ",
    "CCFContextQueryImpl",
    "Matches",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_MatchesL, Teardown)

EUNIT_TEST(
    "MatchesQuery - test ",
    "CCFContextQueryImpl",
    "MatchesQuery",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_MatchesQueryL, Teardown)

EUNIT_TEST(
    "IsSame - test ",
    "CCFContextQueryImpl",
    "IsSame",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextQueryImpl_IsSameL, Teardown)

// CCFContextSubscription class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextSubscription",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscription_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSubscription",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscription_NewLCL, Teardown)

// CCFContextSubscriptionImpl class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextSubscriptionImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextSubscriptionImpl",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_NewLCL, Teardown)

EUNIT_TEST(
    "SetContextTypeL - test ",
    "CCFContextSubscriptionImpl",
    "SetContextTypeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetContextTypeLL, Teardown)

EUNIT_TEST(
    "SetContextSourceL - test ",
    "CCFContextSubscriptionImpl",
    "SetContextSourceL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetContextSourceLL, Teardown)

EUNIT_TEST(
    "SetSubTypeMatch - test ",
    "CCFContextSubscriptionImpl",
    "SetSubTypeMatch",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetSubTypeMatchL, Teardown)

EUNIT_TEST(
    "SetOnlyChangesIndicated - test ",
    "CCFContextSubscriptionImpl",
    "SetOnlyChangesIndicated",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetOnlyChangesIndicatedL, Teardown)

EUNIT_TEST(
    "SetDataObjectEnabled - test ",
    "CCFContextSubscriptionImpl",
    "SetDataObjectEnabled",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetDataObjectEnabledL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFContextSubscriptionImpl",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_ExternalizeLL, Teardown)

EUNIT_TEST(
    "SetSubscriptionListener - test ",
    "CCFContextSubscriptionImpl",
    "SetSubscriptionListener",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_SetSubscriptionListenerL, Teardown)

EUNIT_TEST(
    "EvaluateContextChangeL - test ",
    "CCFContextSubscriptionImpl",
    "EvaluateContextChangeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_EvaluateContextChangeLL, Teardown)

EUNIT_TEST(
    "IsSame - test ",
    "CCFContextSubscriptionImpl",
    "IsSame",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_IsSameL, Teardown)

EUNIT_TEST(
    "Matches - test ",
    "CCFContextSubscriptionImpl",
    "Matches",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextSubscriptionImpl_MatchesL, Teardown)

// CCFKeyValuePair class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFKeyValuePair",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFKeyValuePair",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_NewLCL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CCFKeyValuePair",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_NewL_1L, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFKeyValuePair",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_NewLC_1L, Teardown)

EUNIT_TEST(
    "Key - test ",
    "CCFKeyValuePair",
    "Key",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_KeyL, Teardown)

EUNIT_TEST(
    "Value - test ",
    "CCFKeyValuePair",
    "Value",
    "FUNCTIONALITY",
    SetupL, MT_CCFKeyValuePair_ValueL, Teardown)

// CFServiceMTils class test cases
//
EUNIT_TEST(
    "UpdateBufferL - test ",
    "CFServiceMTils",
    "UpdateBufferL",
    "FUNCTIONALITY",
    SetupL, MT_CFServiceMTils_UpdateBufferLL, Teardown)

EUNIT_TEST(
    "WriteIntoStreamL - test ",
    "CFServiceMTils",
    "WriteIntoStreamL",
    "FUNCTIONALITY",
    SetupL, MT_CFServiceMTils_WriteIntoStreamLL, Teardown)

// CCFContextDataProxy class test cases
//
EUNIT_TEST(
    "NewL - test ",
    "CCFContextDataProxy",
    "NewL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextDataProxy_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CCFContextDataProxy",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextDataProxy_NewLCL, Teardown)

EUNIT_TEST(
    "SetContextDataUid - test ",
    "CCFContextDataProxy",
    "SetContextDataUid",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextDataProxy_SetContextDataUidL, Teardown)

EUNIT_TEST(
    "SetContextData - test ",
    "CCFContextDataProxy",
    "SetContextData",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextDataProxy_SetContextDataL, Teardown)

EUNIT_TEST(
    "ExternalizeL - test ",
    "CCFContextDataProxy",
    "ExternalizeL",
    "FUNCTIONALITY",
    SetupL, MT_CCFContextDataProxy_ExternalizeLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
