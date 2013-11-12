/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPublishContextCmd implementation.
*
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfpublishcontextcmd.h"
#include "CFContextManager.h"
#include "CFContextDataProxy.h"
#include "cftrace.h"

#include <cfcontextobject.h>
#include <s32mem.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd::CCFPublishContextCmd( CCFContextManager& aContextManager )
    :   iContextManager( aContextManager )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFPublishContextCmd::ConstructL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    iContextObject = CCFContextObject::NewL();
    iContextObject->CopyL( aContext );
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFPublishContextCmd::ConstructL( const CCFContextObject& aContext,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    iContextObject = CCFContextObject::NewL();
    iContextObject->CopyL( aContext );

    HBufC8* data = HBufC8::NewLC( aData.Size() );    // CLEANUP<< data
    TPtr8 dataPtr( data->Des() );
    RDesWriteStream writeStream( dataPtr );
    CleanupClosePushL( writeStream );               // CLEANUP<< writeStream
    aData.ExternalizeL( writeStream );
    CleanupStack::PopAndDestroy( &writeStream );    // CLEANUP>> writeStream
    iData = CCFContextDataProxy::NewL();
    iData->SetContextData( data ); // Transfers ownership.
    CleanupStack::Pop( data );                      // CLEANUP>> data
    iData->SetContextDataUid( aData.Uid() );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd* CCFPublishContextCmd::NewL(
    CCFContextManager& aContextManager,
    const CCFContextObject& aContext )
    {
    FUNC_LOG;

    CCFPublishContextCmd* self
        = CCFPublishContextCmd::NewLC( aContextManager, aContext );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd* CCFPublishContextCmd::NewLC(
    CCFContextManager& aContextManager,
    const CCFContextObject& aContext )
    {
    FUNC_LOG;

    CCFPublishContextCmd* self
        = new( ELeave ) CCFPublishContextCmd( aContextManager );
    CleanupStack::PushL ( self );
    self->ConstructL( aContext );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd* CCFPublishContextCmd::NewL(
    CCFContextManager& aContextManager,
    const CCFContextObject& aContext,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    CCFPublishContextCmd* self
        = CCFPublishContextCmd::NewLC( aContextManager, aContext, aData );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd* CCFPublishContextCmd::NewLC(
    CCFContextManager& aContextManager,
    const CCFContextObject& aContext,
    CCFContextDataObject& aData )
    {
    FUNC_LOG;

    CCFPublishContextCmd* self
        = new( ELeave ) CCFPublishContextCmd( aContextManager );
    CleanupStack::PushL ( self );
    self->ConstructL( aContext, aData );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFPublishContextCmd::~CCFPublishContextCmd( )
    {
    FUNC_LOG;

    delete iContextObject;
    delete iData;
    }


// ---------------------------------------------------------------------------
// CCFPublishContextCmd::ExecuteL
// ---------------------------------------------------------------------------
//
void CCFPublishContextCmd::ExecuteL()
    {
    FUNC_LOG;

    INFO_3( "CCFPublishContextCmd::Execute - Publishing context: [%S: %S: %S]",
            &iContextObject->Source(),
            &iContextObject->Type(),
            &iContextObject->Value() );
    if ( iData )
        {
        INFO_1( "CCFPublishContextCmd::Execute - Publishing context data object: [0x%x]",
                static_cast< CCFContextDataObject* >( iData )->Uid() );
        }

    iContextManager.DoPublishContextL( *iContextObject, iData );
    }

// ---------------------------------------------------------------------------
// CCFPublishContextCmd::LogError
// ---------------------------------------------------------------------------
//
void CCFPublishContextCmd::LogError( TInt aError )
    {
    FUNC_LOG;

    if( aError != KErrNone )
        {
        ERROR_3( aError, "CCFPublishContextCmd::LogError - Failed to publish context [%S: %S: %S] as command.",
            &iContextObject->Source(),
            &iContextObject->Type(),
            &iContextObject->Value() );
        if ( iData )
            {
            ERROR_1( aError, "CCFPublishContextCmd::LogError - Failed to publish with context data object: [0x%x]",
                    static_cast< CCFContextDataObject* >( iData )->Uid() );
            }
        }
    }

// End of file
