/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApStartupController class definition. This class
*                implements the MSharedDataNotifyHandler interface and other
*                SysAp functionality that is related to startup.
*
*/


#ifndef SYSAPSTARTUPCONTROLLER_H
#define SYSAPSTARTUPCONTROLLER_H

// INCLUDES
#include <e32base.h>
#include <starterclient.h>
#include "MSysApPropertyResponder.h"

// FORWARD DECLARATIONS
class CSysApAppUi;
class CSysApSubscriber;

// CLASS DECLARATION

/**
*  CSysApStartupController
*
*  @lib   sysap
*  @since 2.7
*/
class CSysApStartupController : public CBase, public MSysApPropertyResponder
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApStartupController* NewL( CSysApAppUi& aSysApAppUi, TBool aOfflineModeSupported );

        /**
        * Destructor.
        */
        virtual ~CSysApStartupController();

        /**
        * Checks the SW state on construction to know the initial states.
        * @param None
        * @return void
        */               
        void DoInitialSwStateCheckL();

    private:
        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @param TBool        aOfflineModeSupported
        * @return void
        */         
        CSysApStartupController( CSysApAppUi& aSysApAppUi, TBool aOfflineModeSupported );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApStartupController();

    protected: // From MSysApPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, const TUint aKey );   

    private:
        // By default, prohibit copy constructor
        CSysApStartupController( const CSysApStartupController& );
    
        // Prohibit assigment operator
        CSysApStartupController& operator= ( const CSysApStartupController& );
    
    public:

        /**
        * Handles change in key KPSGlobalSystemState.
        *
        * @since S60 3.2
        * @param aValue the new value of the key.
        */
        void HandleGlobalSystemStateChangeL( TInt aValue );
        
        /**
        * Returns whether the global system is allowed to be changed. This method
        * is for filtering unnecessary state change calls, even if the API does not
        * require it.
        *
        * @since S60 3.2
        * @return TBool ETrue if call to RStarterSession::SetState is allowed
        */
        TBool GlobalStateChangeAllowed( const RStarterSession::TGlobalState aState ) const;
 
 #ifdef SYSAP_USE_STARTUP_UI_PHASE   
        /**
        * Returns whether both startup UI phase and global system state
        * have reached such state that UI operations can be allowed.
        * 
        * @since S60 3.2
        * @return TBool ETrue if UI operations are allowed
        */
        TBool UiReady() const;
#endif // SYSAP_USE_STARTUP_UI_PHASE        
    
    private:
        CSysApSubscriber*   iGlobalSystemState;
        CSysApAppUi&        iSysApAppUi;
        TBool               iOfflineModeSupported;
        TBool               iFirstNormalState;

        /**
        * Set to ETrue when changes in KPSGlobalSystemState are being handled.
        */
        TBool iSystemStateBeingNotified;
        
        /**
        * Currently notified global system state, updated when received via PubSub key.
        */
        TInt iNotifiedGlobalState;
            
#ifdef SYSAP_USE_STARTUP_UI_PHASE
        /**
        * Subscriber for the key KPSStartupUiPhase.
        * Own.
        */
        CSysApSubscriber* iStartupUiPhaseSubscriber;
        
        /**
        * Whether KPSStartupUiPhase with value EStartupUiPhaseAllDone
        * has been handled.
        */
        TBool iStartupUiAllDoneHandled;
        
        /**
        * Whether KPSStartupUiPhase with value EStartupUiPhaseAllDone
        * has been signaled and also the global system state has such
        * value that UI operations can be allowed.
        */
        TBool iStartupUiAllDone;
        
#endif // SYSAP_USE_STARTUP_UI_PHASE
    };

#endif      // SYSAPSTARTUPCONTROLLER_H
            
// End of File
