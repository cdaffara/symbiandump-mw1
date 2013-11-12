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


#include <e32base.h>
#include <ximpbase.h>
#include <ximpobjectcollection.h>

#include "ximpapiobjbase.h"



// -----------------------------------------------------------------------------
// TXIMPObjectMover::MoveFromPtrArrayToL() implementation.
// -----------------------------------------------------------------------------
//
template< class T1, class T2 >
void TXIMPObjectMover< T1, T2>::MoveFromPtrArrayToL( RPointerArray< T1 >& aSource,
                                                     RPointerArray< T2 >& aTarget )
    {
    const TInt count = aSource.Count();
    for( TInt ix = 0; ix < count; ix++ )
        {
        T2* targetHolder = aSource[ 0 ];
        aTarget.AppendL( targetHolder );
        aSource.Remove( 0 );
        }
    }

// -----------------------------------------------------------------------------
// TXIMPObjectMover::MoveFromCollectionToArrayL() implementation.
// -----------------------------------------------------------------------------
//
template< class T1, class T2 >
void TXIMPObjectMover< T1, T2>::MoveFromCollectionToArrayL( MXIMPObjectCollection& aSource, 
                                                            RPointerArray< T2 >& aTarget,
                                                            TLinearOrder< T2 >& aOrder )
    {
    MXIMPBase* object = NULL;
    while( aSource.GetByType( object, T1::KInterfaceId ) )
        {
        T2* instance = TXIMPGetImpClassOrPanic< T2 >::From( *object );
        CleanupStack::PushL( instance );
        aTarget.InsertInOrderL( instance, aOrder );
        CleanupStack::Pop( instance );
        }
    }


// -----------------------------------------------------------------------------
// TXIMPObjectCloner::CloneLC() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
T* TXIMPObjectCloner<T>::CloneLC( const T& aObj )
    {
    T* clone = T::NewLC();

    CBufSeg* buffer = CBufSeg::NewL( KObjectBufGranurality );
    CleanupStack::PushL( buffer );

    //Externalize
    RBufWriteStream ws;
    ws.Open( *buffer );
    CleanupClosePushL( ws );
    aObj.ExternalizeL( ws );
    CleanupStack::PopAndDestroy(); // ws

    //Internalize
    RBufReadStream rs;
    rs.Open( *buffer );
    CleanupClosePushL( rs );
    clone->InternalizeL( rs );
    CleanupStack::PopAndDestroy(); // rs

    CleanupStack::PopAndDestroy( buffer );

    return clone;
    }


// -----------------------------------------------------------------------------
// TXIMPObjectCloner::CloneL() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
T* TXIMPObjectCloner<T>::CloneL( const T& aObj )
    {
    T* clone = TXIMPObjectCloner::CloneLC( aObj );
    CleanupStack::Pop( clone );
    return clone;
    }


// -----------------------------------------------------------------------------
// TXIMPObjectPacker::PackL() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
HBufC8* TXIMPObjectPacker<T>::PackL( const T& aObj )
	{
    // we don't know how large the returned descriptor is going to be
    CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
    CleanupStack::PushL( packBuf );

    RBufWriteStream ws;
    ws.Open( *packBuf ); // CSI: 65 #
    CleanupClosePushL( ws );

    aObj.ExternalizeL( ws );

    ws.CommitL();
    CleanupStack::PopAndDestroy(); //ws

    // now CBufFlat contains the descriptor
    HBufC8* packBufDesc = packBuf->Ptr(0).AllocL();

    CleanupStack::PopAndDestroy( packBuf );
    return packBufDesc;

	}

// -----------------------------------------------------------------------------
// TXIMPObjectPacker::UnPackL() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
void TXIMPObjectPacker<T>::UnPackL( T& aObj, const TDesC8& aPack )
	{
    if ( ! aPack.Length() )
        {
        // empty, don't attempt to unpack.
        return;
        }

    RDesReadStream rs;
    CleanupClosePushL( rs );
    rs.Open( aPack ); // CSI: 65 #
    aObj.InternalizeL( rs );
    CleanupStack::PopAndDestroy(); // rs

	}

// -----------------------------------------------------------------------------
// TXIMPObjectPacker::PackArrayL() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
HBufC8* TXIMPObjectPacker<T>::PackArrayL( const RPointerArray< T >& aObjs )
	{
    CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
    CleanupStack::PushL( packBuf );

    RBufWriteStream ws;
    ws.Open( *packBuf ); // CSI: 65 #
    CleanupClosePushL( ws );

    // Get count of objects
    TInt objCount( aObjs.Count() );
    // write the count
    ws.WriteInt32L( objCount );
    // objects
    for ( TInt count(0); count < objCount; count++ )
        {
        aObjs[count]->ExternalizeL( ws );
        }

    ws.CommitL();
    CleanupStack::PopAndDestroy(); //ws

    HBufC8* packBufDesc = packBuf->Ptr(0).AllocL();
    CleanupStack::PopAndDestroy( packBuf );

    return packBufDesc;
	}

// -----------------------------------------------------------------------------
// TXIMPObjectPacker::UnPackArrayL() implementation.
// -----------------------------------------------------------------------------
//
template< class T >
void TXIMPObjectPacker<T>::UnPackArrayL( RPointerArray< T >& aObjs,
                                     const TDesC8& aPack )
	{
	// No reset
    if ( ! aPack.Length() )
        {
        // empty, don't attempt to unpack
        return;
        }

    RDesReadStream rs;
    CleanupClosePushL( rs );
    rs.Open( aPack ); // CSI: 65 #

    // read the object count
    TInt objCount( rs.ReadInt32L() );

	for ( TInt count = 0; count < objCount; count++ )
        {
		// instantiate new object
	    T* theObject = T::NewLC();
	    // extract its data
	    theObject->InternalizeL( rs );
	    // append it to the object array
	    aObjs.AppendL( theObject );
	    CleanupStack::Pop( theObject ); // theObject
		}

    CleanupStack::PopAndDestroy(); // rs
	}
