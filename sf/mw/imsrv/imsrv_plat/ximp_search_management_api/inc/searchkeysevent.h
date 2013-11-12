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
* Description:  Event interface for Search keys event.
*
*/

#ifndef MSEARCHKEYSEVENT_H
#define MSEARCHKEYSEVENT_H

#include <ximpbase.h>
#include <searchmanagementifids.hrh>

class MSearchKeyInfo;

/**
 * Event interface for search content.
 *
 * This interface is implemented by the Search Feature Plugin 
 * This event notifies clients 
 * when Search Keys are received from protocol.
 * 
 * @ingroup searchmanagementapi
 */
class MSearchKeysEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MSearchKeysEvent. */
    enum { KInterfaceId = SEARCH_IF_ID_SEARCH_KEYS_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual inline ~MSearchKeysEvent() {};
    
public :
    /**
     * returns the count of the  search key info count.
     *
     * @return The count of he supported search keys.
     */
    virtual TInt InfoCount() const =0;
    
    /**
     * Gets the search key info object by index.
     *
     * @param [in] aIndex
     *        The index of the array.
     *
     * @return The search key info object from given index.
     */    
    virtual const MSearchKeyInfo& SearchKeyInfo( TInt aIndex ) const = 0;
    
    
    };


#endif // MSEARCHKEYSEVENT_H


