/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*      Cleanup utility which resets a supplied pointer at PopAndDestroy().
*
*/


#ifndef CLEANUP_RESET_POINTER_H
#define CLEANUP_RESET_POINTER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
 * Helper class for implementing CleanupResetPointerPushL function.
 * @see CleanupResetPointerPushL
 */
template<class PointerType>
class CleanupResetPointer 
    {
    private:
        /// Nampespace class -- constructor not implemented.
        CleanupResetPointer();

        /**
         * TCleanupItem callback for CleanupResetPointerPushL.
         * @see CleanupResetPointer::PushL
         */
        static void ResetPointerCallback( TAny* aPtrToPtrToReset );

    public:
        /**
         * Pushes a cleanup item on the cleanup stack which calls 
         * ResetPointerCallback on PopAndDestroy() to reset
         * aPointerToReset.
         *
         * @param aPointerToReset   pointer to the pointer to reset to NULL.
         * @exception   KErrNoMemory if cleanup stack runs out of memory. If
         *              this leave occurs the pointer is still reset to NULL
         *              (standard CleanupStack::PushL semantics).
         */
        inline static void PushL( PointerType*& aPointerToReset )
            {
            CleanupStack::PushL( 
            TCleanupItem( ResetPointerCallback, &aPointerToReset ) );
            }
    };

template<class PointerType>
void CleanupResetPointer<PointerType>::ResetPointerCallback(
    TAny* aPtrToPtrToReset )
    {
    *STATIC_CAST(PointerType**, aPtrToPtrToReset) = NULL;
    }

/**
 * Pushes a "reset pointer" cleanup item on the cleanup stack. 
 * When the "reset pointer" item is destroyed with CleanupStack::PopAndDestroy()
 * it resets the pointer variable passed in to NULL.<p>
 * 
 * Example:<br>
 * <pre>
 * CleanupResetPointerPushL(iFooDialog);
 * iFooDialog = new(ELeave) CFooDialog;
 * iFooDialog->ExecuteLD(R_FOO_DIALOG);
 * CleanupStack::PopAndDestroy();  // resets iFooDialog to NULL
 * </pre><p>
 *
 * Note that in the example above iFooDialog will be reset to NULL:
 * <ol>
 * <li>By the CleanupStack::PopAndDestroy() call after the ExecuteLD() call,
 * or</li> <li>by standard cleanup mechanism if ExecuteLD leaves.</li>
 * </ol>
 *
 * <p>The idea is to prevent a TRAP like here:</p>
 * <pre>
 * iFooDialog = new(ELeave) CFooDialog;
 * TRAPD(err, iFooDialog->ExecuteLD(R_FOO_DIALOG));
 * iFooDialog = NULL;
 * if (err) User::Leave(err);
 * </pre>
 *
 * @param aPointerToReset   pointer variable to reset to NULL on 
 *                          CleanupStack::PopAndDestroy() or leave.
 *
 * @exception   KErrNoMemory if cleanup stack runs out of memory. If this
 *              leave occurs the pointer is still reset to NULL (standard
 *              CleanupStack::PushL semantics).
 */
template<class PointerType>
inline void CleanupResetPointerPushL( PointerType*& aPointerToReset )
    {
    CleanupResetPointer<PointerType>::PushL( aPointerToReset );
    }


#endif // CLEANUP_RESET_POINTER_H
            
// End of File

