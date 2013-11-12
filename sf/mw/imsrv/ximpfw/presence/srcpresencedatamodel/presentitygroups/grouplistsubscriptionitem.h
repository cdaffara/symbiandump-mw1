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

#ifndef CPSCGROUPLISTSUBSCRIPTIONITEM_H
#define CPSCGROUPLISTSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "presenceinfoimp.h"
#include "ximpsubscriptionitem.h"
#include "presencetypehelpers.h"
#include "ximpsubscriptionitembase.h"

class MXIMPItemParentBase;
class MXIMPPscContext;
class CDesC16Array;
class CPresentityGroupInfoImp;

/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CGroupListSubscriptionItem : public CXIMPSubscriptionItemBase
    {
    
public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CGroupListSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    ~CGroupListSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CGroupListSubscriptionItem( MXIMPItemParentBase& aParent );

    /**
     * Construction
     */
    void ConstructL();

public: // New methods

    /**
     * Set the new list of groups. Will create and set
     * the temporary lists for created, deleted and updated lists.
     * Ownership is transferred!
     * @param aGroupList The list of groups
     */
    IMPORT_C void SetNewListL( RPrGrpInfoImpArray* aGroupList );

    /**
     * Set the created list of groups. Will set
     * other lists accordingly. If there are no new elements (=no created groups),
     * will simply update the display names of the existing groups, i.e.
     * HandlePresentityGroupDisplayNameUpdatedL call handling.
     * Ownership is transferred!
     * @param aGroupList The list of groups
     */
    IMPORT_C void SetCreatedListL( RPrGrpInfoImpArray* aGroupList );

    /**
     * Set the deleted list of groups. Will set
     * other lists accordingly.
     * Ownership is transferred!
     * @param aGroupList The list of groups
     */
    IMPORT_C void SetDeletedListL( RPrGrpInfoImpArray* aGroupList );
    
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
    RPrGrpInfoImpArray* iCurrentList;

    /**
     * List of created groups (new since last update).
     * Owned.
     */
    RPrGrpInfoImpArray* iCreated;

    /**
     * List of deleted groups (removed since last update).
     * Owned.
     */
    RPrGrpInfoImpArray* iDeleted;

    /**
     * List of updated groups (same as in last update).
     * Owned.
     */
    RPrGrpInfoImpArray* iUpdated;

    };


/* ======================================================================== */



#endif // CPSCGROUPLISTSUBSCRIPTIONITEM_H
