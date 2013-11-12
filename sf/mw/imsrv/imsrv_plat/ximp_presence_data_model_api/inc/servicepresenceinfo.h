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
* Description:  Interface for service presence info object.
*
*/

#ifndef MXIMPFWSERVICEPRESENCEINFO_H
#define MXIMPFWSERVICEPRESENCEINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>

class MXIMPIdentity;
class MPresenceInfoFieldCollection;


/**
 * Interface for service presence info object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MServicePresenceInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MServicePresenceInfo. */
    enum { KInterfaceId = PRES_IF_ID_SERVICE_PRESENCE_INFO };


public:

    /**
     * Public destructor. MServicePresenceInfo object
     * can be deleted through this interface.
     */
    virtual ~MServicePresenceInfo() {}


public:


    /**
     * Returns service type value.
     *
     * Returns string identifying type of service
     * for which this object is containing
     * presence information.
     *
     * Service type is on of the well known strings
     * listed in NPresenceInfo::NServiceType
     * namespace.
     *
     * @return Service type value.
     */
    virtual const TPtrC8 ServiceType() const = 0;
    
    /**
     * Set service type value.
     *
     * Service type is on of the well known strings
     * listed in NPresenceInfo::NServiceType
     * namespace.
     *
     * @return Service type value.
     * @leave KErrArgument if the given string is not in NPresenceInfo::NServiceType
     * @leave KErrNoMemory if failed to allocate memory
     */
    virtual void SetServiceTypeL( const TDesC8& aServiceType ) = 0;

    /**
     * Returns read-only reference to presence information
     * field collection.
     *
     * @return Presence information field collection.
     */
    virtual const MPresenceInfoFieldCollection& Fields() const = 0;


    /**
     * Returns modifiable reference to presence
     * information field collection.
     *
     * @return Presence information field collection.
     */
    virtual MPresenceInfoFieldCollection& Fields() = 0;


    };


#endif //MXIMPFWSERVICEPRESENCEINFO_H



