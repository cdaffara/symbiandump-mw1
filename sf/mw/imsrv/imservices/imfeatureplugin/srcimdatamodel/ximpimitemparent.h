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
* Description:  Item parent interface.
*
*/

#ifndef CXIMPIMITEMPARENT_H
#define CXIMPIMITEMPARENT_H

#include <ximpitemparentbase.h>

class CConversationSubscriptionItem;
class CXIMPIMSubscriptionItemBase;
class CXIMPApiEventBase;
class MXIMPPscContext;
class CXIMPIdentityImp;
class CXIMPConfigurationItemBase;

//FIX ME
#define IM_ITEM_PARENT	0xE0000008

/**
 * Item parent interface.
 *
 * @since S60 v3.2
 */
class MXIMPIMItemParent : public MXIMPItemParentBase
    {
    
	public:
		enum { KInterfaceId = IM_ITEM_PARENT};
	
	public: // Definitions

    /**
     * Remove item
     * @param aChild Item pointer to remove.
     */
    virtual void RemoveMe( CConversationSubscriptionItem* aChild ) = 0;

    /**
     * Add event
     */
    virtual void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext ) = 0;

    
protected: // Destruction
    
    /**
     * Object cannot be delted through this interface
     */
    virtual ~MXIMPIMItemParent(){}
    };

/* ======================================================================== */



#endif // CXIMPIMITEMPARENT_H
