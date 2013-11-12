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

#ifndef XIMPPRESCNTXTEVENTOBSARRAY_H
#define XIMPPRESCNTXTEVENTOBSARRAY_H

#include <e32base.h>

class MXIMPContextObserver;
class MXIMPContext;
class RXIMPSrvContextClient;
class CXIMPContextEventFilter;
class MXIMPUndoTask;
class MXIMPBase;


/**
 * Presence context observer array.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPCntxtEventObsArray )
    : public CBase
    {
    /** Private nested helper classes */
    class CObsEntry;


public:

    static CXIMPCntxtEventObsArray* NewL();
    virtual ~CXIMPCntxtEventObsArray();

private:

    CXIMPCntxtEventObsArray();


public:

    /**
     * Adds new observer to observer list.
     *
     * @param aObserver Client side observer to add.
     * @param aEventFilter Event filter for observer. Can be NULL.
     *
     * @return Undo object. When undo is activated, removes 
     *         just added observer from observers list. 
     */
    MXIMPUndoTask* AddObserverLC( MXIMPContextObserver& aObserver,
                                  const TArray< TInt32 >* aEventFilter );


    /**
     * Updates observer event filter.
     *
     * @param aObserver Client side observer which filter to update. 
     * @param aEventFilter New event filter for observer. Can be NULL.
     *
     * @return Undo object. When undo is activated, takes old 
     *         event filter in to use for observer. 
     */
    MXIMPUndoTask* UpdateObserverLC( MXIMPContextObserver& aObserver,
                                     const TArray< TInt32 >* aEventFilter );


    /**
     * Removes given observer from observers list.
     *
     * @param aObserver Client side observer to remove. 
     */
    void RemoveObserver( MXIMPContextObserver& aObserver );


    /**
     * Collects event filter from currently registered 
     * observers.
     *
     * @return Collected event filter instance.
     */
    CXIMPContextEventFilter* CollectEventFilterLC();


    /**
     * Delivers given event to registered observers.
     *
     * @param aCntxtInterface Source context interface for event.
     * @param aEvent The event object to deliver.
     */
    void DeliverEvent( MXIMPContext& aCntxtInterface,
                       MXIMPBase& aEvent );

    /**
     * Checks is there observers registered.
     * @return ETrue if there is observer registered. Else EFalse. 
     */
    TBool HasObservers() const;


    /**
     * Checks is the given observer registered or not.
     *
     * @param aObserver Observer to check.
     * @return ETrue if observer is registered. Else EFalse.
     */
    TBool IsReqistered( MXIMPContextObserver& aObserver ) const;



private:

    TInt FindEntryPosition( MXIMPContextObserver& aObserver ) const;


private: // data

    /**
     * The observers.
     * Own.
     */
    RPointerArray< CObsEntry > iObservers;
    };



#endif // XIMPPRESCNTXTEVENTOBSARRAY_H
