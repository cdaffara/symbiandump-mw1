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
* Description:  CCallStateSourcePlugIn class declaration.
*
*/



#ifndef C_CALLSTATESOURCEPLUGIN_H
#define C_CALLSTATESOURCEPLUGIN_H

#include <cfcontextsourceplugin.h>
#include <cfcontextobject.h>

#include "PropertyListenerCallBack.h"

class CPropertyListener;

/**
* Call state source.
* Call state source interprets P&S keys related to call state to
* context objects.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCallStateSourcePlugIn ):
    public CCFContextSourcePlugIn,
    public MPropertyListenerCallBack
    {
    public:
    
        // Two phased constructors
        static CCallStateSourcePlugIn* NewL( 
        	TContextSourceInitParams* aParams );
        static CCallStateSourcePlugIn* NewLC( 
        	TContextSourceInitParams* aParams );
            
        // Destructor
        ~CCallStateSourcePlugIn();
    
    public: // From base classes
    
        // @see CCFContextSourcePlugIn
    	void HandleSettingL( CCFContextSourceSettingArray* aSettingList );
            
        // @see CCFContextSourcePlugIn
    	void DefineContextsL();
    	
        // @see CCFContextSourcePlugIn
    	void InitializeL();
    	
        // @see MPropertyListenerCallBack
        void HandlePropertyChangedL();
        
    private: // New methods
    
        // Publishes current call state value
        void PublishContextL();
    
    private:
    
        CCallStateSourcePlugIn( TContextSourceInitParams* aParams );
        
    private:
    
        // Own: Call state property observer
        CPropertyListener* iCallStateProperty;
        
        // Own: Context
        CCFContextObject* iContext;
    };
    
#endif//C_CALLSTATESOURCEPLUGIN_H
