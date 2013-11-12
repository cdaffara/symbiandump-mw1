/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CCMRFrameBuffer.cpp
*
*/



// INCLUDE FILES

#include <fbs.h>                            // For CFbsBitmap
#include "CCMRFrameBuffer.h"

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ============================ MEMBER FUNCTIONS ===============================




// -----------------------------------------------------------------------------
// CCMRFrameBuffer::CCMRFrameBuffer
// Default constructor.
// -----------------------------------------------------------------------------
//
CCMRFrameBuffer::CCMRFrameBuffer() 
    {
    }


// -----------------------------------------------------------------------------
// CCMRFrameBuffer::~CCMRFrameBuffer
// Destructor.
// -----------------------------------------------------------------------------
//
CCMRFrameBuffer::~CCMRFrameBuffer()
    {
    PRINT(( _L( "CCMRFrameBuffer::~CCMRFrameBuffer() entering" ) ));

    if ( iCameraBuffer )
        {
        iCameraBuffer->Release();
        }

    PRINT(( _L( "CCMRFrameBuffer::~CCMRFrameBuffer() returning" ) ));
    }


// -----------------------------------------------------------------------------
// CCMRFrameBuffer::ConstructL
// Symbian 2nd phase constructor that can leave.
// -----------------------------------------------------------------------------
//
void CCMRFrameBuffer::ConstructL( MCameraBuffer* aCameraBuffer )
    {
    PRINT(( _L( "CCMRFrameBuffer::ConstructL() entering" ) ));
    
    iCameraBuffer = aCameraBuffer;
    if ( aCameraBuffer )
        {
        iIndexOfFirstFrameInBuffer = aCameraBuffer->iIndexOfFirstFrameInBuffer;
        iElapsedTime = aCameraBuffer->iElapsedTime;
        }
        
    PRINT(( _L( "CCMRFrameBuffer::ConstructL() returning" ) ));
    }


// -----------------------------------------------------------------------------
// CCMRFrameBuffer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRFrameBuffer* CCMRFrameBuffer::NewL( MCameraBuffer* aCameraBuffer )
    {
    PRINT(( _L( "CCMRFrameBuffer::NewL() entering, aCameraBuffer=%x" ), aCameraBuffer ));

    CCMRFrameBuffer* self = new( ELeave ) CCMRFrameBuffer();

    CleanupStack::PushL( self );
    self->ConstructL( aCameraBuffer );
    CleanupStack::Pop( self );

    PRINT(( _L( "CCMRFrameBuffer::NewL() returning" ) ));

    return self;
    }


// -----------------------------------------------------------------------------
// CCMRFrameBuffer::DataL
// -----------------------------------------------------------------------------
//
TDesC8* CCMRFrameBuffer::DataL(TInt aIndex)
    {
    PRINT(( _L( "CCMRFrameBuffer::DataL() entering" ) ));
    TDesC8* data = NULL;
    if ( iCameraBuffer )
        {
        data = iCameraBuffer->DataL( aIndex );
        iIndexOfFirstFrameInBuffer = iCameraBuffer->iIndexOfFirstFrameInBuffer;
        iElapsedTime = iCameraBuffer->iElapsedTime;
        }
    PRINT(( _L( "CCMRFrameBuffer::DataL() returning" ) ));
    return data;
    }

// -----------------------------------------------------------------------------
// CCMRFrameBuffer::FrameL
// -----------------------------------------------------------------------------
//
CFbsBitmap* CCMRFrameBuffer::FrameL(TInt aIndex)
    {
    PRINT(( _L( "CCMRFrameBuffer::FrameL() entering" ) ));
    CFbsBitmap* data = NULL;
    if ( iCameraBuffer )
        {
        data = &iCameraBuffer->BitmapL( aIndex );
        iIndexOfFirstFrameInBuffer = iCameraBuffer->iIndexOfFirstFrameInBuffer;
        iElapsedTime = iCameraBuffer->iElapsedTime;
        }
    PRINT(( _L( "CCMRFrameBuffer::FrameL() returning" ) ));
    return data;
    }
   
// -----------------------------------------------------------------------------
// CCMRFrameBuffer::Release
// -----------------------------------------------------------------------------
//
void CCMRFrameBuffer::Release()
    {
    PRINT(( _L( "CCMRFrameBuffer::Release() entering" ) ));
    if ( iCameraBuffer )
        {
        iCameraBuffer->Release();
        iCameraBuffer = 0;
        iIndexOfFirstFrameInBuffer = 0;
        iElapsedTime = 0;
        }
    PRINT(( _L( "CCMRFrameBuffer::Release() returning" ) ));
    delete this;
    }

    
//  End of File  
