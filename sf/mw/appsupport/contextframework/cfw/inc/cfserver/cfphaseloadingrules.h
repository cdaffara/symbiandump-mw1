/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFPhaseLoadingRules class declaration.
*
*/


#ifndef C_CFPHASELOADINGRULES_H
#define C_CFPHASELOADINGRULES_H

#include "cfphasebase.h"

/**
 *  CF starter phase loading rules.
 *  In this phase rules are loaded.
 *
 *  @lib -
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CCFPhaseLoadingRules ):
    public CCFPhaseBase
    {
    public:
    
        /**
         * Two-phased constructor.
         * @param aCF Interface for contexts         
         */
        static CCFPhaseLoadingRules* NewL( MCFContextInterface& aCF );
        
        /**
         * Two-phased constructor.
         * @param aCF Interface for contexts         
         */
        static CCFPhaseLoadingRules* NewLC( MCFContextInterface& aCF );
    
        /**
         * Destructor.
         */
        ~CCFPhaseLoadingRules();
        
        void HandleEvent( MCFStarterEventHandler::TCFStarterEvents aEvent );
        
    private: // From base classes
        
        // @see CCFPhaseBase        
        void ExecuteL( TRequestStatus* aStatus );

        // @see CCFPhaseBase        
        TCFPhaseId NextPhase() const;
                     
    private:
        
        CCFPhaseLoadingRules( MCFContextInterface& aCF );            
        
        void ConstructL();
        
    private: // Data
        
        /**
         * Boolean for context source plugin loading
         * ETrue when plugins are loaded
         */        
        TBool iContextSourcePluginsLoaded;
        
        /**
         * Boolean for context source plugin loading
         * ETrue when plugins are loaded
         */
        TBool iActionPluginsLoaded;
    };

#endif // C_CFPHASELOADINGRULES_H
