/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*     This class provides UI for RFS activation. The process can be canceled
*     from any active object's RunL() while the RFS queries are on the screen.
*     (By deleting the object or using Cancel() method)
*
*/


#ifndef RFS_HANDLER_H
#define RFS_HANDLER_H

//  INCLUDES

#include <e32std.h>
#include <etel.h>
#include <etelmm.h>

// CONSTANTS

// Rfs level

enum TRfsType
    {
    ERfsNormal  = 0,
    ERfsDeep    = 1
    };

// FORWARD DECLARATIONS

class CEikonEnv;
class CSecurityHandler;
class CRfsSecurityHandler;
class CRfsPdpObserver;
class CHbDeviceMessageBoxSymbian;

// CLASS DECLARATION

/**
*  This class provides a method to activate the rfs process including all UI.
*  (notes etc.)
*/
class CRfsHandler : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor doesn't allocate any resources.
        */
        IMPORT_C CRfsHandler();

        /**
        * Destructor also calls Cancel()
        */
        IMPORT_C ~CRfsHandler();

    public: // New functions

        /**
        * Activates the RFS process. Returns when the user either canceled RFS
        * or confirmed it, in which case the phone will be rebooted.
        *
        * While the queries are on the screen, the active scheduler is running,
        * allowing the operation to be canceled.
        *
        * In RFS process:
        *
        * Security code is asked and confirmation query is shown
        * (order depending on the parameter aAskSecurityCodeFirst).
        * If the correct security code is entered and the confirmation query
        * approved, the phone is rebooted with RFS reason.
        *
        * @param aMode - rfs level
        * @param aAskSecurityCodeFirst - If true, the security code is asked
        * first and the confirmation query shown after that. If false,
        * confirmation query is shown first.
        *
        * Method leaves if the initialisations can't be done (problems in ETel
        * connection or memory allocations)
        */
        IMPORT_C void ActivateRfsL( TRfsType aMode, 
                                    TBool aAskSecurityCodeFirst = EFalse );


        /**
        * Cancels the RFS process.
        * Deletes the RFS notes/queries from the screen.
        *
        * This can be called from an active object's RunL() while
        * ActivateRfsL() is being executed.
        *
        * Multiple cancel calls have no effect.
        */
        IMPORT_C void Cancel();

    private:    // new functions

        /**
        * Loads the resources needed for the confirmation query.
        *
        * @return resource file offset
        */
        void LoadResourceL();

        /**
        * Sets the default language code in HAL.
        * This must be done before rebooting the phone for deep level RFS.
        */
        void SetDefaultLanguage() const;

        /**
        * Shows the RFS confirmation query.
        * 
        * @param aThisDestroyed  ETrue, if the RFS handler object was destroyed, otherwise EFalse.          
        * @param aType           Represents RFS reason Normal or Deep.              
        * @return                ETrue, if user selected 'YES' as an option at the confirmation query dialog
        *                        otherwise EFalse.
        * @leave                 leaves with one of the system wide error.
        */
        TBool AskConfirmationL( const TBool& aThisDestroyed, TRfsType aType );

        /**
        * Cleans up the mess, by calling Cancel().
        */
        static void DoCleanup( TAny* aAny );

        /**
        * Checks for calls or data sessions and warns if active
        *
        * @return ETrue if there are active connection, EFalse otherwise
        */
        TBool CheckConnectionsL();

        /**
        * Checks if 2G or 3G connection is active
        *
        * @return ETrue if 2G is active or EFalse if 3G
        */
        TBool IsGprs() const;

    private:    // Data

        // resource file
        TInt iResourceFileOffset;

        // UIKON environment
        CEikonEnv* iEnv;

        // flags
        TInt iFlags;

        // confirmation query
        CHbDeviceMessageBoxSymbian* iQuery;

        // for security code query when using SecUI
        CSecurityHandler* iSecurityHandler;

        RTelServer iTelServer;
        RMobilePhone iPhone;

        // pointer to a stack variable in ActivateRfsL()
        TBool* iDestroyedPtr;

        // for security code query when using Security Engine
        CRfsSecurityHandler* iSecHandler;

        TBool iAlwaysOnlineConnectionClosed;

    private:    // friend classes

        friend class CRfsQueryDialog;
    };

#endif          // RFS_HANDLER_H   
            
// End of File
