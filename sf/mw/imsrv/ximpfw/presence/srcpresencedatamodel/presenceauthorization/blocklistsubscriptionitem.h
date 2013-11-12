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
* Description:  Presence Service Connection presence block list subscription implementation.
*
*/

#ifndef CPSCBLOCKLISTSUBSCRIPTIONITEM_H
#define CPSCBLOCKLISTSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitem.h"
#include "presencetypehelpers.h"
#include "ximpsubscriptionitembase.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CDesC16Array;
class CPresenceGrantRequestInfoImp;

/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CBlockListSubscriptionItem : public CXIMPSubscriptionItemBase
    {

public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CBlockListSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CBlockListSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CBlockListSubscriptionItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL();

public: // New methods

    /**
     * Resolves deltas from new list and replaces old current.
     * Ownership is transferred!
     * @param aList New blocklist.
     */
    IMPORT_C void SetNewListL( RPrBlockInfoImpArray* aList );

    /**
     * Add new blocked object to current.
     * Ownership is transferred!
     * @param aReq Information of new blocked item.
     */
    IMPORT_C void SetBlockedL( CPresenceBlockInfoImp* aReq );
    
    /**
     * Cancels block from identity
     * Ownership is transferred!
     * @param aRemoved Identity of item not blocked anymore.
     */
    IMPORT_C void SetCanceledBlockIdL( CXIMPIdentityImp* aRemoved );

    /**
     * Remove the temporary lists.
     */
    IMPORT_C void Clean();

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
     * Current list of groups.
     * Owned.
     */
    RPrBlockInfoImpArray* iAdded;

    /**
     * List of created groups (new since last update).
     * Owned.
     */
    RPrBlockInfoImpArray* iCurrent;

    /**
     * List of deleted groups (removed since last update).
     * Owned.
     */
    RPrBlockInfoImpArray* iRemoved;

    /**
     * List of updated groups (removed since last update).
     * Owned.
     */
    RPrBlockInfoImpArray* iUpdated;

    };


/* ======================================================================== */



#endif // CPSCBLOCKLISTSUBSCRIPTIONITEM_H
