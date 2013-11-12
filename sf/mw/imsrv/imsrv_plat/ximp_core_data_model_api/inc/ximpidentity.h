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
* Description:  Interface for identity object.
*
*/

#ifndef MXIMPFWIDENTITY_H
#define MXIMPFWIDENTITY_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


/**
 * Interface for identity object.
 *
 * Identity objects are used to identify
 * presentities, presentity groups and
 * presentity group members, XIMP framework
 * client applications etc. entities in
 * XIMP Framework.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPIdentity : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPIdentity. */
    enum { KInterfaceId = XIMP_IF_ID_IDENTITY };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPIdentity() {}


public:


    /**
     * Returns identity string.
     *
     * @return The identity string.
     */
    virtual const TDesC16& Identity() const = 0;



    /**
     * Sets identity.
     *
     * @param [in] aIdentity
     *        Identity string.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetIdentityL(
                const TDesC16& aIdentity ) = 0;

    };


#endif // MXIMPFWIDENTITY_H
