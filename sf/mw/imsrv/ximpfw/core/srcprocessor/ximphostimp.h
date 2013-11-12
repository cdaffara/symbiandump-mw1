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

#ifndef CPSCHOSTIMP_H
#define CPSCHOSTIMP_H

#include <e32base.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpcontext.h>

#include "ximpapiobjbase.h"
#include "ximphost.h"
#include "ximpoperationdefs.h"
#include "ximprequestidbuilder.h"



class CXIMPOperationBase;
class CXIMPEventManager;
class MXIMPObjectFactory;
class CXIMPObjectFactoryImp;
class CXIMPPscContext;
class MPresenceDataCache;
class CPresenceDataCache;
class CXIMPContextStateEventImp;
class CPeriodic;
class MXIMPProtocolConnection;
class CProtocolPresenceAuthorizationDataHostImp;
class MProtocolPresenceAuthorizationDataHost;
class CProtocolPresencePublishingDataHostImp;
class MProtocolPresencePublishingDataHost;
class CProtocolPresenceWatchingDataHostImp;
class MProtocolPresenceWatchingDataHost;
class CProtocolPresentityGroupsDataHostImp;
class MProtocolPresentityGroupsDataHost;
class CXIMPRestrictedCollectionImp;
class CXIMPProtocolRequestCompleteEventImp;
class MProtocolPresenceDataHost;
class CProtocolPresenceDataHostImp;
class MProtocolDataHost;

#define UNUSED_FORMAL_PARAM(p) (void) p


/**
 * Presence connection host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
class CXIMPHost : public CXIMPApiObjBase,
                 public MXIMPProtocolConnectionHost,
                 public MXIMPHost

    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPPSCHOST };

public:

    IMPORT_C static CXIMPHost* NewLC( MXIMPProtocolConnection& aConnection, TInt32 aProtocolUid );

    /**
     * Mark a reference to this object.
     */
    void Open();

    /**
     * Remove a reference to this object.
     * Once all references are removed, the class destroys itself.
     */
    void Close();

protected:

    /**
     * Reference counted. Use Close to remove a reference, and Open to open it.
     * Can't be CObject-derived, since already derives from another C-class.
     */
    virtual ~CXIMPHost();

    CXIMPHost( MXIMPProtocolConnection& aConnection, TInt32 aProtocolUid );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: // From MXIMPProtocolConnectionHost

    void HandleRequestCompleted( TXIMPRequestId aReqId, TInt aResultCode );
    void HandleRequestCompleted( TXIMPRequestId aReqId, MXIMPStatus* aResult );
    void HandleRequestCompleted( TXIMPRequestId aReqId,
                                 MXIMPStatus* aResult,
                                 MXIMPBase* aParameter );
    TXIMPRequestId HandleConnectionTerminated( MXIMPStatus* aReason );
    MXIMPObjectFactory& ObjectFactory();
    MProtocolPresenceDataHost& ProtocolPresenceDataHost();
    

   
    void RegisterObserverL(
            MXIMPProtocolConnectionHostObserver& aObserver,
            const TArray<TInt32>* aEventFilter = NULL );
    void UnregisterConnectionHostObserver(
            MXIMPProtocolConnectionHostObserver& aObserver );


public: // MXIMPHost

    MXIMPProtocolConnection& GetConnection() const;

    void AddEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext  );
    void ActivateEventL( CXIMPApiEventBase& aEvent, MXIMPPscContext* aContext );

    void UnregisterSession( CXIMPPscContext* aContext );

    void FakeCompleteRequest( TXIMPRequestId aReqId, TInt aResult );
    void HandleAdaptationRequestCompleted( TXIMPRequestId aReqId, TInt aResult );
    void HandleSessionMgmtRequestCompleted( TXIMPRequestId aReqId, TInt aResult );
    void HandleInternalRequestCompleted( TXIMPRequestId aReqId, TInt aResult );

    CProtocolPresenceDataHostImp& PresenceDataAccess();
    

    // get corresponding protocol host from the array of hosts
    TAny* GetProtocolHost(TInt aInterfaceId) ;
    
/*    CPresenceDataCache& DataAccess();
    CProtocolPresenceAuthorizationDataHostImp& AuthorizationDataAccess();
    CProtocolPresencePublishingDataHostImp& PublishingDataAccess();
    CProtocolPresenceWatchingDataHostImp& WatchingDataAccess();
    CProtocolPresentityGroupsDataHostImp& GroupsDataAccess();
*/
    void AddNewOperationL( CXIMPOperationBase& aOperation );
    TXIMPRequestId AddNewOperationL( CXIMPOperationBase* aOperation );

    void ForcedTeardownL();

    void StartCleanerTimer();
    
    TXIMPRequestId AddNewOperationL( 
                TInt aOperation, MXIMPBase* aParam, MXIMPBase* aParam2 = NULL );    
    TXIMPRequestId AddNewOperationL( 
                TInt aOperation, CXIMPRestrictedObjectCollectionImp* aParams );
    
    TInt32 ProtocolUID( );
    
    // to set the specific prtocol hosts
    void SetProtocolHosts();

