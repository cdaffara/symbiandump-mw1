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
* Description:  Declaration of CSAnimEngine class
*
*/


#ifndef SANIMENGINE_H
#define SANIMENGINE_H

#include <e32base.h>
#include <gdi.h> // For TDisplayMode.

class MSAnimObserver;
class RFs;

/**
*  Start-up Animation Engine interface class.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimEngine ) : public CBase
    {

public:

    /**
    * Constructs a CSAnimEngine object.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aObserver Observer for animation events..
    * @return The new object
    */
    IMPORT_C static CSAnimEngine* NewL( RFs& aFs, MSAnimObserver& aObserver );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimEngine();

    /**
    * Set the file name identifying the image / animation to show and other
    * properties that affect how the image / animation is shown.
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
    * Zero indicates infinite. For future use - not currently used.
    * @return KErrNone if file was found, a plug-in for the file type was found
    * and successfully loaded. One of the system-wide error codes otherwise.
    */
    virtual TInt SetImageProperties(
        const TDesC& aImageFileName,
        const TDisplayMode aDisplayMode,
        const TSize& aSize,
        const TTimeIntervalMicroSeconds32& aFrameDelay,
        const TBool aScalingEnabled,
        const TInt aRepeatCount ) = 0;

    /**
    * Set the file name identifying the tone to play and other properties that
    * affect how the tone is played.
    *
    * @since S60 3.2
    *
    * @param aToneFileName Identifies the tone file to use.
    * @param aVolume Volume level to use.
    * @param aVolumeRamp Volume ramp: the period over which the volume level is
    * to rise smoothly from nothing to the normal volume level.
    * @param aRepeatCount How many times to repeat the tone.
    * Zero indicates infinite. For future use - not currently used.
    * @return KErrNone if file was found, a plug-in for the file type was found
    * and successfully loaded. One of the system-wide error codes otherwise.
    */
    virtual TInt SetToneProperties(
        const TDesC& aToneFileName,
        const TInt aVolume,
        const TTimeIntervalMicroSeconds& aVolumeRamp,
        const TInt aRepeatCount ) = 0;

    /**
    * Load the parts of the show (image/animation and/or tone) from file(s).
    * Completes immediately with KErrNone if the show has no parts.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when loading is complete.
    */
    virtual void Load( TRequestStatus& aStatus ) = 0;

    /**
    * Get the backgroud colour of the image.
    * Should only be called after successfully loading the image.
    *
    * @since S60 3.2
    *
    * @return The backgroud colour requested for the image.
    */
    virtual TRgb BackroundColour() const = 0;

    /**
    * Start the show (image/animation and/or tone).
    * Completes immediately with KErrNone if the show has no parts.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when all parts of the show have
    * finished.
    */
    virtual void Start( TRequestStatus& aStatus ) = 0;

    /**
    * Cancel activity.
    *
    * @since S60 3.2
    */
    virtual void Cancel() = 0;

protected:

    /**
    * Constructor.
    *
    * @since S60 3.2
    */
    CSAnimEngine();

private:

    /** Copy constructor. */
    CSAnimEngine( const CSAnimEngine& );
    /** Assignment operator. */
    CSAnimEngine& operator=( const CSAnimEngine& );

    };

#endif // SANIMENGINE_H
