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
* Description:  Presence Service Connection group list subscription implementation.
*
*/

#ifndef CPSCWATCHERLISTSUBSCRIPTIONITEM_H
#define CPSCWATCHERLISTSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitem.h"
#include "presencetypehelpers.h"
#include "ximpsubscriptionitembase.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CDesC16Array;
class CPresenceWatcherInfoImp;

/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CWatcherListSubscriptionItem : public CXIMPSubscriptionItemBase
    {

public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CWatcherListSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CWatcherListSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CWatcherListSubscriptionItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL();

public: // New methods

    /**
     * Resolve deltas and replace old current with new list.
     * @param aList New current list.
     */
    IMPORT_C void SetNewListL( RPrWatLstInfoImpArray* aList );
    
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
    RPrWatLstInfoImpArray* iCurrentList;

    /**
     * List of created groups (new since last update).
     * Owned.
     */
    RPrWatLstInfoImpArray* iNewWatchers;

    /**
     * List of deleted groups (removed since last update).
     * Owned.
     */
    RPrWatLstInfoImpArray* iDisappeared;

    };


/* ======================================================================== */



#endif // CPSCWATCHERLISTSUBSCRIPTIONITEM_H
