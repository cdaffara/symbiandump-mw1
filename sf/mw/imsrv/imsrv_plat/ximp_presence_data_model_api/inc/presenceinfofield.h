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
* Description:  Interface for presence info field object.
*
*/

#ifndef MXIMPFWPRESENCEINFOFIELD_H
#define MXIMPFWPRESENCEINFOFIELD_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


/**
 * Interface for presence info field object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoField : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoField. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_FIELD };


public:

    /**
     * Public destructor. MPresenceInfoField object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoField() {}


public:


    /**
     * Returns this presence information field's name.
     *
     * @return Presence information field's name.
     */
    virtual const TDesC8& FieldType() const =0;
    
    /**
     * Set this presence information field's name.
     * @param aFieldType The new field type
     */
    virtual void SetFieldTypeL( const TDesC8& aFieldType ) =0;

    /**
     * Returns read-only reference to this presence
     * information field's value storage.
     *
     * @return Presence information field's value storage.
     */
    virtual const MXIMPBase& FieldValue() const =0;


    /**
     * Returns modifiable reference to this presence
     * information field's value storage.
     *
     * @return Presence information field's value storage.
     */
    virtual MXIMPBase& FieldValue() =0;
    
    /**
     * Set the field value object for this field's value storage.
     * Ownership transferred.
     * @param aFieldValue The field value object
     */
    virtual void SetFieldValue( MXIMPBase* aFieldValue ) = 0;

    };



#endif //MXIMPFWPRESENCEINFOFIELD_H



