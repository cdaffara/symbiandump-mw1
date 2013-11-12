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
* Description:  Interface for XIMP protocol info object.
*
*/

#ifndef MXIMPFWPROTOCOLINFO_H
#define MXIMPFWPROTOCOLINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


class MXIMPFeatureInfo;


/**
 * Interface for XIMP protocol info object.
 *
 * Holds information about the XIMP protocol
 * implementation.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPProtocolInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPProtocolInfo. */
    enum { KInterfaceId = XIMP_IF_ID_PROTOCOL_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPProtocolInfo() {}


public:

    /**
     * Returns human readable protocol display name.
     *
     * @return Protocol display name
     */
    virtual const TDesC16& DisplayName() const = 0;


    /**
     * Returns protocol implementation UID.
     *
     * @return The protocol implementation UID.
     */
    virtual TUid ImplementationUid() const = 0;


    /**
     * Retrieves XIMP Protocol features.
     *
     * Retrieves features supported by the XIMP Protocol.
     * Note that XIMP Protocol may indicate here more
     * features than what will be actually available
     * in XIMP Context since since connected remote
     * XIMP Service may limit available XIMP Context
     * features.
     *
     * @return Features supported in XIMP Protocol.
     *         Exact feature IDs are defined in NXIMPFeature
     *         namespace. Object ownership is returned
     *         to caller.
     */
    virtual MXIMPFeatureInfo* GetProtocolFeaturesLC() const = 0;

    };


#endif // MXIMPFWPROTOCOLINFO_H
