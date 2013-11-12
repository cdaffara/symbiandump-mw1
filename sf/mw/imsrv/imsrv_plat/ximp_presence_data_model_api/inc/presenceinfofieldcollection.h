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
* Description:  Interface for presence field collection object.
*
*/

#ifndef MXIMPFWPRESENCEINFOFIELDCOLLECTION_H
#define MXIMPFWPRESENCEINFOFIELDCOLLECTION_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


class MPresenceInfoField;

/**
 * Interface for presence field collection object.
 *
 *
 * @todo Interface might be extended with new methods.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoFieldCollection : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoFieldCollection. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO_FIELD_COLLECTION };


public:

    /**
     * Public destructor. MPresenceInfoFieldCollection object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoFieldCollection() {}


public:


    /**
     * Returns the count of presence information
     * fields in this collection.
     *
     * @return Count of presence information fields
     *         in this collection.
     */
    virtual TInt FieldCount() const = 0;


    /**
     * Returns a read-only presence information
     * field from this collection.
     *
     * @param [in] aIndex
     *        Zero-based index of the field to return.
     *
     * @return Reference to a field at aIndex.
     *         Returned reference is guaranteed to remain valid
     *         only until next field is retrieved.
     */
    virtual const MPresenceInfoField& FieldAt(
                TInt aIndex ) const = 0;


    /**
     * Looks up the field having given name.
     *
     * @param [in,out] aPresenceInfoField
     *        On entry, contains the field object from where to start
     *        the lookup or NULL if lookup should be started from
     *        beginning of the collection. If field object given in
     *        entry is not a member of collection, lookup is started
     *        from beginning.
     *        On return, if an matching field object is found, this is
     *        set to point to the found field, else NULL.
     *        Found field object ownership is not returned to caller.
     *
     * @param [in] aMatch
     *        Presence information field type match pattern.
     *        Match pattern may contain question mark and asterisk
     *        characters ("?", "*") resepectively to match single
     *        or multiple characters. Field type values are
     *        folded in pattern matching.
     */
    virtual void LookupFieldByFieldType(
                MPresenceInfoField*& aPresenceInfoField,
                const TDesC8& aMatch ) const = 0;
                
    
    /**
     * Looks up and delete the field having given name.
     *
     * @param [in] aMatch
     *        Presence information field type match pattern.
     *        Match pattern may contain question mark and asterisk
     *        characters ("?", "*") resepectively to match single
     *        or multiple characters. Field type values are
     *        folded in pattern matching.
     * @return KErrNone if deletion successful otherwise KErrNotFound
     */
    virtual TInt DeleteFieldByFieldType(const TDesC8& aMatch ) = 0;


    /**
     * Adds new presence information field to the set.
     * If there exists already a presence information field
     * object in the set with equal field type, existing
     * object is deleted and replaced with given one.
     *
     * @param [in] aPresenceInfoField
     *        New new presence information field object
     *        to add to set. Takes object ownership in success.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void AddOrReplaceFieldL(
                MPresenceInfoField* aPresenceInfoField ) = 0;

    };



#endif //MXIMPFWPRESENCEINFOFIELDCOLLECTION_H



