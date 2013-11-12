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
* Description:  Declaration of CSAnimCtrl class
*
*/


#ifndef SANIMCTRL_H
#define SANIMCTRL_H

#include <coecntrl.h>
#include "sanimobserver.h"

class CFbsBitmap;
class CSAnimEngine;

/**
*  Start-up Animation UI control class.
*  Draws animation centered to the control.
*
*  @lib sanimctrl.lib
*  @since S60 3.2
*/
class CSAnimCtrl : public CCoeControl, public MSAnimObserver
    {

public:

    /**
    * Constructs a CSAnimCtrl object.
    *
    * @since S60 3.2
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for this control.
    * @return The new object
    */
    IMPORT_C static CSAnimCtrl* NewL(
        const TRect& aRect,
        const CCoeControl& aContainer );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimCtrl();

    /**
    * Load the parts of the show (image/animation and/or tone) from file(s).
    * Completes immediately with KErrNone if the show has no parts.
    *
    * @since S60 3.2
    *
    * @param aImageFileName Identifies the image file to use.
    * @param aFrameDelay Frame delay to use.
    * @param aScalingEnabled Identifies whether to try to scale the image to fit
    * the screen size or just use the target size of the animation as it is.
    * @param aAnimRepeatCount How many times to repeat the animation.
    * Zero indicates infinite.
    * @param aToneFileName Identifies the tone file to use.
    * @param aVolume Volume level to use.
    * @param aVolumeRamp Volume ramp: the period over which the volume level is
    * to rise smoothly from nothing to the normal volume level.
    * @param aToneRepeatCount How many times to repeat the tone.
    * @param aStatus Request to complete when loading is complete.
    */
    virtual void Load(
        const TDesC& aImageFileName,
        const TTimeIntervalMicroSeconds32& aFrameDelay,
        const TBool aScalingEnabled,
        const TInt aAnimRepeatCount,
        const TDesC& aToneFileName,
        const TInt aVolume,
        const TTimeIntervalMicroSeconds& aVolumeRamp,
        const TInt aToneRepeatCount,
        TRequestStatus& aStatus );

    /**
    * Start the show (image/animation and/or tone).
    * Completes immediately with KErrNone if the show has no parts.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when all parts of the show have
    * finished.
    */
    virtual void Start( TRequestStatus& aStatus );

    /**
    * Cancel activity.
    *
    * @since S60 3.2
    */
    virtual void Cancel();

    /**
    * Clear drawing data.
    *
    * @since S60 3.2
    */
    virtual void Clear();

protected:

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @return The number of controls contained in this control.
    */
    virtual TInt CountComponentControls() const;

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @param aIndex Identifies the component control to return.
    * @return The component control at index given as parameter.
    */
    virtual CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * From CCoeControl.
    *
    * @since S60 3.2
    *
    * @param aRect The rectangle to draw into.
    */
    virtual void Draw( const TRect& aRect ) const;

    /**
    * Draw an animation frame on the screen with a mask.
    * From MSAnimObserver.
    *
    * @since S60 3.2
    *
    * @param aFrame Contains the animation frame to draw on the screen.
    * @param aMask Contains mask for the animation frame to draw on the screen.
    */
    virtual void UpdateScreen( const CFbsBitmap& aFrame, const CFbsBitmap& aMask );

    /**
    * Draw an animation frame on the screen without a mask.
    * From MSAnimObserver.
    *
    * @since S60 3.2
    *
    * @param aFrame Contains the animation frame to draw on the screen.
    */
    virtual void UpdateScreen( const CFbsBitmap& aFrame );

protected:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    IMPORT_C CSAnimCtrl();

    /**
    * Second phase constructor for derived classes.
    *
    * @since S60 3.2
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for this control.
    */
    IMPORT_C void BaseConstructL(
        const TRect& aRect,
        const CCoeControl& aContainer );

private:

    /** Copy constructor. */
    CSAnimCtrl( const CSAnimCtrl& );
    /** Assignment operator. */
    CSAnimCtrl& operator=( const CSAnimCtrl& );

protected: // data

    /** Start-up Animation engine. Owned. May not be NULL. */
    CSAnimEngine* iEngine;

    /** Bitmap to draw next. Not owned. May be NULL. */
    const CFbsBitmap* iImage;

    /** Mask for the bitmap to draw next. Not owned. May be NULL. */
    const CFbsBitmap* iMask;

    /** Background colour to use when drawing. */
    TRgb iBackgroundColour;

private: // data

    TAny* iReserved1; /** Reserved for future use. */
    TAny* iReserved2; /** Reserved for future use. */
    TInt iReserved3; /** Reserved for future use. */
    TInt iReserved4; /** Reserved for future use. */

    };

#endif // SANIMCTRL_H
