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

#ifndef CSEARCHITEMPARENT_H
#define CSEARCHITEMPARENT_H

#include <ximpitemparentbase.h>


class CXIMPApiEventBase;
class MXIMPPscContext;
class CSearchSubscriptionItem;
class CSearchKeysSubscriptionItem;


#define SEARCH_ITEM_PARENT	0xE000000A

/**
 * Item parent interface.
 *
 * 
 */
class MSearchItemParent : public MXIMPItemParentBase
    {
    
	public:
		enum { KInterfaceId = SEARCH_ITEM_PARENT};
	
	public: // Definitions

    /**
     * Remove item
     * @param aChild Item pointer to remove.
     */
    virtual void RemoveMe( CSearchSubscriptionItem* aChild ) = 0;
    
    virtual void RemoveMe( CSearchKeysSubscriptionItem* aChild ) = 0;

    /**
     * Add event to event pump
     */
    virtual void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext ) = 0;

    
protected: // Destruction
    
    /**
     * Object cannot be delted through this interface
     */
    virtual ~MSearchItemParent(){}
    };

/* ======================================================================== */



#endif // CXIMPIMITEMPARENT_H
