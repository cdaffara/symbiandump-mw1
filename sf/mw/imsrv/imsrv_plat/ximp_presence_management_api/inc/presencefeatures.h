/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence context.
*
*/

#ifndef MPRESENCEFEATURES_H
#define MPRESENCEFEATURES_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencemanagementifids.hrh>
#include <ximpcontext.h>


class MPresenceObjectFactory;
class MPresenceDataCache;
class MPresenceWatching;
class MPresencePublishing;
class MPresentityGroups;
class MPresenceAuthorization;

/**
 * Interface for presence features.
 * This interface is implemented by the XIMP Framework Presence Feature plug-in.
 *
 * MPresenceFeatures is root interface for accessing
 * and updating presence data to remote presence service.
 * XIMP FW client binds MXIMPContext interface
 * to desired remote presence service and creates
 * MPresenceFeatures interface to access the XIMP FW presence related 
 * methods to publish and access presence data.
 *
 * Following sub interfaces can be accessed from
 * from MPresenceFeatures:
 *   - MPresenceObjectFactory
 *   - MPresenceWatching
 *   - MPresencePublishing
 *   - MPresentityGroups
 *   - MPresenceAuthorization
 *
 * All Presence events (context state, presence data,
 * request completion etc. ones) are delivered through
 * MPresenceContextObserver interface.
 *
 * @ingroup ximppresmanagementapi
 * @since S60 v3.2
 */
class MPresenceFeatures : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceContext. */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_PRESENCE_INTERFACE };


public:

    /**
     * Factory method to instantiate MPresenceFeatures.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default presence framework isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new presence client object. Object
     *         ownership is returned to caller.
     */
    static inline MPresenceFeatures* NewL( MXIMPContext* aContext );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MPresenceFeatures();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MPresenceFeatures();


public: //Sub interfaces for presence context


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate presence objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MPresenceObjectFactory& PresenceObjectFactory() const = 0;


    /**
     * Gets reference to object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate presence objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    //virtual MPresenceDataCache& PresenceDataAccess() const = 0;
    

    /**
     * Gets reference to presence watching interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MPresenceWatching& PresenceWatching() const = 0;



    /**
     * Gets reference to presence publishing interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MPresencePublishing& PresencePublishing() const = 0;



    /**
     * Gets reference to presentity groups interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MPresentityGroups& PresentityGroups() const = 0;


    /**
     * Gets reference to presence authorization interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MPresenceAuthorization& PresenceAuthorization() const = 0;

private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <presencefeatures.inl>


#endif // MPRESENCEFEATURES_H



