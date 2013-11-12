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
* Description:  Declaration of CSAnimEngineImpl class
*
*/


#ifndef SANIMENGINEIMPL_H
#define SANIMENGINEIMPL_H

#include "sanimengine.h"

class CSAnimImagePlugin;
class CSAnimPluginCtrl;
class CSAnimTonePlugin;
class RFs;

/**
*  Start-up Animation Engine implementation class.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimEngineImpl ) : public CSAnimEngine
    {

public:

    /**
    * Constructor.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aObserver Observer for animation events..
    */
    CSAnimEngineImpl( RFs& aFs, MSAnimObserver& aObserver );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimEngineImpl();

    /**
    * Call-back to be called when a plug-in completes its activity.
    *
    * @since S60 3.2
    */
    void PluginFinished();

protected:

    /**
    * Set the file name identifying the image / animation to show and other
    * properties that affect how the image / animation is shown.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    *
    * @param aImageFileName Identifies the image file to use.
    * @param aDisplayMode Display mode to use, if supported by the image.
    * @param aSize Requested size for the image.
    * @param aFrameDelay Frame delay to use.
    * @param aScalingEnabled Identifies whether to try to scale the image to fit
    * the screen size or just use the target size of the animation as it is.
    * @param aRepeatCount How many times to repeat the animation.
    * Zero indicates infinite.
    * @return KErrNone if file was found, a plug-in for the file type was found
    * and successfully loaded. One of the system-wide error codes otherwise.
    */
    virtual TInt SetImageProperties(
        const TDesC& aImageFileName,
        const TDisplayMode aDisplayMode,
        const TSize& aSize,
        const TTimeIntervalMicroSeconds32& aFrameDelay,
        const TBool aScalingEnabled,
        const TInt aRepeatCount );

    /**
    * Set the file name identifying the tone to play and other properties that
    * affect how the tone is played.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    *
    * @param aToneFileName Identifies the tone file to use.
    * @param aVolume Volume level to use.
    * @param aVolumeRamp Volume ramp: the period over which the volume level is
    * to rise smoothly from nothing to the normal volume level.
    * @param aRepeatCount How many times to repeat the tone.
    * Zero indicates infinite.
    * @return KErrNone if file was found, a plug-in for the file type was found
    * and successfully loaded. One of the system-wide error codes otherwise.
    */
    virtual TInt SetToneProperties(
        const TDesC& aToneFileName,
        const TInt aVolume,
        const TTimeIntervalMicroSeconds& aVolumeRamp,
        const TInt aRepeatCount );

    /**
    * Load the parts of the show (image/animation and/or tone) from file(s).
    * Completes immediately with KErrNone if the show has no parts.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when loading is complete.
    */
    virtual void Load( TRequestStatus& aStatus );

    /**
    * Get the backgroud colour of the image.
    * Should only be called after successfully loading the image.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    *
    * @return The backgroud colour requested for the image.
    */
    virtual TRgb BackroundColour() const;

    /**
    * Start the show (image/animation and/or tone).
    * Completes immediately with KErrNone if the show has no parts.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when all parts of the show have
    * finished.
    */
    virtual void Start( TRequestStatus& aStatus );

    /**
    * Cancel activity.
    * From CSAnimEngine.
    *
    * @since S60 3.2
    */
    virtual void Cancel();

private:

    /**
    * Store client request status to member variable.
    *
    * @since S60 3.2
    *
    * @param aStatus Request status from client.
    */
    void SetClientRequest( TRequestStatus& aStatus );

    /**
    * Set client request to pending state.
    *
    * @since S60 3.2
    */
    void SetRequestPending();

    /**
    * Complete client request with the given status code.
    *
    * @since S60 3.2
    *
    * @param aStatusCode KErrNone or one of Symbian error codes.
    */
    void CompleteClientRequest( const TInt aStatusCode );

private:

    /** Copy constructor. */
    CSAnimEngineImpl( const CSAnimEngineImpl& );
    /** Assignment operator. */
    CSAnimEngineImpl& operator=( const CSAnimEngineImpl& );

protected: // data

    /** File server session. */
    RFs& iFs;

    /** Observer for animation events. */
    MSAnimObserver& iObserver;

    /** Controller for the image plug-in. Owned. May be NULL. */
    CSAnimPluginCtrl* iImagePluginCtrl;

    /** Controller for the tone plug-in. Owned. May be NULL. */
    CSAnimPluginCtrl* iTonePluginCtrl;

    /** To be used to inform client about request completion. May be NULL. Not owned. */
    TRequestStatus* iClientStatus;

    };

#endif // SANIMENGINEIMPL_H
