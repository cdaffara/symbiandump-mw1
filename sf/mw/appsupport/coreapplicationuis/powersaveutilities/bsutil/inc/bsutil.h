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
* Description:  Battery state utility component providing percentual information
*                about battery capacity.
*
*/


#ifndef BSUTIL_H
#define BSUTIL_H

// CONSTANTS
const TInt KBatteryLevelCount(8);

/**
*  Class of the battery state utility component.
*
*  @lib BSUTIL.DLL
*  @since S60 5.0
*/
NONSHARABLE_CLASS(CBSUtil) : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        *
        */
        IMPORT_C static CBSUtil* NewL();

        /**
        * Two-phased constructor.
        * Leaves instance to cleanup stack.
        *
        */
        IMPORT_C static CBSUtil* NewLC();

        /**
        * Destructor.
        */
        virtual ~CBSUtil();

    public: // Other exported functions
    
        IMPORT_C TUint8 GetBatteryCapacity();

    private:

        /**
        * C++ default constructor.
        */
        CBSUtil();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
         * Read battery levels from cenrep.
         */
        void ReadBatteryLevelsL();
        
        /**
         * Updates battery level and capacity.
         */
        TUint8 UpdateCapacity();

    private:    // Data
    
        TInt iBattelyLevels[KBatteryLevelCount]; // Configured battery levels in percentage
        TUint8 iBatteryCapacity; // Battery capacity in percentages
    };
    
#endif      // BSUTIL_H   
            
// End of File
