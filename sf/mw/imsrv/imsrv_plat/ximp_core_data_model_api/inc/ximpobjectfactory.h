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
* Description:  Interface for XIMP Framework object factory.
*
*/

#ifndef MXIMPOBJECTFACTORY_H
#define MXIMPOBJECTFACTORY_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


class MXIMPStatus;
class MXIMPIdentity;
class MXIMPObjectCollection;
class MXIMPRestrictedObjectCollection;
class MXIMPDataSubscriptionState;


/**
 * Interface for XIMP Framework object factory.
 *
 * XIMP object factory is used to instantiate
 * data objects declared in Data Model API.
 *
 * MXimpFwObjectFactory interface can be obtained from
 * MXIMPContext or MXIMPProtocolConnectionHost
 * interfaces.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPObjectFactory : public MXIMPBase
    {
public:

    /** Interface ID for the MXimpFwObjectFactory. */
    enum { KInterfaceId = XIMP_IF_ID_OBJECT_FACTORY };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MXIMPObjectFactory() {}



public: // Base object types


    /**
     * Instantiates new status object.
     *
     * @return New status object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPStatus* NewStatusLC() = 0;


    /**
     * Instantiates new identity object.
     *
     * @return New identity object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPIdentity* NewIdentityLC() = 0;


    /**
     * Instantiates new restricted object collection object.
     *
     * @return New restricted object collection instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPRestrictedObjectCollection* NewRestrictedObjectCollectionLC() = 0;


    /**
     * Instantiates new object collection object.
     *
     * @return New object collection instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPObjectCollection* NewObjectCollectionLC() = 0;


    /**
     * Instantiates new data subscription state object.
     *
     * @return New data subscription state instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MXIMPDataSubscriptionState* NewDataSubscriptionStateLC() = 0;


    };

#endif // MXIMPOBJECTFACTORY_H



