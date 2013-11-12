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
* Description:  rbsobjowningptrarray.inl
*
*/



#include <e32base.h>

// -----------------------------------------------------------------------------
// RBSObjOwningPtrArray<T>::RBSObjOwningPtrArray() implementation.
//
// -----------------------------------------------------------------------------
//
template <class T>
inline RBSObjOwningPtrArray<T>::RBSObjOwningPtrArray()
    {
    }


// -----------------------------------------------------------------------------
// RBSObjOwningPtrArray<T>::Close() implementation.
// Calls ResetAndDestroy() to destroy holded objects.
// -----------------------------------------------------------------------------
//
template <class T>
inline void RBSObjOwningPtrArray<T>::Close()
    {
    RPointerArray<T>::ResetAndDestroy();
    }
