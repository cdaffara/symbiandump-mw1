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
* Description:  Helpers for handling RBuf.
*
*/

#ifndef XIMPRBUFHELPERS_H
#define XIMPRBUFHELPERS_H

#include <e32cmn.h>
#include <s32strm.h>



/**
 * RBuf16 handling helper.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
class XIMPRBuf16Helper
    {
public:

    /**
     * Grows RBuf hold given max length.
     *
     * @param aBuf RBuf reference which to grow.
     * @param aMaxLength Needed max length.
     */
    IMPORT_C static void GrowIfNeededL( RBuf16& aBuf, const TInt aMaxLength );


    /**
     * Sets new value for RBuf.
     *
     * @param aBuf RBuf reference where to set.
     *        ReAllocated if needed during the set.
     * @param aValue Value to set.
     */
    IMPORT_C static void SetNewValueL( RBuf16& aBuf, const TDesC16& aValue );


    /**
     * Externalizes RBuf to stream.
     *
     * @param aBuf RBuf which to externalize.
     * @param aStream The stream to write.
     */
    IMPORT_C static void ExternalizeL( const RBuf16& aBuf, RWriteStream& aStream );


    /**
     * Internalizes referenced RBuf from stream.
     * ReAllocates the RBuf if needed during the internalization.
     *
     * @param aBuf RBuf reference where to internalize.
     * @param aStream The stream from to read.
     */
    IMPORT_C static void InternalizeL( RBuf16& aBuf, RReadStream& aStream );

    };



/**
 * RBuf8 handling helper.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
class XIMPRBuf8Helper
    {
public:

    /**
     * Grows RBuf hold given max length.
     *
     * @param aBuf RBuf reference which to grow.
     * @param aMaxLength Needed max length.
     */
    IMPORT_C static void GrowIfNeededL( RBuf8& aBuf, const TInt aMaxLength );


    /**
     * Sets new value for RBuf.
     *
     * @param aBuf RBuf reference where to set.
     *        ReAllocated if needed during the set.
     * @param aValue Value to set.
     */
    IMPORT_C static void SetNewValueL( RBuf8& aBuf, const TDesC8& aValue );


    /**
     * Externalizes RBuf to stream.
     *
     * @param aBuf RBuf which to externalize.
     * @param aStream The stream to write.
     */
    IMPORT_C static void ExternalizeL( const RBuf8& aBuf, RWriteStream& aStream );


    /**
     * Internalizes referenced RBuf from stream.
     * ReAllocates the RBuf if needed during the internalization.
     *
     * @param aBuf RBuf reference where to internalize.
     * @param aStream The stream from to read.
     */
    IMPORT_C static void InternalizeL( RBuf8& aBuf, RReadStream& aStream );

    };


#endif      //XIMPRBUFHELPERS_H

