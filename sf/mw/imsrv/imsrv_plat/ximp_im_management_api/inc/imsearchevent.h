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
* Description:  Event interface for ImSearch.
*
*/

#ifndef MIMSEARCHEVENT_H
#define MIMSEARCHEVENT_H

#include <e32std.h>
#include <ximpbase.h>
#include "immanagementifids.hrh"
class MImSearchInfo;


/**
 * Event interface for message content.
 *
 * This interface is implemented by the IM Feature Plugin and
 * event object is signalled to registered client observers
 * through MImSearchContextObserver observer interface.
 * This event notifies clients about changes in
 * search result.
 *
 * IM signals this event when:
 * - Search result is received from protocol.
 * @ingroup Immanagementapievents
 * 
 */
class MImSearchEvent : public MXIMPBase
    {
public:
    /** Interface ID for the MImSearchEvent. */
    enum { KInterfaceId = IM_IF_ID_SEARCH_EVENT };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImSearchEvent() {}


public: 

	
    /**
     * Gets the count of the searched results.
     *
     * @return The count of count of the searched results.
     */
    virtual TInt SearchResultCount() const = 0;
    
    
    /**
     * Gets the searched result by index.
     *
     * @param [in] aIndex
     *        The index of the search.
     *
     * @return The searched result from given index.
     */
    virtual const MImSearchInfo& SearchResult( TInt aIndex ) const = 0;
    
    
	

    };


#endif // MIMSEARCHEVENT_H


