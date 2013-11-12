/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "tsrunningappsession.h"
#include "tstaskmonitorglobals.h"

#include "tsdataobservertask.h"
#include "tsdataprovidertask.h"
#include "tsdatatask.h"

_LIT(KErrObserverExists, "Observer already exists");
// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 */
CTsRunningAppSession::CTsRunningAppSession( 
                                    const MTsDataProvider& aDataProvider )
:
    iDataProvider(aDataProvider)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CTsRunningAppSession::~CTsRunningAppSession()
    {
    iDataStorages.Close();
    iObservers.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
/**
 * Two-phased constructor.
 * @param aObserverStorage - observers storage
 * @param aDataProvider - serialized data provider
 * @param aDataStorages - data storages
 */
CTsRunningAppSession* CTsRunningAppSession::NewL(
                                  const MTsDataProvider& aDataProvider,
                                  const TArray<MTsDataStorage*> &aDataStorages )
    {
    CTsRunningAppSession* self = 
        new (ELeave) CTsRunningAppSession( aDataProvider );
    CleanupStack::PushL( self );
    self->ConstructL( aDataStorages );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
/**
* EPOC default constructor for performing 2nd stage construction
*/
void CTsRunningAppSession::ConstructL(
                                 const TArray<MTsDataStorage*> &aDataStorages )
    {
    for( TInt iter(0); iter < aDataStorages.Count(); ++iter )
        {
        iDataStorages.AppendL( aDataStorages[iter] );
        }
    }

// -----------------------------------------------------------------------------
/**
 * Implements interface
 * @see void CSession2::ServiceL(const RMessage2&)
 */
void CTsRunningAppSession::ServiceL(const RMessage2& aMessage)
    {
    switch ( aMessage.Function() )
        {
        case SubscribeRunningAppChanges:
            CTsDataObserverTask::ExecuteLD( *this, aMessage );
            break;
        
        case CancelSubscribeRunningAppChanges:
            iObservers.ResetAndDestroy();
            aMessage.Complete(KErrNone);
            break;
        
        case GetRunningAppInfo:
        case FlushData:
            TsDataProviderTask::ExecuteL( iDataProvider, aMessage );
            break;
        
        case RegisterScreenshotMessage:
        case UnregisterScreenshotMessage:
        case VisibilityChange:
        case OpenTaskMessage:
        case CloseTaskMessage:
            TsDataTask::ExecuteL( iDataStorages.Array(), aMessage );
            break;
        
        default:
            aMessage.Complete(CServer2::EBadMessageNumber);
        }
    }

// -----------------------------------------------------------------------------
/**
 * Implements interface
 * @see void MTsDataObserver::DataChanged()
 */
void CTsRunningAppSession::DataChanged()
    {
    TInt iter(iObservers.Count()-1);
    iMissedDataUpdate = ( iter < 0 );
    for( ; iter >= 0; --iter )
        {
        iObservers[iter]->DataChanged();
        }
    }

// -----------------------------------------------------------------------------
/**
 * Implements interface
 * @see void MTsDataObserverStorage::PushL(CTsDataObserver*)
 */
void CTsRunningAppSession::PushL(CTsDataObserver* aObserver)
    {
    ( 0 <= iObservers.Find( aObserver ) ) ?
            User::Panic( KErrObserverExists, KErrAlreadyExists ) :
            iObservers.AppendL( aObserver );
    if( iMissedDataUpdate )
        {
        aObserver->DataChanged();
        iMissedDataUpdate = EFalse;
        }
    }

// -----------------------------------------------------------------------------
/**
 * Implements interface
 * @see void MTsDataObserverStorage::Pop(CTsDataObserver*)
 */
void CTsRunningAppSession::Pop(CTsDataObserver* aObserver)
    {
    const TInt offset( iObservers.Find( aObserver ) );
    if( 0 <= offset )
        {
        iObservers.Remove(offset);
        }
    }
