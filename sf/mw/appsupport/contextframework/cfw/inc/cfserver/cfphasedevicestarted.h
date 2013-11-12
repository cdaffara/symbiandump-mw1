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
* Description:  CFPhaseDeviceStarted class declaration.
*
*/


#ifndef C_CFPHASEDEVICESTARTED_H
#define C_CFPHASEDEVICESTARTED_H

#include "cfphasebase.h"
#include "cfpropertylistenercallback.h"

class CCFPropertyListener;

/**
* CF starter phase device started.
* This phase is run right after starter informs that device has been started.
*
* @since S60 4.0
* @lib -
*/
NONSHARABLE_CLASS( CCFPhaseDeviceStarted ):
    public CCFPhaseBase,
    public MCFPropertyListenerCallBack
    {
    public:
    
        // Two phased constructors
        static CCFPhaseDeviceStarted* NewL( MCFContextInterface& aCF );
        static CCFPhaseDeviceStarted* NewLC( MCFContextInterface& aCF );
    
        // C++ destructor
        ~CCFPhaseDeviceStarted();
        
    private: // From base classes
        
        // @see CCFPhaseBase        
        void ExecuteL( TRequestStatus* aStatus );

        // @see CCFPhaseBase        
        TCFPhaseId NextPhase() const;
        
        // @see MCFPropertyListenerCallBack
        void HandlePropertyChangedL();
        
    private:
    
        // Completes request if it is ready
        void DoComplete();

    private:
    
        CCFPhaseDeviceStarted( MCFContextInterface& aCF );
        void ConstructL();
        
    private: // Data
    
        // Own: Property listener
        CCFPropertyListener* iPropertyListener;
        
        // Own: Starter state
        TInt iStarterState;
    };

#endif
