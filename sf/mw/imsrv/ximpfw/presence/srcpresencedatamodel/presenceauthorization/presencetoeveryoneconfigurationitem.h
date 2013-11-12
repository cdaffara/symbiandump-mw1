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
* Description:  Presence to everyone configuration item
*
*/

#ifndef CPSCPRESENCETOEVERYONECONFIGURATIONITEM_H
#define CPSCPRESENCETOEVERYONECONFIGURATIONITEM_H

#include <e32base.h>
#include "ximpconfigurationitembase.h"

class CPresenceInfoFilterImp;

/**
 * Presence to everyone configuration item
 *
 * @since S60 v3.2
 */
class CPresenceToEveryoneConfigurationItem : public CXIMPConfigurationItemBase
    {
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CPresenceToEveryoneConfigurationItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CPresenceToEveryoneConfigurationItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CPresenceToEveryoneConfigurationItem( MXIMPItemParentBase& aParent );

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
    };


/* ======================================================================== */



#endif // CPSCPRESENCETOEVERYONECONFIGURATIONITEM_H
