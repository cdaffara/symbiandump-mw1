/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Implements the Session class for the Rfs Server.
*
*
*/


// INCLUDE FILES

#include <e32std.h>
#include "rfsSession.h"
#include "rfsServer.h"
#include "rfsClient.h"
#include "rfsCompleter.h"
#include "RfsTraces.h"

// TYPE DEFINITIONS
const TUid KStarterUid = { 0x100059C9 };
const TUid KPhoneAppUid = { 0x100058B3 };
const TUid KGSUid = { 0x100058EC };

// ================= MEMBER FUNCTIONS ==========================================

CRfsSession::CRfsSession() : CSession2()
    {
    }

CRfsSession* CRfsSession::NewL()
    {
    CRfsSession* self = new( ELeave ) CRfsSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// RHandleBase::Close() causes this destructor to be called

CRfsSession::~CRfsSession()
    {
    if ( iEpocCompleter )
        {
        iEpocCompleter->Cancel();
        delete iEpocCompleter;
        }

    if ( iDosCompleter )
        {
        iDosCompleter->Cancel();
        delete iDosCompleter;
        }

    Server()->DecrementSessions();
    }

// -----------------------------------------------------------------------------
// CRfsSession::ConstructL()
// -----------------------------------------------------------------------------
    
void CRfsSession::ConstructL()
    {
    // adds completers in active scheduler

    iEpocCompleter = CRfsCompleter::NewL( this );
    iDosCompleter = CRfsCompleter::NewL( this );
    }

// -----------------------------------------------------------------------------
// CRfsSession::DispatchMessageL()
// -----------------------------------------------------------------------------

TInt CRfsSession::DispatchMessageL( const RMessage2 &aMessage )
    {
    User::LeaveIfError(CheckSecurity( aMessage ));

    switch ( aMessage.Function() )
        {
        case ERfsServRunScriptInFile:
            return RunScriptL( EScriptInFile );

        case ERfsServRunScriptInDescriptor:
            return RunScriptL( EScriptInDescriptor );

        case ERfsServPerformRfs:
            return PerformRfsL();

        case ERfsServPerformPostEikonRfs:
            return PerformPostEikonRfs();

        default:
            PanicClient( ERfsBadRequest );
            return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CRfsSession::ServiceL()
// -----------------------------------------------------------------------------

void CRfsSession::ServiceL(const RMessage2& aMessage)
    {
    TInt ret( KErrNone );

    iCurrentMessage = aMessage;

    iEpocCompleter->Start();
    iDosCompleter->Start();

    // status of the last CD command
    iChangeDirStatus = KErrNone;

    TRAPD( err, {ret = DispatchMessageL( aMessage );} );

    // If the request didn't complete the normal way,
    // complete it with error code.

    if ( ret != KRfsRequestCompleted )
        {
        iDosCompleter->Complete( KErrNone );
        iEpocCompleter->Complete( err != KErrNone ? err : ret );
        }
    }

// -----------------------------------------------------------------------------
// CRfsSession::RunScriptL()
// -----------------------------------------------------------------------------

TInt CRfsSession::RunScriptL( TRfsScriptLocation aLocation )
    {

    // 1. message parameter is the descriptor address
    // 2. message parameter is the descriptor length

    HBufC* buffer = HBufC::NewLC( Message().Int1() );
    TPtr bufferPtr = buffer->Des();
    Message().ReadL( 0, bufferPtr );

    TInt ret;

    if ( aLocation == EScriptInDescriptor )
        {
        ret = Server()->RunScriptInDescriptorL( bufferPtr, this );
        }
    else
        {
        ret = Server()->HandleScriptInFileL( bufferPtr, this );
        Server()->PerformPluginScriptsL(this);
        }

    CleanupStack::PopAndDestroy();   // buffer
    return ret;
    }

// -----------------------------------------------------------------------------
// CRfsSession::PerformRfsL()
// -----------------------------------------------------------------------------

TInt CRfsSession::PerformRfsL()
    {
    // if DOS RFS is excluded, mark it done
    TRACES("CRfsSession::PerformRfsL()");
    if ( !Message().Int1() )
        {
        iDosCompleter->Complete( KErrNone );
        }       
    TInt ret_val = Server()->PerformRfsL( (TRfsType)Message().Int0(), this );
    TRACES1("CRfsSession::PerformRfsL(): End: returns %d", ret_val);
    return ret_val;
    }

// -----------------------------------------------------------------------------
// CRfsSession::PerformPostEikonRfs()
// -----------------------------------------------------------------------------

TInt CRfsSession::PerformPostEikonRfs()
    {
    iDosCompleter->Complete( KErrNone );
    TInt ret = Server()->PerformPostEikonRfs( (TRfsType)Message().Int0(), this );
    iEpocCompleter->Complete( ret );

    return KRfsRequestCompleted;
    }

// -----------------------------------------------------------------------------
// CRfsSession::PanicClient()
// -----------------------------------------------------------------------------

void CRfsSession::PanicClient( TRfsPanic aPanic ) const
    {
    iCurrentMessage.Panic( KRfsPanicCategory, aPanic );
    }

// -----------------------------------------------------------------------------
// CRfsSession::Message
// Returns the current message in process 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMessage2& CRfsSession::Message()
    {
    return iCurrentMessage;
    }

// -----------------------------------------------------------------------------
// CRfsSession::CheckSecurity
// Returns NULL if security is ok, otherwise returns error code
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CRfsSession::CheckSecurity( const RMessage2 &aMessage )
    {
    TRACES("CRfsSession::CheckSecurity()");
    TInt ret_val(KErrPermissionDenied);

    TRACES("CRfsSession::CheckSecurity(): Check UID");
    TUid clientUid = aMessage.Identity();
    TRACES1("CRfsSession::CheckSecurity(): clientUid = %d", clientUid);
    if (clientUid == KStarterUid ||
        clientUid == KPhoneAppUid ||
        clientUid == KGSUid)
        {
        ret_val = KErrNone;
        TRACES("CRfsSession::CheckSecurity(): UID allowed");
        }
    else
        {
        TRACES("CRfsSession::CheckSecurity(): Check capability");
        TRAP(ret_val, aMessage.HasCapabilityL(ECapabilityDiskAdmin));
        TRACES1("CRfsSession::CheckSecurity(): Capability check: %d", ret_val);
        }
    TRACES1("CRfsSession::CheckSecurity(): End: returns %d", ret_val);
    return ret_val;
    }
//  End of File  
