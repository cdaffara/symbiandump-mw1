/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*           This class the handles the received indications from the Public and
*           Subscribe.
*
*/


#ifndef STARTUPMEDIATORPLUGINPSOBSERVER_H
#define STARTUPMEDIATORPLUGINPSOBSERVER_H

//  INCLUDES
#include <e32property.h>
#include "MStartupMediatorPluginPropertyResponder.h"
#include "StartupMediatorPlugin.h"

// CLASS DECLARATION
class CStartupMediatorPlugin;
class CStartupMediatorPluginSubscriber;

class CStartupMediatorPluginPSObserver : public CBase, public MStartupMediatorPluginPropertyResponder
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        */
        CStartupMediatorPluginPSObserver( CStartupMediatorPlugin* aStartupMediatorPlugin );
        
        /**
        * Two-phased constructor.
        */
        static CStartupMediatorPluginPSObserver* NewL( CStartupMediatorPlugin* aStartupMediatorPlugin );
        
        /**
        * Destructor.
        */
        ~CStartupMediatorPluginPSObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        CStartupMediatorPluginPSObserver();

    protected: // From MSysApPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        CStartupMediatorPlugin*             iStartupMediatorPlugin; //uses

        RProperty                           iProperty;
        CStartupMediatorPluginSubscriber*   iAutoLockStatusSubscriber;
        CStartupMediatorPluginSubscriber*   iKeyGuardStatusSubscriber;
        CStartupMediatorPluginSubscriber*   iGlobalSystemStateSubscriber;
    };

#endif      // STARTUPMEDIATORPLUGINPSOBSERVER_H   
            
// End of File
