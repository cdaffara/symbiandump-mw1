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
* Description:  CFPhaseDeviceStarting class declaration.
*
*/


#ifndef C_CFPHASEDEVICESTARTING_H
#define C_CFPHASEDEVICESTARTING_H

#include "cfphasebase.h"

/**
* CF starter phase device starting.
* This phase is run right after context framework server has been
* started.
*
* @since S60 4.0
* @lib -
*/
NONSHARABLE_CLASS( CCFPhaseDeviceStarting ): public CCFPhaseBase
    {
    public:
    
        // Two phased constructors
        static CCFPhaseDeviceStarting* NewL( MCFContextInterface& aCF );
        static CCFPhaseDeviceStarting* NewLC( MCFContextInterface& aCF );
    
        // C++ destructor
        ~CCFPhaseDeviceStarting();
        
    private: // Implementation specific
        
        // @see CCFPhaseBase        
        void ExecuteL( TRequestStatus* aStatus );
        
        // @see CCFPhaseBase        
        TCFPhaseId NextPhase() const;
        
    private:
    
        CCFPhaseDeviceStarting( MCFContextInterface& aCF );
        void ConstructL();
    };

#endif
