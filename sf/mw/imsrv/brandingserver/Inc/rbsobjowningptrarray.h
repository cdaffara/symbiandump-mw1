/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  RBSOBJOWNINGPTRaRRAY.H
*
*/


#ifndef _RBSOBJOWNINGPTRARRAY_H
#define _RBSOBJOWNINGPTRARRAY_H

#include <e32std.h>



/**
 * Templated array of pointers. Array owns the pointed objects
 * and overwrites the normal RPointerArray::Close()
 * behaviour to destroy objects, before reseting the
 * array contents.
 *
 * Array is used with Presence Framework interface objects.
 *
 * @since S60 v3.2
 */
template< class T >
class RBSObjOwningPtrArray : public RPointerArray<T>
    {
 public:

    /**
     * Default C++ constructor.
     */
    inline RBSObjOwningPtrArray();


    /**
     * Overwritten Close().
     *
     * Calls ResetAndDestroy() to delete holded objects
     * before reseting array contents. Thus array can be
     * pushed on the cleanup stack with CleanupClosePushL().
     */
    inline void Close();
    };




#include "rbsobjowningptrarray.inl"

#endif // _RBSOBJOWNINGPTRARRAY_H



