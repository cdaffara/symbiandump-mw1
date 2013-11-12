/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for presence cache reader and writer.
*
*/

#ifndef PRESENCECACHECLIENT_H
#define PRESENCECACHECLIENT_H

#include <presencecachereader.h>
#include <presencecachewriter.h>

#include "presencecachedefs.h"
#include "presenceapiobjbase.h"


class MPresenceInfo;
class MPersonPresenceInfo;
class MPresenceInfoField;
class MPresenceBuddyInfo;
class MPresCacheWriteHandler;
class MPresCacheReadHandler;
class MXIMPIdentity;
class MPresenceBuddyInfoList;
class MPresenceObjectFactory;
class CPresenceObjectFactoryImp;

class CPresenceCacheClientNotification;

//Include Cache server namespace
using namespace NCacheSrv;

/**
 * Implementation for presence cache reader and writer. This object is 
 * instantiated either by MPresenceCacheReader or MPresenceCacheWriter using ecom framework.
 * Apart from implementing these two objects, this is also a Symbian OS client and
 * it communicates with presence cache server. Its also an active object.
 *
 * @ingroup ximpprescacheapi
 * @since S60 v3.2
 */
class CPresenceCacheClient : public CActive,
														 public MPresenceCacheReader,
                             public MPresenceCacheWriter,
                             public RSessionBase
                             
    {
public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCECACHECLIENT };


public:

    /**
     * Factory method to instantiate CPresenceCacheClient through the ECom. 
     *
     * @return The new CPresenceCacheClient object. Object
     *         ownership is returned to caller.
     */
    IMPORT_C static CPresenceCacheClient* NewL();
    
    /**
     * Factory method to instantiate CPresenceCacheClient through the ECom.
     * It's returns MPresenceCacheReader
     *
     * @return The new CPresenceCacheClient object. Object
     *         ownership is returned to caller.
     */
    IMPORT_C static MPresenceCacheReader* CreateReaderL();
    
    /**
     * Factory method to instantiate CPresenceCacheClient through the ECom.
     * It's returns MPresenceCacheWriter
     *
     * @return The new CPresenceCacheClient object. Object
     *         ownership is returned to caller.
     */
    IMPORT_C static MPresenceCacheWriter* CreateWriterL();

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~CPresenceCacheClient();
    
    /**
    * Start the server
    * @return TInt Error code
    */
    TInt StartServer();
    
    /**
    * Version.
    * Gets the version number.
    * @return The version.
    */
    TVersion Version() const;
        
        
private:

    /**
    * C++ constructor
    */
    CPresenceCacheClient();
    
    /**
    * Symbian second phase constructor
    */
    void ConstructL();
    
    /**
    * Connect.
    * Connects to the server and create a session.
    * @return Error code.
    */
    TInt Connect();
    
    /**
    * Packs MPresenceBuddyInfo object into a buffer
    * 
    * @param aBuddyPresInfo, MPresenceBuddyInfo
    * @return HBufC8* buffer
    */
    HBufC8* PackBuddyPresenceInfoLC(const MPresenceBuddyInfo* aBuddyPresInfo);
    
    /**
    * Packs MPresenceBuddyInfoList object into a buffer
    * 
    * @param aList, MPresenceBuddyInfoList
    * @return HBufC8* buffer
    */
    HBufC8* PackPresenceBuddyListLC(const MPresenceBuddyInfoList* aList);
    
    /**
    * Packs MXIMPIdentity object into a buffer
    * 
    * @param MXIMPIdentity, aIdentity
    * @return HBufC8* buffer
    */
    HBufC8* PackIdentityLC(const MXIMPIdentity& aIdentity);
    
    /**
    * Handle asynchronous call from server that buddies presence have been prepared.
    * This function does further processing and calls the client.
    * 
    * @param aError, Error came from server.
    */
    void HandlePreparedAllBuddiesPresenceL(TInt aError);
    
    /**
    * Return true if given identity is valid xsp identity
    * 
    * @param ETrue if valid xsp identity
    */
    TBool ValidateXspId(const TDesC& aXspId);
    
    
    /**
     * Internalize the services packet from given buffer
     * 
     * @param aServicesBuffer, services buffer recieved from server.
     */
    //CDesCArray* UnPackServicesLC(TDesC8& aServicesBuffer);
    

public: //From MPresenceCacheReader and MPresenceCacheWriter
    
    MXIMPIdentity* NewIdentityLC();
      
public: //From MPresenceCacheReader


    MPresenceInfo* PresenceInfoLC(const MXIMPIdentity& aIdentity);
    
    TInt BuddyCountInAllServices();
    
    TInt BuddyCountInService(const TDesC& aServiceName);
    
    TInt ServicesCount();
    
    //CDesCArray* ServicesLC();
    
    TInt CancelRead();                                        
                                                
    TInt SubscribePresenceBuddyChangeL(const MXIMPIdentity& aIdentity);
                                                
    void UnSubscribePresenceBuddyChangeL(const MXIMPIdentity& aIdentity);                             
                                                

public: //From MPresenceCacheReader, Asynchronous methods

    TInt AllBuddiesPresenceInService(const TDesC& aServiceName, 
                                        MPresCacheReadHandler* aHandler);

    TInt SetObserverForSubscribedNotifications(
                        MPresCacheReadHandler* aHandler);                             

public: //From MPresenceCacheWriter

    TInt WritePresenceL(const MPresenceBuddyInfo* aPresenceBuddyInfo);
    
    TInt DeleteService(const TDesC& aServiceName);
    
    TInt DeletePresenceL(const MXIMPIdentity& aIdentity);
    
    MPresenceObjectFactory& PresenceObjectFactory() const;
    
    MPresenceInfo* NewPresenceInfoLC();
    
    MPresenceBuddyInfo* NewBuddyPresenceInfoLC();
    
    MPresenceBuddyInfoList* NewPresenceBuddyInfoListLC(const TDesC& aServiceName);
    
    TInt CancelWrite();
    

public: //From MPresenceCacheWriter, Asynchronous methods

    TInt WritePresenceL(const MPresenceBuddyInfoList* aPresenceBuddyInfos,
                                    MPresCacheWriteHandler* aHandler);
                                    
public:  //Froom CActive

    void RunL();

    void DoCancel();
    
private: // Data
    
    //Not Owned
    MPresCacheWriteHandler* iWriteHandler;
    
    //Not Owned
    MPresCacheReadHandler* iReadHandler;
    
    //To store async request status
    NRequest::TOpAsyncCodes iAsyncReq;
    
    /**
     * Object factory sub interface.
     */
    CPresenceObjectFactoryImp* iPresObjFactory;   
    
    /**
     * Client who subscribed cache notifications
     * NOT OWN
     */
    MPresCacheReadHandler* iNotifyClient;   
    
    /**
     * Handler for subscribed cache notifications
     * OWN
     */    
    CPresenceCacheClientNotification* iNotifyHandler;
    
private: // friend classes

	friend class CPresenceCacheClientNotification;

    };

#endif // PRESENCECACHECLIENT_H



