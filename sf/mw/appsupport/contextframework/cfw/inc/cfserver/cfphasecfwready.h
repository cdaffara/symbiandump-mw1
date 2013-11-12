/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPhaseCfwReady class declaration.
*
*/


#ifndef C_CFPHASECFWREADY_H
#define C_CFPHASECFWREADY_H

#include "cfphasebase.h"

/**
* CF starter phase.
* This phase is run completed when Context Framework
* has fully started and initialized.
* Main functionality is to publish a context which
* can be utilized in scripts.
*
* @since S60 5.0
* @lib -
*/
NONSHARABLE_CLASS( CCFPhaseCfwReady ): public CCFPhaseBase
    {
    public:
    
        // Two phased constructors
        static CCFPhaseCfwReady* NewL( MCFContextInterface& aCF );
        static CCFPhaseCfwReady* NewLC( MCFContextInterface& aCF );
    
        // C++ destructor
        ~CCFPhaseCfwReady();
        
    private: // Implementation specific
        
        // @see CCFPhaseBase        
        void ExecuteL( TRequestStatus* aStatus );
        
        // @see CCFPhaseBase        
        TCFPhaseId NextPhase() const;
        
    private:
    
        CCFPhaseCfwReady( MCFContextInterface& aCF );
        void ConstructL();
    };

#endif
