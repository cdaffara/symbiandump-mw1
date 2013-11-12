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
* Description:  Presence context event filter.
*
*/

#include "ximpcontexteventfilter.h"

const TInt KXIMPEventListGranurality = 3;


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextEventFilter* CXIMPContextEventFilter::NewLC( 
    const TArray< TInt32 >* aAcceptedEvents )
    {
    CXIMPContextEventFilter* self = 
        new( ELeave ) CXIMPContextEventFilter;
    CleanupStack::PushL( self );
    self->SetFilterL( aAcceptedEvents );
    return self;
    }

// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextEventFilter* CXIMPContextEventFilter::NewL( 
    const TArray< TInt32 >* aAcceptedEvents )
    {
    CXIMPContextEventFilter* self = NewLC( aAcceptedEvents );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::NewCombinedLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextEventFilter* 
    CXIMPContextEventFilter::NewCombinedLC( 
    const TArray< CXIMPContextEventFilter* >& aFilters )
    {
    CXIMPContextEventFilter* self = 
        new( ELeave ) CXIMPContextEventFilter;
    CleanupStack::PushL( self );
    self->CombineFilterL( aFilters );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::~CXIMPContextEventFilter()
// ---------------------------------------------------------------------------
//
CXIMPContextEventFilter::~CXIMPContextEventFilter()
    {
    delete iAcceptedEvents;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::CXIMPContextEventFilter()
// ---------------------------------------------------------------------------
//
CXIMPContextEventFilter::CXIMPContextEventFilter()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::IsEventAccepted()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPContextEventFilter::IsEventAccepted( 
    TInt32 aEvent ) const
    {
    TBool accepted = ETrue;


    //If there isn't iAcceptedEvents list limiting accepted event types,
    //all types are accepted by default
    if( iAcceptedEvents )
        {
        TInt pos;
        if( !FindEventPosition( aEvent, *iAcceptedEvents, pos ) )
            {
            accepted = EFalse; 
            }
        }

    return accepted;
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventFilter::ExternalizeL( 
    RWriteStream& aStream ) const
    {
    if( !iAcceptedEvents )
        {
        aStream.WriteInt32L( 0 );
        }
    else
        {
        TInt count = iAcceptedEvents->Count();        
        aStream.WriteInt32L( count );
        for( TInt ix = 0; ix < count; ++ix )
            {
            aStream.WriteInt32L( ( *iAcceptedEvents )[ ix ] );
            }
        }
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextEventFilter::InternalizeL( 
    RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    if( count )
        {
        //We are internalizing data which we have previously externalized 
        // => Internalized array can be directly set as current array
        
        CArrayFix< TInt32 >* events = 
                new (ELeave) CArrayFixFlat< TInt32 >( KXIMPEventListGranurality );
        CleanupStack::PushL( events );

        for( TInt ix = 0; ix < count; ++ix )
            {
            events->AppendL( aStream.ReadInt32L() );
            }
        delete iAcceptedEvents;
        iAcceptedEvents = events;

        CleanupStack::Pop( events );
        }

    else
        {
        delete iAcceptedEvents;
        iAcceptedEvents = NULL;
        }        
    }
    
    
// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::SetFilterL()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventFilter::SetFilterL(
    const TArray< TInt32 >* aAcceptedEvents )
    {
    if( aAcceptedEvents )
        {
        CArrayFix< TInt32 >* events = 
                new (ELeave) CArrayFixFlat< TInt32 >( KXIMPEventListGranurality );
        CleanupStack::PushL( events );

        CopyUniqueEventsL( *aAcceptedEvents, *events );

        delete iAcceptedEvents;
        iAcceptedEvents = events;
        CleanupStack::Pop( events );
        }

    else
        {
        //Set all accepted
        delete iAcceptedEvents;
        iAcceptedEvents = NULL;
        }
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::CombineFilterL()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventFilter::CombineFilterL(
    const TArray< CXIMPContextEventFilter* >& aFilters )
    {
    const TInt filterCount = aFilters.Count();

    CArrayFix< TInt32 >* combinedEvents = new (ELeave) CArrayFixFlat< TInt32 >( KXIMPEventListGranurality );
    CleanupStack::PushL( combinedEvents );

    for( TInt ix = 0; ix < filterCount; ix++ )
        {
        const CXIMPContextEventFilter* sourceFilter = aFilters[ ix ];
        if( sourceFilter->iAcceptedEvents )
            {
            CopyUniqueEventsL( sourceFilter->iAcceptedEvents->Array(),
                               *combinedEvents );            
            }

        else
            {
            //Set all accepted
            delete combinedEvents;
            combinedEvents = NULL;
            break;
            }
        }


    delete iAcceptedEvents;
    iAcceptedEvents = combinedEvents;

    CleanupStack::Pop(); //combinedEvents
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::CopyUniqueEventsL()
// ---------------------------------------------------------------------------
//
void CXIMPContextEventFilter::CopyUniqueEventsL(
    const TArray< TInt32 >& aSource, 
    CArrayFix< TInt32 >& aTarget )
    {

    const TInt eventCount = aSource.Count();

    for( TInt ix = 0; ix < eventCount; ix++ )
        {
        const TInt32 event = aSource[ ix ];
        TInt pos;
        if( !FindEventPosition( event, aTarget, pos ) )
            {
            aTarget.InsertL( pos, event );
            }
        }
    }


// ---------------------------------------------------------------------------
// CXIMPContextEventFilter::FindEventPosition()
// ---------------------------------------------------------------------------
//
TBool CXIMPContextEventFilter::FindEventPosition( 
    const TInt32 aEvent, const CArrayFix< TInt32 >& aEvents, TInt& aPos ) const
    {
    TKeyArrayFix key( 0, ECmpTInt32 );
    if( aEvents.FindIsq( aEvent, key, aPos ) == 0 )
        {
        //FindIsq() returns zero, if element is found => this is mapped to ETrue
        return ETrue;        
        }
    
    return EFalse;
    }

