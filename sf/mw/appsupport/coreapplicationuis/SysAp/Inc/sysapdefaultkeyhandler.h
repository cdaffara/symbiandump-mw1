/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Default key handling of System Application
*
*/


#ifndef SYSAPDEFAULTKEYHANDLER_H
#define SYSAPDEFAULTKEYHANDLER_H

#include <e32base.h>
#include <coedef.h>
//#include <AknCapServerClient.h>

class RWindowGroup;
class MSysapCallback;

//class RAknKeylock2;
//class CSysApAppUi;


/**
 *  Default key handling of System Application.
 *
 *  System Application is responsible for handling certain keys and launching
 *  functionality related to them. This class contains the platform default implementation
 *  of System Application's key handling.
 *
 *  @lib None.
 *  @since S60 3.2
 */
class CSysApDefaultKeyHandler : public CBase
    {

    public:

        /**
        * Symbian two-phased constructor.
        * 
        * @since S60 3.2
        * @param aCallback reference to System Application callback interface.
        * @return pointer to a new instance of CSysApDefaultKeyHandler.
        */
        static CSysApDefaultKeyHandler* NewL(MSysapCallback& callback);

        /**
        * Destructor.
        */
        virtual ~CSysApDefaultKeyHandler();
        
        /**
        * Handles key events
        * This method impelements the default key handling functionality of System Application.
        *
        * @since S60 3.2
        * @param aKeyEvent contains information about the key event
        * @param aType contains information about the type of the key event
        * @return   EKeyWasNotConsumed    no key handling applied
        *           EKeyWasConsumed       key was handled
        */
    	TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    	
    	/**
    	* Returns whether the key event is considered to be a device mode key event,
    	* which usually causes a transition in the device mode (e.g. grip, flip or twist).
    	*
    	* @since S60 3.2
    	* @param aKeyEvent key event
    	* @return TBool ETrue key event is considered to be a device mode key
    	*               EFalse not a device moed key
    	*/
    	static TBool IsDeviceModeKey( const TKeyEvent& aKeyEvent );

    protected:
        /**
        * Constructor.
        *
        * @param aCallback reference to System Application callback interface.
        */
        CSysApDefaultKeyHandler(MSysapCallback& callback );

    private:
    
        /**
        * C++ default constructor.
        * No implementation.
        */
        CSysApDefaultKeyHandler();
        
        /**
        * Symbian second-phase constructor.
        *
        * @since S60 3.2
        */
        void ConstructL();

        /**
        * Starts an application or brings an already running application
        * to foreground.
        *
        * @since S60 3.2
        * @param aUid UID of the application that is activated
        * @leave a system wide error code
        */
        void ActivateApplicationL( const TUid aUid ) const;
        
        /**
        * Launches PoC application.
        *
        * @since S60 3.2
        * @leave a system wide error code
        */
        void LaunchPocL();
        
#ifndef SYSAP_USE_STARTUP_UI_PHASE 
        /**
        * Returns whether system is in normal operational state, e.g.
        * startup activities has been performed.
        * When the normal state has been reached, it is allowed to activate GUI applications.
        *
        * @since S60 3.2
        * @return TBool ETrue if the normal operational state has been reached.
        */
        TBool IsStateNormal() const;
#endif // SYSAP_USE_STARTUP_UI_PHASE         
        
        /**
        * Returns whether UI is ready after boot.
        */
        TBool UiReady() const;
        
        /**
        * Returns whether device lock is on or not.
        *
        * @since S60 3.2
        * @return TBool ETrue if device lock is enabled, EFalse otherwise.
        */
        TBool IsDeviceLocked() const;
        
        /**
        * Sets display state according to current flip state.
        *
        * @since S60 3.2
        * @param aFlipOpen flip status, ETrue if flip is open.
        */
        void SetDisplayState( TBool aFlipOpen ) const;
        
        /**
        * Shows keys locked note if keypad/device lock is enabled.
        *
        * @since S60 3.2
        * @return TBool ETrue if keys are locked and note was shown
        */
        TBool DoShowKeysLockedNote();
        
    private: // data
        
        // Our window group from SysappUi
 //       RWindowGroup& iRootWindowGroup;
        
        /**
        * Reference to callback object.
        */
        MSysapCallback& iCallback;
        
        /**
        * Stores camera feature flag state. If ETrue, the system has camera installed.
        */        
        TBool iCameraSupported;
        
        /**
        * Stores cover display feature flag state. If ETrue, the system has cover display.
        */        
        TBool iCoverDisplaySupported;
        
        /**
        * Pointer to sysap-owned RAknKeylock2*
        * Not own.
        */
//        RAknKeylock2* iKeylock;
        
        /**
        * Handle of the captured camera key event.
        */
        TInt iCapturedEKeyCamera;
        
        /**
        * Handle of the captured twist key open event.
        */
        TInt iCapturedEKeyTwistOpen;
        
        /**
        * Handle of the captured twist key close event.
        */
        TInt iCapturedEKeyTwistClose;
        
        /**
        * Handle of the captured grip key open event.
        */
        TInt iCapturedEKeyGripOpen;
        
        /**
        * Handle of the captured grip key close event.
        */
        TInt iCapturedEKeyGripClose;
        
        /**
        * Handle of the captured PoC key event.
        */
        TInt iCapturedEKeyPoC;
        
        /**
        * Avkon UI Server connection state.
        */ 
        TBool iAknUiServerConnected;
        
        /**
        * Avkon UI Server
        */
//        RAknUiServer iAknUiServer;
        
    };

#endif // SYSAPDEFAULTKEYHANDLER_H
