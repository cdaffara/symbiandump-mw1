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
* Description:  Interface for IM context.
*
*/

#ifndef MIMFEATURES_H
#define MIMFEATURES_H

#include <e32std.h>
#include <ximpbase.h>
#include <immanagementifids.hrh>
#include <ximpcontext.h>


class MImObjectFactory;
class MImGroups;
class MImConversation;
class MImInvitation;
class MImSearch;

/**
 * Interface for IM features.
 * This interface is implemented by the IM.
 *
 * MImFeatures is root interface for accessing
 * and updating im data to remote im service.
 * IM client binds MXIMPContext interface
 * to desired remote im service and creates
 * MImFeatures interface to access the Im feature plugin related 
 * methods to publish and access IM data.
 *
 * Following sub interfaces can be accessed from
 * from MImFeatures:
 *   - MImObjectFactory
 *   - MImGroupInfo
 *   - MImConversation
 *   - MImInvitation
 *   - MImSearch
 *
 * All IM events (context state, im data,
 * request completion etc. ones) are delivered through
 * MImContextObserver interface.
 *
 * @ingroup immanagementapi
 * @since S60 
 */
class MImFeatures : public MXIMPBase
    {
public:

    /** Interface ID for the MImFeatures. */
   enum { KInterfaceId = XIMP_ECOM_IF_UID_IM_INTERFACE };


public:

    /**
     * Factory method to instantiate MImFeatures.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default IM Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new Im client object. Object
     *         ownership is returned to caller.
     */
    static inline MImFeatures* NewL( MXIMPContext* aContext );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MImFeatures();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MImFeatures();


public: //Sub interfaces for im context


    /**
     * Gets reference to im object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate im objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MImObjectFactory& ImObjectFactory() const = 0;


    /**
     * Gets reference to im object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate im objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    //virtual MImDataCache& ImDataAccess() const = 0;
    

    /**
     * Gets reference to im groups interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    //virtual MImGroups& ImGroups() const = 0;



    /**
     * Gets reference to im conversation interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MImConversation& ImConversation() const = 0;



    /**
     * Gets reference to Im invitation interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    //virtual MImInvitation& ImInvtation() const = 0;


    /**
     * Gets reference to im search interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    //virtual MImSearch& ImSearch() const = 0;

private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <imfeatures.inl>


#endif // MIMFEATURES_H



