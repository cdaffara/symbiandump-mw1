/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base class for API implementations.
*
*/


#include <s32strm.h>
#include <s32mem.h>

#include "ximpeventcodec.h"
#include "ximprequestcompleteeventimp.h"
#include "ximpcontextstateeventimp.h"


/**
 * Type definition for exact API event constructor signature.
 */
typedef CXIMPApiEventBase* (*TApiEventConstructor)( RReadStream& );



/**
 * Structure for mapping event interface IDs to
 * to corect API event constructors.
 */
struct SApiEventConstructorMap
    {
    //Interface id
    TInt32    iInterfaceId;

    //Function pointer to event interface implementation
    TApiEventConstructor iConstructorPtr;
    } ;



/**
 * Helper macro to initialise KApiEventConstructorTable members.
 *
 * Macro forces that each event implementation class to have static
 * NewFromStreamLC() memeber function to instantiate the object.
 *
 * See TApiEventConstructor type definition for exact constructor
 * signature.
 */
#define CONSTRUCTOR_ENTRY( TheImplementedIf, TheClass ) \
    { TheImplementedIf::KInterfaceId, TheClass::NewFromStreamLC } \



/**
 * Constructor function mapping for event interface implementations.
 *
 * When new event types are implemented, add them here.
 */
const SApiEventConstructorMap KApiEventConstructorTable[] =
    {
    CONSTRUCTOR_ENTRY( MXIMPRequestCompleteEvent, CXIMPRequestCompleteEventImp ),
    CONSTRUCTOR_ENTRY( MXIMPContextStateEvent, CXIMPContextStateEventImp )
    };


/**
 * Count of constructor mappings.
 */
const TInt KApiEventConstructorCount = sizeof( KApiEventConstructorTable )
                                        / sizeof( SApiEventConstructorMap );



// ============================ HELPER FUNCTIONS =============================


/**
 * Helper function to locate constructor function for
 * event interface ID.
 *
 * @param aEventInterfaceId The event interface ID.
 * @return Event object constructor function.
 */
TApiEventConstructor ConstructorForInterface( TInt32 aEventInterfaceId )
    {
    //Locate correct constructor
    for( TInt ix = 0; ix < KApiEventConstructorCount; ix++ )
        {
        const SApiEventConstructorMap map = KApiEventConstructorTable[ ix ];
        if( map.iInterfaceId == aEventInterfaceId )
            {
            return map.iConstructorPtr;
            }
        }


    //If assertion below fails, check that event implementation
    //class implementing requested event interface (aEventInterfaceId)
    //is registered to KApiEventConstructorTable
//    __ASSERT_DEBUG( EFalse,
//                    User::Panic( _L("XIMPEventCodec"), KErrUnknown ) );

    return NULL;
    }


/**
 * Helper function to instantiate new event object
 * of requested type and construct it from the stream.
 *
 * @param aEventInterfaceId
 * @return Event object constructor function.
 */
CXIMPApiEventBase* NewEventObjectFromStreamLC( TInt32 aEventInterfaceId,
                                               RReadStream& aStream )
    {
    TApiEventConstructor newFromStreamLC = NULL;

    //Locate correct constructor for interface ID
    newFromStreamLC = ConstructorForInterface( aEventInterfaceId );

    if ( newFromStreamLC )
        {
        //Instantiate the object
        CXIMPApiEventBase* eventObject = newFromStreamLC( aStream );
        return eventObject;
        }


    // if no constructor
    return NULL;
    }



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// XIMPEventCodec::PackL()
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* XIMPEventCodec::PackL( CXIMPApiEventBase& aEventObj,
                                        TInt32& aTypeOfEvent )
    {
    CBufFlat* buffer = CBufFlat::NewL( 10 ); // initial granularity to 10
    CleanupStack::PushL( buffer );

    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *buffer ); // CSI: 65 #

    //Ask real event type through the event base interface
    TInt32 eventIfId = aEventObj.Base().GetInterfaceId();

    //And write both event type and data
    ws.WriteInt32L( eventIfId );
    aEventObj.ExternalizeL( ws );
    ws.CommitL();
    CleanupStack::PopAndDestroy();  //ws

    HBufC8* heapBuf = buffer->Ptr( 0 ).AllocL();

    CleanupStack::PopAndDestroy( buffer );


    aTypeOfEvent = eventIfId;
    return heapBuf;
    }


// ---------------------------------------------------------------------------
// XIMPEventCodec::PackL()
// ---------------------------------------------------------------------------
//
EXPORT_C void XIMPEventCodec::PackL( CXIMPApiEventBase& aEventObj,
                                     TInt32& aTypeOfEvent,
                                     CBufFlat* aBuffer )
    {
    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *aBuffer ); // CSI: 65 #

    //Ask real event type through the event base interface
    TInt32 eventIfId = aEventObj.Base().GetInterfaceId();

    //And write both event type and data
    ws.WriteInt32L( eventIfId );
    aEventObj.ExternalizeL( ws );
    ws.CommitL();
    CleanupStack::PopAndDestroy();  //ws
    
    aTypeOfEvent = eventIfId;
    }


// ---------------------------------------------------------------------------
// XIMPEventCodec::UnPackL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPApiEventBase* XIMPEventCodec::UnPackL( const TDesC8& aEventData,
                                                     TInt32& aTypeOfEvent )
    {
    RDesReadStream rs;
    rs.Open( aEventData ); // CSI: 65 #
    CleanupClosePushL( rs );

    TInt32 eventIfId = rs.ReadInt32L();
    CXIMPApiEventBase* eventObject = NewEventObjectFromStreamLC( eventIfId, rs );
    
    if ( eventObject )
        {
        CleanupStack::Pop(); //eventObject    
        }
        
    CleanupStack::PopAndDestroy(); // rs


    aTypeOfEvent = eventIfId;
    return eventObject;
    }


// End of file

