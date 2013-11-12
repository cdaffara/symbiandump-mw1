/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionPlugInImpl class declaration.
*
*/


#ifndef C_CFACTIONPLUGINIMPL_H
#define C_CFACTIONPLUGINIMPL_H

#include <e32base.h>

class CCFActionPlugIn;
class CCFActionPlugInThread;

/**
* Action plug-in implementation.
* This class wraps inside thread communication.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCFActionPlugInImpl ): public CBase
    {
    public:
    
        /**
        * Symbian two phased contrstuctors.
        *
        * @since S60 4.0
        * @param aPlugIn Pointer to the plugin.
        * @return CCFActionPlugInImpl*
        */
        static CCFActionPlugInImpl* NewL( CCFActionPlugIn* aPlugIn, CCFActionPlugInThread* aPlugInThread );
        static CCFActionPlugInImpl* NewLC( CCFActionPlugIn* aPlugIn, CCFActionPlugInThread* aPlugInThread );
            
        /**
        * Destructor.
        */
        ~CCFActionPlugInImpl();
        
    public: // New methods
    
    	void AsyncExecutionCompleted();
        
    private:
    
        CCFActionPlugInImpl( CCFActionPlugIn* aPlugIn, CCFActionPlugInThread* aPlugInThread );
        void ConstructL();

    private:

    	CCFActionPlugInThread* iPlugInThread;

        // Ref:
        CCFActionPlugIn* iPlugIn;
    };
    
#endif
