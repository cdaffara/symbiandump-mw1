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
* Description:  Presence Service Connection group member subscription implementation.
*
*/

#ifndef CPSCGROUPCONTENTSUBSCRIPTIONITEM_H
#define CPSCGROUPCONTENTSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitembase.h"
#include "presencetypehelpers.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CDesC16Array;
class MXIMPIdentity;
class CXIMPIdentityImp;
class MGroupContentEventObserver;

/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CGroupContentSubscriptionItem : public CXIMPSubscriptionItemBase
    {

public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CGroupContentSubscriptionItem* NewLC( MXIMPItemParentBase& aParent,
                                                        const CXIMPIdentityImp& aGroupId );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    virtual ~CGroupContentSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CGroupContentSubscriptionItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL( const CXIMPIdentityImp& aGroupId );

public: // New methods

    /**
     * Add subscriber for group content if it hasn't subscribed already.
     * @param aContext. Context to be added.
     */
    IMPORT_C void AddOrRenewSubscriberL( MXIMPPscContext* aContext );

    /**
     * Set the new list of members. Will create and set
     * the temporary lists for added, removed and updated lists.
     * Ownership is transferred!
     * @param aMemberList The list of groups
     */
    IMPORT_C void SetNewListL( RPrGrpMemInfoImpArray* aMemberList );

    /**
     * Set the added list of members.
     * Ownership is transferred!
     * @param aMemberList The list of groups
     */
    IMPORT_C void SetAddedListL( RPrGrpMemInfoImpArray* aMemberList );

    /**
     * Set the removed list of members. Will set
     * other lists accordingly.
     * Ownership is transferred!
     * @param aMemberList The list of groups
     */
    IMPORT_C void SetRemovedListL( RPrGrpMemInfoImpArray* aMemberList );

    /**
     * Used as method for TLinearOrder when ordering items.
     * @param aA First object for ordering.
     * @param aB Second object for ordering.
     * @return Order of params like compare function( -1 0 1 )
     */
    IMPORT_C static TInt Order( const CGroupContentSubscriptionItem& aA,
                         const CGroupContentSubscriptionItem& aB );

    /**
     * Used as method for TLinearOrder when ordering items.
     * @param aA First object used as key for ordering.
     * @param aB Second object for ordering.
     * @return Order of params like compare function( -1 0 1 )
     */
    IMPORT_C static TInt IdentityOrder( const CXIMPIdentityImp* aA,
                         const CGroupContentSubscriptionItem& aB );
                         
    /**
     * Access to group id.
     * @return Group id.
     */
    IMPORT_C const CXIMPIdentityImp& GroupId() const;
    
    /**
     * Remove the temporary lists.
     */
    IMPORT_C void Clean();

    /**
     * Check if group member exists.
     * @param aMemberInfo Group member to check
     * @return ETrue if group member exists. EFalse if not.
     */    
    IMPORT_C TBool GroupMemberExistsL( CPresentityGroupMemberInfoImp& aMemberInfo ) const;
    
    /**
     * Check if group member is updated.
     * @param aMemberInfo Group member to check
     * @return ETrue if group member updated. EFalse if not.
     */
    IMPORT_C TBool IsGroupMemberUpdatedL( CPresentityGroupMemberInfoImp& aMemberInfo ) const;
    
public: // Methods for accessing array of current members

    /**
     * Count of current members
     * @return Count of current members
     */
    TInt MemberCount() const;
    
    /**
     * Access member in index.
     * @param aIndex. Index of group member info to get.
     * @return Group member info from index.
     */
    CPresentityGroupMemberInfoImp& MemberAt( TInt aIndex );
    
    /**
     * Register event observer
     * @param aEvent Observer New observer to observe events.
     */
    void RegisterEventObserverL( MGroupContentEventObserver* aEventObserver );
    
    /**
     * Unregister event observer
     * @param aEventObserver Canceling observer.
     */
    void UnregisterEventObserver( MGroupContentEventObserver* aEventObserver );
    
public: // From MXIMPSubscriptionItem

    /**
     * @see MXIMPSubscriptionItem
     */
    void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent );
    
    /**
     * @see MXIMPSubscriptionItem
     */
    void SynthesiseSubscriptionEventToAllCtxsL();
    
private: // From CXIMPSubscriptionItemBase
    
    /**
     * @see CXIMPSubscriptionItemBase
     */
    void CleanExpired();    

private: // Helper methods

    /**
     * Inform observer about change.
     */
    void InformObserversL();

private: // Data

    /**
     * Group identity
     * Own.
     */
    CXIMPIdentityImp* iGroupId;

    /**
     * Current members.
     * Own.
     */
    RPrGrpMemInfoImpArray* iCurrentMembers;

    /**
     * List of added members (new since last update).
     * Own.
     */
    RPrGrpMemInfoImpArray* iAdded;

    /**
     * List of removed members (removed since last update).
     * Own.
     */
    RPrGrpMemInfoImpArray* iRemoved;

    /**
     * List of updated members (same as in last update).
     * Own.
     */
    RPrGrpMemInfoImpArray* iUpdated;
    
    /**
     * Event observers of subscription item.
     */
    RPointerArray< MGroupContentEventObserver > iEventObservers;
    };


/* ======================================================================== */



#endif // CPSCGROUPCONTENTSUBSCRIPTIONITEM_H
