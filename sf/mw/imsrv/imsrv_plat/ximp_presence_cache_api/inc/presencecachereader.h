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
* Description:  Interface for presence cache reader.
*
*/

#ifndef MPRESENCECACHEREADER_H
#define MPRESENCECACHEREADER_H

#include <e32std.h>
#include <e32base.h>
#include <presencemanagementifids.hrh>

class MPresenceInfo;
class MPresenceBuddyInfoList;
class MPersonPresenceInfo;
class MPresCacheReadHandler;
class MXIMPIdentity;

/**
 * Interface for presence cache reader API. This interface is implemented 
 * by the XIMP Framework Presence Cache client. Clients must call 
 * REComSession::FinalClose() from their destructors.  Notice that an xsp id is
 * in the form ovi:xyz@example.com
 *
 * @ingroup ximpprescacheapi
 * @since S60 v3.2
 */
class MPresenceCacheReader
    {
public:

    /** Interface ID for the MPresenceCacheReader. */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_PRESENCE_CACHE_READER };


public:

    /**
     * Factory method to instantiate MPresenceCacheReader through 
     * the ECom.
     *
     * @return The new MPresenceCacheReader object. Object
     *         ownership is returned to caller.
     */
    static inline MPresenceCacheReader* NewL();

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MPresenceCacheReader();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MPresenceCacheReader();


public: //Sub interfaces for presence cache reader, Synchonous methods

    /**
     * Instantiates new identity object.
     *
     * @return New Identity instance. Object ownership is returned to caller.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPIdentity* NewIdentityLC() = 0;
    
    /**
     * Reads presence info from cache for a given identity, returns immediately.
     *
     * @param aIdentity buddy id, identity must be in xsp id format
     * @return MPresenceInfo for the given buddy. Ownership to caller.
     *         Null if service or buddy not found, not on Cleanup in that case.    
     */
    virtual MPresenceInfo* PresenceInfoLC(const MXIMPIdentity& aIdentity) = 0;
    
    /**
     * Returns total number of buddies stored in presence cache.
     * It includes all services, probably useful while deciding performance 
     * overheads or informing to the user.
     *
     * @return TInt Total number of presence infos in presence cache.
     */                           
    virtual TInt BuddyCountInAllServices() = 0;
    
    /**
     * Returns total number of buddies (presence infos) stored in a specific
     * service in presence cache. It includes only the given service. 
     *
     * @param aServiceName as in device's service table
     * @return TInt buddy count or error code. e.g. in case of service not
     *         found return value is KErrNotFound.
     */                           
    virtual TInt BuddyCountInService(const TDesC& aServiceName) = 0;
    
    /**
     * Returns total number services in presence cache.
     *
     * @return TInt total number of services in presence cache.
     */                           
    virtual TInt ServicesCount() = 0;
    
    /**
     * Returns all service names present in presence cache.
     *
     * @returned Service names are returned here, ownership to caller.
     */                           
    //virtual CDesCArray* ServicesLC() = 0;
    
    /**
     * Cancels any async. read operation going on. After calling this
     * method no callback will be received from asycn read method.
     *
     * @return error code, KErrNone if successfully cancelled.
     */
    virtual TInt CancelRead() = 0;
                                                

public: //Sub interfaces for presence cache reader, Asynchronous methods

    /**
     * Reads all presence info from cache for a given service. Returns in 
     * MPresCacheReadHandler's HandlePresenceReadL.
     *
     * @param aServiceName Service name to search from
     * @param aHandler to handle async. callback
     * @return error code, in case of error callback wont be called.
     */    
    virtual TInt AllBuddiesPresenceInService(const TDesC& aServiceName, 
                                MPresCacheReadHandler* aHandler) = 0;

public: //Sub interfaces for presence cache reader, Asynchronous methods
    
    /**
     * Sets the observer to receive Presence Buddy Change notifications.
     * Observer must be set before using SubscribePresenceBuddyChangeL.
     * If client changes the observer using this method, all the notifications
     * related to existing and new subscriptions will be delivered to 
     * the new observer.
     *
     * @return Error code, if KErrNone observer is successfully set.
     */
    virtual TInt SetObserverForSubscribedNotifications(
                                    MPresCacheReadHandler* aHandler) = 0;
    
    /**
     * Subscribe for given presence buddy's change in presence. An observer
     * must be set before using this method by SetObserverForSubscribedNotifications.
     * Notifcations are delivered through MPresCacheReadHandler's HandlePresenceNotificationL
     *
     * @param aIdentity buddy id, identity must be in xsp id format
     * @return KErrNone if successful, KErrArgument if observer not set
     * @leave Only in system error cases e.g. if failed to allocate memory.
     */
    virtual TInt SubscribePresenceBuddyChangeL(const MXIMPIdentity& aIdentity) = 0;
                                                
    /**
     * Unsubscribe for given presence buddy's change in presence. The request
     * is ignored if invalid.
     *
     * @param aIdentity buddy id, identity must be in xsp id format
     * @leave Only in system error cases e.g. if failed to allocate memory.
     */
    virtual void UnSubscribePresenceBuddyChangeL(const MXIMPIdentity& aIdentity) = 0;

            
private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <presencecachereader.inl>


#endif // MPRESENCECACHEREADER_H



