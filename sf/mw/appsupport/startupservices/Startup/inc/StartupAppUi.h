/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*       AppUi class of the application.
*       Includes the execution sequence which contains
*           1. selftest check
*           2. security check
*           3. user welcome note showing
*           4. first boot check
*           5. time and date value queries
*           6. real time clock status check
*
*/


#ifndef STARTUPAPPUI_H
#define STARTUPAPPUI_H

#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>


// USER INCLUDES
#include "startup.hrh"          //internal state types
#include "StartupDefines.h"     //some common defines
 #include <eikappui.h>

// FORWARD DECLARATIONS

class CStartupUserWelcomeNote;
class CStartupPubSubObserver;

 class CStartupAnimationWrapper;
 class CStartupView;

/**
*  'AppUi' class.
*
*/
class CStartupAppUi : public CEikAppUi //: public CAknAppUi
{
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CStartupAppUi();

        /**
        * Destructor.
        */
        ~CStartupAppUi();

        // from CCoeAppUiBase
        void PrepareToExit();

    public:

        /**
        *  Actually stops the timer that is used for showing
        *  User Welcome Note.
        *  @return    void
        */
        void StopTimingL(); // indirectly called

        /**
        *  Just perform Exit(); from the callback function.
        *  @return    void
        */
        void ExitApplication(); // indirectly called

        /**
        *  Callback function to stop timer
        *  This timer is used for showing user welcome note.
        *  @return    TInt
        */
        static TInt DoStopTimingL(TAny* aObject);

        /**
        *  This callback function is used for exiting the application after
        *  some code in somewhere else in this application is finished.
        */
        static TInt DoExitApplication(TAny* aObject);

        /**
        * Check boot reason from the SysUtil
        * @param        None
        * @return       TBool
        */
        TBool HiddenReset();


        /**
        * Called when welcome or operator animation has finished.
        *
        * @since S60 3.2
        */
        void AnimationFinished();

        /**
        * Sets iCleanBoot to EStartupCleanBoot.
        * @param        None
        * @return       void
        */
        void SetCleanBoot();



        void SetCriticalBlockEndedL();
        void WaitingCriticalBlockEndingL();

        /**
        * Starts to wait permission to start startup animation.
        *
        * @since S60 3.2
        */
        void SetWaitingStartupAnimationStartL();

        /**
        * Called when it is allowed to start startup animation.
        *
        * @since S60 3.2
        */
        void WaitingStartupAnimationStartL();

        /** System state has changed to EmergencyCallsOnly. Skip the animations. */
        void SetEmergencyCallsOnlyL();


        /**
        * Check if animation should be loaded in advance and do it.
        */
        void TryPreLoadAnimation();


        /**
        * Propagates fatal startup error state.
        * @param        aPropertyChanged
        *               (ETrue, if state received via property changed event)
        * @return       void
        */
        void SwStateFatalStartupErrorL( TBool aPropertyChanged );

        /**
        * Return value of the__SIMCARD feature
        * @param None
        * @return TBool
        */
        TBool SimSupported();

       

        /**
        *  Checks if DOS is in Offline Mode
        *
        *  @return    TBool
        */
        TBool DosInOfflineModeL();

        /**
        *  This cancels the animation.
        */
        void CancelAnimation();

        /**
        * Checks if boot reason is SIM changed reset
        * @param        None
        * @return       TBool
        */
        TBool SimStatusChangedReset();

    public: // from MEikMenuObserver

        /**
        * EPOC default constructor.
        */
        void ConstructL();

    private: // from CEikAppUi


        /**
         * Functionality Commented as no support from Qt
        * From CAknAppUi.
        * Handles a change to the application's resources which are shared across
        * the environment.
        *
        * @since S60 3.2
        *
        * @param aType The type of resources that have changed.
        
        void HandleResourceChangeL( TInt aType );
        */


        /**
        *  Takes care of command handling.
        *  @param   aCommand        Command to be handled.
        *  @return  void
        */
        void HandleCommandL(TInt aCommand);

        /**
        *  Handles key events.
        *  @param  aKeyEvent           Event to be handled.
        *  @param  aType               Type of the key event
        *  @return TKeyResponse
        */
        virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    private:
        /**
        *  Checks the validity of change and perform needed actions
        *  @param   toState     next phase state phase
        *  @return  void
        */
        void DoNextStartupPhaseL( TStartupInternalState toState );

