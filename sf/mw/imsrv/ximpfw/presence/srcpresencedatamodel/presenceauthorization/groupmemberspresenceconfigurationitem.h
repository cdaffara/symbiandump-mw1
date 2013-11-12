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
* Description:  Group members presence configuration item
*
*/

#ifndef CPSCGROUPMEMBERSPRESENCECONFIGURATIONITEM_H
#define CPSCGROUPMEMBERSPRESENCECONFIGURATIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpconfigurationitembase.h"
#include "groupcontenteventobserver.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CPresenceInfoFilterImp;
class MXIMPIdentity;
class CXIMPIdentityImp;
class CGroupContentSubscriptionItem;

/**
 * Group members presence configuration item
 *
 * @since S60 v3.2
 */
class CGroupMembersPresenceConfigurationItem : public CXIMPConfigurationItemBase, 
                                               public MGroupContentEventObserver
    {
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CGroupMembersPresenceConfigurationItem* NewLC( MXIMPItemParentBase& aParent,
                                                          CGroupContentSubscriptionItem& aContentItem );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CGroupMembersPresenceConfigurationItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CGroupMembersPresenceConfigurationItem( MXIMPItemParentBase& aParent, 
                                            CGroupContentSubscriptionItem& aContentItem );

    /**
     * Construction
     */
    void ConstructL();

public: // New methods


    /**
     * Current presence information filter.
     * @return Current presence information filter.
     */
    IMPORT_C CPresenceInfoFilterImp& ConfigurationPif();
    
    /**
     * Set current presence information filter. Overwrites existing one.
     * Ownership is transferred to callee.
     * @param aPif Current presence information filter.
     */
    IMPORT_C void SetConfigurationPif( CPresenceInfoFilterImp* aPif );

    /**
     * Collect aggregated presence information filter without given context.
     * @param aContext. Exluded context from aggregated pif.
     * @return Aggregated pif without exluded context.
     */    
    IMPORT_C CPresenceInfoFilterImp* CollectConfigurationPifWithoutCtxL( 
                                                    MXIMPPscContext* aContext );
    
    /**
     * For ordering items.
     * @param aA Key item for ordering.
     * @param aB Item to compare
     * @return -1 if aB is greater than aA
     *          0 if they are same
     *          1 if aA if greater than aB.
     */    
    IMPORT_C static TInt Order( const CGroupMembersPresenceConfigurationItem& aA,
                         const CGroupMembersPresenceConfigurationItem& aB );
    /**
     * For ordering items using identity as key.
     * @param aA Key identity for ordering.
     * @param aB Item to compare
     * @return -1 if aB is greater than aA
     *          0 if they are same
     *          1 if aA if greater than aB.
     */    
    IMPORT_C static TInt IdentityOrder( const CXIMPIdentityImp* aA,
                         const CGroupMembersPresenceConfigurationItem& aB );
    /**
     * Identity of item.
     * @return Identity of item.     
     */
    IMPORT_C const MXIMPIdentity& Identity() const;

    /**
     * Set identity for item
     * @param aIdentity. New identity for item. Ownership is transferred.
     */
    IMPORT_C void SetIdentity( CXIMPIdentityImp* aIdentity );
    
public: // From MXIMPSubscriptionItem

    /**
     * @see MXIMPSubscriptionItem
     */
    void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent );
    void ActivateSubscriberL( MXIMPPscContext* aContext );
    
private: // From CXIMPSubscriptionItemBase

    /**
     * @see CXIMPSubscriptionItemBase
     */
    void CleanExpired();

public: // From CXIMPConfigurationItemBase

    /**
     * @see CXIMPConfigurationItemBase
     */
    IMPORT_C void AddConfiguratorL( MXIMPPscContext* aContext );
    IMPORT_C void RemoveConfigurator( MXIMPPscContext* aContext );    
        
private: // From MGroupContentSubscriptionEventObserver    

    /** 
     * @see MGroupContentSubscriptionEventObserver
     */
    void HandleChangeL( RPrGrpMemInfoImpArray& aAdded, RPrGrpMemInfoImpArray& aRemoved );
    void HandleSynthesiseL( RPrGrpMemInfoImpArray& aAdded );
    
private: // Data

    /**
     * Current presence information filter.
     * Own
     */
    CPresenceInfoFilterImp* iPif;
        
    /**
     * Identity of item.
     * Own
     */
    CXIMPIdentityImp* iIdentity;
    
    /**
     * Owns reference to content item. Use close for delete.
     */
    CGroupContentSubscriptionItem& iContentItem;
    };


/* ======================================================================== */



#endif // CPSCGROUPMEMBERSPRESENCECONFIGURATIONITEM_H
