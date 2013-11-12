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
* Description:  Declaration of CSecureFormatterTest class
*
*/


#ifndef C_SECUREFORMATTERTEST_H
#define C_SECUREFORMATTERTEST_H

#include <testscripterinternal.h>

/**
*  Test cases for CSecureFormatterTest class.
*
*  @lib None
*/
NONSHARABLE_CLASS( CSecureFormatterTest ) : public CScriptBase
    {

public:

    /**
    * Constructs a CSecureFormatterTest object.
    *
    * @return The new object
    */
    static CSecureFormatterTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CSecureFormatterTest();

public: // Functions from base classes

    TInt RunMethodL( CStifItemParser& aItem );

private:

    /**
    * First phase constructor.
    */
    CSecureFormatterTest( CTestModuleIf& aTestModuleIf );

    /**
    * Second phase constructor.
    */
    void ConstructL();

private: // New methods

    TInt RunTestL( CStifItemParser& aItem );
    TInt RunSpecialTestFileOpenL( CStifItemParser& aItem );
    TInt RunSpecialTestAttributesL( CStifItemParser& aItem );
    TInt RunAppSpecialTestL( CStifItemParser& aItem );
    TInt RunAppSpecialReplaceTestL( CStifItemParser& aItem );

    void SetupDataL( CStifItemParser& aItem, TDes& aSpecFile );
    void SetupAppSpecialDataL( CStifItemParser& aItem, TDes& aSpecFile, TBool aReplace );
    void RunAndVerifyL( const TDesC& aSpecFile, const TDesC& aExcludeList );

    };

#endif // C_SECUREFORMATTERTEST_H
