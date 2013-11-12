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
* Description:  Media recorder observer interface
*
*/


#ifndef CCMRMEDIARECORDEROBSERVER_H
#define CCMRMEDIARECORDEROBSERVER_H

// CLASS DECLARATION

/**
*  Media recorder observer interface
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/

class MCMRMediaRecorderObserver 
    {
    public:

        /**
        * Informs the user that PrepareL() has been completed with or
        * without errors. If aError != KErrNone, media recorder remains in 
        * open state
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroPrepareComplete(TInt aError) = 0;    

        /**
        * Informs the user about a state change. 
        * Useful to find out when a new prepare is needed after 
        * a setting was changed
        * @since 2.1
        * @param aState     New state (CCMRMediaRecorder - TRecorderState)
        * @param aError     Error code
        * @return void
        */
        virtual void MmroStateChange(TInt aState, TInt aError) = 0;

        /**
        * Informs the user about a temporary runtime error. 
        * Recording can't continue right now but it may
        * be possible to continue later (e.g. audio HW was 
        * taken by a higher priority application)
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroTemporaryError(TInt aError) = 0;    

        /**
        * Informs the user about a fatal runtime error (e.g. OOM). 
        * MediaRecorder must be deleted and recreated.
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroFatalError(TInt aError) = 0;
    
    };

#endif  // CCMRMEDIARECORDEROBSERVER_H

// End of file
