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
* Description:  Interface for data subscription state object.
*
*/

#ifndef MXIMPFWDATASUBSCRIPTIONSTATE_H
#define MXIMPFWDATASUBSCRIPTIONSTATE_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>

/**
 * Interface for data subscription state object.
 *
 * Data subscription state object contains information
 * about certain data item subscription state.
 *
 * Protocol adaptation can use this object type to check
 * and update item subscription state.
 *
 * To client direction XIMP Framework uses this
 * object type to inform client about item current
 * status in data change events.
 *
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPDataSubscriptionState : public MXIMPBase

    {
public:

    /** Interface ID for the MXIMPDataSubscriptionState. */
    enum { KInterfaceId = XIMP_IF_ID_DATA_SUBSCRIPTION_STATE };


    /** Data subscription state enumeration. */
    enum TSubscriptionState
        {
        /** Data item subscription isn't active. */
        ESubscriptionInactive,

        /** Data item subscription is active. */
        ESubscriptionActive
        };



    /** Data state enumeration. */
    enum TDataState
        {
        /** Data content isn't available. */
        EDataUnavailable,

        /** Data content is available. */
        EDataAvailable
        };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPDataSubscriptionState() {}



public:


    /**
     * Returns data subscription state.
     *
     * @return The data subscription state.
     */
    virtual TSubscriptionState SubscriptionState() const = 0;



    /**
     * Returns data state.
     *
     * @return The data state.
     */
    virtual TDataState DataState() const = 0;



    /**
     * Sets data subscription state value.
     *
     * @param [in] aSubscriptionState
     *        New data subscription state value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetSubscriptionStateL(
                TSubscriptionState aSubscriptionState ) = 0;



    /**
     * Sets data state value.
     *
     * @param [in] aDataState
     *        New data state value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetDataStateL(
                TDataState aDataState ) = 0;
    };


#endif // MXIMPFWDATASUBSCRIPTIONSTATE_H


