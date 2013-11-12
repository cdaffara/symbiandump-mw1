/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  xmppsettingsfetchermtCases.cpp
*
*/



// [INCLUDE FILES] - do not remove
#include <e32math.h>
#include "xmppsettingsfetchermt.h"
#include "xmppsettingsfetcher.h"
#include "xmppparameters.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cxmppsettingsfetchermt::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo Cxmppsettingsfetchermt::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    xmppsettingsfetchermt.cpp file and to xmppsettingsfetchermt.h 
    *    header file.
    *
    * 2) Add entry to following KCases array either by using:
    *
    * 2.1: FUNCENTRY or ENTRY macro
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    *
    * FUNCENTRY macro takes only test case function name as a parameter and
    * uses that as a test case name and test case function name.
    *
    * Or
    *
    * 2.2: OOM_FUNCENTRY or OOM_ENTRY macro. Note that these macros are used
    * only with OOM (Out-Of-Memory) testing!
    *
    * OOM_ENTRY macro takes five parameters: test case name, test case 
    * function name, TBool which specifies is method supposed to be run using
    * OOM conditions, TInt value for first heap memory allocation failure and 
    * TInt value for last heap memory allocation failure.
    * 
    * OOM_FUNCENTRY macro takes test case function name as a parameter and uses
    * that as a test case name, TBool which specifies is method supposed to be
    * run using OOM conditions, TInt value for first heap memory allocation 
    * failure and TInt value for last heap memory allocation failure. 
    */ 

    static TCaseInfoInternal const KCases[] =
        {
        // [test cases entries] - do not remove
        
        // NOTE: When compiled to GCCE, there must be Classname::
        // declaration in front of the method name, e.g. 
        // Cxmppsettingsfetchermt::PrintTest. Otherwise the compiler
        // gives errors.
        
        ENTRY( "write and read valid settings", Cxmppsettingsfetchermt::SetAndReadSettingsL ),
        ENTRY( "read unexisting settings", Cxmppsettingsfetchermt::ReadUnexistingSettingsL ),
        ENTRY( "write and read incomplete settings", Cxmppsettingsfetchermt::ReadIncompleteSettingsL ),
        ENTRY( "descriptor values test", Cxmppsettingsfetchermt::ReadDescSettingsL ),
        ENTRY( "integer values test", Cxmppsettingsfetchermt::ReadIntSettingsL ),

//        FUNCENTRY( Cxmppsettingsfetchermt::PrintTest ),

        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", Cxmppsettingsfetchermt::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( Cxmppsettingsfetchermt::PrintTest, ETrue, 1, 3 ),
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        null.iIsOOMTest = EFalse;
        null.iFirstMemoryAllocation = 0;
        null.iLastMemoryAllocation = 0;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    tmp.iIsOOMTest = KCases[ aCaseNumber ].iIsOOMTest;
    tmp.iFirstMemoryAllocation = KCases[ aCaseNumber ].iFirstMemoryAllocation;
    tmp.iLastMemoryAllocation = KCases[ aCaseNumber ].iLastMemoryAllocation;
    return tmp;

    }

// -----------------------------------------------------------------------------
// Cxmppsettingsfetchermt::LoopTest
// Another printing to UI test.
// -----------------------------------------------------------------------------
//
TInt Cxmppsettingsfetchermt::SetAndReadSettingsL( TTestResult& aResult )
    {
    TUint32 settingId = 0;
    CXmppSettingsApi* api = CXmppSettingsApi::NewLC();

    TRAPD( error, api->CreateSettingsRecordL( settingId ) );
    User::LeaveIfError( error );
	
	WriteSettingsL( api, settingId, EPositiveTest );
    CleanupStack::PopAndDestroy( api );
	
    CXmppParameters* xmppParams = CXmppParameters::NewL();
    CleanupStack::PushL( xmppParams );
    CXmppSettingsFetcher::ReadSettingsL( settingId, *xmppParams );
    
    //verify that all settings read are correct
    TBool verify = VerifySettingsL( xmppParams, settingId, EPositiveTest );
    CleanupStack::PopAndDestroy( xmppParams );
    
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    if(verify)
    	aResult.SetResult( KErrNone, KTestcasePassed );
    else
    	aResult.SetResult( KErrGeneral, KVerificationFailed );

    // Case was executed
    return KErrNone;
    }


