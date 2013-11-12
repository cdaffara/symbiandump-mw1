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
* Description: XIMP Framework Test Code 
*
*/

#include "t_rootservices.h"

#include "eunittools.h"
#include <eunitmacros.h>
#include <ecom/ecom.h>
#include <badesca.h>

//XIMP headers
#include <ximpbase.h>
#include <ximpclient.h>
#include <ximpprotocolinfo.h>
#include <ximpfeatureinfo.h>

#include "prfwtestrobustnesstools.h"
#include "prfwtestprotocols.h"



// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_RootServices* T_RootServices::NewL()
    {
    T_RootServices* self = new( ELeave ) T_RootServices;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_RootServices::~T_RootServices()
    {
    }


void T_RootServices::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_RootServices::T_RootServices()
    {
    }



// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//
void T_RootServices::Setup_L()
    {
    PrfwTestRobustness::DoPreCleaning();
    }



void T_RootServices::Teardown()
    {
    REComSession::FinalClose();
    PrfwTestRobustness::DoPreCleaning();
    }



// ===========================================================================
// USAGE SCENARIO:
//
// ===========================================================================
//
void T_RootServices::Test_ListProtocolsAndTheirCapabilities_L()
    {
    //Initialize XIMP client
    MXIMPClient* presClient = MXIMPClient::NewClientL();
    CleanupDeletePushL( presClient );

    RXIMPObjOwningPtrArray< MXIMPProtocolInfo > protocolList;
    CleanupClosePushL( protocolList );

    //List & dump all available protocols
    presClient->GetProtocolsL( protocolList );
    __DumpProtocolListL( protocolList );

    //Verify that atleast main test protocol exist in list
    //And for it is reported corect properties
    TBool mainTestProtocolFound( EFalse );
    for ( TInt index( 0 ); index < protocolList.Count(); index++ )
        {
        MXIMPProtocolInfo& protocol = *protocolList[ index ];
        if (protocol.DisplayName() == K_PRFW_TST_PROTOCOL_1_DISPLAY_NAME )
            {
            if ( protocol.ImplementationUid() == TUid::Uid( K_PRFW_TST_PROTOCOL_1_IMPLEMENTATION_UID ) ) 
                {
                mainTestProtocolFound = ETrue;
                break;
                }
            }
        }

    EUNIT_ASSERT_DESC( mainTestProtocolFound, "Primary test protocol not found." );

    CleanupStack::PopAndDestroy(); //protocolList
    CleanupStack::PopAndDestroy(); //presClient
    }




// ===========================================================================
// HELPERS
// ===========================================================================
//
void T_RootServices::__DumpProtocolListL(
    RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocolList )
    {
    const TInt count = aProtocolList.Count();
    EUNIT_PRINT( _L( "Dumping %d protocols:"), count );

    //Iterate through the list of protocols and dump details
    for( TInt ix = 0; ix < count; ix++ )
        {
        MXIMPProtocolInfo& protocol = *aProtocolList[ ix ];
        MXIMPFeatureInfo* feats = protocol.GetProtocolFeaturesLC();

            {
            TBuf< 1000 > buffer;

            //Print protocol details
            buffer.AppendFormat( _L("  Protocol %d: [%08x] [%S]"),
                                 ix,
                                 protocol.ImplementationUid(),
                                 &protocol.DisplayName() );
            EUNIT_PRINT( buffer );


            //Print feature details
            const MDesC8Array& featureIds = feats->FeatureIds();

            const TInt featCount = featureIds.MdcaCount();
            buffer.Zero();
            buffer.AppendFormat( _L("  Supported features %d: "), featCount );

            for( TInt ix = 0; ix < featCount; ix++ )
                {
                TBuf< 50 > tmpBuf;
                tmpBuf.Copy( featureIds.MdcaPoint( ix ) );
                buffer.AppendFormat( _L("%S "), &tmpBuf );
                }

            EUNIT_PRINT( buffer );
            }

        CleanupStack::PopAndDestroy(); //feats
        }
    }




// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
    T_RootServices,
    "XIMP root services",
    "MODULE" )

    PRFW_NOT_DECORATED_TEST(
        "List available protocols and their capabilities",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        Test_ListProtocolsAndTheirCapabilities_L,
        Teardown )


EUNIT_END_TEST_TABLE





// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_RootServices::NewL();
    }


// end of file






