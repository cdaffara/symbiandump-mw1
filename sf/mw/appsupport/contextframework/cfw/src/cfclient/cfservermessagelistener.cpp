/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFServerMessageListener class declaration.
*
*/


// SYSTEM INCLUDES
#include <e32svr.h>
#include <s32mem.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourcecommand.h>

// USER INCLUDES
#include "cfservermessagelistener.h"
#include "cflistener.h"
#include "CFClientSession.h"
#include "cftrace.h"
#include "cfcommon.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CFCLIENT" );

// Panic reason
enum TPanicReason
    {
    ENoBufferCreated,
    EAlreadyReceiving
    };

// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

// If buffer grows bigger than 1kb it will be deleted
static const TInt KMessageTreshold = 1024;

// MEMBER FUNCTIONS

//----------------------------------------------------------------------------
// CCFServerMessageListener::NewL
//----------------------------------------------------------------------------
//
CCFServerMessageListener* CCFServerMessageListener::NewL(
    MCFListener& aListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;

    CCFServerMessageListener* self =
        CCFServerMessageListener::NewLC( aListener, aSession );
    CleanupStack::Pop( self );

    return self;
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::NewLC
//----------------------------------------------------------------------------
//
CCFServerMessageListener* CCFServerMessageListener::NewLC(
    MCFListener& aListener,
    RCFClientSession& aSession )
    {
    FUNC_LOG;

    CCFServerMessageListener* self =
        new( ELeave ) CCFServerMessageListener( aListener, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::~CCFServerMessageListener
//----------------------------------------------------------------------------
//
CCFServerMessageListener::~CCFServerMessageListener()
    {
    FUNC_LOG;

    Cancel();
    delete iBuffer;
    if( iDied )
        {
        *iDied = ETrue;
        }
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::CCFServerMessageListener
//----------------------------------------------------------------------------
//
CCFServerMessageListener::CCFServerMessageListener(
    MCFListener& aListener,
    RCFClientSession& aSession ):
    CActive( EPriorityStandard ),
    iSession( aSession ),
    iListener( aListener ),
    iBufferPtr( 0, 0 ),
    iBufferSizePckg( iBufferSize )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::ConstructL
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::ConstructL()
    {
    FUNC_LOG;

    // Initialize buffer into minimum size
    iBuffer = KNullDesC8().AllocL();
    }

// METHODS

//----------------------------------------------------------------------------
// CCFServerMessageListener::ReceiveMessages
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::ReceiveMessages()
    {
    FUNC_LOG;

    __ASSERT_DEBUG( iBuffer, Panic( ENoBufferCreated ) );
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyReceiving ) );

    iBufferPtr.Set( iBuffer->Des() );
    iBufferPtr.Zero();

    TIpcArgs args( &iBufferPtr, &iBufferSizePckg );
    iSession.SendAsync( EReceiveMessage, args, iStatus );
    SetActive();
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::RunL
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::RunL()
    {
    FUNC_LOG;

    TBool died = EFalse;
    iDied = &died;
    if( iStatus == KErrNone )
        {
        // Server message succesfully received
        // Interpret server message from stream and forward to client
        NotifyMessageL();
        }
    else
        {
        TInt err = iStatus.Int();

        if( iStatus == KErrServerTerminated )
            {
            INFO( "Asynchronous request error: [KErrServerTerminated]" );
            died = ETrue;
            }
        else if( iStatus == KErrTooBig )
            {
            // Buffer too small to receive all data from server
            // Increase buffer, receive data and notify client
            INFO( "Insufficient buffer size - increasing..." );

            UpdateBufferL();
            TIpcArgs args( &iBufferPtr );
            TInt err = iSession.SendSync( EGetMessage, args );
            if( err == KErrNone )
                {
                // Latest server message succesfully received, inform client
                NotifyMessageL();
                }
            else
                {
                // Some other error occured, inform client
                NotifyMessageErrorL();
                }
            }
        else
            {
            // Some other error occured, inform client
            ERROR ( err, "Asynchronous request-error." );
            NotifyMessageErrorL();
            }
        }

    if( !died )
        {
        if( iBufferSize > KMessageTreshold )
            {
            // Zero buffer so that we do not accidentally keep huge amount
            // of memory reserved
            iBufferSize = 0;
            UpdateBufferL();
            }
        ReceiveMessages();
        iDied = NULL;
        }
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::DoCancel
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::DoCancel()
    {
    FUNC_LOG;

    iSession.SendSync( ECancelReceiveMessage );
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::RunError
//----------------------------------------------------------------------------
//
TInt CCFServerMessageListener::RunError( TInt /*aError */)
    {
    FUNC_LOG;

    // Minimize buffer since we propably are here due to OOM
    iBufferSize = 0;
    TInt err = KErrNone;
    TRAP( err, UpdateBufferL() );
    if( iBuffer && err == KErrNone )
        {
        // Continue receiving
        ReceiveMessages();

        // Notify client about general error
        iListener.HandleContextFrameworkError( MCFListener::EGeneralError,
            KNullDesC,
            KNullDesC );
        }
    else
        {
        err = KErrNoMemory;
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::NotifyMessageL
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::NotifyMessageL()
    {
    FUNC_LOG;

    TAny* extension = iListener.Extension( KCFContextSourceInterfaceUid );
    if ( !extension )
        {
        return; // Skip notifying because client does not have extension.
        }

    MCFContextSource* contextSource
        = reinterpret_cast< MCFContextSource* >( extension );

    // Read the streamed message
    RDesReadStream readStream( iBufferPtr );
    readStream.PushL();                                 // CLEANUP<< readStream

    TInt messageType = readStream.ReadInt32L();
    switch( messageType )
        {
        case ESubscribers:
            {
            HBufC* source = DescriptorLC( readStream ); // CLEANUP<< source
            HBufC* type = DescriptorLC( readStream );   // CLEANUP<< type

            contextSource->Subscribers( *source, *type );

            CleanupStack::PopAndDestroy( type );        // CLEANUP>> type
            CleanupStack::PopAndDestroy( source );      // CLEANUP>> source

            break;
            }
        case ENoSubscribers:
            {
            HBufC* source = DescriptorLC( readStream ); // CLEANUP<< source
            HBufC* type = DescriptorLC( readStream );   // CLEANUP<< type

            contextSource->NoSubscribers( *source, *type );

            CleanupStack::PopAndDestroy( type );        // CLEANUP>> type
            CleanupStack::PopAndDestroy( source );      // CLEANUP>> source

            break;
            }
        case EHandleCommand:
            {
            CCFContextSourceCommand* command
                = CCFContextSourceCommand::NewLC();     // CLEANUP<< command
            command->InternalizeL( readStream );

            TPtrC commandName( command->Name() );
            INFO_1( "Notifying CFClient listener from context source command: %S",
                    &commandName );

            contextSource->HandleCommand( *command );

            CleanupStack::PopAndDestroy( command );     // CLEANUP>> command

            break;
            }
        default:
            {
            ERROR_GEN_1( "Unknown server message, type number=%d", messageType );
            }
        }

    CleanupStack::PopAndDestroy( &readStream );         // CLEANUP>> readStream
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::NotifyMessageErrorL
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::NotifyMessageErrorL()
    {
    FUNC_LOG;

    // Check error and stream possible context related to subscription
    MCFListener::TCFError error;
    switch( iStatus.Int() )
        {
        default:
            {
            error = MCFListener::EGeneralError;
            break;
            }
        }

    // Notify client
    iListener.HandleContextFrameworkError( error, KNullDesC, KNullDesC );
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::UpdateBufferL
//----------------------------------------------------------------------------
//
void CCFServerMessageListener::UpdateBufferL()
    {
    FUNC_LOG;

    delete iBuffer;
    iBuffer = NULL;
    iBuffer = HBufC8::NewL( iBufferSize );
    iBufferPtr.Set( iBuffer->Des() );
    }

//----------------------------------------------------------------------------
// CCFServerMessageListener::DescriptorLC
//----------------------------------------------------------------------------
//
HBufC* CCFServerMessageListener::DescriptorLC( RReadStream& aReadStream )
    {
    FUNC_LOG;

    TInt length = aReadStream.ReadInt32L();
    HBufC* descriptor = HBufC::NewLC( length );     // CLEANUP<< descriptor
    TPtr descriptorPtr = descriptor->Des();
    aReadStream.ReadL( descriptorPtr, length  );

    return descriptor; // Left to cleanup stack
    }