TInt Cxmppsettingsfetchermt::ReadUnexistingSettingsL( TTestResult& aResult )
    {
    CXmppParameters* xmppParams = CXmppParameters::NewL();
    CleanupStack::PushL( xmppParams );
    CXmppSettingsFetcher::ReadSettingsL( 9999, *xmppParams ); //assuming 9999 records have not been created :)
    
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    CleanupStack::PopAndDestroy( xmppParams );
	aResult.SetResult( KErrNone, KTestcasePassed );

    // Case was executed
    return KErrNone;
    }


TInt Cxmppsettingsfetchermt::ReadIncompleteSettingsL( TTestResult& aResult )
    {
    TUint32 settingId = 0;
    CXmppSettingsApi* api = CXmppSettingsApi::NewLC();

    TRAPD( error, api->CreateSettingsRecordL( settingId ) );
    User::LeaveIfError( error );
	
	WriteIncompleteSettingsL( api, settingId );
    CleanupStack::PopAndDestroy( api );
	
    CXmppParameters* xmppParams = CXmppParameters::NewL();
    CleanupStack::PushL( xmppParams );
    TRAP( error, CXmppSettingsFetcher::ReadSettingsL( settingId, *xmppParams ) );
    CleanupStack::PopAndDestroy( xmppParams );
    
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    if( error == KErrNotFound )
    	aResult.SetResult( KErrNone, KTestcasePassed );
    else
    	aResult.SetResult( KErrGeneral, KTestcaseFailed );

    // Case was executed
    return KErrNone;
    }


TInt Cxmppsettingsfetchermt::ReadDescSettingsL( TTestResult& aResult )
    {
    TUint32 settingId = 0;
    CXmppSettingsApi* api = CXmppSettingsApi::NewLC();

    TRAPD( error, api->CreateSettingsRecordL( settingId ) );
    User::LeaveIfError( error );
	
	WriteSettingsL( api, settingId, EDescriptorTest );
    CleanupStack::PopAndDestroy( api );
	
    CXmppParameters* xmppParams = CXmppParameters::NewL();
    CleanupStack::PushL( xmppParams );
    CXmppSettingsFetcher::ReadSettingsL( settingId, *xmppParams );
    
    //verify that all settings read are correct
    TBool verify = VerifySettingsL( xmppParams, settingId, EDescriptorTest );
    CleanupStack::PopAndDestroy( xmppParams );
    
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    if(verify)
    	aResult.SetResult( KErrNone, KTestcasePassed );
    else
    	aResult.SetResult( KErrGeneral, KVerificationFailed );

    // Case was executed
    return KErrNone;
    }


TInt Cxmppsettingsfetchermt::ReadIntSettingsL( TTestResult& aResult )
    {
    TUint32 settingId = 0;
    CXmppSettingsApi* api = CXmppSettingsApi::NewLC();

    TRAPD( error, api->CreateSettingsRecordL( settingId ) );
    User::LeaveIfError( error );
	
	WriteSettingsL( api, settingId, EIntegerTest );
    CleanupStack::PopAndDestroy( api );
	
    CXmppParameters* xmppParams = CXmppParameters::NewL();
    CleanupStack::PushL( xmppParams );
    CXmppSettingsFetcher::ReadSettingsL( settingId, *xmppParams );
    
    //verify that all settings read are correct
    TBool verify = VerifySettingsL( xmppParams, settingId, EIntegerTest );
    CleanupStack::PopAndDestroy( xmppParams );
    
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    if(verify)
    	aResult.SetResult( KErrNone, KTestcasePassed );
    else
    	aResult.SetResult( KErrGeneral, KVerificationFailed );

    // Case was executed
    return KErrNone;
    }


void Cxmppsettingsfetchermt::WriteSettingsL( CXmppSettingsApi* aApi, TUint32 aSettingId, TTestCase aTestCase )
{
	TInt error = KErrNone;
	
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamHttpsProxyServerPort(),
                                              httpsProxyPort ) );
	if( aTestCase == EIntegerTest )	//integer test
		{
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamServerPort(),
	                                              portNegative ) ); //negative value
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamSnapId(),
	                                              iapIdZero ) ); //zero
		}
	else
		{
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamServerPort(),
	                                              port ) );
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamSnapId(),
	                                              iapId ) );
		}	

    // buffers
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamXmppServerAddress(),
                                              KXmppServerAddress ) );

	if( aTestCase == EDescriptorTest )	//descriptor test
		{
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamUsername(),
	                                              KNullUsername ) ); //empty string
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamPassword(),
	                                              KNullDesC ) ); //null descriptor
		}
    else
	    {
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamUsername(),
	                                              KUsername ) );
	    TRAP( error, aApi->SetParamL( aSettingId,
	                                              XmppParams::KXmppParamPassword(),
	                                              KPassword ) );
	    }

	// gabble specific things.........
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamResource(),
                                              KResouceText ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamHttpsProxyServerAddress(),
                                              KHttpsProxyServerAddressText ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamAlais(),
                                              KAliasText ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamOldSSLRequired(),
                                              oldSSL ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamRegisterRequired(),
                                              resigter ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                  XmppParams::KXmppParamConnMgrBus(),
                                  KConnMgrBus ) );
                                  
    TRAP( error, aApi->SetParamL( aSettingId,
                                  XmppParams::KXmppParamConnMgrPath(),
                                  KConnMgrPath ));
    
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamProtocol(),
                                              KParamProtocol ));

    }


