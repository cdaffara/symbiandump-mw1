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
* Description:  Base interface and related helpers for XIMPFw interfaces.
*
*/

#include <e32base.h>

// -----------------------------------------------------------------------------
// RXIMPObjOwningPtrArray<T>::RXIMPObjOwningPtrArray() implementation.
//
// -----------------------------------------------------------------------------
//
template <class T>
inline RXIMPObjOwningPtrArray<T>::RXIMPObjOwningPtrArray()
    {
    }

// -----------------------------------------------------------------------------
// RXIMPObjOwningPtrArray<T>::Close() implementation.
// Calls ResetAndDestroy() to destroy holded objects.
// -----------------------------------------------------------------------------
//
template <class T>
inline void RXIMPObjOwningPtrArray<T>::Close()
    {
    RPointerArray<T>::ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// RXIMPObjOwningPtrArray<T>::~RXIMPObjOwningPtrArray()
// @todo R-class destrcutor - will be removed from interface.
// -----------------------------------------------------------------------------
//
template <class T>
inline RXIMPObjOwningPtrArray<T>::~RXIMPObjOwningPtrArray()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// TXIMPRequestId::TXIMPRequestId()
// C++ default constructor.
// -----------------------------------------------------------------------------
//
inline TXIMPRequestId::TXIMPRequestId()
    {
    Mem::FillZ( iRequestId, TXIMPRequestId::EMaxSize );
    }

// -----------------------------------------------------------------------------
// TXIMPRequestId::operator==()
// Tests request ID for equality with the specified request ID.
// -----------------------------------------------------------------------------
//
inline TBool TXIMPRequestId::operator==( const TXIMPRequestId& aRhs ) const
    {
    if ( Mem::Compare( aRhs.iRequestId,
                       TXIMPRequestId::EMaxSize,
                       iRequestId,
                       TXIMPRequestId::EMaxSize ) == 0 )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// TXIMPRequestId::operator!=()
// Tests this request ID for inequality with the specified request ID.
// -----------------------------------------------------------------------------
//
inline TBool TXIMPRequestId::operator!=( const TXIMPRequestId& aRhs ) const
    {
    if ( Mem::Compare( aRhs.iRequestId,
                       TXIMPRequestId::EMaxSize,
                       iRequestId,
                       TXIMPRequestId::EMaxSize ) != 0 )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// TXIMPRequestId::Null()
// Returns a null-valued request ID
// -----------------------------------------------------------------------------
//
inline TXIMPRequestId TXIMPRequestId::Null()
    {
    return TXIMPRequestId();
    }

// -----------------------------------------------------------------------------
// TXIMPRequestId::Init()
// Returns a integer-valued request ID
// -----------------------------------------------------------------------------
//
inline TXIMPRequestId TXIMPRequestId::Init( TInt aValue )
    {
    TXIMPRequestId id;
    Mem::Copy( &id.iRequestId, &aValue, sizeof( TInt ) );
    return id;
    }

// -----------------------------------------------------------------------------
// TXIMPGetInterface<T>::From()
// Const interface query and type cast.
// -----------------------------------------------------------------------------
//
template <class T>
inline T* TXIMPGetInterface<T>::From( const MXIMPBase& aObj,
                                      MXIMPBase::TIfGetOps aOps )
    {
    const TAny* ifPtr = aObj.GetInterface( T::KInterfaceId, aOps );
    const T* ifTypePtr = static_cast< const T* >( ifPtr );
    return ifTypePtr;
    }

// -----------------------------------------------------------------------------
// TXIMPGetInterface<T>::From()
// Non-const interface query and type cast.
// -----------------------------------------------------------------------------
//
template< class T >
inline T* TXIMPGetInterface<T>::From( MXIMPBase& aObj,
                                      MXIMPBase::TIfGetOps aOps )
    {
    TAny* ifPtr = aObj.GetInterface( T::KInterfaceId, aOps );
    T* ifTypePtr = static_cast< T* >( ifPtr );
    return ifTypePtr;
    }
