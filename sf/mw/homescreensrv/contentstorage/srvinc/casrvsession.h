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

#ifndef CASRVSESSION_H
#define CASRVSESSION_H

//  INCLUDES

#include <e32base.h>
#include <e32hashtab.h>
#include "casrv.h"
#include "cadef.h"

// CLASS DECLARATION
class CCaSrvNotifier;
class CCaInnerEntry;
class CCaDataManager;
class CCaInnerQuery;

/**
 * Interface for session changing events.
 *
 * @since S60 v5.0
 */
class MCaSessionNorifier
    {

public:
    
    /**
     * Pure virtual method.
     * @param aEntry was changed.
     * @param aChangeType - update, add, remove.
     * @param aParentIds - array with parents ids.
     */
    virtual void EntryChanged( const CCaInnerEntry* aEntry, TChangeType aChangeType,
            const RArray<TInt>& aParentIds ) = 0;

    /**
     * Pure virtual method.
     * @param aId entry(group) id.
     */
    virtual void EntryTouched( TInt aId ) = 0;

    /**
     * Pure virtual method.
     * @param aParentIds entries(groups) id.
     */
    virtual void GroupContentChanged( const RArray<TInt>& aParentIds ) = 0;
    };

/**
 * Content Arsenal Server Session.
 */
NONSHARABLE_CLASS( CCaSrvSession ): public CSession2, public MCaSessionNorifier
    {
    enum TGetListType
        {
        EFull,
        EIds
        };

public:
    
    /**
     * Destructor.
     */
    virtual ~CCaSrvSession();

    /**
     * Two phase constructor. Leaves on failure.
     * @param aMenuSrv Menu Server.
     */
    static CCaSrvSession* NewL( CCaSrv& aCaSrv );

protected:
    // construct

    /**
     * Constructor. Leaves on failure.
     * @param aMenuSrv Menu Server.
     */
    CCaSrvSession( CCaSrv& aCaSrv );

    /**
     * Second phase constructor. Leaves on failure.
     */
    void ConstructL();

private:
    // From CSession2

    /**
     * Service message.
     * @param aMessage Message.
     */
    virtual void ServiceL( const RMessage2& aMessage );

private:
    //new methods

    /**
     * Selects correct function from message.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     * @param aPanicedClient returns ETrue 
     * if server paniced a client.
     */
    void DispatchMessageL( const RMessage2& aMessage,
            TBool& aPanicedClient );

    /**
     * Get Data request - first phase.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     * @param aOnlyIds ETrue if array of ids is going to be fetched,
     * EFalse if entries array will be fetched.
     */
    void GetListSizeL( const RMessage2& aMessage, TGetListType aType );

    /**
     * Gets entries from the storage.
     * @param aQuery query.
     * @return size of serialized descriptor containing output entries.
     */
    TInt GetEntriesL( const CCaInnerQuery* aQuery );
    
    /**
     * Gets entries ids from the storage.
     * @param aQuery query.
     * @return size of serialized descriptor containing output ids.
     */
    TInt GetIdsL( const CCaInnerQuery* aQuery );

    /**
     * Get Data request - second phase.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void GetListDataL( const RMessage2& aMessage );

    /**
     * Get Entries Ids request - second phase.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void GetIdsDataL( const RMessage2& aMessage );

    /**
     * Add data request.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void AddL( const RMessage2& aMessage);

    /**
     * Remove data request.
     * @param aMessage message from client
     * (containing requested operation and any data).
     */
    void RemoveL( const RMessage2& aMessage );

    /**
     * Organize data request.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void OrganizeL( const RMessage2& aMessage );

    /**
     * Touch data request.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void TouchL( const RMessage2& aMessage );
    
    /**
     * Custom sort data request.
     * @param aMessage message from client 
     * (containing requested operation and any data).
     */
    void CustomSortL( const RMessage2& aMessage );

    /**
     * Create new notifier object,
     * write its subsession handle back in slot 3.
     * @param aMessage Message.
     */
    void NewNotifierL( const RMessage2& aMessage );

    /**
     * Get notifier by handle. Leaves with KErrBadHandle (==panics client)
     * if not found.
     * @param aHandle Handle.
     * @return Notifier.
     */
    CCaSrvNotifier* NotifierL( TInt aHandle );

    /**
     * Remove notifier by handle from message.
     * Leaves with KErrBadHandle (==panics client) if not found.
     * @param aMessage Message.
     */
    void RemoveNotifierL( const RMessage2& aMessage );

    /**
     * Get Change Info from notifier.
     * @param aMessage Message.
     */
    void GetChangeInfoL( const RMessage2& aMessage );

public:
    // from MCaSessionNorifier

    /**
     * Call EntryChangeL for each one CCaSrvNotifier object.
     * @param aEntry was changed.
     * @param aChangeType - update, add, remove.
     * @param aParentIds - array with parents ids.
     */
    void EntryChanged( const CCaInnerEntry* aEntry, TChangeType aChangeType,
            const RArray<TInt>& aParentIds );

    /**
     * Call EntryTouchedL for each one CCaSrvNotifier object.
     * @param aId entry(group) id.
     */
    void EntryTouched( TInt aId );

    /**
     * Call GroupContentChangedL for each one CCaSrvNotifier object.
     * @param aParentIds entries(groups) id.
     */
    void GroupContentChanged( const RArray<TInt>& aParentIds );

private:
    // Data

    /*
     * Server. Not own.
     */
    CCaSrv& iCaSrv;

    /*
     * Data Manager. Own.
     */
    CCaDataManager* iDataManager;
    
    /**
     * Hash map containing all CCaSrvNotifier of this session.
     */
    RHashMap<TInt, CCaSrvNotifier*> iNotifiers;
    
    /**
     * Data storage. Not own.
     */
    CCaStorageProxy& iStorageProxy;
    

    /**
     * Serialized GetEntries operation result. Own
     */
    HBufC8* iSerializedGetEntriesResult;

    /**
     * Serialized GetIds operation result. Own
     */
    HBufC8* iSerializedGetIdsResult;
    };

#endif // CASRVSESSION_H
// End of File
