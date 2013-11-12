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
* Description:  CCMRFrameBuffer.h
*
*/



#ifndef CCMRFRAMEBUFFER_H
#define CCMRFRAMEBUFFER_H

//  INCLUDES
#include <ecam.h>
#include <e32std.h>

// CONSTANTS

// FORWARD DECLARATIONS

class MCameraBuffer;

// CLASS DECLARATIONS

/**
* Wrapper class from MCameraBuffer to MFrameBuffer
*/

class CCMRFrameBuffer:  public CBase, public MFrameBuffer 
    {


    public: // Construction and destruction

        static CCMRFrameBuffer* NewL( MCameraBuffer* aCameraBuffer );

        virtual ~CCMRFrameBuffer();

    
    public: // General methods / From MFrameBuffer
    
    	/**
    	Gets a non-bitmap frame in the buffer.

	    @param  aIndex
	            The index of the required, non-bitmap, frame.

	    @leave  KErrArgument if aIndex is out of range 
	    @leave  KErrNotSupported if the frame format is bitmap.
	    @return A pointer to the specified non-bitmap format frame of video data.
	    */
	    TDesC8* DataL(TInt aIndex);

	    /** 
	    Gets a bitmap frame in the buffer.

	    @param  aIndex 
	            The index of the required, bitmap format, frame.

    	@leave  KErrArgument if aIndex is out of range and 
	    @leave  KErrNotSupported if
	            the frame format is not a bitmap.
    	@return A pointer to the specified bitmap format frame of video data. 
	    */
	    CFbsBitmap* FrameL(TInt aIndex);

	    /** 
	    Releases the buffer for re-use by the camera once the client has processed
	    the frame data.

	    Signals to CCamera that the buffer data has been used and that the buffer
	    is free for re-use. 
	    */
	    void Release();
	
  
    private: // Private member methods

        /**
        * C++ constructor.
        */
        CCMRFrameBuffer();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        * @param aCameraBuffer  New MCameraBuffer object
        * @return void
        */
        void ConstructL( MCameraBuffer* aCameraBuffer );


    private: // Data
    
        // Stored MCameraBuffer that is owned by Camera-API.
        MCameraBuffer* iCameraBuffer;

    };


#endif // CCMRFRAMEBUFFER_H
