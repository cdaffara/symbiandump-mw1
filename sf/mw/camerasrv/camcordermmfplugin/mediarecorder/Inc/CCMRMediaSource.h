/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface class for media source 
*
*/


#ifndef CCMRMEDIASOURCE_H
#define CCMRMEDIASOURCE_H


// INCLUDES
#include <e32base.h>
#include "CCMRMediaSink.h"


// CLASS DECLARATION

/**
*  Internal media source class, a common interface from active output objects to audio & video sources
*  
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class MCMRMediaSource
    {
        
    public:

        /**
        * Sink (output active object) is ready to accept new data
        * @since 2.1
        * @param aStatus    status object for active object
        * @return void
        */
        virtual void RequestNewData(TRequestStatus& aStatus) = 0;

        /**
        * Cancel previous "request new data" request
        */
        virtual void RequestNewDataCancel(TRequestStatus& aStatus) = 0;

        /**
        * Get the next output buffer
        * @since 2.1
        * @return CCMRMediaBuffer* buffer, or NULL if doesn't exist
        */
        virtual CCMRMediaBuffer* GetNextBuffer() = 0;

        /**
        * Get the number of output buffers waiting in the source
        * @since 2.1
        * @return Number of buffers
        */
        virtual TInt NumBuffersWaiting() = 0;

        /**
        * Return the latest time stamp from the input stream
        * @since 2.1
        * @param TTimeIntervalMicroSeconds& aTimeStamp
        */
        virtual void LatestTimeStampL(TTimeIntervalMicroSeconds& aTimeStamp) const = 0;

        /**
        * Return the duration of the recording
        * @since 2.6
        * @param TTimeIntervalMicroSeconds& aDuration
        */
        virtual void DurationL(TTimeIntervalMicroSeconds& aDuration) const = 0;

        /**
        * Return a previously given buffer back to the source
        * @since 2.1
        * @param aBuffer The buffer to return
        * @return void
        */
        virtual void ReturnBuffer(CCMRMediaBuffer* aBuffer) = 0;

    };


#endif // CCMRMEDIASOURCE_H


// End of file
