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
* Description:  Helpers for handling objects
*
*/

#include "ximpobjecthelpers.h"

// ==================== TXIMPHBuf8Packer MEMBER FUNCTIONS ====================

// -----------------------------------------------------------------------------
// TXIMPHBuf8Packer::PackArrayL()
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TXIMPHBuf8Packer::PackArrayL( const RPointerArray< HBufC8 >& aBufferArray )
    {
    CBufFlat* packBuf = CBufFlat::NewL( KObjectBufGranurality );
    CleanupStack::PushL( packBuf );

    RBufWriteStream ws;
    ws.Open( *packBuf ); // CSI: 65 #
    CleanupClosePushL( ws );

    // Get count of objects
    TInt objCount( aBufferArray.Count() );
    // write the count
    ws.WriteInt32L( objCount );
    // objects
    for ( TInt count(0); count < objCount; count++ )
        {
        ws.WriteInt32L( aBufferArray[ count ]->Size() );
        ws.WriteL( *aBufferArray[ count ] );
        }

    ws.CommitL();
    CleanupStack::PopAndDestroy(); //ws

    HBufC8* packBufDesc = packBuf->Ptr(0).AllocL();
    CleanupStack::PopAndDestroy( packBuf );

    return packBufDesc;
    }


// -----------------------------------------------------------------------------
// TXIMPHBuf8Packer::UnPackArrayL()
// -----------------------------------------------------------------------------
//
EXPORT_C void TXIMPHBuf8Packer::UnPackArrayL( RPointerArray< HBufC8 >& aBufferArray, const TDesC8& aPack  )
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
        TInt length = rs.ReadInt32L();
        HBufC8* theObject = HBufC8::NewLC( length );
        TPtr8 theObjectPtr = theObject->Des();
        
        rs.ReadL( theObjectPtr, length );
        aBufferArray.AppendL( theObject );
		}
    CleanupStack::Pop( objCount ); // all the objects
    CleanupStack::PopAndDestroy(); // rs    
    }
// End of file
