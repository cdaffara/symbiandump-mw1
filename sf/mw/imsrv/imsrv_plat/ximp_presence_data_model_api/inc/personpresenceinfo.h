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
* Description:  Interface for person presence info object.
*
*/

#ifndef MXIMPFWPERSONPRESENCEINFO_H
#define MXIMPFWPERSONPRESENCEINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MPresenceInfoFieldCollection;


/**
 * Interface for person presence info object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPersonPresenceInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MPersonPresenceInfo. */
    enum { KInterfaceId = PRES_IF_ID_PERSON_PRESENCE_INFO };


public:

    /**
     * Public destructor. MPersonPresenceInfo object
     * can be deleted through this interface.
     */
    virtual ~MPersonPresenceInfo() {}


public:

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



#endif //MXIMPFWPERSONPRESENCEINFO_H



