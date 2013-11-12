/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*           This class handles the received indications from the Public and
*           Subscribe.
*
*/


#ifndef OUTOFDISKPSOBSERVER_H
#define OUTOFDISKPSOBSERVER_H

// SYSTEM INCLUDES
#include <e32property.h>
#include <UikonInternalPSKeys.h>
#include <coreapplicationuisdomainpskeys.h>

// USER INCLUDES
#include "moutofdiskpropertyresponder.h"

// CLASS DECLARATION
class COutOfDiskMonitor;
class COutOfDiskSubscriber;

class COutOfDiskPSObserver : public CBase, public MOutOfDiskPropertyResponder
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        */
        COutOfDiskPSObserver( COutOfDiskMonitor* aOutOfDiskMonitor );
        
        /**
        * Two-phased constructor.
        */
        static COutOfDiskPSObserver* NewL( COutOfDiskMonitor* aOutOfDiskMonitor );
        
        /**
        * Destructor.
        */
        ~COutOfDiskPSObserver();

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        COutOfDiskPSObserver();

    protected: // From MSysApPropertyResponder

        void HandlePropertyChangedL( const TUid& aCategory, TUint aKey );

    private:    // Data
        //reference to application class
        COutOfDiskMonitor*              iOutOfDiskMonitor; //uses

        COutOfDiskSubscriber*           iUikGlobalNotesAllowedSubscriber;
        COutOfDiskSubscriber*           iAutolockStatusSubscriber;
        
        TBool                           iGlobalNotesAllowed;
        TBool                           iStartupReady;
        TInt                            iAutolockStatus;
        
    };

#endif      // OUTOFDISKPSOBSERVER_H   
            
// End of File
