/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CFileCreator class
*
*/


#include <f32file.h>

#include "filecreator.h"
#include "trace.h"

// Number of bytes to write to a file at once
const TInt KWriteBlockLength( 0x800 );
// File fill character
const TChar KWriteFillChar( 'Z' );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFileCreator::NewL
// ---------------------------------------------------------------------------
//
CFileCreator* CFileCreator::NewL( RFs& aFs )
    {
    CFileCreator* self = new( ELeave ) CFileCreator( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CFileCreator::~CFileCreator
// ---------------------------------------------------------------------------
//
CFileCreator::~CFileCreator()
    {
    FUNC_LOG;

    delete iWriteBuf;
    }


// ---------------------------------------------------------------------------
// CFileCreator::CreateOneFileL
// ---------------------------------------------------------------------------
//
void CFileCreator::CreateOneFileL( const TDesC& aName, TInt aSizeInBytes )
    {
    FUNC_LOG;
    INFO_2( "File to create: %S (%d bytes)", &aName, aSizeInBytes );

    RFile file;
    TInt err = file.Replace( iFs, aName, EFileWrite );
    ERROR_1( err, "Failed to create file %S", &aName );
    //User::LeaveIfError( err );
    CleanupClosePushL( file );

    TInt blockSize = iWriteBuf->Size();
    TInt lastBlock = aSizeInBytes / blockSize;
    for ( TInt i = 0; i <= lastBlock; i++ )
        {
        err = file.Write(
            i * blockSize,  // Position
            *iWriteBuf,     // Content
            i == lastBlock ? aSizeInBytes % blockSize : blockSize ); // Length
        }
    CleanupStack::PopAndDestroy( &file );
    }


// ---------------------------------------------------------------------------
// CFileCreator::CFileCreator
// ---------------------------------------------------------------------------
//
CFileCreator::CFileCreator( RFs& aFs )
  : iFs( aFs )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CFileCreator::ConstructL
// ---------------------------------------------------------------------------
//
void CFileCreator::ConstructL()
    {
    FUNC_LOG;

    iWriteBuf = HBufC8::NewL( KWriteBlockLength );
    iWriteBuf->Des().Fill( KWriteFillChar, KWriteBlockLength );
    }
