/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFEcomObserver class implementation.
*
*/


#include "cfecomobserver.h"
#include "cftrace.h"
#include "cfstateobserver.h"

// LOCAL DEFINITIONS

#ifdef _DEBUG
_LIT( KPanicCat, "CFEcomObs" );

// Panic codes
enum TPanicReason
    {
    EAlreadyActive,
    EInvalidObserver
    };

// LOCAL FUNCTIONS
LOCAL_C void Panic( TInt aReason )
    {
    User::Panic( KPanicCat, aReason );
    }
#endif

//-----------------------------------------------------------------------------
// CCFEComObserver::CCFEComObserver
//-----------------------------------------------------------------------------
//
CCFEComObserver::CCFEComObserver(): CActive( EPriorityStandard )
	{
    FUNC_LOG;

    CActiveScheduler::Add( this );
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::NewLC
//-----------------------------------------------------------------------------
//
CCFEComObserver* CCFEComObserver::NewLC()
	{
    FUNC_LOG;
    
	CCFEComObserver* self = new ( ELeave ) CCFEComObserver();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::NewL
//-----------------------------------------------------------------------------
//
CCFEComObserver* CCFEComObserver::NewL()
	{
    FUNC_LOG;
    
	CCFEComObserver* self = CCFEComObserver::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::ConstructL
//-----------------------------------------------------------------------------
//
void CCFEComObserver::ConstructL()
	{
    FUNC_LOG;
    
	iEComSession = &REComSession::OpenL();
	StartObserving();
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::~CCFEComObserver
//-----------------------------------------------------------------------------
//
CCFEComObserver::~CCFEComObserver()
	{
    FUNC_LOG;
    
    // Cancel any request, if outstanding
	Cancel();
    
    // Clean up memebers
    iObservers.Close();
    if( iEComSession )
        {
        iEComSession->Close();
        }
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::DoCancel
//-----------------------------------------------------------------------------
//
void CCFEComObserver::DoCancel()
	{
    FUNC_LOG;
    
	iEComSession->CancelNotifyOnChange( iStatus );
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::StartObserving
//-----------------------------------------------------------------------------
//
void CCFEComObserver::StartObserving()
	{
    FUNC_LOG;
    
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyActive) );
    
    SetActive();
	iEComSession->NotifyOnChange( iStatus ); 
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::AddObserverL
//-----------------------------------------------------------------------------
//
void CCFEComObserver::AddObserverL( MCFStateObserver* aObserver )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aObserver, Panic( EInvalidObserver ) );
    
    iObservers.AppendL( aObserver );
    }


//-----------------------------------------------------------------------------
// CCFEComObserver::RunL
//-----------------------------------------------------------------------------
//
void CCFEComObserver::RunL()
	{
    FUNC_LOG;
    
    // Store completion status
    TInt status = iStatus.Int();

    // Continue request
    StartObserving();

    // Notify observers
    if( status == KErrNone )
        {
        NotifyObservers();
        }    
	}

//-----------------------------------------------------------------------------
// CCFEComObserver::RunError
//-----------------------------------------------------------------------------
//
TInt CCFEComObserver::RunError( TInt aError )
	{
	return aError;
	}

//------------------------------------------------------------------------------
// CCFEComObserver::NotifyObservers
//------------------------------------------------------------------------------
//
void CCFEComObserver::NotifyObservers()
    {
    FUNC_LOG;

    INFO( "CCFEComObserver::NotifyObservers - New plug-ins detected, updating" );

    TInt count = iObservers.Count();
    for( TInt i = 0; i < count; i++ )
        {
        TInt err = KErrNone;
        TRAP( err, iObservers[i]->UpdatePlugInsL( ) );
        ERROR( err, "Failed to update plug-ins" );
        }
    }

// End of file

