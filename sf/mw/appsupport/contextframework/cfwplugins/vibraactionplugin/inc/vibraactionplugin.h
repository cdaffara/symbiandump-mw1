/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CVibraActionPlugIn class declaration.
*
*/



#ifndef C_VIBRAACTIONPLUGIN_H
#define C_VIBRAACTIONPLUGIN_H

#include <cfactionplugin.h>

#include "vibraactionobserver.h"

class CVibraAction;

/**
* Vibra action plug-in.
* Vibra action can start and stop vibra. Starting can be configured
* via interval and intensity.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CVibraActionPlugIn ): public CCFActionPlugIn,
    public MVibraActionObserver
    {
    public:
    
        // Two phased constructors
        static CVibraActionPlugIn* NewL();
        static CVibraActionPlugIn* NewLC(); 
            
        // Destructor
        ~CVibraActionPlugIn();
        
    private: // From base classes
    
        // @see CCFActionPlugIn
        void InitializeL();
    
        // @see CCFActionPlugIn
        TExecutionTime ExecuteL( CCFActionIndication* aActionIndication );
            
        // @see CCFActionPlugIn
        void GetActionsL( CDesCArray& aActionList ) const;
        
        // @see CCFActionPlugIn
        const TSecurityPolicy& SecurityPolicy() const;
        
        // @see MVibraActionObserver
        void VibraActionCompletedL();
        
    private: // New methods
    
        // Converts string to int
        TInt ConvertToInt( const TDesC& aDesc ) const;

    private:
    
        CVibraActionPlugIn();
        
    private: // Data
    
        /** Vibra controller */
        CVibraAction* iVibraAction;
    };
    
#endif
