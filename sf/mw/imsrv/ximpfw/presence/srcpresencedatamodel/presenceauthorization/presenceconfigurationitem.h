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
* Description:  Presence configuration item
*
*/

#ifndef CPSCPRESENCECONFIGURATIONITEM_H
#define CPSCPRESENCECONFIGURATIONITEM_H

#include <e32base.h>
#include "ximpconfigurationitembase.h"

class CPresenceInfoFilterImp;
class MXIMPIdentity;
class CXIMPIdentityImp;

/**
 * Presence configuration item
 *
 * @since S60 v3.2
 */
class CPresenceConfigurationItem : public CXIMPConfigurationItemBase
    {
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CPresenceConfigurationItem* NewLC( MXIMPItemParentBase& aParent, const CXIMPIdentityImp& aIdentity );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CPresenceConfigurationItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CPresenceConfigurationItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL( const CXIMPIdentityImp& aIdentity );

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
    
    IMPORT_C const MXIMPIdentity& Identity() const;

    IMPORT_C void SetIdentity( CXIMPIdentityImp* aIdentity );
    
    /**
     * For ordering items.
     */
    IMPORT_C static TInt Order( const CPresenceConfigurationItem& aA,
                         const CPresenceConfigurationItem& aB );
    IMPORT_C static TInt IdentityOrder( const CXIMPIdentityImp* aA,
                         const CPresenceConfigurationItem& aB );
    
    
public: // From MXIMPSubscriptionItem

    /**
     * @see MXIMPSubscriptionItem
     */
    void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent );

public: // From MXIMPConfigurationItem

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
     * Identity of item
     * Own
     */
    CXIMPIdentityImp* iIdentity;
    };


/* ======================================================================== */



#endif // CPSCPRESENCECONFIGURATIONITEM_H
