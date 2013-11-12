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
* Description:  Interface for device presence info object.
*
*/

#ifndef MXIMPFWDEVICEPRESENCEINFO_H
#define MXIMPFWDEVICEPRESENCEINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MPresenceInfoFieldCollection;


/**
 * Interface for device presence info object.
 *
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MDevicePresenceInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MDevicePresenceInfo. */
    enum { KInterfaceId = PRES_IF_ID_DEVICE_PRESENCE_INFO };


public:

    /**
     * Public destructor. MDevicePresenceInfo object
     * can be deleted through this interface.
     */
    virtual ~MDevicePresenceInfo() {}


public:

    /**
     * Returns device name value.
     *
     * Returns string identifying type of device.
     *
     * @return Device name.
     */
    virtual const TPtrC8 DeviceName() const = 0;

    /**
     * Set service name.
     *
     * @leave KErrNoMemory if failed to allocate memory
     */
    virtual void SetDeviceNameL( const TDesC8& aDeviceName ) = 0;

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



#endif //MXIMPFWDEVICEPRESENCEINFO_H



