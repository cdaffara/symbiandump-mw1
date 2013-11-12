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
* Description:  Implements application shell SAT interface for external 
*                applications.
*
*/



#ifndef MENUSATINTERFACE_H
#define MENUSATINTERFACE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
*  Interface for accessing application shell externally.
*
*  @lib MenuInterface.dll
*  @since Series 60 3.1
*/
class CMenuSATInterface : public CBase
    {

    public:  // Constructors and destructor
        /**
		*
		* Constructor
		*
		*/
		IMPORT_C CMenuSATInterface();

    public: // New functions
        /**
        * 
        * Shows/Hides Sat UI (SIM Services) from 
        * application shell. 
        * Uses Shared Data to pass the update info
        * to AppShell
        * @since Series 60 3.1
        * @param aVisible Flag to indicate if Sat Ui is
        * made visible or hidden. 
        * @param aName New name for the application. 
        * @return Error code
        */
        IMPORT_C TInt MakeSatUiVisible(TBool aVisible, const TDesC& aName=KNullDesC()); 
        
        /**
        * 
        * Shows/Hides Sat UI (SIM Services) from application shell. 
        * Uses Shared Data to pass the update info to AppShell
        * @since Series 60 3.1
        * @param aVisible Flag to indicate if Sat Ui is made visible or hidden. 
        * @param aName New name for the application. 
        * @param aIconInfoId ID for icon info that can be used by AppShell for 
        *                    fecthing SatUi icon from SatServer.
        * @return Error code
        */
        IMPORT_C TInt MakeSatUiVisible
            (TBool aVisible, const TDesC& aName, TUint8 aIconInfoId);

    private:

        /**
        * Function for changing the values in central repository. Internal.
        */
        TInt SetSatUiVisibilityData( TBool aVisible, const TDesC& aName,
            TInt8 aIconInfoId );
};
#endif // MENUSATINTERFACE_H

// End of File
