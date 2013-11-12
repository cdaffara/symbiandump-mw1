/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CSAnimIhlCtrl class
*
*/


#ifndef SANIMIHLCTRL_H
#define SANIMIHLCTRL_H

#include <e32base.h>
#include <MIHLViewerObserver.h>

class MIHLBitmap;
class MIHLFileImage;
class MIHLImageViewer;
class MSAnimObserver;
class RFs;

/**
*  Controls interaction with IHL.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimIhlCtrl ) : public CActive,
                                     public MIHLViewerObserver
    {

public:

    /**
    * Constructs a CSAnimIhlCtrl object.
    *
    * @since S60 3.2
    *
    * @param aObserver Observer for screen update events.
    * @param aFs Fileserver reference.
    * @param aFileName Name of the image file to load.
    * @param aDisplayMode Display mode to use, if supported by the image.
    * @param aSize Requested size for the image.
    * @param aFrameDelay Frame delay for animation, unless specified in the
    * animation file itself.
    * @param aScalingEnabled Identifies whether to try to scale the image to fit
    * the screen size or just use the target size of the animation as it is.
    * @return The new object
    */
    static CSAnimIhlCtrl* NewL(
        MSAnimObserver& aObserver,
        RFs& aFs,
        const TDesC& aFileName,
        const TDisplayMode aDisplayMode,
        const TSize& aSize,
        const TTimeIntervalMicroSeconds32& aFrameDelay,
        const TBool aScalingEnabled );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimIhlCtrl();

    /**
    * Get the backgroud colour of the image.
    * Should only be called after successfully loading the image.
    *
    * @since S60 3.2
    *
    * @return The backgroud colour requested for the image.
    */
    TRgb BackroundColour() const;

    /**
    * Start loading the animation.
    *
    * @since S60 3.2
    *
    * @param aCallBack To be called when the animation has been loaded.
    */
    void Load( const TCallBack& aCallBack );

    /**
    * Start loading the animation.
    *
    * @since S60 3.2
    *
    * @param aCallBack To be called when the animation has been loaded.
    */
    void Play( const TCallBack& aCallBack );

    /**
    * Returns a Symbian error code indicating the success / failure if loading
    * a frame. Only returns useful data when called after a call-back event,
    * before re-issuing LoadNextFrame.
    *
    * @since S60 3.2
    *
    * @return One of Symbian error codes.
    */
    TInt SuccessCode() const;

    /**
    * Stop loading and playing the animation.
    *
    * @since S60 3.2
    */
    void Stop();


protected:

    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2
    */
    virtual void DoCancel();

    /**
    * From CActive.
    * Handles an active object's request completion event.
    * Never leaves.
    *
    * @since S60 3.2
    */
    virtual void RunL();


private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    *
    * @param aObserver Observer for screen update events.
    * @param aFrameDelay Frame delay for animation, unless specified in the
    * animation file itself.
    */
    CSAnimIhlCtrl(
        MSAnimObserver& aObserver,
        const TTimeIntervalMicroSeconds32& aFrameDelay );

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aFileName Name of the image file to load.
    * @param aDisplayMode Display mode to use, if supported by the image.
    * @param aSize Requested size for the image.
    * @param aScalingEnabled Identifies whether to try to scale the image to fit
    * the screen size or just use the target size of the animation as it is.
    */
    void ConstructL(
        RFs& aFs,
        const TDesC& aFileName,
        const TDisplayMode aDisplayMode,
        const TSize& aSize,
        const TBool aScalingEnabled );

    /**
    * Select size to load the image to.
    *
    * @since S60 3.2
    *
    * @param aSize Requested size for the image.
    * @param aScalingEnabled Identifies whether to try to scale the image to fit
    * the screen size or just use the target size of the animation as it is.
    * @return A size that most closely matches the requested size but still
    * can be used as a load size for the image.
    */
    TSize SelectSize( const TSize& aSize, const TBool aScalingEnabled ) const;

    /**
    * Find an item in the array which is
    * a) as close as the target as possible and
    * b) smaller that the target (both width and height) and
    * c) closer to the target than the optimal size.
    * If there is no such item in the array, return the optimal size.
    *
    * @since S60 3.2
    *
    * @param aTarget Requested size.
    * @param aOptimalSize The optimal size for the image.
    * @param aOptions The available options.
    * @return Selected item in the array, or TSize( 0, 0 ).
    */
    TSize FindClosestMatch(
        const TSize& aTarget,
        const TSize& aOptimalSize,
        const RArray<TSize>& aOptions ) const;


    /**
    * Start loading the image in a multi-frame file.
    *
    * @since S60 3.2
    */
    void LoadNextSubImage();

    /**
    * If there are frames left, increment frame index, show the next frame and
    * set the timer. Otherwise call the call-back function.
    *
    * @since S60 3.2
    */
    void ShowNextFrame();

    /**
    * Update frame to screen.
    *
    * @since S60 3.2
    *
    * @param aFrameIndex Frame index to update.
    */
    void UpdateFrameToScreen( TInt aFrameIndex ) const;

    /**
    * Starts frame timer.
    *
    * @since S60 3.2
    */
    void StartFrameTimer();

    /**
    * Stops load or play before notifying client.
    *
    * @since S60 3.2
    *
    * @param aError System wide error code used in notification.
    * @param aNextState Next state to enter
    */
    void StopAndNotify( TInt aError, TInt aNextState );

    /**
    * Notifies client.
    *
    * @since S60 3.2
    *
    * @param aError System wide error code used in notification.
    * @param aNextState Next state to enter
    */
    void Notify( TInt aError, TInt aNextState );

private:

    /**
    * From MIHLViewerObserver.
    * Handles notifications from IHL image viewer.
    *
    * @since S60 3.2
    */
    void ViewerBitmapChangedL();

    /**
    * From MIHLViewerObserver.
    * Handles error from IHL image viewer.
    *
    * @since S60 3.2
    *
    * @param aError System wide error code.
    */
    void ViewerError( TInt aError );


private: // data

    /** Handle to the file containing the animation to use. */
    RFile iFile;

    /** Observer for animation events. */
    MSAnimObserver& iObserver;

    /**
    * Frame delay for animation, unless specified in the animation file itself.
    */
    TTimeIntervalMicroSeconds32 iFrameDelay;

    /** Image object from IHL. Owned. May not be NULL. */
    MIHLFileImage* iImage;

    /** Identifies the number of animation frames in the image. */
    TInt iFrameCount;

    /** Identifies the animation frame currently being loaded. */
    TInt iFrameIndex;

    /** Animation frames . */
    RPointerArray<MIHLBitmap> iFrames;

    /** To be called when a frame has been loaded. */
    TCallBack iCallBack;

    /**
    * One of Symbian error codes indicating whether the loading of a frame
    * was successful.
    */
    TInt iSuccessCode;

    enum TType
        {
        EStillImage = 1,
        EAnimation,
        EMultiFrame
        };
    /** Type of the image. */
    TType iType;

    enum TState
        {
        EInitial = 1,
        ELoading,
        ELoaded,
        EPlaying,
        EFinished,
        EFailed
        };

    /** Internal state of the object. */
    TInt iState;

    /** Timer for timing the frame showing durations. */
    RTimer iTimer;

    /** Image viewer object from IHL. Owned. May be NULL. */
    MIHLImageViewer* iViewer;

    /** Size for IHL image viewer. */
    TSize iViewerSize;
    
    /* Buffer for image */
	HBufC8* iBuffer;
    };

#endif // SANIMIHLCTRL_H
