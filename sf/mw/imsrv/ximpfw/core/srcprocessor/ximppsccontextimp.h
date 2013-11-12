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
* Description:  Client context data storage.
*
*/

#ifndef C_XIMPPSCCONTEXT_H
#define C_XIMPPSCCONTEXT_H


#include "ximpbase.h"
#include "ximppsccontext.h"
#include <e32base.h>

#include "ximprequestidbuilder.h"

class CXIMPContextEventQueue;
class CPresenceInfoImp;
class CPresenceInfoFilterImp;
class CXIMPOperationBase;
class CXIMPServiceInfoImp;
class CXIMPFeatureInfoImp;
class CXIMPHost;
class CXIMPContextClientInfoImp;
class CPresenceFilterStore;
class CXIMPIdentityImp;

/**
 * Client data for context.
 *
 * @since S60 v4.0
 */
class CXIMPPscContext : public CObject, 
                        public MXIMPPscContext
    {
public:

    /**
     * Construction
     */
    IMPORT_C static CXIMPPscContext* NewL( TUint32 aSessionId );

    /**
     * Destruction
     */
    virtual ~CXIMPPscContext();

private:

    /**
     * Construction
     */
    CXIMPPscContext( TUint32 aSessionId );

    /**
     * Construction
     */
    void ConstructL();


public: // New methods

    /**
     * Access to eventqueue of context.
     * return Reference to eventqueue.
     */
    IMPORT_C CXIMPContextEventQueue& EventQueue() const;
    
    /**
     * Session id of context.
     * @return session id.
     */
    IMPORT_C TUint32 SessionId() const;
    
    /**
     * TLinearOrder method.
     */
    IMPORT_C static TInt OrderOfContexts( const CXIMPPscContext& aFirst, 
                                          const CXIMPPscContext& aSecond );

    /**
     * Access to current presence interest. 
     * @param Type of filter to access.
     * @param aIdentity Identity to map filter. Default NULL
     * @return Presence interest. NULL if not defined.
     */
    CPresenceInfoFilterImp* PresenceInfoFilter( TFilterStoreType aTypeOfFilter, 
                                                    const CXIMPIdentityImp* aIdentity = NULL ) const;

    /**
     * Set new presence interest filter
     * @param aTypeOfFilter Type of filter to access.
     * @param aPif New presence info filter to set.
     * @param aIdentity Identity to map filter. Default NULL
     */
    IMPORT_C void SetPresenceInfoFilterL( TFilterStoreType aTypeOfFilter,
                                         CPresenceInfoFilterImp* aPif,
                                         const CXIMPIdentityImp* aIdentity = NULL );
        
    /**
     * Ownership is transferred
     */
    IMPORT_C CXIMPOperationBase* GetCachedUnbind();

    /**
     * When bind operation fails, the cached unbind
     * must be destroyed.
     */
    IMPORT_C void DestroyCachedUnbind();

    /**
     * Set service info
     */
    IMPORT_C void SetServiceInfo( CXIMPServiceInfoImp* aServiceInfo );

    /**
     * Set service info
     */
    IMPORT_C void SetClientInfo( CXIMPContextClientInfoImp* aClientInfo );


    /**
     * Service info of context.
     */
    IMPORT_C CXIMPServiceInfoImp* ServiceInfo() const;


    /**
     * Append given operation to the end of the proper operation queue.
     * The queue resides in the host associated with the given context.
     * @param aOperation The operation to append
     * @return Request id
     */ 
    IMPORT_C TXIMPRequestId AppendToOpQueueUpdateReqIdL(
            CXIMPOperationBase* aOperation );
    IMPORT_C void AppendToOpQueueL(
            CXIMPOperationBase* aOperation );
    /**
     * Add new operation to be processed to the proper host.
     *
     * @param aOperation Enumeration describing the operation
     * @param aReqParams Parameters for the operation
     * @return The request id
     */
    IMPORT_C TXIMPRequestId AddNewOperationL( 
            TInt aOperation,
            const TDesC8& aReqParams );

    /**
     * Find cached features for this session
     * @return The cached features
     */
    IMPORT_C CXIMPFeatureInfoImp* FeaturesForSessionLC();
    
    /**
     * Client info of context
     */
    IMPORT_C CXIMPContextClientInfoImp* ClientInfo() const;
    
    /**
     * Check if host is assigned.
     * @return ETrue if host is assigned. EFalse if not
     */
    IMPORT_C TBool IsHostAssigned() const;
    
private:
    
    /**
     * Resolve host for operation
     */
    void ResolveHostL();

private: // data

    /**
     * Identifier for this session.
     */
    const TUint32 iSessionId;

    /**
     * Event queue for context-
     */
    CXIMPContextEventQueue* iEventQueue;
    
    /**
     * Current presence interest.
     */
    CPresenceInfoFilterImp* iOwnPif;

    /**
     * Owned. Identity to filter mapping for this context.
     */
    CPresenceFilterStore* iPresentityFilterStore;

    /**
     * Owned. Identity to filter mapping for this context.
     */
    CPresenceFilterStore* iPresentityGroupFilterStore;
    
    /**
     * Current grant everyone pif.
     */
    CPresenceInfoFilterImp* iGrantEveryonePif;
    
    /**
     * Owned. Identity to filter mapping for this context.
     */
    CPresenceFilterStore* iGrantFilterStore;

    /**
     * Owned. Identity to filter mapping for this context.
     */
    CPresenceFilterStore* iGrantGroupFilterStore;
    
    /** 
     * Unbind operation for need cases.
     */
    CXIMPOperationBase* iCachedUnbind;
    
    /**
     * Service information structure.
     * Owns.
     */
    CXIMPServiceInfoImp* iServiceInfo;

    /**
     * The host associated with this context.
     * Owned via reference counting.
     * Host has to be reference counted, since multiple contexts can own a
     * reference to one host.
     */
    CXIMPHost* iHost;

    /**
     * Counter for the operations, used to build the request id.
     */
    TInt iOperationIndex;
    
    /**
     * Client information structure
     */
    CXIMPContextClientInfoImp* iClientInfo;
    
    /**
     * TXIMPRequestId builder
     */
    TXIMPRequestIdBuilder iReqIdBuilder;
    };


#endif // C_XIMPPSCCONTEXT_H
