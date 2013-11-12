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
* Description:  Interface for presence buddy info list.
*
*/

#ifndef MPRESENCEBUDDYINFOLIST_H
#define MPRESENCEBUDDYINFOLIST_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>

class MPresenceBuddyInfo;
class MXIMPIdentity;

/**
 * Interface for buddy presence info list. This object has a pointer array of
 * MPresenceBuddyInfo(s). 
 * This interface is implemented by the XIMP Framework Presence Data model. 
 * Users of XIMP can not create this object themselves, instead it's created from 
 * MPresenceCacheWriter interface.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceBuddyInfoList : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceBuddyInfoList. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_BUDDY_INFO_LIST };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresenceBuddyInfoList() {}
    
    /**
     * Gets reference to object collection.
     *
     * @return MXIMPObjectCollection. Ownership remains to this object.
     */
    virtual RPointerArray<MPresenceBuddyInfo>& GetObjectCollection() = 0;
    

    /**
     * Service name accessor.
     *
     * @return Service name.
     */    
    virtual TPtrC ServiceName() const = 0;
    
    /**
     * Number of MPresenceBuddyInfo.
     *
     * @return TInt count.
     */    
    virtual TInt Count() const = 0;
    
    /**
     * Add or replace the buddy presence info object in this collection. 
     * The object will be replaced if found same used id and service. 
     * Takes Ownership.
     * @param aBuddyPresenceInfo object to add or replace.
     * @return error code
     */
    virtual TInt AddOrReplace(MPresenceBuddyInfo* aBuddyPresenceInfo) = 0;
    
    /**
     * Find and remove the buddy presence info object matching given identity. 
     * The first found object will be removed with same identity.
     * @param aIdentity to be matched for.
     * @return error code
     */
    virtual TInt FindAndRemove(const MXIMPIdentity& aIdentity) = 0;
    
    /**
     * Find and get the first found buddy presence info object matching given identity. 
     * A Null pointer will be returned if object with same identity not found.
     * ownership remains to callee.
     * @param aIdentity to be matched for.
     */
    virtual MPresenceBuddyInfo* FindAndGet(const MXIMPIdentity& aIdentity) const = 0;
    
    };

#endif // MPRESENCEBUDDYINFOLIST_H



