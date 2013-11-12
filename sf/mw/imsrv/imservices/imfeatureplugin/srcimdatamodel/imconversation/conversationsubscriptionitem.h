/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  IM Service Connection IM conversation subscription implementation.
*
*/

#ifndef CPSCCONVERSATIONSUBSCRIPTIONITEM_H
#define CPSCCONVERSATIONSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "ximpsubscriptionitem.h"
#include "imtypehelpers.h"
#include "ximpsubscriptionitembase.h"

class MXIMPIMItemParent;
class MXIMPPscContext;
class CXIMPPscContext;
class CDesC16Array;
class CImConversationInfoImp;
class MImConversationInfo;
/**
 * PSC group list subscription implementation
 *
 * @since S60 v3.2
 */
class CConversationSubscriptionItem : public CXIMPSubscriptionItemBase
    {

public: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    static CConversationSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    IMPORT_C virtual ~CConversationSubscriptionItem();

private: // Construction and destruction

    /**
     * Construction
     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
     */
    CConversationSubscriptionItem( MXIMPItemParentBase& aParent );

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
    IMPORT_C void  SetConversationInfoL(CImConversationInfoImp* aImMessage);
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

private : 
     /**
     *  conversation info :Not owned
     */
	
    CImConversationInfoImp* iConvinfo;
    
    //MXIMPItemParentBase& iParent;

    };


/* ======================================================================== */



#endif // CPSCCONVERSATIONSUBSCRIPTIONITEM_H
