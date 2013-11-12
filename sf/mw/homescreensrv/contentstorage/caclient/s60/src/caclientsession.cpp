/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cautils.h"
#include "caclientsession.h"
#include "caclientsubsession.h"
#include "caclientproxy.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "caidsarray.h"
#include "caentriesarray.h"
#include "caclientnotifierproxy.h"

const TInt KInnerEntryExpandSize = 1024;

// Function prototypes
static TInt StartServer();
static TInt CreateServerProcess();

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
RCaClientSession::RCaClientSession() :
    RSessionBase()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TInt RCaClientSession::Connect()
    {
    TInt error =
            CreateSession( KCaSrvName, Version() );
    if( error == KErrNone )
        {
        ShareAuto();
        }
    return error;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TInt RCaClientSession::StartServer()
    {
    return ::StartServer();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::Close()
    {
    RSessionBase::Close();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TVersion RCaClientSession::Version() const
    {
    return ( TVersion( KCaMajorVersion, KCaMinorVersion, KCaBuild ) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::GetListL( const CCaInnerQuery& aQuery,
        RCaEntriesArray& aResult )
    {
    TInt resultSize(0);
    TInt error = GetSizeL( aQuery, EContentArsenalGetList_GetSize,
        resultSize );

    if( error == KErrNone )
        {
        error = GetResultL( resultSize, EContentArsenalGetList_GetData,
                aResult );
        }

    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::GetEntryIdsL( const CCaInnerQuery& aQuery,
        RCaIdsArray& aResult )
    {
    TInt resultSize( 0 );
    TInt error = GetSizeL( aQuery, EContentArsenalGetIds_GetSize,
            resultSize );

    if( error == KErrNone )
        {
        error = GetResultL( resultSize, EContentArsenalGetIds_GetData,
                aResult );
        }

    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TInt RCaClientSession::GetSizeL( const CCaInnerQuery& aQuery,
        const TCaServerRequests aServerRequest, TInt& aResultSize )
    {
    TIpcArgs args;
    HBufC8* serializedQuery = MenuUtils::MarshalDataL( aQuery, 128 );
    CleanupStack::PushL( serializedQuery );
    args.Set( KInputPosition1, serializedQuery );
    TPckg<TInt> sizeDes( aResultSize );
    args.Set( KOutputPosition, &sizeDes );
    TInt error = SendReceive( aServerRequest, args );
    CleanupStack::PopAndDestroy( serializedQuery );
    return error;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::AddL( CCaInnerEntry& aEntry )
    {
    TIpcArgs args;
    HBufC8* serializedEntry = MenuUtils::MarshalDataL( aEntry, KInnerEntryExpandSize );
    CleanupStack::PushL( serializedEntry );
    args.Set( KInputPosition1, serializedEntry );
    TInt id( 0 );
    TPckg<TInt> resultId( id );
    args.Set( KOutputPosition, &resultId );

    TInt error = SendReceive( EContentArsenalAdd, args );
    User::LeaveIfError( error );
    aEntry.SetId( id );

    CleanupStack::PopAndDestroy( serializedEntry );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::RemoveL( const RCaIdsArray& aEntries )
    {
    TIpcArgs args;
    HBufC8* inbuf = aEntries.MarshalDataLC();
    args.Set( KInputPosition1, inbuf );
    User::LeaveIfError( SendReceive( EContentArsenalRemove, args ) );
    CleanupStack::PopAndDestroy( inbuf );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::OrganizeL( const RCaIdsArray& aEntries,
        TCaOperationParams aParams )
    {
    TIpcArgs args;
    HBufC8* inbuf = aEntries.MarshalDataLC();
    args.Set( KInputPosition1, inbuf );
    TPckg<TCaOperationParams> data( aParams );
    args.Set( KInputPosition2, &data );
    User::LeaveIfError( SendReceive( EContentArsenalOrganize, args ) );
    CleanupStack::PopAndDestroy( inbuf );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::TouchL( const CCaInnerEntry& aEntry )
    {
    TIpcArgs args;
    HBufC8* serializedEntry = MenuUtils::MarshalDataL( aEntry, KInnerEntryExpandSize );
    CleanupStack::PushL( serializedEntry );
    args.Set( KInputPosition1, serializedEntry );
    
    User::LeaveIfError( SendReceive( EContentArsenalTouch, args ) );
	
    CleanupStack::PopAndDestroy( serializedEntry );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RCaClientSession::CustomSortL( const RCaIdsArray& aEntries,
        const TInt aGroupId )
    {
    TIpcArgs args;
    HBufC8* inbuf = aEntries.MarshalDataLC();
    args.Set( KInputPosition1, inbuf );
    TPckg<TInt> groupId( aGroupId );
    args.Set( KInputPosition2, &groupId );
    User::LeaveIfError( SendReceive( EContentArsenalCustomSort, args ) );
    CleanupStack::PopAndDestroy( inbuf );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename SerializableObject>
TInt RCaClientSession::GetResultL( TInt aSize, TCaServerRequests aRequest,
        SerializableObject& aObject )
    {
    TIpcArgs args;
    RBuf8 outbuf;
    outbuf.CleanupClosePushL();
    outbuf.CreateL( aSize );
    args.Set( KOutputPosition, &outbuf );
    TInt error = SendReceive( aRequest, args );
    if( error == KErrNone )
        {
        RDesReadStream stream( outbuf );
        CleanupClosePushL( stream );
        aObject.InternalizeL( stream );
        CleanupStack::PopAndDestroy( &stream );
        }
    CleanupStack::PopAndDestroy( &outbuf );
    return error;
    }

// -----------------------------------------------------------------------------
// StartServer
// Static function to start the server.
// -----------------------------------------------------------------------------
//
static TInt StartServer()
    {
    TInt result;

    TFindServer findServer( KCaSrvName );
    TFullName name;

    result = findServer.Next( name );
    if( result != KErrNone )
        {
        // Server not running
        result = CreateServerProcess();
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CreateServerProcess
// Static function to create the server process.
// -----------------------------------------------------------------------------
//
static TInt CreateServerProcess()
    {
    TInt err( KErrNone );
    RProcess server;
    err = server.Create( KCaSrvExe, KNullDesC, EOwnerThread );
    if( !err )
        {
        TRequestStatus status;
        server.Rendezvous( status );
        if( status != KRequestPending )
            {
            server.Kill( 0 ); // Abort startup.
            }
        else
            {
            server.Resume(); // Logon OK - start the server.
            }
        User::WaitForRequest( status ); // Wait for start or death.
        // We can't use the 'exit reason' if the server panicked as this
        // is the panic 'reason' and may be '0' which cannot be distinguished
        // from KErrNone.
        err = ( server.ExitType() == EExitPanic ) ? KErrGeneral
                : status.Int();
        server.Close();
        }
    return err;
    }

