/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Server Message interface.
 *
*/

#ifndef XIMPSRVMESSAGE_H
#define XIMPSRVMESSAGE_H

#include "ximppanics.h"
#include <e32base.h>


/**
 * Server Message interface.
 */
class MXIMPSrvMessage
    {
public:

    /**
     * Message argument indexes.
     */
    enum TParamIndex
        {
        Ep0 = 0,
        Ep1 = 1,
        Ep2 = 2,
        Ep3 = 3
        };

public:     // Methods

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPSrvMessage() {};


    /**
     * Completes the client request with given reason.
     *
     * @param aReason The completion code.
     */
    virtual void Complete( TInt aReason ) = 0;


    /**
     * Tests whether this message is completed or not.
     *
     * @return ETrue if the message is completed.
     *         Else EFalse.
     */
    virtual TBool IsCompleted() const = 0;


    /**
     * Returns an integer describing the function being called.
     *
     * @return The function being called over IPC
     */
    virtual TInt Function() const = 0;


    /**
     * Gets message argument as an integer value.
     *
     * @param aIndex The index value identifying the argument.
     * @return The message argument.
     */
    virtual TInt Int( TParamIndex aIndex ) const = 0;


    /**
     * Gets the length of a descriptor argument in the client's process,
     * leaving on failure.
     *
     * @param aIndex The index value identifying the argument.
     * @return The length of the descriptor.
     */
    virtual TInt GetDesLengthL( TParamIndex aIndex ) const = 0;


    /**
     * Reads data from the specified argument into the specified
     * 8-bit target descriptor, and leaving on failure.
     *
     * @param aIndex The index value identifying the argument.
     * @param aDes The target descriptor into which the client
     *             data is to be written.
     */
    virtual void ReadL( TParamIndex aIndex, TDes8& aDes ) const = 0;


    /**
     * Writes data from the specified source descriptor to the
     * 8-bit descriptor argument, and leaving on failure.
     *
     * @param aIndex The index value identifying the argument.
     * @param aDes The source descriptor containing the data to be written.
     */
    virtual void WriteL( TParamIndex aIndex, const TDesC8& aDes ) const = 0;


    /**
     * Panics the client.
     *
     * @param aReason The panic code.
     */
    virtual void PanicClientAndLeaveL( NXIMPPanic::TReason aReason ) const = 0;


    /**
     * Moves message ownership from previous pointer to new given one.
     * (NULLifies existing pointer)
     *
     * @param aNewSelfPtr New self pointer where to assign object ownership.
     *                    Only one place at time can own the message object.
     */
    virtual void PlaceOwnershipHere( MXIMPSrvMessage*& aNewSelfPtr ) = 0;
    };


#endif      //  XIMPSRVMESSAGE_H





