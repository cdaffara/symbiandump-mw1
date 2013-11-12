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
* Description:  Presence Service Connection own presence subscription implementation.
*
*/

#ifndef CPSCOWNPRESENCESUBSCRIPTIONITEM_H
#define CPSCOWNPRESENCESUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitem.h"
#include "ximpsubscriptionitembase.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CPresenceInfoFilterImp;
class COwnPresenceEventImp;

/**
 * PSC own presence subscription implementation
 *
 * @since S60 v3.2
 */
class COwnPresenceSubscriptionItem : public CXIMPSubscriptionItemBase
    {
    
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static COwnPresenceSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    virtual ~COwnPresenceSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    COwnPresenceSubscriptionItem( MXIMPItemParentBase& aParent );
    
    /**
     * Construction
     */
    void ConstructL();
    
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
     * Collect aggregated presence information filter wihtout given context.
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
            
public: // From MXIMPSubscriptionItem

    /**
     * Synthesise own presence information to given context.
     * Presence information is filtered with context pif.
     * @param aContext. Target context for own presence information.
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
     * Current presence info
     */
    CPresenceInfoImp* iPresenceInfo;
    };


/* ======================================================================== */



#endif // CPSCOWNPRESENCESUBSCRIPTIONITEM_H
