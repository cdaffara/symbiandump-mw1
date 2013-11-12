/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence buddy presence info.
*
*/

#ifndef MPRESENCEBUDDYINFO_H
#define MPRESENCEBUDDYINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>

class MPresenceInfo;
class MPersonPresenceInfo;
class MXIMPIdentity;

/**
 * Interface buddy presence info. This object has MPresenceInfo and MXIMPIdentity
 * with and service Uid. This interface is implemented by the XIMP Framework 
 * Presence Data model. 
 * Users of XIMP can not create this object themselves, instead it's created from 
 * MPresenceCacheWriter interface.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceBuddyInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceBuddyInfo. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_BUDDY_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MPresenceBuddyInfo() {}
    
public:

    /**
     * Sets the buddy identity, service and presence info. Takes ownerships.
     *
     * @param aService Service UID of the buddy
     * @param aPresenceInfo for the buddy. Takes ownership.  
     * @return Error code
     */
    virtual TInt Set( MXIMPIdentity* aIdentity, 
                      MPresenceInfo* aPresenceInfo) = 0;
                                
    /**
     * Sets the presence info.
     *
     * @param aPresenceInfo for the buddy. Takes ownership.
     * @return Error code
     */
    virtual TInt SetPresenceInfo(MPresenceInfo* aPresenceInfo) = 0;
    
    
    /**
     * Sets the Buddy Identity.
     *
     * @param aIdentity identity of the buddy. Takes ownership.
     * @return Error code
     */
    virtual TInt SetBuddyId(MXIMPIdentity* aIdentity) = 0;
                                
    /**
     * Sets the buddy's person presence info. Leaves in system errors only.
     *
     * @param aPersonPresenceInfo for the buddy. Takes ownership.
     * @return Error code
     */
    virtual TInt SetPersonPresenceL(MPersonPresenceInfo* aPersonPresenceInfo) = 0;                                                        
                                
    /**
     * Reads presence info.
     *
     * @return MPresenceInfo for the given buddy. Ownership remains to this object.
     */
    virtual const MPresenceInfo* PresenceInfo() const = 0;
    
    /**
     * Reads buddy Id.
     *
     * @return MXIMPIdentity id.
     */
    virtual const MXIMPIdentity* BuddyId() const = 0;
                                                
    
    /**
     * Return true if identity is same with other instance.
     *
     * @param aOtherInstance other instance of this object.
     * @return ETrue if same.
     */
    virtual TBool EqualsIdentity(
                    const MPresenceBuddyInfo* aOtherInstance ) const = 0;
    
    };

#endif // MPRESENCEBUDDYINFO_H