        /**
        *  First part of the startup phases.
        *  This part does some initial things and in the end
        *  calls DoNextStartupPhaseL() with parameter of next phase
        *  @return    void
        */
        void DoStartupStartPartL();

        /**
        *  This part of the startup phases
        *  shows the welcome animation.
        */
        void DoStartupShowWelcomeAnimationL();

        /**
        *  This part of the startup phases
        *  shows the operator animation.
        */
        void DoStartupShowOperatorAnimationL();

        /**
        *  Predictive Time and Country selection support
        *  Returns ETrue when enabled.
        *  @return TBool
        */
		TBool PredictiveTimeEnabled();
		
        /**
        *  This part takes care of asking some values
        *  from the user in case of first boot or
        *  real time clock value is invalid
        *  @return    void
        */
        void DoStartupFirstBootAndRTCCheckL();

        /**
        *  Last part of the startup phases.
        *  This part does some cleaning things and
        *  calls the Exit().
        *  @return    void
        */
        void DoStartupEndPart();

        /**
        * Check Real Time Clock status from the SysUtil
        * @param        None
        * @return       TBool
        */
        TBool RTCStatus();

        /**
        *  Shows welcome animation to the user.
        *  @return    void
        */
        void ShowWelcomeAnimationL();

        /**
        *  Shows operator animation to the user.
        *  @return    void
        */
        void ShowOperatorAnimationL();

        /**
        *  Shows UserWelcomeNote to the user.
        *  It can be default animation, or user defined text or image.
        *  @return    void
        */
        void ShowUserWelcomeNoteL();

        /**
        *  Shows Off-line Mode query to the user in case
        *  the device is being booted into Off-line Mode.
        *  Sends System Application user's answers via
        *  a Shared Data notification.
        *
        *  @return    void
        */
        void ShowOfflineModeQueryL();

        /**
        * Check Off-line Mode status from the UI side
        * @param None
        * @return TBool : Off-line Mode status
        */
        TBool UiInOfflineMode();

      

        /**
        *  Returns information about is this the first boot happening.
        *  @return    TBool
        */
        TBool FirstBoot();

        /**
        *  Mark first boot happened for the next boot.
        *  @return   void
        */
        void MarkFirstBoot();

        /**
        *  Includes routines that are performed
        *  when global state change to EStartupSystemFatalError.
        *  This method is called only from DoNextStartupPhaseL()!!
        */
        void SystemFatalErrorL();

        
        /**
        * Updates startup UI phase to Publish&Subscribe key KPSStartupUiPhase.
        *
        * @param aValue the new value to be written to the key KPSStartupUiPhase.
        */
        void UpdateStartupUiPhase( TInt aValue );


    private: // ***** Member Data ********************************************


        // The only window-owning control of the Startup application.
        CStartupView* iMainView;

        // Used for showing Welcome Animation. Owned. May not be NULL.
        CStartupAnimationWrapper* iAnimation;



        



        //used for showing user welcome note
        CPeriodic* iNoteTimer; //owns

        //used for exiting application, smoothly without tricky errors
        CPeriodic* iExitTimer; //owns
        
       
        //internal execution state
        TStartupInternalState iInternalState;


        //is used for quarantee only one time continuing
        TBool iStartupFirstBootAndRTCCheckAlreadyCalled;

        //is used for indicating charger or alarm boot
        TBool iChargingOrAlarmBoot;

        //is used to inform if this is the first boot
        TInt iFirstBoot;

        //is used to inform if "clean boot key" is pressed during animation
        TInt iCleanBoot;

        //is used for telling if offline mode is supported
        TBool iOfflineModeSupported;

        //is used for telling if offline mode query is already shown
        TBool iOfflineModeQueryShown;

        //is used for telling if Starter has completed critical block starting
        TBool iCriticalBlockEnded;

        // The result of offline query
        TBool iBootIntoOffline;

        // fatal error during startup
        TBool iSwStateFatalStartupError;

        // Indicates whether it is allowed to start the UI sequence containing animation(s).
        TBool iStartupWaitingShowStartupAnimation;

        CStartupPubSubObserver* iStartupPubSubObserver; //owns

        //is used for telling if SIM card is supported
        TBool iSimSupported;

     
     

  

        TTime iTime;


};

#endif // STARTUPAPPUI_H

// End of file
