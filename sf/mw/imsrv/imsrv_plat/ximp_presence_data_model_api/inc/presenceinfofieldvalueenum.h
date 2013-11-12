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
* Description:  Interface for enum presence info field value object.
*
*/

#ifndef MXIMPFWPRESENCEINFOFIELDVALUEENUM_H
#define MXIMPFWPRESENCEINFOFIELDVALUEENUM_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


/**
 * Interface for presence information field value having
 * an enumerated integer value.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoFieldValueEnum : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoFieldValueEnum. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO_FIELD_VALUE_ENUM };

public:

    /**
     * Public destructor. MPresenceInfoFieldValueEnum object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoFieldValueEnum() {}


public:

    /**
     * Returns minimum value of enumeration.
     *
     * @return Minimum allowed value for enumeration
     */
    virtual TInt MinValue() const = 0;


    /**
     * Returns maximum value of enumeration.
     *
     * @return Maximum allowed value for enumeration
     */
    virtual TInt MaxValue() const = 0;


    /**
     * Returns Value of enumeration.
     *
     * @return Value of enumeration
     */
    virtual TInt Value() const = 0;


    /**
     * Set the value of the enumeration.
     *
     * @param [in] aValue
     *             New enum value to set
     *
     * @leave KErrUnderFlow If the enum value is less than the allowed minimum
     * @leave KErrOverFlow If the enum value is more than the allowed maximum
     */
    virtual void SetValueL( TInt aValue ) = 0;

    /**
     * Set the minimum and maximum allowed by the enumeration.
     * The value range set is inclusive, that is [aMin..aMax].
     * @param aMin The minimum
     * @param aMax The maximum
     */
    virtual void SetLimits( TInt aMin, TInt aMax ) = 0;

    };

#endif //MXIMPFWPRESENCEINFOFIELDVALUEENUM_H



