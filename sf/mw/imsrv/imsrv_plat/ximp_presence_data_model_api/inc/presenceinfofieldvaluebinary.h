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
* Description:  Interface for binary presence info field value object.
*
*/

#ifndef MXIMPFWPRESENCEINFOFIELDVALUEBINARY_H
#define MXIMPFWPRESENCEINFOFIELDVALUEBINARY_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>



/**
 * Interface for binary presence info field value object.
 *
 * Interface for presence information field value having
 * binary format. Contained binary value might be
 * image or similar data object.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoFieldValueBinary : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoFieldValueBinary. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO_FIELD_VALUE_BINARY };



public:

    /**
     * Public destructor. MPresenceInfoFieldValueBinary object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoFieldValueBinary() {}


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
     * Returns the binary value.
     *
     * @return The binary value.
     */
    virtual TPtrC8 BinaryValue() const = 0;


    /**
     * Returns the mime type of the contained value.
     *
     * @return The mime type.
     */
    virtual const TPtrC8 MimeType() const = 0;


    /**
     * Sets the binary value.
     *
     * @param [in] aValue
     *        New binary value to set.
     *
     * @leave KErrOverflow if value length exceeds field specific maxlength.
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetBinaryValueL(
                const TDesC8& aValue ) = 0;


    /**
     * Sets the mime type of the contained object.
     *
     * @param [in] aMimeType
     *        New mime type value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetMimeTypeL(
                const TDesC8& aMimeType ) = 0;


    };

#endif //MXIMPFWPRESENCEINFOFIELDVALUEBINARY_H