public: // New functions

    void AddEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext  );
    void ActivateEventL( CXIMPApiEventBase& aEvent, CXIMPPscContext* aContext );


    void RegisterSessionL( CXIMPPscContext* aContext );

private:

    /**
     * Delete old operation and try to launch processing.
     */
    void StartProcessing();

    /**
     * Activate next operation for processing and start processing.
     */
    void ActivateProcessing();

    /**
     * Helper for completing the request, to avoid cyclical
     * call chain from StartProcessingL to HandleRequestCompleted and back.
     * @param aResult The error code
     * @see HandleRequestCompleted
     */
    void CompleteRequest( TXIMPRequestId aReqId );

    /**
     * Callback method for process starting.
     */
    static TInt StartProcessCallback( TAny *aInstance );

    /**
     * Leaving version of Callback method for process starting.
     */
    void DoStartProcessCallback();

    /**
     * Callback method for request complete.
     */
    static TInt CompleteRequestCallback( TAny *aInstance );
    static TInt CompleteAdaptationRequestCallback( TAny* aInstance );
    static TInt CompleteSessionMgmtRequestCallback( TAny* aInstance );
    static TInt CompleteInternalRequestCallback( TAny* aInstance );

    /**
     * Leaving version of Callback method for request complete.
     */
    void DoCompleteRequestCallback();
    void DoCompleteAdaptationRequestCallback();
    void DoCompleteSessionMgmtRequestCallback();
    void DoCompleteInternalRequestCallback();

    /**
     * Callback for timer cleaner
     * @see TCallBack
     */
    static TInt CleanerCallback( TAny* aMyself );

    /**
     * Actual callback called from CleanerCallback.
     */
    void DoCleanerCallback();

    /**
     * Leaving version of cleaner timer callback.
     */
    void DoCleanerCallbackL();
    
    /**
     * This panics if active operation is not found for requestid.
     */
    CXIMPOperationBase& OperationForRequestId( TXIMPRequestId aReqId );

    TXIMPRequestId AddNewOperationL( NXIMPOps::TXIMPOpTypes aOpType );
    
     /**
     * This is to load all the protocol hosts 
     */
    void LoadDataHostArrayL() ;
    
private: // data

    /**
     * iConnection
     */
    MXIMPProtocolConnection& iConnection;

    /**
     * Uid for protocol to use for request id builder
     */
    TInt32 iProtocolUid;
    
    /**
     * Reference counter
     */
    TInt iReferences;

    /**
     * State identifier
     */
     TBool iProcessing;

    /**
     * Container for the operations. Owns the operations.
     * Own.
     */
    RXIMPObjOwningPtrArray< CXIMPOperationBase > iOperations;

    TBool iRunFromContextActivated;
    CXIMPOperationBase* iCurrentOperation;

    // not owned

    CXIMPEventManager* iEventManager;


    /**
     * Container for session datas that are registered to this
     * host.
     * Shared ownership. Cleanup for items must be done with close method. Not delete.
     */
    RPointerArray< CXIMPPscContext > iRegSessions;

    /**
     * Internal state of the host
     */
    NXIMPHost::THostStates iState;

    /**
     * Timer for clean operation creation.
     */
    CPeriodic* iCleanerTimer;

    /**
     * Object factory.
     */
    CXIMPObjectFactoryImp*  iObjFactory;

    /**
     * Data cache
     */
//    CPresenceDataCache* iDataCache;

    /**
     * Data hosts
     */
     
    CProtocolPresenceDataHostImp* iPresenceDataHost;
    
    /**
     * array to hold data hosts
     */
    CArrayFixFlat<MProtocolDataHost*>* iDataHostArray;
    
/*    CProtocolPresenceAuthorizationDataHostImp* iAuthDataHost;
    CProtocolPresencePublishingDataHostImp* iPublishingDataHost;
    CProtocolPresenceWatchingDataHostImp* iWatchingDataHost;
    CProtocolPresentityGroupsDataHostImp* iGroupsDataHost;
*/
    /**
     * Operation index for host-originated operations
     * (currently only reconnect operation)
     */
    TInt iHostOriginatedOpIndex;

    /**
     * Active object for async calls.
     */
    //CAsyncCallBack* iCallBack;
    enum
        {
        ECompleteRequestCallback = 0,
        ECompleteAdaptationRequestCallback,
        ECompleteSessionMgmtRequestCallback,
        ECompleteInternalRequestCallback,
        EStartProcessCallback
        };
    RPointerArray< CAsyncCallBack > iCallBack;

    /**
     *
     */
    TInt iAdaptationOperationIndex;


    /**
     * Builder for TXIMPRequestIds. Owned.
     */
    TXIMPRequestIdBuilder iReqIdBuilder;
    
    /**
     * Observers for adaptation requests.
     */
    RPointerArray< MXIMPProtocolConnectionHostObserver > iConnectionHostObservers;
    
    
    /**
     * Adaptation request complete event.
     */
    CXIMPProtocolRequestCompleteEventImp* iAdaptationReqCompleteEvent;
    
    /**
     * Forced tear down flag.
    */
    
    TBool iForcedTearDown;
    };


#endif // CPSCHOST_H

