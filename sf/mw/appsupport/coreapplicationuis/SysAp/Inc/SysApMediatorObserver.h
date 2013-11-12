/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*/


#ifndef SYSAPMEDIATOROBSERVER_H
#define SYSAPMEDIATOROBSERVER_H

//  INCLUDES

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
  #include <MediatorCommandInitiator.h>
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

#include <MediatorCommandResponder.h>
#include <MediatorEventProvider.h>
#include <w32std.h>   // Remove this when SecondaryDisplayStartupAPI.h includes it properly.
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>

#include "SysApAppUi.h"

// CLASS DECLARATION
class CSysApAppUi;
class CSysApMediatorObserver : public CBase,
                            #ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
                               public MMediatorCommandResponseObserver,
                            #endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
                               public MMediatorCommandObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSysApMediatorObserver* NewL( CSysApAppUi* aSysApAppUi );

        /**
        * Destructor.
        */
        ~CSysApMediatorObserver();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Issues a command
        *
        * @return Error
        */
        TInt IssueCommand(TInt aCommandId, TInt aData);
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

        /**
        * Sends shutdown animation skip event
        */
        void ShutdownAnimationSkipped();

#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        /**
        * Synchronizes shutdown animation with cover display
        *
        * @return Error
        */
        TInt SyncShutdownAnimation();

    public: // MMediatorCommandResponderObserver
        void CommandResponseL( TUid aDomain,
                               TUid aCategory,
                               TInt aCommandId,
                               TInt aStatus,
                               const TDesC8& aData );
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION

    public: // MMediatorCommandObserver
        void MediatorCommandL( TUid aDomain,
                               TUid aCategory,
                               TInt aCommandId,
                               TVersion aVersion,
                               const TDesC8& aData );

    public: // MMediatorCommandObserver
        void CancelMediatorCommand( TUid aDomain,
                                    TUid aCategory,
                                    TInt aCommandId );

    private:

        /**
        * C++ constructor.
        */
        CSysApMediatorObserver( CSysApAppUi* aSysApAppUi );

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

    private: //data

        CSysApAppUi*                iSysApAppUi; //not owned
#ifndef RD_STARTUP_ANIMATION_CUSTOMIZATION
        CMediatorCommandInitiator*  iCommandInitiator;
#endif // RD_STARTUP_ANIMATION_CUSTOMIZATION
        CMediatorCommandResponder*  iCommandResponder;
        CMediatorEventProvider*     iEventProvider;

    };

#endif      // SysApMEDIATOROBSERVER_H

// End of File
