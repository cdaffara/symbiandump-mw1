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
* Description:  Interface for restricted object collection.
*
*/

#ifndef MXIMPFWRESTRICTEDOBJECTCOLLECTION_H
#define MXIMPFWRESTRICTEDOBJECTCOLLECTION_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>



/**
 * Interface for restricted object collection.
 *
 * Object collection is a generic container, capable to
 * contain MXIMPBase derived objects, implemented by the
 * XIMP Framework.
 *
 * Restricted object collection forces each contained
 * object instance to be distinct interface type.
 * I.e. at time there can be just single object
 * instance of certain MXIMPBase derived type
 * at the collection.
 *
 * Object collection supports adding and getting objects,
 * and looking up objects by their interface type.
 * Object collection takes the ownership of added
 * objects.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPRestrictedObjectCollection : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPRestrictedObjectCollection. */
    enum { KInterfaceId = XIMP_IF_ID_RESTRICTED_OBJECT_COLLECTION };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPRestrictedObjectCollection() {}



public: // Object lookup

    /**
     * Looks up the object having given interface type.
     *
     * @param [out] aObject
     *        Object of given interface type or NULL if
     *        no object having given interface type is found.
     *        Object ownership is not returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     */
    virtual void LookupByType(
                const MXIMPBase*& aObject,
                TInt32 aInterfaceId ) const = 0;



public: // Object management


    /**
     * Adds new object to the set. If there exists already
     * a object in the set with equal interface type,
     * existing object is deleted and replaced with new one.
     *
     * @param [in] aObject
     *        New object to add to set. Takes object
     *        ownership in success.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void AddOrReplaceTypeL(
                MXIMPBase* aObject ) = 0;



    /**
     * Gets the object having given interface type,
     * and removes object from the set and returns object
     * ownership to caller.
     *
     * @param [out] aObject
     *        Object of given interface type or NULL if
     *        no object having given interface type is found.
     *        Object ownership is returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     */
    virtual void GetByType(
                MXIMPBase*& aObject,
                TInt32 aInterfaceId ) = 0;

    };


#endif // MXIMPFWRESTRICTEDOBJECTCOLLECTION_H

