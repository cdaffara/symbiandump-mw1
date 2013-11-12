/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*           This class is the container class of the CStartupUerWelcomeNote.
*           It is used for showing predefined animation.
*
*/



#ifndef SYSAPSHUTDOWNANIMATION_H
#define SYSAPSHUTDOWNANIMATION_H

#ifdef RD_STARTUP_ANIMATION_CUSTOMIZATION

#include "sanimstartupctrl.h"

class CSysApShutdownImage;

/**
*  Wraps asynchronous operations of a CSAnimStartupCtrl to a call-back calls.
*
*  @lib None
*  @since S60 3.2
*/
class CSysApShutdownAnimation : public CActive
    {

public:

    /**
    * Constructs a CSysApShutdownAnimation object.
    *
    * @since S60 3.2
    *
    * @param aView Parent control for the animation control(s).
    * @return The new object
    */
    static CSysApShutdownAnimation* NewL( CSysApShutdownImage& aView );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSysApShutdownAnimation();

    /**
    * Return ETrue if the animation has any content (image or tone).
    *
    * @since S60 3.2
    *
    * @return ETrue if the animation has any content (image or tone).
    */
    TBool HasContent() const;

    /**
    * Create a startup animation control and load the animation. After animation
    * has been loaded, start playing it.
    *
    * @since S60 3.2
    *
    * @param aCallBack Function to call when animation has finished.
    */
    void Play( const TCallBack& aCallBack );

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
    * @param aView Parent control for the animation control(s).
    */
    CSysApShutdownAnimation( CSysApShutdownImage& aView );

private: // data

    /** Parent control for the animation control(s). */
    CSysApShutdownImage& iView;

    /** Startup animation control. Owned. May be NULL. */
    CSAnimStartupCtrl* iCtrl;

    /** Function to call when animation has finished. */
    TCallBack iCallBack;

    enum TState
        {
        EInitial = 1,
        ELoading,
        EPlaying,
        EFinished
        };
    /** Internal state of the object. */
    TState iState;

    };


#else // RD_STARTUP_ANIMATION_CUSTOMIZATION

// INCLUDES
#include <coecntrl.h>
#include <data_caging_path_literals.hrh>

// CONSTANTS

// FORWARD DECLARATIONS
class CAknBitmapAnimation;
class CSysApAppUi;
class CSysApShutdownTone;

// CLASS DECLARATION

/**
*  This class takes care of showing welcome animatio to the user.
*/
class CSysApShutdownAnimation : public CCoeControl , MCoeControlObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSysApShutdownAnimation* NewL( CSysApAppUi* aSysApAppUi);

        /**
        *  Destructor
        */
        ~CSysApShutdownAnimation();

        /**
        *  This handles the key events in this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

        /**
        * Prepares to show shutdown animation
        *
        * @param iShortPowerKeyPressInitiated indicates whether shutdown has been
        *                                     started with short or long press of power key
        */
        void StartL( TBool iShortPowerKeyPressShutdown );

        /**
        *  This makes the animation module to stop showing animation.
        */
        void EndAnimation();

        /**
        *  Returns the whole duration of the animation, in milliseconds.
        */
        TInt ShowingTime();

        /**
        * This function is called by CSysApShutdownTone to notify that tone has been initialized
        */
        void ToneReady();

        /**
        * This function performs the actual animation showing and tone playing.
        */
        void ContinueStart();

    private:

        /**
        *  C++ default constructor.
        */
        CSysApShutdownAnimation( CSysApAppUi* aSysApAppUi );

        /**
        *  EPOC default constructor
        */
        void ConstructL();

        /**
        *  Is called when size is changed.
        */
        void SizeChanged();

        /**
        *  Returns the count of the components in the container.
        */
        TInt CountComponentControls() const;

        /**
        *  Returns the component specified by aIndex parameter.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        *  Handles the event of the control.
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        /**
        *  Is called by Draw()-function and contains
        *  the drawing intelligence about different states of the execution.
        */
        virtual void DoDrawingL() const;

    private: // Functions from base classes

        /**
        *  Returns the count of the components in the container.
        */
        void Draw(const TRect& aRect) const;

    protected: // Data

        //Used for showing animation
        CAknBitmapAnimation *iAnim; //owns

        //Used for showing white background
        CFbsBitmap* iBackgroundBitmap; //owns

        //Is used for setting the timer over the animation
        TInt iShowingTime; //in milli

        //is used when user cancels the welcome note showing by
        //pressing any key.
        CSysApAppUi* iSysApAppUi; //uses

        //is used when user cancels the welcome note showing by
        //pressing any key. The reason for using callback in OfferKeyEvent()
        //guarantees that EKeyWasConsumed is returned properly before application
        //continues the tight execution.
        CPeriodic* iAnimTimer; //owns

        //used for telling when the animation is showing
        TBool iAnimationShowing;

        //used for telling if animation is cancelled by user.
        TBool iAnimationCancelled;

        //used for playing shutdown tone
        CSysApShutdownTone* iShutdownTone; //owned

        //Indicates that tone is ready
        TBool iToneReady;

        //Indicates that this class is ready for tone sync
        TBool iReadyForToneSync;

        /**
        * Used for checking whether event key should be handled.
        */
        TBool iHandleNextKey;
    };

#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#endif      // SYSAPSHUTDOWNANIMATION_H

// End of File
