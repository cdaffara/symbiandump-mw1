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
* Description:  CFStarter class declaration.
*
*/


#ifndef C_CFSTARTER_H
#define C_CFSTARTER_H

#include <e32base.h>

#include "cfphasebase.h"
#include "cfstartereventhandler.h"

class MCFStarterObserver;

/**
* Asynchronous Context Framework starter which provides a simple
* state machine.
*/
NONSHARABLE_CLASS( CCFStarter ): public CActive,
                                 public MCFStarterEventHandler
    {
    public:
    
        // Symbian two phased constructors
        static CCFStarter* NewL( MCFContextInterface& aCF );
        static CCFStarter* NewLC( MCFContextInterface& aCF );
        
        // C++ destructor
        ~CCFStarter();
        
    public: // New methods
    
        /**
        * Adds a new observer.
        *
        * @since S60 4.0
        * @param aObserver New observer.
        * @return None.
        */
        void AddObserverL( MCFStarterObserver* aObserver );

        /**
        * Starts CF starter.
        * CF initialization will start and observers are informed to
        * initialize in phases.
        *
        * @since S60 4.0
        * @param None.
        * @return None.
        */
        void StartL();
        
    private: // From base classes
    
        // @see CActive
        void RunL();

        // @see CActive
        void DoCancel();

        // @see CActive
        TInt RunError( TInt aError );
        
        // @see MCFStarterEventHandler
        void HandleEvent( TCFStarterEvents aEvent );
        
    private: // New methods
    
        // Switch to next phase, returns previous phase
        CCFPhaseBase::TCFPhaseId SwitchToNextPhase();
        
        // Runs current phase
        void RunNextL();
    
    private:
    
        CCFStarter( MCFContextInterface& aCF );
        void ConstructL();
        
    private:
    
        // Own: Current init phase
        CCFPhaseBase::TCFPhaseId iCurrentPhase;
        
        // Own: Observers - pointers not owned
        RPointerArray<MCFStarterObserver> iObservers;
        
        // Own: Phases
        RPointerArray<CCFPhaseBase> iPhases;
        
        // Ref: Context Access API
        MCFContextInterface& iCF;
    };

#endif
