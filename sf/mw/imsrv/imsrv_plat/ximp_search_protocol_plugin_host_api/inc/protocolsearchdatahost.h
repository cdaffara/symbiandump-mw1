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
* Description:  Interface for Search Feature Plugin  host
 *
*/

#ifndef MPROTOCOLSEARCHDATAHOST_H
#define MPROTOCOLSEARCHDATAHOST_H

#include <ximpbase.h>
#include <searchprotocolpluginhostifids.hrh>
#include <e32std.h>

class MXIMPStatus;
class MSearchObjectFactory;
class MXIMPProtocolConnectionHostObserver;
class MProtocolImConversationDataHost;
class MProtocolImGroupDataHost;
class MProtocolImInvitationDataHost;
class MProtocolImSearchDataHost;
class MXIMPHost;

class MProtocolSearchFeatureDataHost;
/**
 * search connection host. 
 * Main interface class for protocol plugin.
 * protocol plugin can get sub interfaces to features from this.
 * 
 * @ingroup ProtocolSearchpluginhostapi
 */
 
 
class MProtocolSearchDataHost

    {
public:

    
    /** Interface ID for the MProtocolSearchDataHost. */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_SEARCH_PROCESSOR_INTERFACE };

    
protected:

    /**
     * Protected destructor. MProtocolSearchDataHost instancies
     * can't be destroyed via this interface.
     */
    virtual inline ~MProtocolSearchDataHost() {};


    

public: // Access to sub-interfaces


    /**
     * Gets reference to object factory interface.
     *
     * search protocol connection implementation
     * uses factory interface to instantiate Im
     * objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
   
	virtual MSearchObjectFactory& SearchObjectFactory() = 0;
	
	
	/**
     * Gets reference to
     * MProtocolSearchFeatureDataHost interface.
     *
     * Search protocol connection implementation uses returned
     * data host interface to manage the 
     * search data.
     *
     * @return MProtocolSearchFeatureDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */

    
    virtual MProtocolSearchFeatureDataHost& SearchFeatureDataAccess() =0;
    
    
    };


#endif // MPROTOCOLIMDATAHOST_H

