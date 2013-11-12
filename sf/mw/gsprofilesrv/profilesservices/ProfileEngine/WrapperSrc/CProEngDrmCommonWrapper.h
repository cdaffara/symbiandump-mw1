/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The wrapper of DRMCommon class.
*
*/


#ifndef CPROENGDRMCOMMONWRAPPER_H
#define CPROENGDRMCOMMONWRAPPER_H


// INCLUDE FILES
#include    <e32base.h>

// FORWARD DECLARATIONS
class DRMCommon;

// CLASS DECLARATION

/**
* This class is a wrapper around the DRMCommon class.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngDrmCommonWrapper ) : public CBase
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngDrmCommonWrapper* NewL();

        /**
         * Destructor.
         */
        virtual ~CProEngDrmCommonWrapper();

    public:  // New functions

        /**
         * Checks if the given file is protected.
         * @param aFileName the file to be checked.
         * @return ETrue if protected, EFalse otherwise.
         */
        TBool IsProtected( const TDesC& aFileName );
        
        /**
         * Checks if the given file is protected.
         * @param aFileName the file to be checked.
         * @return ETrue if protected, EFalse otherwise.
         */
        TBool IsProtectedL( const TDesC& aFileName );

    private:

        /**
         * C++ default constructor.
         */
        CProEngDrmCommonWrapper();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        /// Own: The real DRMCommon 
        DRMCommon* iDrmCommon;

    };

#endif      //  CPROENGDRMCOMMONWRAPPER_H

//  End of File

