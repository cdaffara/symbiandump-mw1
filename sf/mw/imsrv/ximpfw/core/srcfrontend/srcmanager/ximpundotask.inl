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
* Description:  UndoTask related helpers.
*
*/

#include "ximppanics.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryAdd::NewLC()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryAdd<T>* CXIMPUndoPtrArrayEntryAdd<T>::NewLC()
    {
    CXIMPUndoPtrArrayEntryAdd<T>* self = new (ELeave) CXIMPUndoPtrArrayEntryAdd;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryAdd::~CXIMPUndoPtrArrayEntryAdd()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryAdd<T>::~CXIMPUndoPtrArrayEntryAdd()
    {
    if( iObjArray )
        {
        TInt index = iObjArray->Find( iObj );
        if( index != KErrNotFound )
            {
            iObjArray->Remove( index );
            }

        delete iObj;
        }
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryAdd::CXIMPUndoPtrArrayEntryAdd()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryAdd<T>::CXIMPUndoPtrArrayEntryAdd()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryAdd::InitToRemoveAndDestroyOnUndo()
// ---------------------------------------------------------------------------
//
template< class T >
void CXIMPUndoPtrArrayEntryAdd<T>::InitToRemoveAndDestroyOnUndo(
    T& aObj,
    RPointerArray< T >& aObjArray )
    {
    __ASSERT_ALWAYS( !iObjArray,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EPtrArrayEntryAddUndoInitialized ) );

    iObjArray = &aObjArray;
    iObj = &aObj;
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryAdd::CancelUndo()
// ---------------------------------------------------------------------------
//
template< class T >
void CXIMPUndoPtrArrayEntryAdd<T>::CancelUndo()
    {
    iObjArray = NULL;
    iObj = NULL;
    }




// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryReplace::NewLC()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryReplace<T>* CXIMPUndoPtrArrayEntryReplace<T>::NewLC()
    {
    CXIMPUndoPtrArrayEntryReplace<T>* self =
            new (ELeave) CXIMPUndoPtrArrayEntryReplace;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryReplace::~CXIMPUndoPtrArrayEntryReplace()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryReplace<T>::~CXIMPUndoPtrArrayEntryReplace()
    {
    if( iObjArray )
        {
        const TInt index = iObjArray->Find( iTarget );
        if( index != KErrNotFound )
            {
            (*iObjArray)[ index ] = iSource;
            delete iTarget;
            }
        else
            {
            delete iSource;
            }
        }
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryReplace::CXIMPUndoPtrArrayEntryReplace()
// ---------------------------------------------------------------------------
//
template< class T >
CXIMPUndoPtrArrayEntryReplace<T>::CXIMPUndoPtrArrayEntryReplace()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryReplace::InitToReplaceOnUndo()
// ---------------------------------------------------------------------------
//
template< class T >
void CXIMPUndoPtrArrayEntryReplace<T>::InitToReplaceOnUndo(
    T& aTarget,
    RPointerArray< T >& aObjArray,
    T& aSource )
    {
    __ASSERT_ALWAYS( !iObjArray,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EPtrArrayEntryReplaceUndoInitialized ) );

    iObjArray = &aObjArray;
    iTarget = &aTarget;
    iSource = &aSource;
    }


// ---------------------------------------------------------------------------
// CXIMPUndoPtrArrayEntryReplace<T>::CancelUndo()
// ---------------------------------------------------------------------------
//
template< class T >
void CXIMPUndoPtrArrayEntryReplace<T>::CancelUndo()
    {
    iObjArray = NULL;
    iTarget = NULL;

    delete iSource;
    iSource = NULL;
    }



