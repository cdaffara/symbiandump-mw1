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
* Description:  Header for MXIMPHost
*
*/



#ifndef M_PSCHOST_H
#define M_PSCHOST_H

#include <ximpbase.h>

#include "ximphoststates.h"
#include "ximpoperationdefs.h"

class MXIMPProtocolConnection;
class CXIMPApiEventBase;
class MXIMPPscContext;
class CXIMPPscContext;
class CXIMPServiceInfoImp;
class CPresenceDataCache;
class CXIMPOperationBase;
class CXIMPRestrictedObjectCollectionImp;
class CProtocolPresenceAuthorizationDataHostImp;
class CProtocolPresencePublishingDataHostImp;
class CProtocolPresenceWatchingDataHostImp;
class CProtocolPresentityGroupsDataHostImp;
class CProtocolPresenceDataHostImp;
class MProtocolDataHost;

/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class MXIMPHost
    {  
public:

    /**
     * Mark a reference to this object.
     */
    virtual void Open() = 0;

    /**
     * Remove a reference to this object.
     * Once all references are removed, the class destroys itself.
     */
    virtual void Close() = 0;

    /**
     * @return The currently used connection, Can be NULL if protocol not in use.
     */
    virtual MXIMPProtocolConnection& GetConnection() const = 0;

    /**
     * Add the given event to the event manager. The event will
     * go to the given session.
     * @param aEvent The event
     * @param aContext The session context. If NULL -> send to all contexts.
     */
    virtual void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext = NULL ) = 0;
    
    /**
     * Add preserved event to the event manager. The event will 
     * go to the given session.
     * @param aEvent The event
     * @param aContext The session context
     */
    virtual void ActivateEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext ) = 0;

    /**
     * Unregister session when not needed anymore.
     * @param aContext Unregistered context.
     */
    virtual void UnregisterSession( CXIMPPscContext* aContext ) = 0;

    /**
     * Fake a request complete event to
     * currently run operation.
     * @param aResult The result
     */
    virtual void FakeCompleteRequest( TXIMPRequestId aReqId, TInt aResult ) = 0;

    /**
     * Request complete for adaptation operation
     * @param aResult The result    
     */    
    virtual void HandleAdaptationRequestCompleted( TXIMPRequestId aReqId, TInt aResult ) = 0;
    
    /**
     * Request complete for adaptation operation, related
     * to session management
     * @param aResult The result    
     */    
    virtual void HandleSessionMgmtRequestCompleted( TXIMPRequestId aReqId, TInt aResult ) = 0;

    /**
     * Request complete for internal operation
     * @param aResult The result    
     */    
    virtual void HandleInternalRequestCompleted( TXIMPRequestId aReqId, TInt aResult ) = 0;
    
    /**
     * Access to data cache.
     */
    virtual CProtocolPresenceDataHostImp& PresenceDataAccess() = 0;

    /**
     * to get corresponding protocol host from the array of hosts.
     * @param aInterfaceId host object interface id
     */
	virtual TAny* GetProtocolHost(TInt aInterfaceId) = 0 ;

/*    virtual CPresenceDataCache& DataAccess() = 0;
    virtual CProtocolPresenceAuthorizationDataHostImp& AuthorizationDataAccess() = 0;
    virtual CProtocolPresencePublishingDataHostImp& PublishingDataAccess() = 0;
    virtual CProtocolPresenceWatchingDataHostImp& WatchingDataAccess() = 0;
    virtual CProtocolPresentityGroupsDataHostImp& GroupsDataAccess() = 0;
*/    
    
    
    /**
     * Add new operation to the internal queue of operations. Ownership is transferred.
     * @param aOperation The operation to add.
     */
    virtual void AddNewOperationL( CXIMPOperationBase& aOperation ) = 0;

    /** 
     * Add new operation for processing.
     */
    virtual TXIMPRequestId AddNewOperationL( CXIMPOperationBase* aOperation ) = 0;
        
    /**
     * Do forced teardown by activating all valid cached unbinds.
     */
    virtual void ForcedTeardownL() = 0;
    
    /**
     * Start cleaner timer if not started already.
     */
    virtual void StartCleanerTimer() = 0;    

    /**
     * Create operation and add it to process queue.
     */    
    virtual TXIMPRequestId AddNewOperationL( 
                TInt aOperation, MXIMPBase* aParam, MXIMPBase* aParam2 = NULL ) = 0;


    /**
     * Return the protocol ID of the host
     */    

    virtual TInt32 ProtocolUID( ) = 0;
    
protected:

    /**
     * Protected destructor.
     * Instancies can't be destroyed via this interface.
     */
    virtual ~MXIMPHost() {};

    };


#endif // M_PSCHOST_H


