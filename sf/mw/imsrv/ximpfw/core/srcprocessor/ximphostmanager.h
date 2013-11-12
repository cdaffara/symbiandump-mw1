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
* Description:  implementation of CXIMPHostManager
 *
*/

#ifndef C_PSCHOSTMANAGER_H
#define C_PSCHOSTMANAGER_H


#include <e32base.h>
#include <ximpprotocolpluginhost.h>
#include "ximphostimp.h"

#include "ximpbase.h"

class CXIMPOperationBase;
class CXIMPFeatureInfoImp;
class CXIMPPscContext;
class CXIMPServiceInfoImp;
class CXIMPProtocolPluginBase;

/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class CXIMPHostManager : public CXIMPApiObjBase,
                        public MXIMPProtocolPluginHost
    {

public: // Definitions

    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPPSCHOSTMANAGER };


public:

    IMPORT_C static CXIMPHostManager* NewL();

    virtual ~CXIMPHostManager();


private:

    CXIMPHostManager();
    void ConstructL();

public:

    /**
     * Dissociate the given host from the list of known
     * hosts.
     * @param aHost The host to disassociate.
     */
    IMPORT_C void DisassociateHost( CXIMPHost* aHost );
    
    /**
     * Acquire host for context.
     */
    CXIMPHost* AcquireHostForContextLC( CXIMPPscContext& aContext );
    
public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS
    

private: // From MXIMPProtocolPluginHost
    
    MXIMPObjectFactory& ObjectFactory();

    void HandleRequestCompleted( TXIMPRequestId aReqId,
                                 TInt aResultCode );
                                 
    void HandleRequestCompleted( TXIMPRequestId aReqId,
                                 MXIMPStatus* aResult );    
                                 
    void HandleRequestCompleted( TXIMPRequestId aReqId,
                                 MXIMPStatus* aResult,
                                 MXIMPBase* aParameter );    

private: // Helper methods

    /**
     * Get host for connection. If does not exists, create one.
     */
    CXIMPHost* GetHostForConnectionLC( MXIMPProtocolConnection& aConnection, 
                                      TInt32 aProtocolUid  );

private: // data

    /**
     * List of known hosts.
     * Does not own, only knows the hosts.
     */
    RPointerArray<CXIMPHost> iHosts;
    
    /**
     * Object factory.
     */
    CXIMPObjectFactoryImp*  iObjFactory;  
    
    /**
     * Protocol
     */
    CXIMPProtocolPluginBase* iProtocol;
    };


#endif // C_PSCHOSTMANAGER_H
