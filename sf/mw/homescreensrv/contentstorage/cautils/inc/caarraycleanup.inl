/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#ifndef CA_CLENAUPARRAY_
#define CA_CLENAUPARRAY_

/**
 * Cleanup support method. Call ResetAndDestroy() on the array.
 * @param RPointerArray* as TAny*
 */
template<class T>
inline void ArrayResetAndDestroy( TAny* aArray )
    {
    ( reinterpret_cast<RPointerArray<T>*> ( aArray ) )->ResetAndDestroy();
    }

/**
 * Push a ResetAndDestroy() on the cleanup stack.
 * @param aArray Array.
 */
template<class T>
inline void CleanupResetAndDestroyPushL( RPointerArray<T>& aArray )
    {
    void(*pointerToFunction)( TAny* ) = ArrayResetAndDestroy<T>;
    TCleanupItem item( pointerToFunction,
            reinterpret_cast<TAny*> ( &aArray ) );
    CleanupStack::PushL( item );
    }

#endif
