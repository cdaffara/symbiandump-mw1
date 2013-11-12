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
 * Description:  ?Description
 *
 */

#ifndef CASRVNOTIFIER_H
#define CASRVNOTIFIER_H

#include <e32base.h>
#include "cadef.h"

// FORWARD DECLARATION

class RMessage2;
class CCaInnerNotifierFilter;
class CCaInnerEntry;
class CDesC16ArrayFlat;
// CLASS DECLARATION

/**
 * Server side notifier object.
 */
NONSHARABLE_CLASS( CCaSrvNotifier ): public CBase
    {

public:
    
    /**
     * Destructor.
     */
    virtual ~CCaSrvNotifier();

    /**
     * Two phase constructor. Leaves on failure.
     */
    static CCaSrvNotifier* NewL();

    /**
     * Two phase constructor. Leaves on failure.
     */
    static CCaSrvNotifier* NewLC();

    /**
     * Notification requested.
     * @param aMessage Message requesting the notification.
     */
    void NotifyL( const RMessage2& aMessage );

    /**
     * Cancel request, if any. 
     * Base implementation completes the message with KErrCancel. 
     * Override and add real cancellation of outstanding requests.
     */
    virtual void Cancel();
    
    /**
     * Complete message if filter is correct.
     * @param aEntry was changed.
     * @param aChangeType - update, add, remowe.
     * @param aParentIds - array with parents ids.
     */
    void EntryChangedL( const CCaInnerEntry* aEntry, TChangeType aChangeType,
            const RArray<TInt>& aParentIds );

    /**
     * Complete message if filter is correct.
     * @param aId entry id.
     */
    void EntryTouchedL( TInt aId );

    /**
     * Complete message if filter is correct.
     * @param aParentIds entries(groups) id.
     */
    void GroupContentChangedL( const RArray<TInt>& aParentIds );

    /**
     * Get info from latest changes.
     * @param aMessage message from client.
     */
    void GetChangeInfoL( const RMessage2& aMessage );

private:
    
    /**
     * Constructor. Leaves on failure.
     */
    CCaSrvNotifier();

    /**
     * Second phase constructor. Leaves on failure.
     */
    void ConstructL();

    /**
     * Set this message as pending (take ownership of completion).
     * Call this as LAST thing in the request method,
     * and DO NOT LEAVE AFTER this is called.
     * Leaving after this method is called causes double completion of the
     * message (once by this object, once by the sessions ServiceError()).
     * @param aMessage Message to complete. Will be completed by this object.
     */
    void SetPendingL( const RMessage2& aMessage );

    /**
     * Panic aMessage and leave if this object is pending. 
     * Call this as first thing in request method. (Sanity checking.)
     * @param aMessage Message.
     */
    void PanicIfPendingL( const RMessage2& aMessage );

    /**
     * Completes the message, if pending. Does nothing if not pending.
     * @param aReason Completion code.
     */
    void Complete( TInt aReason = KErrNone );

    /**
     * Check filter for entry which was changed.
     * @param aEntry to check.
     * @param aParentIds - array with parents ids.
     * @return ETrue if entry is correct with filter.
     */
    TBool CheckFilterForEntry( const CCaInnerEntry* aEntry,
            const RArray<TInt>& aParentIds );

    /**
     * Prepare simply entry only with id.
     * @param aId entry id.
     */
    void AddEntryWithIdL( TInt aId );

    /**
     * If aIds array is not empty then it should contains aId.
     * @param aIds array with ids.
     * @param aId entry id which was changed.
     * @return EFalse if aIds is not empty and does not contain aId.
     */
    TBool CheckIds( const RArray<TInt>& aIds, TInt aId );

    /**
     * If aTypeNames array is not empty then it should contains aTypeName.
     * @param aTypeNames array with type names.
     * @param aTypeName entry type name which was changed.
     * @return EFalse if aTypeNames is not empty.
     * and does not contain aTypeName.
     */
    TBool CheckTypeName( const CDesC16ArrayFlat* aTypeNames,
            const RBuf& aTypeName );

private:
    // data

    /*
     * Pending. (Owner of an incompleted message).
     */
    TBool iPending;
    
    /*
     * The message (valid only if iPending). Not own.
     */
    RMessage2 iMessage; 
    
    /*
     * Filter. Own.
     */
    CCaInnerNotifierFilter* iFilter; 
    
    /*
     * Array with externalized changed entries. Own.
     */
    RPointerArray<HBufC8> iBufArrayInfo;
    
    /*
     * Array with change type. Own.
     */
    RArray<TChangeType> iArrayChangeType;
    };

#endif /* CASRVNOTIFIER_H */
