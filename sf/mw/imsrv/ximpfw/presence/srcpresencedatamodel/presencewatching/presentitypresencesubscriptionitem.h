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
* Description:  Presence Service Connection presentity presence subscription implementation.
*
*/

#ifndef CPSCPRESENTITYPRESENCESUBSCRIPTIONITEM_H
#define CPSCPRESENTITYPRESENCESUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitembase.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CPresenceInfoFilterImp;
class MXIMPIdentity;
class CXIMPIdentityImp;


/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CPresentityPresenceSubscriptionItem : public CXIMPSubscriptionItemBase
    {
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CPresentityPresenceSubscriptionItem* NewLC( MXIMPItemParentBase& aParent, 
                                                              const CXIMPIdentityImp& aIdentity );
    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CPresentityPresenceSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CPresentityPresenceSubscriptionItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL( const CXIMPIdentityImp& aIdentity );

public: // New methods


    /**
     * Current presence information filter.
     * @return Current presence information filter.
     */
    IMPORT_C CPresenceInfoFilterImp& SubscriptionPif();
    
    /**
     * Set current presence information filter. Overwrites existing one.
     * Ownership is transferred to callee.
     * @param aPif Current presence information filter.
     */
    IMPORT_C void SetSubscriptionPif( CPresenceInfoFilterImp* aPif );

    /**
     * Collect aggregated presence information filter without given context.
     * @param aContext. Exluded context from aggregated pif.
     * @return Aggregated pif without exluded context.
     */    
    IMPORT_C CPresenceInfoFilterImp* CollectSubscriptionPifWithoutCtxL( 
                                                    MXIMPPscContext* aContext );
    
    /**
     * Set presence information. Overwrites existing one.
     * @param aPresenceInfo. New presence information.
     */
    IMPORT_C void SetPresenceInfoL( CPresenceInfoImp* aPresenceInfo );
    
    
    /**
     * For ordering items.
     * @param aA Key item for ordering.
     * @param aB Item to compare
     * @return -1 if aB is greater than aA
     *          0 if they are same
     *          1 if aA if greater than aB.
     */    
    IMPORT_C static TInt Order( const CPresentityPresenceSubscriptionItem& aA,
                         const CPresentityPresenceSubscriptionItem& aB );

    /**
     * For ordering items using identity as key.
     * @param aA Key identity for ordering.
     * @param aB Item to compare
     * @return -1 if aB is greater than aA
     *          0 if they are same
     *          1 if aA if greater than aB.
     */    
    IMPORT_C static TInt IdentityOrder( const CXIMPIdentityImp* aA,
                         const CPresentityPresenceSubscriptionItem& aB );
    /**
     * Identity of item.
     * @return Identity of item.     
     */
    IMPORT_C const MXIMPIdentity& Identity() const;

    /**
     * Group Identity of item.
     * @return the Group identity of item.     
     */
    IMPORT_C const MXIMPIdentity* GroupIdentity() const;

    /**
     * Set identity for item
     * @param aIdentity. New identity for item. Ownership is transferred.(??)
     */
    IMPORT_C void SetIdentity( CXIMPIdentityImp* aIdentity );
    
     /**
     * Set group identity for item
     * @param aGroup. New identity for item. Ownership is transferred.
     */
    IMPORT_C void SetGroupIdentity( CXIMPIdentityImp* aGroup );

    
public: // From MXIMPSubscriptionItem

    /**
     * @see MXIMPSubscriptionItem
     */
    void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent );
    
private: // From CXIMPSubscriptionItemBase

    /**
     * @see CXIMPSubscriptionItemBase
     */
    void CleanExpired();

private: // Data

    /**
     * Current presence information filter.
     */
    CPresenceInfoFilterImp* iPif;
        
    /**
     * Current presence info for item
     * Own
     */
    CPresenceInfoImp* iPresenceInfo;
    
    /**
     * Identity of item
     * Own
     */
    CXIMPIdentityImp* iIdentity;

    /**
     * Identity of group item
     * TODO: must be an array
     * own
     */
    CXIMPIdentityImp* iGroupIdentity;
    };


/* ======================================================================== */



#endif // CPSCPRESENTITYPRESENCESUBSCRIPTIONITEM_H