TBool Cxmppsettingsfetchermt::VerifySettingsL( CXmppParameters* aXmppParams, TUint32 aSettingsId, TTestCase aTestCase )
{
	switch (aTestCase)
		{
		case EPositiveTest:
			if ( 
				( aXmppParams->SettingsId() == aSettingsId ) &&
				( aXmppParams->ServerPort() == port ) &&
				( aXmppParams->ProxyPort() == httpsProxyPort ) &&
				( aXmppParams->IapId() == iapId ) &&
				( aXmppParams->ServerAddress() == KXmppServerAddress ) &&
				( aXmppParams->UserName() == KUsername ) &&
				( aXmppParams->Passwd() == KPassword ) &&
				( aXmppParams->Resource() == KResouceText ) &&
				( aXmppParams->ProxyServer() == KHttpsProxyServerAddressText ) &&
				( aXmppParams->Ssl() == oldSSL )
			   )
				return ETrue;
			else
				return EFalse;	
		case EDescriptorTest:
			if ( 
				( aXmppParams->SettingsId() == aSettingsId ) &&
				( aXmppParams->ServerPort() == port ) &&
				( aXmppParams->ProxyPort() == httpsProxyPort ) &&
				( aXmppParams->IapId() == iapId ) &&
				( aXmppParams->ServerAddress() == KXmppServerAddress ) &&
				( aXmppParams->UserName() == KNullUsername ) && //empty string
				( aXmppParams->Passwd() == KNullDesC ) && //null descriptor
				( aXmppParams->Resource() == KResouceText ) &&
				( aXmppParams->ProxyServer() == KHttpsProxyServerAddressText ) &&
				( aXmppParams->Ssl() == oldSSL )
			   )
				return ETrue;
			else
				return EFalse;	
		case EIntegerTest:
			if ( 
				( aXmppParams->SettingsId() == aSettingsId ) &&
				( aXmppParams->ServerPort() == portNegative ) && //negative
				( aXmppParams->ProxyPort() == httpsProxyPort ) && 
				( aXmppParams->IapId() == iapIdZero ) && //zero
				( aXmppParams->ServerAddress() == KXmppServerAddress ) &&
				( aXmppParams->UserName() == KUsername ) &&
				( aXmppParams->Passwd() == KPassword ) &&
				( aXmppParams->Resource() == KResouceText ) &&
				( aXmppParams->ProxyServer() == KHttpsProxyServerAddressText ) &&
				( aXmppParams->Ssl() == oldSSL )
			   )
				return ETrue;
			else
				return EFalse;	
		}
	return EFalse;
 }


void Cxmppsettingsfetchermt::WriteIncompleteSettingsL( CXmppSettingsApi* aApi, TUint32 aSettingId )
{
	TInt error = KErrNone;
	
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamServerPort(),
                                              port ) );

    // buffers
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamXmppServerAddress(),
                                              KXmppServerAddress ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamUsername(),
                                              KUsername ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamPassword(),
                                              KPassword ) );
	// gabble specific things.........
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamHttpsProxyServerAddress(),
                                              KHttpsProxyServerAddressText ) );
    TRAP( error, aApi->SetParamL( aSettingId,
                                              XmppParams::KXmppParamOldSSLRequired(),
                                              oldSSL ) );
    }

// -----------------------------------------------------------------------------
// ?classname::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
?type ?classname::?member_function(
   ?arg_type arg,
   ?arg_type arg )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ?function_name implements...
// ?implementation_description.
// Returns: ?value_1: ?description
//          ?value_n: ?description
//                    ?description
// -----------------------------------------------------------------------------
//
/*
?type  ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg )  // ?description
    {

    ?code

    }
*/
//  [End of File] - do not remove
