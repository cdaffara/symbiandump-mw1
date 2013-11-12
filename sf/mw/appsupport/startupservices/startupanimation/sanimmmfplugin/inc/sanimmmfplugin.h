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
* Description:  Declaration of CSAnimMmfPlugin class
*
*/


#ifndef SANIMMMFPLUGIN_H
#define SANIMMMFPLUGIN_H

#include <mdaaudiosampleplayer.h>
#include "sanimtoneplugin.h"

class CMdaAudioPlayerUtility;

/**
*  Start-up Tone plug-in using MMF to load and play tones.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimMmfPlugin )
  : public CSAnimTonePlugin,
    public MMdaAudioPlayerCallback
    {

public:

    /**
    * Constructs a CSAnimMmfPlugin object.
    *
    * @since S60 3.2
    *
    * @return The new object
    */
    static CSAnimMmfPlugin* NewL();

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimMmfPlugin();

private:

    /**
    * Load the tone.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aFileName Name of the tone file to load.
    * @param aStatus Request to complete when tone has been loaded.
    */
    virtual void Load(
        RFs& aFs,
        const TDesC& aFileName,
        TRequestStatus& aStatus );

    /**
    * Play the tone.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when playing the tone has finished.
    */
    virtual void Start( TRequestStatus& aStatus );

    /**
    * Cancel loading / playing the tone.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    */
    virtual void Cancel();

	/**
	* From MMdaAudioPlayerCallback.
	* An attempt to open and initialise an audio sample has completed,
	* successfully or otherwise.
    *
    * @since S60 3.2
    *
	* @param aError The status of the audio sample after initialisation.
    * @param aDuration The duration of the audio sample.
	*/
	virtual void MapcInitComplete(
	    TInt aError,
	    const TTimeIntervalMicroSeconds& aDuration );

	/**
	* From MMdaAudioPlayerCallback.
	* An attempt to playback an audio sample has completed, successfully or
	* otherwise.
    *
    * @since S60 3.2
    *
	* @param aError The status of playback.
	*/
	virtual void MapcPlayComplete( TInt aError );

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimMmfPlugin();

private:

    /** Copy constructor. */
    CSAnimMmfPlugin( const CSAnimMmfPlugin& );
    /** Assignment operator. */
    CSAnimMmfPlugin& operator=( const CSAnimMmfPlugin& );

private: // data

    /** Audio player class used to play the requested tone. Owned. May be NULL. */
    CMdaAudioPlayerUtility* iAudioPlayer;

    };

#endif // SANIMMMFPLUGIN_H
