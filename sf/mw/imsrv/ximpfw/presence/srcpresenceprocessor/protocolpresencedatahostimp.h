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
* Description:  Presence connection host
 *
*/

#ifndef CPROTOCOLPRESENCEDATAHOSTIMP_H
#define CPROTOCOLPRESENCEDATAHOSTIMP_H

//#include <e32base.h>
//#include <ximpprotocolconnectionhost.h>
//#include <ximpcontext.h>
#include <protocolpresencedatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "ximpitemparent.h"
//#include "ximphost.h"
#include "ximpoperationdefs.h"
#include "ximprequestidbuilder.h"


class MXIMPHost;
class MPresenceObjectFactory;
class CPresenceObjectFactoryImp;

//class CXIMPPscContext;
class MPresenceDataCache;
class CPresenceDataCache;
/*class CXIMPContextStateEventImp;
class CPeriodic;
class MXIMPProtocolConnection;*/
class CProtocolPresenceAuthorizationDataHostImp;
class MProtocolPresenceAuthorizationDataHost;
class CProtocolPresencePublishingDataHostImp;
class MProtocolPresencePublishingDataHost;
class CProtocolPresenceWatchingDataHostImp;
class MProtocolPresenceWatchingDataHost;
class CProtocolPresentityGroupsDataHostImp;
class MProtocolPresentityGroupsDataHost;
//class CXIMPRestrictedCollectionImp;
//class CXIMPProtocolRequestCompleteEventImp;



/**
 * Presence connection host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
class CProtocolPresenceDataHostImp : public CXIMPApiObjBase,
                                     public MProtocolPresenceDataHost

    {
public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPROTOCOLPRESENCEDATAHOSTIMP };

public:

    IMPORT_C static CProtocolPresenceDataHostImp* NewL( MXIMPHost& aHost );

    IMPORT_C static CProtocolPresenceDataHostImp* NewLC( MXIMPHost& aHost );

    virtual ~CProtocolPresenceDataHostImp();

private:

    CProtocolPresenceDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: // MProtocolPresenceDataHost
   
    MPresenceObjectFactory& PresenceObjectFactory();

    MProtocolPresenceAuthorizationDataHost& AuthorizationDataHost();

    MProtocolPresencePublishingDataHost& PublishingDataHost();

    MProtocolPresenceWatchingDataHost& WatchingDataHost();

    MProtocolPresentityGroupsDataHost& GroupsDataHost();

public: 
    
    IMPORT_C CPresenceDataCache& PresenceDataCache();
    IMPORT_C CProtocolPresenceAuthorizationDataHostImp& AuthorizationDataAccess();
    IMPORT_C CProtocolPresencePublishingDataHostImp& PublishingDataAccess();
    IMPORT_C CProtocolPresenceWatchingDataHostImp& WatchingDataAccess();
    IMPORT_C CProtocolPresentityGroupsDataHostImp& GroupsDataAccess();
    
// MXIMPHost

/*    MXIMPProtocolConnection& GetConnection() const;

    void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext  );
    void ActivateEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext );

    void UnregisterSession( CXIMPPscContext* aContext );

    void FakeCompleteRequest( TXIMPRequestId aReqId, TInt aResult );
    void HandleAdaptationRequestCompleted( TXIMPRequestId aReqId, TInt aResult );
    void HandleSessionMgmtRequestCompleted( TXIMPRequestId aReqId, TInt aResult );
    void HandleInternalRequestCompleted( TXIMPRequestId aReqId, TInt aResult );

    CPresenceDataCache& DataAccess();
    CProtocolPresenceAuthorizationDataHostImp& AuthorizationDataAccess();
    CProtocolPresencePublishingDataHostImp& PublishingDataAccess();
    CProtocolPresenceWatchingDataHostImp& WatchingDataAccess();
    CProtocolPresentityGroupsDataHostImp& GroupsDataAccess();

    void AddNewOperationL( CXIMPOperationBase& aOperation );
    TXIMPRequestId AddNewOperationL( CXIMPOperationBase* aOperation );

    void ForcedTeardownL();

    void StartCleanerTimer();
    
    TXIMPRequestId AddNewOperationL( 
                TInt aOperation, MXIMPBase* aParam, MXIMPBase* aParam2 = NULL );    
    TXIMPRequestId AddNewOperationL( 
                TInt aOperation, CXIMPRestrictedObjectCollectionImp* aParams );

public: // New functions

    void AddEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  );
    void ActivateEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext );


    void RegisterSessionL( CXIMPPscContext* aContext );*/

private: // data

    MXIMPHost& iHost;
    /**
     * Data cache
     */
    CPresenceDataCache* iPresenceDataCache;
    
    CPresenceObjectFactoryImp* iPresenceObjFactory;

    /**
     * Data hosts
     */
    CProtocolPresenceAuthorizationDataHostImp* iAuthDataHost;
    CProtocolPresencePublishingDataHostImp* iPublishingDataHost;
    CProtocolPresenceWatchingDataHostImp* iWatchingDataHost;
    CProtocolPresentityGroupsDataHostImp* iGroupsDataHost;


    };


#endif // CPROTOCOLPRESENCEDATAHOSTIMP_H

