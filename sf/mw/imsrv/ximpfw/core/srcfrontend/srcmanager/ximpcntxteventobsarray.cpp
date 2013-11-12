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
* Description:  Presence context observer array.
*
*/

#include "ximpcntxteventobsarray.h"
#include "ximpcontexteventfilter.h"
#include "ximpundotask.h"
#include <ximpcontextobserver.h>



// ============================ HELPER CLASS =================================

/**
 * Array entry helper holding pointer to actual observer and event
 * filter that is used to filter events delivered to the observer.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPCntxtEventObsArray::CObsEntry )
    : public CBase
    {
public:

    /**
     * Two phased constructor.
     *
     * @param aObserver The observer.
     * @param aAcceptedEvents The accepted events. May be NULL.
     */
    static CObsEntry* NewLC( MXIMPContextObserver& aObserver, // CSI: 42 #
                             const TArray< TInt32 >* aAcceptedEvents );


    /**
     * Destructor.
     */
    virtual ~CObsEntry();

private:

    CObsEntry( MXIMPContextObserver& aObserver );

public:

    /**
     * Gets the observer.
     * @return The observer.
     */
    MXIMPContextObserver& Observer();


    /**
     * Gets the event filter that should
     * be used to filter events delivered to observer.
     * @return The event filter.
     */
    const CXIMPContextEventFilter& EventFilter() const;



private: // data

    /**
     * The observer.
     * Not own.
     */
    MXIMPContextObserver& iObserver;


    /**
     * The event filter associated to the observer.
     * Own.
     */
    CXIMPContextEventFilter* iEventFilter;
    };





// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CObsEntry::NewLC()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray::CObsEntry*
    CXIMPCntxtEventObsArray::CObsEntry::NewLC(
    MXIMPContextObserver& aObserver,
    const TArray< TInt32 >* aAcceptedEvents )
    {
    CObsEntry* self = new( ELeave ) CObsEntry( aObserver );
    CleanupStack::PushL( self );

    self->iEventFilter = CXIMPContextEventFilter::NewLC( aAcceptedEvents );
    CleanupStack::Pop( self->iEventFilter );

    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CObsEntry::~CObsEntry()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray::CObsEntry::~CObsEntry()
    {
    delete iEventFilter;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CObsEntry::CObsEntry()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray::CObsEntry::CObsEntry(
    MXIMPContextObserver& aObserver )
    : iObserver( aObserver )
    {
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CObsEntry::Observer()
// ---------------------------------------------------------------------------
//
MXIMPContextObserver&
    CXIMPCntxtEventObsArray::CObsEntry::Observer()
    {
    return iObserver;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CObsEntry::EventFilter()
// ---------------------------------------------------------------------------
//
const CXIMPContextEventFilter&
    CXIMPCntxtEventObsArray::CObsEntry::EventFilter() const
    {
    return *iEventFilter;
    }



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::NewL()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray*
    CXIMPCntxtEventObsArray::NewL()
    {
    CXIMPCntxtEventObsArray* self = new( ELeave ) CXIMPCntxtEventObsArray();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CXIMPCntxtEventObsArray()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray::~CXIMPCntxtEventObsArray()
    {
    iObservers.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CXIMPCntxtEventObsArray()
// ---------------------------------------------------------------------------
//
CXIMPCntxtEventObsArray::CXIMPCntxtEventObsArray()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::AddObserverLC()
// ---------------------------------------------------------------------------
//
MXIMPUndoTask* CXIMPCntxtEventObsArray::AddObserverLC(
    MXIMPContextObserver& aObserver,
    const TArray< TInt32 >* aEventFilter )
    {
    __ASSERT_ALWAYS( !IsReqistered( aObserver ),
                     User::Leave( KErrAlreadyExists ) );

    //Create undo task
    CXIMPUndoPtrArrayEntryAdd< CObsEntry >* undoTask =
    CXIMPUndoPtrArrayEntryAdd< CObsEntry >::NewLC();

    //Create observer entry and add it to observer array
    CObsEntry* observerEntry = CObsEntry::NewLC( aObserver, aEventFilter );
    iObservers.AppendL( observerEntry );
    CleanupStack::Pop( observerEntry );

    //Initialize undo task to remove and destroy just added
    //observer entry from observer array on undoTask destruction
    undoTask->InitToRemoveAndDestroyOnUndo( *observerEntry, iObservers );
    return undoTask;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::UpdateObserverLC()
// ---------------------------------------------------------------------------
//
MXIMPUndoTask* CXIMPCntxtEventObsArray::UpdateObserverLC(
    MXIMPContextObserver& aObserver,
    const TArray< TInt32 >* aEventFilter )
    {
    __ASSERT_ALWAYS( IsReqistered( aObserver ),
                     User::Leave( KErrAlreadyExists ) );

    //Create undo task
    CXIMPUndoPtrArrayEntryReplace< CObsEntry >* undoTask =
    CXIMPUndoPtrArrayEntryReplace< CObsEntry >::NewLC();

    //Create entry & filter for observer
    CObsEntry* newEntry = CObsEntry::NewLC( aObserver, aEventFilter );
    CleanupStack::Pop( newEntry );

    //Replace existing entry with new one
    const TInt index = FindEntryPosition( aObserver );
    CObsEntry* oldEntry = iObservers[ index ];
    iObservers[ index ] = newEntry;

    //Initialize undo task to switch back to old entry in case of undo
    //Undo task takes care of deleting of the entry
    //which remains out of array
    undoTask->InitToReplaceOnUndo( *newEntry, iObservers, *oldEntry );
    return undoTask;
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::RemoveObserver()
// ---------------------------------------------------------------------------
//
void CXIMPCntxtEventObsArray::RemoveObserver(
    MXIMPContextObserver& aObserver )
    {
    const TInt entryPos = FindEntryPosition( aObserver );
    if( entryPos != KErrNotFound )
        {
        delete iObservers[ entryPos ];
        iObservers.Remove( entryPos );
        }
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::CollectEventFilterLC()
// ---------------------------------------------------------------------------
//
CXIMPContextEventFilter*
    CXIMPCntxtEventObsArray::CollectEventFilterLC()
    {
    RPointerArray < CXIMPContextEventFilter > tempArray;
    CleanupClosePushL( tempArray );
    for (TInt ii( 0 ); ii < iObservers.Count(); ii++ )
        {
        const CXIMPContextEventFilter* tempFilter = &(iObservers[ii]->EventFilter());
        tempArray.AppendL( tempFilter );
        }
    TArray <CXIMPContextEventFilter* > daArray = tempArray.Array();
    CXIMPContextEventFilter* retFilter = CXIMPContextEventFilter::NewCombinedLC( daArray );
    CleanupStack::Pop(); // retFilter
    CleanupStack::PopAndDestroy(); // tempArray
    CleanupStack::PushL( retFilter );
    return retFilter;
    }


// ---------------------------------------------------------------------------
// DeliverEventToObserver()
// ---------------------------------------------------------------------------
//
void DeliverEventToObserver( MXIMPContextObserver& aObsrver,
              const MXIMPContext& aCntxtInterface,
              const MXIMPBase& aEvent )
    {
    aObsrver.HandlePresenceContextEvent( aCntxtInterface, aEvent );
    }



// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::DeliverEvent()
// ---------------------------------------------------------------------------
//
void CXIMPCntxtEventObsArray::DeliverEvent(
    MXIMPContext& aCntxtInterface,
    MXIMPBase& aEvent )
    {
    TInt32 eventType = aEvent.GetInterfaceId();

    const TInt entryCount = iObservers.Count();
    for( TInt ix = 0; ix < entryCount; ix++ )
        {
        CObsEntry* obsEntry = iObservers[ ix ];
        if( obsEntry->EventFilter().IsEventAccepted( eventType ) )
            {
            MXIMPContextObserver& obs = obsEntry->Observer();
            __ASSERT_ALWAYS_NO_LEAVE( DeliverEventToObserver( obs,
                                                              aCntxtInterface,
                                                              aEvent ) );
            }
        }
    }



// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::HasObservers()
// ---------------------------------------------------------------------------
//
TBool CXIMPCntxtEventObsArray::HasObservers() const
    {
    return ( iObservers.Count() > 0 );
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::IsReqistered()
// ---------------------------------------------------------------------------
//
TBool CXIMPCntxtEventObsArray::IsReqistered(
    MXIMPContextObserver& aObserver ) const
    {
    return ( FindEntryPosition( aObserver ) != KErrNotFound );
    }


// ---------------------------------------------------------------------------
// CXIMPCntxtEventObsArray::FindEntryPosition()
// ---------------------------------------------------------------------------
//
TInt CXIMPCntxtEventObsArray::FindEntryPosition(
    MXIMPContextObserver& aObserver ) const
    {
    const TInt entryCount = iObservers.Count();
    for( TInt ix = 0; ix < entryCount; ix++ )
        {
        CObsEntry* observerEntry = iObservers[ ix ];
        if( &observerEntry->Observer() == &aObserver )
            {
            return ix;
            }
        }

    return KErrNotFound;
    }







