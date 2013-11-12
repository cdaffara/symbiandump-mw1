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
* Description:  Interface for status object.
*
*/

#ifndef MXIMPFWSTATUS_H
#define MXIMPFWSTATUS_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


/**
 * Interface for status object.
 *
 * Status object is used in XIMP Framework API
 * to present result code and additional textual
 * details from request completions, context
 * state changes and similar events.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPStatus : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPStatus. */
    enum { KInterfaceId = XIMP_IF_ID_STATUS };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPStatus() {}


public:

    /**
     * Gets request completion code.
     *
     * Result code can be:
     *  - one of the system wide errors
     *  - one of the ximp core specific errors defined in in ximperrors.hrh
     *  - one of the presence specific errors defined in in presenceerrors.hrh
     *  - error specific to the underlying communication channel
     *
     * @return The result code.
     */
    virtual TInt ResultCode() const = 0;


    /**
     * Gets the protocol level response code.
     *
     *
     * @return Protocol level response code.
     *         KErrNone if no protocol level response code available.
     */
    virtual TInt ProtocolResultCode() const = 0;



    /**
     * Get the textual result description.
     * Textual descriptions are protocol specific.
     *
     * @return The textual result description.
     *         Empty descriptor if no description available.
     */
    virtual const TDesC16& ProtocolResultDescription() const = 0;



    /**
     * Sets request completion code.
     *
     * @param [in] aCode
     *        The result code.
     */
    virtual void SetResultCode(
                TInt aCode ) = 0;



    /**
     * Sets textual result description.
     *
     * @param [in] aCode
     */
    virtual void SetProtocolResultCode(
                TInt aCode ) = 0;



    /**
     * Sets textual result description.
     *
     * @param [in] aDescription
     *        The textual result description.
     */
    virtual void SetProtocolResultDescriptionL(
                const TDesC16& aDescription ) = 0;

    };


#endif // MXIMPFWSTATUS_H
