/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A wrapper interface around MMF (Multimedia Framework) to get
*                information about MIME-types supported by MMF.
*
*/



#ifndef PROENGMMFINFOUTILITY_H
#define PROENGMMFINFOUTILITY_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CDesC8Array;

// CLASS DECLARATION

/**
*  A helper class used by Profiles Engine Wrapper to get the list of supported
*  MIME types from MMF.
*
*  @lib ProfileEngine.lib
*  @since Series 60 3.1
*/
NONSHARABLE_CLASS( ProEngMmfInfoUtility )
    {
    public: // New functions

        /**
         * Gets the list of supported MIME types from MMF.
         * @param aMimeArray the array that contains the list of supported MIME
         *        types on return
         */
        static void GetSupportedMimeTypesL( CDesC8Array& aMimeArray );

    private:  // Constructor and destructor

        /**
         * Default constructor. This is private since this class is not intended
         * to be instantiated.
         */
        ProEngMmfInfoUtility() {};

        // Destructor.
        ~ProEngMmfInfoUtility() {};
    };

#endif      // PROENGMMFINFOUTILITY_H

// End of File
