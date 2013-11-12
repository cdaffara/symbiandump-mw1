/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFScriptSubscription class implementation.
*
*/



// INCLUDES
#include "cfscriptsubscription.h"
#include "cfscriptlistener.h"
#include "CFContextSubscriptionImpl.h"
#include "cftrace.h"

#include <cfcontextinterface.h>
#include <cfcontextsubscriptionlistener.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFScriptSubscription::CCFScriptSubscription
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFScriptSubscription::CCFScriptSubscription( MCFContextInterface& aCF,
    MCFContextSubscriptionListener& aCFListener  )
    :   iCF( aCF ),
        iCFListener( aCFListener ),
        iSubscribed( EFalse )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFScriptSubscription::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFScriptSubscription::ConstructL( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    if ( !aListener )
        {
        ERROR( KErrBadHandle, "CCFScriptSubscription::ConstructL - MCFScriptListener is NULL!" );
        User::Leave( KErrBadHandle );
        }

    iSubscription = CCFContextSubscriptionImpl::NewL();
    iSubscription->SetContextSourceL( aListener->Source() );
    iSubscription->SetContextTypeL( aListener->Type() );
    iSubscription->SetSubscriptionListener( iCFListener );

    iListeners.AppendL( aListener );
    DoSubscribeL( aListener );
    }

// ---------------------------------------------------------------------------
// CCFScriptSubscription::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFScriptSubscription* CCFScriptSubscription::NewL( MCFContextInterface& aCF,
    MCFContextSubscriptionListener& aCFListener,
    MCFScriptListener* aListener )
    {
    FUNC_LOG;

    CCFScriptSubscription* self = NewLC( aCF, aCFListener, aListener );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCFScriptSubscription::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFScriptSubscription* CCFScriptSubscription::NewLC( MCFContextInterface& aCF,
    MCFContextSubscriptionListener& aCFListener,
    MCFScriptListener* aListener  )
    {
    FUNC_LOG;

    CCFScriptSubscription* self
        = new( ELeave ) CCFScriptSubscription( aCF, aCFListener );
    CleanupStack::PushL( self );
    self->ConstructL( aListener );
    return self;
    }


// Destructor
CCFScriptSubscription::~CCFScriptSubscription()
    {
    FUNC_LOG;

    if ( !iSubscribed )
        {
        delete iSubscription;
        }
    iListeners.Close();
    }


// -----------------------------------------------------------------------------
// CCFScriptSubscription::AddListenerL
// -----------------------------------------------------------------------------
//
void CCFScriptSubscription::AddListenerL( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    iListeners.AppendL( aListener );
    NotifyListenerOfCurrentContextValueL( aListener );
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::RemoveListener
// -----------------------------------------------------------------------------
//
TBool CCFScriptSubscription::RemoveListener( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    TInt index = iListeners.Find( aListener );
    if ( index != KErrNotFound )
        {
        iListeners.Remove( index );
        }

    if ( iListeners.Count() == 0 )
        {
        if ( iSubscribed )
            {
            iCF.UnsubscribeContext( *iSubscription, iCFListener );
            iSubscription = NULL; // Was not owned anymore, and deleted by CF.
            iSubscribed = EFalse;
            }
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::Match
// -----------------------------------------------------------------------------
//
TBool CCFScriptSubscription::Match( const MCFScriptListener& aListener ) const
    {
    FUNC_LOG;

    if ( iSubscription->ContextSource() == aListener.Source()
        && iSubscription->ContextType() == aListener.Type() )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::Match
// -----------------------------------------------------------------------------
//
TBool CCFScriptSubscription::Match( const CCFContextObject& aContext ) const
    {
    FUNC_LOG;

    if ( iSubscription->ContextSource() == aContext.Source()
        && iSubscription->ContextType() == aContext.Type() )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::NotifyListeners
// -----------------------------------------------------------------------------
//
TBool CCFScriptSubscription::NotifyListeners( const CCFContextObject& aContext,
    TInt& aContextLevelDelay ) const
    {
    FUNC_LOG;

    TBool evaluated( EFalse );
    for ( TInt i = 0; i < iListeners.Count(); ++i )
        {
        if ( !iListeners[ i ]->IsAllRequired() )
            {
            TInt delay( 0 );
            if ( iListeners[ i ]->Evaluate( aContext, delay ) )
                {
                // Evaluation flag may not change back to false.
                evaluated = ETrue;
                }

            if ( delay > aContextLevelDelay )
                {
                aContextLevelDelay = delay;
                }
            }
        }

    return evaluated;
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::DoSubscribeL
// Do subscribe and notify listener about current context value.
// -----------------------------------------------------------------------------
//
TInt CCFScriptSubscription::DoSubscribeL( MCFScriptListener* aListener )
    {
    FUNC_LOG;

    RThread thread;
    CleanupClosePushL( thread );                            // CLEANUP<< thread
    TInt err = iCF.SubscribeContext( iSubscription, &iCFListener, thread );
    if( err != KErrNone )
        {
        TPtrC source( aListener->Source() );
        TPtrC type( aListener->Type() );
        ERROR( err, "CCFScriptSubscription::DoSubscribeL - Subscribing context failed!" );
        ERROR_2( err, "- context source [%S], type [%S]", &source, &type );
        }
    else
        {
        iSubscribed = ETrue; // Mark successful subscription.
        NotifyListenerOfCurrentContextValueL( aListener );
        }
    CleanupStack::PopAndDestroy( &thread );                 // CLEANUP>> thread

    return err;
    }

// -----------------------------------------------------------------------------
// CCFScriptSubscription::NotifyListenerOfCurrentContextValue
// -----------------------------------------------------------------------------
//
void CCFScriptSubscription::NotifyListenerOfCurrentContextValueL(
    MCFScriptListener* aListener ) const
    {
    FUNC_LOG;

    RThread thread;
    CleanupClosePushL( thread );                            // CLEANUP<< thread
    // Request current context value for the new subscription.
    CCFContextQuery* query = CCFContextQuery::NewLC();      // CLEANUP<< query
    RContextObjectArray results; // Pointers not owned.
    CleanupClosePushL( results );                           // CLEANUP<< results
    query->SetSourceL( iSubscription->ContextSource() );
    query->SetTypeL( iSubscription->ContextType() );

    iCF.RequestContext( results, *query, thread );
    if ( results.Count() )
        {
        CCFContextObject* context = results[ 0 ];
        TInt delay( 0 ); // Required by the call but not used.
        aListener->Evaluate( *context, delay );
        }

    CleanupStack::PopAndDestroy( &results );                // CLEANUP>> results
    CleanupStack::PopAndDestroy( query );                   // CLEANUP>> query
    CleanupStack::PopAndDestroy( &thread );                 // CLEANUP>> thread
    }
