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
* Description:  CFPhaseLoadingPlugins class declaration.
*
*/



#ifndef C_CFPHASELOADINGPLUGINS_H
#define C_CFPHASELOADINGPLUGINS_H

#include "cfphasebase.h"


/**
 *  CF starter phase loading plugins.
 *  In this phase context source and action plugins are loaded.
 *
 *  @lib -
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CCFPhaseLoadingPlugins ):
    public CCFPhaseBase
    {
    public:
    
        /**
         * Two-phased constructor.
         * @param aCF Interface for contexts         
         */
        static CCFPhaseLoadingPlugins* NewL( MCFContextInterface& aCF );
        
        /**
         * Two-phased constructor.
         * @param aCF Interface for contexts         
         */
        static CCFPhaseLoadingPlugins* NewLC( MCFContextInterface& aCF );
    
        /**
         * Destructor.
         */
        ~CCFPhaseLoadingPlugins();
        
    private: // From base classes
        
        // @see CCFPhaseBase        
        void ExecuteL( TRequestStatus* aStatus );

        // @see CCFPhaseBase        
        TCFPhaseId NextPhase() const;                
        
    private:
    
        // Completes request if it is ready
        void DoComplete();

    private:
    
        CCFPhaseLoadingPlugins( MCFContextInterface& aCF );
        void ConstructL();
        
    private: // Data          
    };

#endif // C_CFPHASELOADINGPLUGINS_H
