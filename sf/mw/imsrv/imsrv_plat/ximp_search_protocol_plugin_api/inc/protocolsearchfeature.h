/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for search feature Protocol  object.
*
*/

#ifndef MPROTOCOLSEARCHFEATURE_H
#define MPROTOCOLSEARCHFEATURE_H

#include <ximpbase.h>
#include <searchmanagementifids.hrh>
#include <ximpcontext.h>



class MProtocolSearch;
class MProtocolSearchDataHost;



/**
 * .
 * Interface must be implemented by a XIMP Framework
 * protocol plug-ins.
 *
 * @ingroup protocolsearchpluginapi
 *  
 */
class MProtocolSearchFeature : public MXIMPBase
    {
    
public:

    /** Interface ID for the MProtocolSearchFeature. */
    enum { KInterfaceId = SEARCH_IF_ID_PROTOCOL_SEARCH_FEATURE };



    /**
     * Protected destructor. MXIMPProtocolConnection
     * objects can't be deleted through this interface.
     *
     * XIMP Framework disposes MXIMPProtocolConnection
     * instances through MXIMPProtocolPlugin::ReleaseConnection().
     */
    virtual inline ~MProtocolSearchFeature()  {};
 
    


public: // Access to sub-interfaces

    /**
     * Gets reference to  search interface.
     *
     * search calls this method to retrieve a reference
     * to protocols MProtocolSearch interface
     * implementation.
     *
     * Call and interface lifetime convention:
     * - This method isn't called before protocol has signalled
     *   MXIMPProtocolConnection::OpenSessionL() succesfully completed.
     *
     * - XIMP Framework may call this method multiple times for
     *   for one MXIMPProtocolConnection instance.
     *
     * - Each call per one MXIMPProtocolConnection instance,
     *   must return reference to same interface instance.
     *
     * - Returned interface instance must remain valid (usable)
     *   untill last client session, using originating im protocol
     *   connection is successfully closed with
     *   MXIMPProtocolConnection::CloseSession().
     *
     * @return Requested interface.
     *         interface ownership is not returned caller.
     */
    virtual MProtocolSearch& Search() = 0;
    
    
   /**
     * Sets reference of serach datahost.
     *
     * @param aSearchHost 
     *			MProtocolSearchDataHost object reference.
     *			ownership is not transfered.
     */
    virtual void SetHost(MProtocolSearchDataHost& aSearchHost) = 0;

    };


#endif // MPROTOCOLSEARCHFEATURES_H
