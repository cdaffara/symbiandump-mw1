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
* Description:  Interface for XIMP Framework /Search Feature 
*
*/

#ifndef MSEARCHFEATURE_H
#define MSEARCHFEATURE_H

#include <ximpbase.h>
#include <searchmanagementifids.hrh>
#include <ximpcontext.h>


class MSearch;
class MSearchObjectFactory;


/**
 * Interface for Search Feature
 *
 * @ingroup searchmanagementapi
 *
 */
class MSearchFeature : public MXIMPBase
{
	
 
public:

    /** Interface ID for the MSearchFeature. */
   enum { KInterfaceId = XIMP_ECOM_IF_UID_SEARCH_INTERFACE };


public:

    /**
     * Factory method to instantiate MSearchFeature.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default search Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new serach client object. Object
     *         ownership is returned to caller.
     */
    static inline MSearchFeature* NewL( MXIMPContext* aContext );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MSearchFeature();
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MSearchFeature();


public: //Sub interfaces for im context


    /**
     * Gets reference to search object factory interface.
     *
     * Returned object factory interface is used
     * to instantiate search objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MSearchObjectFactory& SearchObjectFactory() const = 0;

    /**
     * Gets reference to  search interface.
     *
     * @return Requested interface.
     *         No ownership is returned caller.
     */
    virtual MSearch& Search() const = 0;

private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <searchfeature.inl>


#endif // MSearchFeature_H



