/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GSServerEngine is responsible for initializing the TV
*                driver during the boot and always when the cable is connected.
*
*/



#ifndef GSSERVERENGINE_H
#define GSSERVERENGINE_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// CONSTANTS
const TInt KDefaultTextMaxLength = 30;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRepository;
// CLASS DECLARATION

/**
*  API for reading and changing the TvConfig settings
*
*  @lib GSServerEngine.lib
*  @since Series60 3.1
*/
NONSHARABLE_CLASS( CGSServerEngine ) : public CBase
    {
    public:
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CGSServerEngine* NewL();
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CGSServerEngine* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CGSServerEngine();

    public: // New functions.
        /**
        * Return the current value of the tv system and initializes the driver.
        * @return setting value
        */
        IMPORT_C TInt TvSystemL();
        
        /**
        * Set the current value of the tv system.
        * @param aTvSystem The value.
        */
        IMPORT_C void SetTvSystemL( const TInt aTvSystem );

        /**
        * Return the current value of the aspect ratio.
        * @return setting value
        */
        IMPORT_C TInt AspectRatioL();
        
        /**
        * Set the current value of the aspect ratio and initializes the driver.
        * @param aRatio The value.
        */
        IMPORT_C void SetAspectRatioL( const TInt aRatio );

        /**
        * Return the current value of the flicker filter.
        * @return setting value.
        */
        IMPORT_C TInt FlickerFilterL();
        
        /**
        * Set the current value of the flicker filter and initializes the
        * driver.
        * @param aFlickerFilter The value.
        */
        IMPORT_C void SetFlickerFilterL( TInt aFlickerFilter );

    private:
        /**
        * C++ default constructor.
        */
        CGSServerEngine();

        /**
        * 2nd phase constructor. Create and connect to the driver and Central
        * Repository.
        */
        void ConstructL();

        /**
        * Switching value from 1 to 0 and back
        */
        void SwitchValue( TInt& aValue );
        
        /**
        * Converting aspect ratio value for display driver
        */
        void ConvertAspectRatio( TInt& aValue );
        
        /**
        * Converting tv system value for display driver
        */
        void ConvertTvSystem( TInt& aValue );
        
    private:
    	// Handle to the Central Repository.
    	CRepository*    iRepository;
    };

#endif      // GSSERVERENGINE_H

// End of File
