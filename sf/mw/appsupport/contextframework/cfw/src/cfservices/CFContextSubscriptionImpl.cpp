/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextSubscriptionImpl class implementation.
*
*/


#include <s32strm.h>
#include <s32mem.h>
#include <cfcontextdataobject.h>

#include "CFContextSubscriptionImpl.h"
#include "CFContextQueryImpl.h"
#include "cfcontextsubscriptionlistener.h"
#include "CFContextIndicationImpl.h"
#include "cfcontextobject.h"
#include "cftrace.h"
#include "CFContextDataProxy.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CONTEXTSUBS" );

// Panic reasons
enum TPanicReason
    {
    ENoListenerSet
    };
    
// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

// MEMBER FUNCTIONS

EXPORT_C CCFContextSubscriptionImpl* CCFContextSubscriptionImpl::NewL()
    {
    FUNC_LOG;
    
    CCFContextSubscriptionImpl* self = CCFContextSubscriptionImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextSubscriptionImpl* CCFContextSubscriptionImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFContextSubscriptionImpl* self = new( ELeave ) CCFContextSubscriptionImpl;
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

EXPORT_C CCFContextSubscriptionImpl* CCFContextSubscriptionImpl::NewL(
	RReadStream& aStream)
    {
    FUNC_LOG;
    
    CCFContextSubscriptionImpl* self =
        CCFContextSubscriptionImpl::NewLC( aStream );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextSubscriptionImpl* CCFContextSubscriptionImpl::NewLC(
	RReadStream& aStream )
    {
    FUNC_LOG;
    
    CCFContextSubscriptionImpl* self = new( ELeave ) CCFContextSubscriptionImpl;
	CleanupStack::PushL( self );
    self->ConstructL( aStream );

    return self;
    }

CCFContextSubscriptionImpl::~CCFContextSubscriptionImpl()
    {
    FUNC_LOG;
    
    delete iContextQuery;
    }

CCFContextSubscriptionImpl::CCFContextSubscriptionImpl()
    {
    FUNC_LOG;
    }
    
void CCFContextSubscriptionImpl::ConstructL()
    {
    FUNC_LOG;
    
    iContextQuery = CCFContextQueryImpl::NewL( KNullDesC, KNullDesC );
    }
    
void CCFContextSubscriptionImpl::ConstructL( RReadStream& aStream )
    {
    FUNC_LOG;

    ConstructL();
    InternalizeL( aStream );    
    }
    
// METHODS

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetContextTypeL
//-----------------------------------------------------------------------------
//
void CCFContextSubscriptionImpl::SetContextTypeL(
    const TDesC& aContextType )
    {
    FUNC_LOG;
    
    iContextQuery->SetTypeL( aContextType );
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::ContextType
//-----------------------------------------------------------------------------
//
const TDesC& CCFContextSubscriptionImpl::ContextType() const
    {
    FUNC_LOG;
    
    return iContextQuery->Type();
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetContextSourceL
//-----------------------------------------------------------------------------
//
void CCFContextSubscriptionImpl::SetContextSourceL(
    const TDesC& aContextSource )
    {
    FUNC_LOG;
    
    iContextQuery->SetSourceL( aContextSource );
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::ContextSource
//-----------------------------------------------------------------------------
//
const TDesC& CCFContextSubscriptionImpl::ContextSource() const
    {
    FUNC_LOG;
    
    return iContextQuery->Source();
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SubTypeMatch
//-----------------------------------------------------------------------------
//
TBool CCFContextSubscriptionImpl::SubTypeMatch() const
	{
	FUNC_LOG;
	
	return iContextQuery->SubTypeMatch();
	}

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetSubTypeMatch
//-----------------------------------------------------------------------------
//
void CCFContextSubscriptionImpl::SetSubTypeMatch( 
	TBool aMatchSubTypes )
	{
	FUNC_LOG;
	
	iContextQuery->SetSubTypeMatch( aMatchSubTypes );
	}

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::OnlyChangesIndicated
//-----------------------------------------------------------------------------
//
TBool CCFContextSubscriptionImpl::OnlyChangesIndicated() const
	{
	FUNC_LOG;
	
	return iOnlyChangesIndicated;
	}

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetOnlyChangesIndicated
//-----------------------------------------------------------------------------
//
void CCFContextSubscriptionImpl::SetOnlyChangesIndicated( 
	TBool aIndicateChangesOnly )
	{
	FUNC_LOG;
	
	iOnlyChangesIndicated = aIndicateChangesOnly;
	}

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetOnlyChangesIndicated
//-----------------------------------------------------------------------------
//
void CCFContextSubscriptionImpl::SetDataObjectEnabled( TBool aEnabled )
    {
    FUNC_LOG;
    
    iDataObjectEnabled = aEnabled;
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::DataObjectEnabled
//-----------------------------------------------------------------------------
//
TBool CCFContextSubscriptionImpl::DataObjectEnabled() const
    {
    FUNC_LOG;
    
    return iDataObjectEnabled;
    }
    
//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::ExternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextSubscriptionImpl::ExternalizeL(
    RWriteStream& aStream )
    {
    FUNC_LOG;

    // Context query
    iContextQuery->ExternalizeL( aStream );

    // Indicate only changes flag
    aStream.WriteInt16L( iOnlyChangesIndicated );
    
    // Enable data object
    aStream.WriteInt16L( iDataObjectEnabled );
    
    // Commit stream
    aStream.CommitL();
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::InternalizeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextSubscriptionImpl::InternalizeL(
    RReadStream& aStream)
    {
    FUNC_LOG;
    
    // Context query
    iContextQuery->InternalizeL( aStream );

    // Indicate only changes flag
    iOnlyChangesIndicated = aStream.ReadInt16L();

    // Enable data object
    iDataObjectEnabled = aStream.ReadInt16L();
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::Size
//-----------------------------------------------------------------------------
//
EXPORT_C TInt CCFContextSubscriptionImpl::Size() const
    {
    FUNC_LOG;
    
    TInt size = 0;
    
    // Context query
    size += iContextQuery->Size();
    
    // Indicate only changes flag
    size += sizeof( iOnlyChangesIndicated );
    
    // Enable data object flag
    size += sizeof( iDataObjectEnabled );
    
    return size;
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SubscriptionListener
//-----------------------------------------------------------------------------
//
EXPORT_C MCFContextSubscriptionListener& 
	CCFContextSubscriptionImpl::SubscriptionListener() const
    {
    FUNC_LOG;
    
    return *iSubscriptionListener;
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::SetSubscriptionListener
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextSubscriptionImpl::SetSubscriptionListener(
    MCFContextSubscriptionListener& aListener )
    {
    FUNC_LOG;
    
    iSubscriptionListener = &aListener;
    }
    
//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::EvaluateContextChangeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextSubscriptionImpl::EvaluateContextChangeL(
    const CCFContextObject& aNewContext,
    const CCFContextObject& /*aOldContext*/,
    TBool aValueChanged ) const
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iSubscriptionListener, Panic( ENoListenerSet ) );

    if( NotificationNeeded( aNewContext, aValueChanged ) )    
        {
        CCFContextIndicationImpl* indication =
            CCFContextIndicationImpl::NewLC( KNullUid );
        indication->SetContextL( aNewContext );
        iSubscriptionListener->ContextIndicationL( indication );
        CleanupStack::Pop( indication );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::EvaluateContextChangeL
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFContextSubscriptionImpl::EvaluateContextChangeL(
    const CCFContextObject& aNewContext,
    CCFContextDataObject& aData,
    TBool aValueChanged ) const
    {
    FUNC_LOG;

    __ASSERT_DEBUG( iSubscriptionListener, Panic( ENoListenerSet ) );

    if( NotificationNeeded( aNewContext, aValueChanged ) )    
        {
        CCFContextIndicationImpl* indication =
            CCFContextIndicationImpl::NewLC( aData.Uid() );
        indication->SetContextL( aNewContext );
        
        // If data object associated and it is required by the subscriber, add
        // it in the context indication.
        if( iDataObjectEnabled )
            {
            // Stream data object
            HBufC8* data = HBufC8::NewLC( aData.Size() );
            TPtr8 dataPtr( data->Des() );
            RDesWriteStream writeStream( dataPtr );
            writeStream.PushL();
            aData.ExternalizeL( writeStream );
            writeStream.CommitL();
            indication->SetDataObject( data );
            
            // Clean up
            CleanupStack::PopAndDestroy( &writeStream );
            CleanupStack::Pop( data );
            }
        iSubscriptionListener->ContextIndicationL( indication );
        CleanupStack::Pop( indication );
        }
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::IsSame
//-----------------------------------------------------------------------------
//
EXPORT_C TBool CCFContextSubscriptionImpl::IsSame(
    const CCFContextSubscriptionImpl& aSubscription ) const
    {
    FUNC_LOG;
    
    // Check if we have the same session
    if( &aSubscription.SubscriptionListener() != iSubscriptionListener )
        {
        return EFalse;
        }

    if( CCFContextQueryImpl::IsSame(
        *iContextQuery,
        *aSubscription.iContextQuery ) )
       	{
       	return ETrue;
       	}

    return EFalse;
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::Matches
//-----------------------------------------------------------------------------
//
EXPORT_C TBool CCFContextSubscriptionImpl::Matches(
    const TDesC& aContextType ) const
    {
    FUNC_LOG;
    
	return iContextQuery->Type() == aContextType;
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::Matches
//-----------------------------------------------------------------------------
//
EXPORT_C TBool CCFContextSubscriptionImpl::Matches( const TDesC& aContextType,
    const TDesC& aContextSource ) const
    {
    FUNC_LOG;
    
	return iContextQuery->Matches( aContextType, aContextSource );
    }

//-----------------------------------------------------------------------------
// CCFContextSubscriptionImpl::NotificationNeeded
//-----------------------------------------------------------------------------
//
TBool CCFContextSubscriptionImpl::NotificationNeeded(
    const CCFContextObject& aNewContext,
    TBool aValueChanged ) const
    {
    FUNC_LOG;

    TBool need = EFalse;    
    if( !iOnlyChangesIndicated || ( iOnlyChangesIndicated && aValueChanged ) )
    	{
	    if( iContextQuery->Matches( aNewContext.Type(), aNewContext.Source() ) )
	        {
	        need = ETrue;
	        }
    	}
    	
    return need;
    }
