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

#include "presenceeventcodec.h"
//#include "ximprequestcompleteeventimp.h"
//#include "ximpcontextstateeventimp.h"
#include "presentitygrouplisteventimp.h"
#include "ownpresenceeventimp.h"
#include "presentitypresenceeventimp.h"
#include "presentitygroupcontenteventimp.h"
#include "presencewatcherlisteventimp.h"
#include "presencegrantrequestlisteventimp.h"
#include "presenceblocklisteventimp.h"

#include "ximpapieventbase.h"
#include <s32strm.h>
#include <s32mem.h>

//DATA TYPES


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
//    CONSTRUCTOR_ENTRY( MXIMPRequestCompleteEvent, CXIMPRequestCompleteEventImp ),
//    CONSTRUCTOR_ENTRY( MXIMPContextStateEvent, CXIMPContextStateEventImp ),
    CONSTRUCTOR_ENTRY( MPresentityGroupListEvent, CPresentityGroupListEventImp ),
    CONSTRUCTOR_ENTRY( MOwnPresenceEvent, COwnPresenceEventImp ),
    CONSTRUCTOR_ENTRY( MPresentityPresenceEvent, CPresentityPresenceEventImp ),
    CONSTRUCTOR_ENTRY( MPresentityGroupContentEvent, CPresentityGroupContentEventImp ),
    CONSTRUCTOR_ENTRY( MPresenceWatcherListEvent, CPresenceWatcherListEventImp ),
    CONSTRUCTOR_ENTRY( MPresenceGrantRequestListEvent, CPresenceGrantRequestListEventImp ),
    CONSTRUCTOR_ENTRY( MPresenceBlockListEvent, CPresenceBlockListEventImp )
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
//                    User::Panic( _L("PresenceEventCodec"), KErrUnknown ) );

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
	if( ! newFromStreamLC )
	    {
	    return 	NULL;
	    }
    //Instantiate the object
    CXIMPApiEventBase* eventObject = newFromStreamLC( aStream );


    return eventObject;
    }



// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CPresenceEventCodec::NewL()
// ---------------------------------------------------------------------------
//
CPresenceEventCodec* CPresenceEventCodec::NewL()
    {
    CPresenceEventCodec* self = new( ELeave ) CPresenceEventCodec() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceEventCodec::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceEventCodec::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceEventCodec::CPresenceEventCodec()
// ---------------------------------------------------------------------------
//
CPresenceEventCodec::CPresenceEventCodec()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceEventCodec::~CPresenceEventCodec()
// ---------------------------------------------------------------------------
//
CPresenceEventCodec::~CPresenceEventCodec()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceEventCodec::PackL()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceEventCodec::PackL( CXIMPApiEventBase& aEventObj,
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
// CPresenceEventCodec::PackL()
// ---------------------------------------------------------------------------
//
void CPresenceEventCodec::PackL( CXIMPApiEventBase& aEventObj,
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
// CPresenceEventCodec::UnPackL()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CPresenceEventCodec::UnPackL( const TDesC8& aEventData,
                                                     TInt32& aTypeOfEvent )
    {
    RDesReadStream rs;
    rs.Open( aEventData ); // CSI: 65 #
    CleanupClosePushL( rs );

    TInt32 eventIfId = rs.ReadInt32L();
    CXIMPApiEventBase* eventObject = NewEventObjectFromStreamLC( eventIfId, rs );
    if(eventObject)
    	{
    	CleanupStack::Pop(); //eventObject
    	}
    CleanupStack::PopAndDestroy(); // rs


    aTypeOfEvent = eventIfId;
    return eventObject;
    }


// End of file

