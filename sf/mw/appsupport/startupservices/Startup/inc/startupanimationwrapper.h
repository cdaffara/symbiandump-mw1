/*
* Copyright (c) 2007,2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CStartupAnimationWrapper class
*
*/


#ifndef STARTUPANIMATIONWRAPPER_H
#define STARTUPANIMATIONWRAPPER_H

#include "sanimstartupctrl.h"

class CStartupView;

/**
*  Wraps asynchronous operations of a CSAnimStartupCtrl to a call-back calls.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CStartupAnimationWrapper ) : public CActive
    {

public:

    /**
    * Constructs a CStartupAnimationWrapper object.
    *
    * @since S60 3.2
    *
    * @param aView Parent control for the animation control(s).
    * @return The new object
    */
    static CStartupAnimationWrapper* NewL( CStartupView& aView );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CStartupAnimationWrapper();

    /**
    * Create a startup animation control and load the animation.
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for the control.
    * @param aParams Central Repository keys for finding out details of the
    * animation.
    * @param aPlayDefaultBeep Indicates whether the default 'beep' tone should
    * be played during the animation, if the parameters do not specify a valid
    * tone file.
    * @param aSyncCommand Identifies the parameters for secondary display
    * startup sync command (only required if secondary display is supported).
    */
    void PreLoad(
        const TRect& aRect,
        const CCoeControl& aContainer,
        const CSAnimStartupCtrl::TAnimationParams& aParams,
        const TBool aPlayDefaultBeep,
        const TInt aSyncCommand );

    /**
    * Create a startup animation control (if not preloaded) and play the animation.
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for the control.
    * @param aParams Central Repository keys for finding out details of the
    * animation.
    * @param aPlayDefaultBeep Indicates whether the default 'beep' tone should
    * be played during the animation, if the parameters do not specify a valid
    * tone file.
    * @param aSyncCommand Identifies the parameters for secondary display
    * startup sync command (only required if secondary display is supported).
    * @param aCallBack Function to call when animation has finished.
    */
    void Play(
        const TRect& aRect,
        const CCoeControl& aContainer,
        const CSAnimStartupCtrl::TAnimationParams& aParams,
        const TBool aPlayDefaultBeep,
        const TInt aSyncCommand,
        const TCallBack& aCallBack );

    /**
    * Return ETrue if the animation was cancelled before it finished.
    */
    TBool WasCancelled() const;

    /**
    * Return ETrue if the animation has any content (image or tone).
    * Return value is meaningful only after CStartupAnimationWrapper::Play has been called.
    *
    * @since S60 3.2
    *
    * @return ETrue if the animation has any content (image or tone).
    */
    TBool HasContent() const;

    /**
    * Makes this application the foreground application.
    *
    * @since S60 3.2
    */
    void BringToForeground();

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
    CStartupAnimationWrapper( CStartupView& aView );

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    */
    void ConstructL();

    /**
    * Create a startup animation control and load the animation.
    *
    * @param aRect Rectangle designated for the control.
    * @param aContainer The compound control that is the container for the control.
    * @param aParams Central Repository keys for finding out details of the
    * animation.
    * @param aPlayDefaultBeep Indicates whether the default 'beep' tone should
    * be played during the animation, if the parameters do not specify a valid
    * tone file.
    * @param aSyncCommand Identifies the parameters for secondary display
    * startup sync command (only required if secondary display is supported).
    */
    void LoadL(
        const TRect& aRect,
        const CCoeControl& aContainer,
        const CSAnimStartupCtrl::TAnimationParams& aParams,
        const TBool aPlayDefaultBeep,
        const TInt aSyncCommand );

    /**
    * Start playing an animation which has already been loaded.
    */
    void StartPlaying();

    /**
    * Cancel the animation if active. Remove animation control from parent
    * control and delete it.
    */
    void DestroyCtrl();

private: // data

    /** Parent control for the animation control(s). */
    CStartupView& iView;

    /** Startup animation control. Owned. May be NULL. */
    CSAnimStartupCtrl* iCtrl;

    /** Function to call when animation has finished. Owned. Not NULL. */
    CAsyncCallBack* iCallBackCaller;

    /** Indicates whether to start playing right after loading completes. */
    TBool iPlayImmediately;

    /** Internal state of the object. */
    enum TState
        {
        EIdle = 1,
        ELoading,
        EReady,
        EPlaying,
        ECancelled
        };
    TState iState;

    /** ETrue if the animation has any content (image or tone). */
    TBool iHasContent;
    };

#endif // STARTUPANIMATIONWRAPPER_H
