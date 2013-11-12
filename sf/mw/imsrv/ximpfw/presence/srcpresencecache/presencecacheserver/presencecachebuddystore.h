/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Store object for single buddy in a service
*
*/

#ifndef CXIMPPRESENCECACHEBUDDYSTORE_H
#define CXIMPPRESENCECACHEBUDDYSTORE_H


#include "presenceapiobjbase.h"
#include "ximpapiobjbase.h"

class MPresenceBuddyInfo;
class CXIMPIdentityImp;
class CPresenceCacheSession;
class MXIMPIdentity;
class CPresenceCacheServiceStore;

/**
 * CPresenceCacheBuddyStore. Used in Presence cache for presence storage. The 
 * identity part is always exists in this class, subscribers and presence info
 * are not guaranteed. However, if this object doesn't contain any subscribers 
 * or presence info, it doesnt serve its purpose and should be deleted.
 *
 * @lib presencecacheserver.exe
 * @since S60 v3.2
 */
class CPresenceCacheBuddyStore: public CBase
    {

public:

    /**
     * Symbian constructors and c++ destructor
     *
     * @param aServiceStore a parent store for this object
     * @param aIdentity identity for this object, must be in xsp format
     */
    IMPORT_C static CPresenceCacheBuddyStore* NewLC(
                                    CPresenceCacheServiceStore* aServiceStore,
                                    MXIMPIdentity* aIdentity);
    IMPORT_C static CPresenceCacheBuddyStore* NewL(
                                    CPresenceCacheServiceStore* aServiceStore,
                                    MXIMPIdentity* aIdentity);
    virtual ~CPresenceCacheBuddyStore();

private:

    /**
     * c++ constructor
     *
     * @param aServiceStore Parent service store for this object
     */
    CPresenceCacheBuddyStore(CPresenceCacheServiceStore* aServiceStore);
    
    /**
     * Symbian second phase constructor
     * @param aIdentity, uses this identity if not NULL, otherwise creates new
     */
    void ConstructL(MXIMPIdentity* aIdentity);


public: 

    /**
    * Service name of this object, taken from xsp prefix of the indentity
    *
    * @return TPtrC service name of this object
    */
    TPtrC ServiceName() const;
    
    /**
    * Sets the presnece info to this object. Takes ownership. It deletes
    * any old presence info. Calling this function multiple times with
    * different presence infos doesnt harm.
    *
    * @param aPresBuddyInfo buddy presence info to be set to this object
    */
    void SetPresenceBuddyInfo(MPresenceBuddyInfo* aPresBuddyInfo);
    
    /**
    * Removes presence info from this object and set it to NULL.
    */
    void RemovePresenceBuddyInfo();
    
    /**
    * Get the subscribed sessions pointer array. The array contains all those
    * sessions which are currently subscribed to receive changes in this object.
    *
    * @return reference to RPointerArray<CPresenceCacheSession>
    */
    RPointerArray<CPresenceCacheSession>& GetSubscribedSessions();
    
    /**
    * Add session to the list of subscribed sessions. It does not take ownership.
    * ignores if session is already there.
    *
    * @param aSession to add in subscribed list
    */
    void AddSubscribedSession(CPresenceCacheSession* aSession);
    
    /**
    * Remove session to the list of subscribed sessions. It does not take ownership.
    * ignores if session is not found.
    *
    * @param aSession to remove from subscribed list
    */
    void RemoveSubscribedSession(CPresenceCacheSession* aSession);

    /**
    * Sets the identity to this object. Takes ownership. It deletes
    * any old identity if present. Identity must be in xsp format.
    *
    * @param aIdentity identity to be set to this object
    */
    void SetBuddyId(MXIMPIdentity* aIdentity);
                                
    /**
    * Gets presence info from this object.
    *
    * @return pointer to MPresenceBuddyInfo
    */
    MPresenceBuddyInfo* PresenceBuddyInfo() const;
    
    /**
    * Gets identity from this object.
    *
    * @return constant pointer to MXIMPIdentity
    */
    const MXIMPIdentity* BuddyId() const;
    
    /**
    * Matches this object's identity with that of given object. Return True if
    * matches.
    *
    * @param aOtherInstance other CPresenceCacheBuddyStore
    * @return ETrue if identity matches.
    */
    TBool EqualsIdentity(
                    const CPresenceCacheBuddyStore* aOtherInstance ) const;
                    
    /**
    * Matches this object's identity with that of given object. Return True if
    * matches.
    *
    * @param aOtherInstance MPresenceBuddyInfo object
    * @return ETrue if identity matches.
    */
    TBool EqualsIdentity(
                        const MPresenceBuddyInfo* aOtherInstance ) const;
                    
    /**
    * Matches this object's identity with that of given identity. Return True if
    * matches.
    *
    * @param aOtherIdentity other identity object
    * @return ETrue if identity matches.
    */
    TBool EqualsIdentity(
                            const MXIMPIdentity& aOtherIdentity ) const;
                    
    /**
    * Finds whether this object has any presence info.
    *
    * @return ETrue if this object contains any presence info.
    */
    TBool HasPresence() const;
    
    /**
    * Finds whether this object has any subscribers.
    *
    * @return ETrue if this object has any subscriber(s).
    */
    TBool HasSubscribers() const;          

private: // data

     /**
     * OWN: Contains presence info or NULL
     */
    MPresenceBuddyInfo* iPresenceBuddyInfo;

    /**
     * OWN: Contains identity
     */
    CXIMPIdentityImp* iIdentity;

    /**
    *OWN: Sessions which  are subscribed to receive change in this buddy's presence
    */
    RPointerArray<CPresenceCacheSession> iSubscribedSessions;
    
    /**
    *Not Owned: Pointer to this object's service store
    */
    CPresenceCacheServiceStore* iServiceStore;
    
    };


#endif // CXIMPPRESENCECACHEBUDDYSTORE_H
