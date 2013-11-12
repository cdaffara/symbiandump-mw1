/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class is the container class of the CSysApShutdownTone.
*                It is used for playing shutdown animation tone.
*
*/


#ifndef __SYSAPSHUTDOWNTONE_H__
#define __SYSAPSHUTDOWNTONE_H__

// INCLUDES
#include <MdaAudioSamplePlayer.h>

// FORWARD DECLARATIONS
class CSysApShutdownAnimation;

// CLASS DECLARATION
class CSysApShutdownTone : public CBase, 
                           public MMdaAudioPlayerCallback
    {
    public: //Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSysApShutdownTone* NewL(TDesC& aShutdownTonePath, CSysApShutdownAnimation* aCallback);

        /**
        * Destructor
        */
        virtual ~CSysApShutdownTone();

    public: // New Functions

        /**
        * Play tone
        */
        TInt Play();

        /**
        * Stop tone
        */
        void Stop();

        /**
        * Check is tone currectly playing
        */        
        TBool Playing();

    private:

        /**
        * C++ constructor.
        */
        CSysApShutdownTone(CSysApShutdownAnimation* aCallback);

        /**
        * Two phase constructor - this creates the audio player object
        */
        void ConstructL(TDesC& aShutdownTonePath);

        /**
        */
        TInt GetRingingToneVolumeL();

    public: // Functions from base classes

        /**
        * From MMdaAudioPlayerCallback, audio initialization complete (Audio ready)
        * @param aError
        * @param aDuration not used internally
        */
        void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);

        /**
        * From MMdaAudioPlayerCallback, audio playing complete
        * @param aError
        */
        void MapcPlayComplete(TInt aError);

    private: //data
        CSysApShutdownAnimation*    iCallback;
        CMdaAudioPlayerUtility*     iTone;
        TBool                       iAudioReady;
        TBool                       iPlaying;

        TInt                        iVolume;
    };

#endif // __SYSAPSHUTDOWNTONE_H__

// End of File
