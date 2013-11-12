/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/




#ifndef __UT_CNATFWUNSAFNONCEATTRIBUTE_H__
#define __UT_CNATFWUNSAFNONCEATTRIBUTE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWUNSAFNonceAttribute;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class.
 *
 */
NONSHARABLE_CLASS( UT_CNATFWUNSAFNonceAttribute )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWUNSAFNonceAttribute* NewL();
        static UT_CNATFWUNSAFNonceAttribute* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWUNSAFNonceAttribute();

    private:    // Constructors and destructors

        UT_CNATFWUNSAFNonceAttribute();
        void ConstructL();

    public:     // From observer interface

    private:    // New methods

        void SetupL();

        void Setup2L();

        void Teardown();

        void TestGettersL();

        void TestEncodeAndDecodeL();

        void TestInvalidNonceL();

    private:    // Nonce

        EUNIT_DECLARE_TEST_TABLE;

        CNATFWUNSAFNonceAttribute* iAttribute;

        HBufC8* iNonce;

    };

#endif      //  __UT_CNATFWUNSAFNONCEATTRIBUTE_H__

// End of file
