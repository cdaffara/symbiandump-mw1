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
* Description:  Interface for presence cache writer.
*
*/

#ifndef MPRESENCECACHEWRITER_H
#define MPRESENCECACHEWRITER_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>


class MPresenceInfo;
class MPersonPresenceInfo;
class MPresenceInfoField;
class MPresenceBuddyInfo;
class MPersonPresenceInfo;
class MPresCacheWriteHandler;
class MPresenceBuddyInfoList;
class MXIMPIdentity;
class MPresenceObjectFactory;


/**
 * Interface for presence cache writer API. This interface is implemented 
 * by the XIMP Framework Presence Cache client. Clients must call 
 * REComSession::FinalClose() from their destructors. Notice that an xsp id is
 * in the form ovi:xyz@example.com
 *
 * @ingroup ximpprescacheapi
 * @since S60 v3.2
 */
class MPresenceCacheWriter //: public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceCacheWriter. */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_PRESENCE_CACHE_WRITER };


public:

    /**
     * Factory method to instantiate MPresenceCacheWriter through 
     * the ECom.
     *
     * @return The new MPresenceCacheWriter object. Object
     *         ownership is returned to caller.
     */
    static inline MPresenceCacheWriter* NewL();

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MPresenceCacheWriter();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MPresenceCacheWriter();


public: //Sub interfaces for presence cache writer, Synchonous methods

    /**
     * Writes presence info to cache for a given buddy. Leaves only in system
     * errors.
     *
     * @param aPresenceBuddyInfo contains xsp identity and presence
     * @return Error code
     */
    virtual TInt WritePresenceL(const MPresenceBuddyInfo* aPresenceBuddyInfo) = 0;
    
    
    /**
     * Instantiates new Presence object Factory object. With this factory presence
     * related ximp structures can be created.
     *
     * @return reference to MPresenceObjectFactory. No ownership to caller.
     */
    virtual MPresenceObjectFactory& PresenceObjectFactory() const = 0;
    
    
    /**
     * Instantiates new identity object.
     *
     * @return New Identity instance. Object ownership is returned to caller.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPIdentity* NewIdentityLC() = 0;


    /**
     * Instantiates new presence info object.
     *
     * @return New presence info instance. Object ownership
     *         is returned to caller.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfo* NewPresenceInfoLC() = 0;
    
    
    /**
     * Instantiates new buddy presence info object.
     *
     * @return New buddy presence info instance. Object ownership
     *         is returned to caller.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceBuddyInfo* NewBuddyPresenceInfoLC() = 0;
    
    /**
     * Instantiates new buddy presence info list object.
     *
     * @param aServiceName service name for this list, if set to KNullDesC
     *        the list can be appended with identities from different services
     * @return New buddy presence info list instance. Object ownership
     *         is returned to caller.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceBuddyInfoList* NewPresenceBuddyInfoListLC(const TDesC& aServiceName) = 0;
    
    
    /**
     * Delete all buddies presence associated with given service.
     *
     * @param aServiceName service name as in sp table
     * @return error code
     */
    virtual TInt DeleteService(const TDesC& aServiceName) = 0;
    
    /**
     * Deletes all presence related to given xsp identity.
     * Leaves only in system errors.
     *
     * @param MXIMPIdentity identity for buddy to be removed, in xsp format
     * @return error code
     */
    virtual TInt DeletePresenceL(const MXIMPIdentity& aIdentity) = 0;
    
    /**
     * Cancels any async. write operation going on. After calling this
     * method no callback will be received from asycn write method.
     *
     * @return error code, KErrNone if successfully cancelled.
     */
    virtual TInt CancelWrite() = 0;
    

public: //Sub interfaces for presence cache writer, Asynchronous methods

    /**
     * Writes buddy presence info objects to cache. Returns in 
     * MPresCacheWriteHandler's HandlePresenceWriteL. Provide a null pointer
     * to receive no call back. Leaves only in system errors.
     *
     * @param aBuddyPresenceInfos presence infos for all buddies. 
     *         Ownership remains to caller.
     * @param MPresCacheWriterHandler to handle async calls. A 'NULL' pointer
     *         can be provided to receive no call back.
     * @return TInt error code.
     */    
    virtual TInt WritePresenceL(const MPresenceBuddyInfoList* aBuddyPresenceInfos,
                                    MPresCacheWriteHandler* aHandler) = 0;

   
private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <presencecachewriter.inl>


#endif // MPRESENCECACHEWRITER_H