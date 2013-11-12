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
* Description:  Interface for textual presence info field value object.
*
*/

#ifndef MXIMPFWPRESENCEINFOFIELDVALUETEXT_H
#define MXIMPFWPRESENCEINFOFIELDVALUETEXT_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>



/**
 * Interface for textual presence info field value object..
 *
 * Interface for presence information field value having
 * textual format. Contained text can be rendered to
 * end user as is.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoFieldValueText : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoFieldValueText. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO_FIELD_VALUE_TEXT };


public:

    /**
     * Public destructor. MPresenceInfoFieldValueText object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoFieldValueText() {}


public:

    /**
     * Returns maximum length of the field value.
     *
     * @return Maximum length of the field value or
     *         KXIMPFieldValueLengthUnlimited if the value
     *         length is limited only by the available resources.
     */
    virtual TInt MaxLength() const = 0;


    /**
     * Returns the textual value.
     *
     * @return The textual value.
     */
    virtual TPtrC TextValue() const =0;


    /**
     * Sets the textual data.
     *
     * @param [in] aValue
     *        New textual value to set.
     *
     * @leave KErrOverflow if value length exceeds field specific maxlength.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetTextValueL(
                const TDesC& aValue ) =0;

    };


#endif //MXIMPFWPRESENCEINFOFIELDVALUETEXT_H



