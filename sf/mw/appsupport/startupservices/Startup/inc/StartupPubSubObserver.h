/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CStartupPubSubObserver class.
*
*/


#ifndef STARTUPPUBSUBOBSERVER_H
#define STARTUPPUBSUBOBSERVER_H

//  INCLUDES
#include <e32property.h>
#include "MStartupPropertyResponder.h"
#include "StartupAppUi.h"

// CLASS DECLARATION
class CStartupAppUi;
class CStartupSubscriber;

/**
* Observes changes in P&S keys containing information which Startup application
* is interested in such as global system state.
*/
class CStartupPubSubObserver : public CBase, public MStartupPropertyResponder
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        */
        CStartupPubSubObserver( CStartupAppUi* aStartupAppUi );

        /**
        * Two-phased constructor.
        */
        static CStartupPubSubObserver* NewL( CStartupAppUi* aStartupAppUi );

        /**
        * Destructor.
        */
        ~CStartupPubSubObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CStartupPubSubObserver();

    protected: // From MSysApPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        CStartupAppUi*        iStartupAppUi; //uses

        CStartupSubscriber*   iGlobalSystemStateSubscriber;
        CStartupSubscriber*   iStartupPhaseSubscriber;

#if defined (RD_SCALABLE_UI_V2) && !defined(RD_STARTUP_ANIMATION_CUSTOMIZATION)
        CStartupSubscriber*   iTouchScreenCalibSyncSubscriber;
        TBool   iTouchScreenCalibSupport;
#endif // RD_SCALABLE_UI_V2 && !RD_STARTUP_ANIMATION_CUSTOMIZATION
    
    };

#endif      // STARTUPPUBSUBOBSERVER_H

// End of File
