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
* Description:  Templated algorithms to simplify object handling
*
*/

#ifndef XIMPOBJECTHELPERS_H
#define XIMPOBJECTHELPERS_H

#include <s32mem.h>


class MXIMPObjectCollection;
const TInt KObjectBufGranurality = 100;


/**
 * Templated object mover to move
 * objects from pointer array to another.
 *
 * @since S60 v3.2
 */
template< class T1, class T2 >
class TXIMPObjectMover
    {

public:
    /**
     * Templated object move method.
     *
     * Moves objects from one RPointerArray to another.
     * Object types in the arrays can be different as long as
     * source type is castable to target type.
     *
     * @param aSource Source array from to move.
     * @param aTarget Target array to move.
     */
    static inline void MoveFromPtrArrayToL( RPointerArray< T1 >& aSource,
                                            RPointerArray< T2 >& aTarget );
                                            
    /**
     * Templated object move method.
     *
     * Moves objects from object collection to array.
     * Object types in object collection 
     * hast to be castable to target type.
     *
     * @param aSource Source object collection from to move.
     * @param aTarget Target array to move.
     */
    static inline void MoveFromCollectionToArrayL( MXIMPObjectCollection& aSource,
                                                   RPointerArray< T2 >& aTarget,
                                                   TLinearOrder< T2 >& aOrder );

private:   //Prohibited constructor
    TXIMPObjectMover();
    };

/**
 * Templated object cloner.
 *
 * Object type must support: NewLC(),
 * ExternalizeL() and InternalizeL().
 *
 * @since S60 v3.2
 */
template< class T >
class TXIMPObjectCloner
    {
 public:

    /**
     * Templated object clone method.
     *
     * Instantiates a new object and copies data from
     * given source object with externalize / internalize
     * to new object. Returns new object ownership to caller.
     *
     * @param aObj Templated object to clone.
     *             Object type must support: NewLC(),
     *             ExternalizeL() and InternalizeL() methods.
     *
     * @return New cloned object. Returns new object
     *         ownership to caller. Object is left to
     *         CleanupStack.
     */
    static inline T* CloneLC( const T& aObj );


    /**
     * Templated object clone method.
     *
     * Similar to CloneLC() but object isn't
     * left to CleanupStack.
     *
     * @param aObj Templated object to clone.
     *
     * @return New cloned object. Returns new object
     *         ownership to caller.
     */
    static inline T* CloneL( const T& aObj );


private:   //Prohibited constructor
    TXIMPObjectCloner();

    };








/**
 * Templated object packer.
 *
 * Object type must support: NewLC(),
 * ExternalizeL() and InternalizeL().
 *
 * @since S60 v3.2
 */
template< class T >
class TXIMPObjectPacker
    {
 public:

    /**
     * Templated method to pack object.
     *
     * Packs given object data to flat buffer and
     * returns buffer to caller.
     *
     * @param aObj Templated object to pack.
     *             Object type must support ExternalizeL() method.
     *
     * @return Buffer containing externalized object data.
     *         Buffer ownership is returned to caller.
     */
    static inline HBufC8* PackL( const T& aObj );



    /**
     * Templated method to unpack object.
     *
     * Extracts object data from given flat buffer and
     * returns objects ownership to caller.
     *
     * @param aObj Object which data to internalize from package.
     *             Concrete object type must support InternalizeL() method.
     *
     * @Param aPack Buffer containing object data.
     */
    static void UnPackL( T& aObj,
                         const TDesC8& aPack );


    /**
     * Templated method to pack object array.
     *
     * Packs given object array data to flat buffer and
     * returns buffer to caller.
     *
     * @param aObjs Array of templated objects to pack.
     *              Concrete object type must support
     *              ExternalizeL() method.
     *
     * @return Buffer containing externalized object data.
     *         Buffer ownership is returned to caller.
     */
    static inline HBufC8* PackArrayL( const RPointerArray< T >& aObjs );


    /**
     * Templated method to unpack object array.
     *
     * Instantiates needed amount of new objects,
     * extracts their data from given flat buffer and
     * returns objects ownership to caller.
     *
     * @param aObjs Array where to store extracted objects.
     *              Concrete object type must support
     *              NewLC() and InternalizeL() methods.
     *              Array contents are not reseted during
     *              unpack.
     *
     * @Param aPack Buffer containing object data.
     */
    static inline void UnPackArrayL( RPointerArray< T >& aObjs,
                                     const TDesC8& aPack );




private:   //Prohibited constructor
    TXIMPObjectPacker();

    };


class TXIMPHBuf8Packer
    {
public:
    /**
     * Pack array of buffers to buffer.
     * @param aBufferArray Array of buffers to pack.
     * @return Buffer containing buffers.
     */
    IMPORT_C static HBufC8* PackArrayL( const RPointerArray< HBufC8 >& aBufferArray );
    
    /**
     * Unpack array of buffers to array.
     * @param aBufferArray. Target array for new buffers. Ownhersip of items in array is transferred to caller.
     * @param aPack Packet containing array of buffers.
     */
    IMPORT_C static void UnPackArrayL( RPointerArray< HBufC8 >& aBufferArray, const TDesC8& aPack  );
    };


#include "ximpobjecthelpers.inl"


#endif // XIMPOBJECTHELPERS_H
