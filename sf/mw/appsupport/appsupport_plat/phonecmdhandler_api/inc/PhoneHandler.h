/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef CPHONEHANDLER_H
#define CPHONEHANDLER_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CPhoneHandlerControl;
class CRemConInterfaceSelector;

/**
*  Provides interface for client to load PhoneCmdHandler.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandler ): public CBase  
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneHandler* NewL();
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneHandler* NewL( CRemConInterfaceSelector& aIfSelector );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneHandler();

    public: // New functions
            
    public: // Functions from base classes
	        
    protected:  // New functions
        
    protected:  // Functions from base classes
        
    private:

        /**
        * C++ default constructor.
        */
        CPhoneHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( CRemConInterfaceSelector* aIfSelector = NULL );
    
    public:     // Data
        
    protected:  // Data
    
    private:    // Data
    
        // Controls call handling
        CPhoneHandlerControl* iControl; // owned

    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };

#endif      // CPHONEHANDLER_H   
            
// End of File
