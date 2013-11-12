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
* Description:  CFPhaseBase class declaration.
*
*/


#ifndef C_CFPHASEBASE_H
#define C_CFPHASEBASE_H

#include <e32base.h>
#include "cfstartereventhandler.h"

class MCFContextInterface;

/**
* CF starter phase base class.
* Note:
* Do not use iCF in oncstruct phase. iCF is still being initiazlied.
* iCF can be used when RunL is called.
*/
NONSHARABLE_CLASS( CCFPhaseBase ): public CBase
    {
    public:
    
        // Currently supported phases
        enum TCFPhaseId
            {
            // Device start up
            ECFDeviceStarting,
            
            // Device start up completed
            ECFDeviceStarted,
            
            // Loading plugins
            ECFLoadingPlugins,
            
            // Loading rules
            ECFLoadingRules,
            
            // Context Framework initialized and ready
            ECFCfwReady,
            
            // Keep this last!
            ECFStartEnd
            };
    
    public:
    
        // C++ destructor
        ~CCFPhaseBase();
        
    public: // Implementation specific
        
        /**
        * States is allowed to run.
        * Complete aStatus when the phase has been fully completed.
        * 
        * @since S60 4.0
        * @param aStatus Client status to complete.
        * @return None.
        */
        virtual void ExecuteL( TRequestStatus* aStatus ) = 0;
        
        /**
        * Current phase instructions for next phase.
        *
        * @since S60 4.0
        * @param None.
        * @return TCFPhaseId
        */
        virtual TCFPhaseId NextPhase() const = 0;
        
        /**
        * Hanldes CFW starter events.
        *
        * @since S60 3.2
        * @param CFStarterEventHandler::TCFStarterEvents.
        * @return TCFPhaseId
        */
        virtual void HandleEvent( MCFStarterEventHandler::TCFStarterEvents /*aEvent*/ );
        
    public: // New methods
    
        /**
        * Returns phase id.
        *
        * @since S60 4.0
        * @param None.
        * @return TInt
        */
        TInt PhaseId() const;

        /**
         * Cancels the outgoing phase execution.
         * 
         * @since S60 5.1
         * @param None.
         * @return None.
         */
        void Cancel();
        
    protected: // New methods
    
        // Completes phase
        void Complete( TInt aError );
        
    protected:
    
        CCFPhaseBase( TCFPhaseId aId, MCFContextInterface& aCF );
        void ConstructL();
        
    protected: // Data
    
        /** Starter request status */
        TRequestStatus* iStarterRequest;
        
        /** Phase id */
        TCFPhaseId iId;
        
        /** Context Access API */
        MCFContextInterface& iCF;
    };

#endif
