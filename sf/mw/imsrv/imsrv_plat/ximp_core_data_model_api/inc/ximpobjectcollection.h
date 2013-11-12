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
* Description:  Interface for object collection.
*
*/

#ifndef MXIMPFWOBJECTCOLLECTION_H
#define MXIMPFWOBJECTCOLLECTION_H

#include <e32std.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>



/**
 * Interface for object collection.
 *
 * Object collection is a generic container, capable to
 * contain MXIMPBase derived objects, implemented by the
 * XIMP Framework.
 *
 * Object collection can contain multiple instancies
 * from single MXIMPBase derived object type at time.
 * Object collection supports also attaching name to
 * contained objects to further identify objects.
 *
 * Object collection supports adding and getting objects,
 * and looking up objects by their interface type.
 * Object collection takes the ownership of added
 * objects.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPObjectCollection : public MXIMPBase
    {
public:

    /** Interface ID for the MXIMPObjectCollection. */
    enum { KInterfaceId = XIMP_IF_ID_OBJECT_COLLECTION };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPObjectCollection() {}


public: // Object lookup


    /**
     * Looks up the object having given interface type
     * and zero length name. Returns the first found
     * object.
     *
     * @param [in,out] aObject
     *        Object of given interface type and zero length name
     *        or NULL if no object having given interface type is
     *        found.
     *        On entry, contains the object from where to start
     *        the lookup or NULL if lookup should be started from
     *        beginning of the set. If object given in entry is not
     *        found from set, lookup is started from beginning
     *        of the set.
     *        On return, if an matching object is found, this is
     *        set to point to the found object, else NULL.
     *        Found object ownership is not returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     */
    virtual void LookupByType(
                const MXIMPBase*& aObject,
                TInt32 aInterfaceId ) const = 0;


    /**
     * Looks up the object having given interface
     * type and matching object name.
     *
     * @param [in,out] aObject
     *        On entry, contains the object from where to start
     *        the lookup or NULL if lookup should be started from
     *        the beginning of the set. 
     *        If object given in entry is not found from set, 
     *        lookup is started from the beginning of the set.
     *        On return, if an matching object is found, this is
     *        set to point to the found object, else NULL.
     *        Found object ownership is not returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     *
     * @param [in] aMatch
     *        The object name match pattern. Match pattern may contain
     *        question mark and asterisk characters ("?", "*")
     *        resepectively to match single or multiple
     *        characters. Object names are handled as folded
     *        in pattern matching.
     */
    virtual void LookupByTypeAndName(
                const MXIMPBase*& aObject,
                TInt32 aInterfaceId,
                const TDesC8& aMatch ) const = 0;



public: // Set management


    /**
     * Adds new object to the set, and assignes zero length
     * name for it.
     *
     * @param [in] aObject
     *        New object to add to set. Takes object
     *        ownership in success.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
    virtual void AddObjectL(
                MXIMPBase* aObject ) = 0;



    /**
     * Adds new object to the set. Allows multiple
     * instancies from object type.
     *
     * @param [in] aObject
     *        New object to add to set. Takes object
     *        ownership in success. NULL value not
     *        allowed.
     *
     * @param [in] aName
     *        Name for the added object. Zero length descriptor
     *        or valid Symbian OS object name. I.e. asterisk,
     *        question mark and single colon characters
     *        ("*", "?", ":") are not allowed in the name.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     * @leave KErrBadName if object name does not conform required syntax.
     */
    virtual void AddObjectWithNameL(
                MXIMPBase* aObject,
                const TDesC8& aName ) = 0;



    /**
     * Gets the object having given interface type,
     * and removes object from the set and returns object
     * ownership to caller. Searching is done from the beginning,
     * i.e. least recently used order.
     *
     * Searches objects having zero length name
     * and return the oldest inserted object
     * among of those.
     *
     * @param [out] aObject
     *        Object of given interface type or NULL if
     *        no object having given interface type is found.
     *        Object ownership is returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     *
     * @return ETrue, if an object was found.
     */
    virtual TBool GetByType(
                MXIMPBase*& aObject,
                TInt32 aInterfaceId ) = 0;


    /**
     * Gets up the object having given interface type and
     * matching name, and removes object from the set and
     * returns object ownership to caller. Searching is done 
     * from the beginning, i.e. least recently used order.
     *
     * @param [out] aObject
     *        Object of given interface type or NULL if
     *        no object having given interface type is found.
     *        Object ownership is returned to caller.
     *
     * @param [in] aInterfaceId
     *        The interface type of the object
     *        to be looked up.
     *
     * @param [in] aMatch
     *        The object name match pattern. Match pattern may contain
     *        question mark and asterisk characters ("?", "*")
     *        resepectively to match single or multiple
     *        characters. Names are folded in pattern matching.
     *
     * @return ETrue, if an object was found.
     */
    virtual TBool GetByTypeAndName(
            MXIMPBase*& aObject,
            TInt32 aInterfaceId,
            const TDesC8& aMatch ) = 0;

    };


#endif // MXIMPFWOBJECTCOLLECTION_H

