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
* Description:
*
*/


#include <eunitmacros.h>
#include <ecom/ecom.h>
#include <badesca.h>
#include <e32property.h>
#include <e32base.h>
#include <e32std.h>

#include "t_utils.h"

#include "eunittools.h"

#include <ximpbase.h>
#include <presenceinfo.h>

#include "ximpeventcodec.h"
#include "ownpresenceeventimp.h"
#include "prfwtestfiletool.h"
#include "ximppanics.h"

#include "prfwtestrobustnesstools.h"

#include "ximpobjectfactoryimp.h"
#include "ximpobjectcollection.h"
#include "ximprestrictedobjectcollection.h"
#include "ximpidentityimp.h"
#include "ximpstatusimp.h"
#include "ximprequestidbuilder.h"



// ============================ MEMBER FUNCTIONS ===============================

// CONSTRUCTION
T_Utils* T_Utils::NewL()
    {
    T_Utils* self = new( ELeave ) T_Utils;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


T_Utils::~T_Utils()
    {
    }


void T_Utils::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


T_Utils::T_Utils()
    {
    }

// ===========================================================================
// TEST CASE SETUP & TEARDOWN METHODS
// ===========================================================================
//

void T_Utils::Setup_L()
    {
    }

void T_Utils::Teardown()
    {
    }

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_Utils::T_FileToolTest_L()
    {
    // TODO add more brains to the test
    EUNIT_PRINT( _L("FileTool test.") );

    /*
    CXIMPTestFileTool* tool = CXIMPTestFileTool::NewL();
    CleanupStack::PushL( tool );
    tool->CleanL( 0 );
    tool->PrepareL( 0 );
    TInt files = tool->NumObjectsL( 0 );

    EUNIT_ASSERT_DESC( files == 0, "Non-zero count of files, Clean or Prepare did not work" );

    // make a proper object and store it.
    MPresenceInfo::TPresenceInfoStatus status;
    status = MPresenceInfo::EDataSubscriptionPending;
    COwnPresenceEventImp* event = COwnPresenceEventImp::NewLC( status );

    CBufFlat* buf = CBufFlat::NewL( 50 );
    CleanupStack::PushL( buf );
    TInt32 eventType = 0;
    XIMPEventCodec::PackL( *event, eventType, buf );
    HBufC8* extObj = buf->Ptr( 0 ).AllocLC();

    // create more objects
    tool->StoreL( 0, *extObj );
    tool->StoreL( 0, *extObj );
    tool->StoreL( 0, *extObj );
    tool->StoreL( 0, *extObj );
    tool->StoreL( 0, *extObj );

    files = tool->NumObjectsL( 0 );
    EUNIT_ASSERT_DESC( files == 5, "Bad count of files" );

    COwnPresenceEventImp* object = static_cast<COwnPresenceEventImp*>( tool->RestoreLC( 0, 2 ) );
    EUNIT_ASSERT_DESC( status == object->Status(), "Wrong status from restored object." );

    CleanupStack::PopAndDestroy( 4, event ); // object, extObj, buf, event

    CleanupStack::PopAndDestroy( tool );
    */
    }

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_Utils::T_KillServers_L()
    {
    EUNIT_PRINT( _L("Utility to kill the servers. Always succeeds.") );

    PrfwTestRobustness::DoPreCleaning();
    }

_LIT8( KTestObjectName,    "John Doe" );     // regular
_LIT8( KTestObjectName2,   "Jane Doe" );     // regular
_LIT8( KTestObjectNameWC,  "Jo* Doe" );      // wildcards
_LIT8( KTestObjectNameWC2, "Ja* Doe" );      // wildcards

_LIT( KIdentity1, "Identity 1" );
_LIT( KIdentity2, "Identity 2" );

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_Utils::T_ObjectCollection_Unnamed_L()
    {
    EUNIT_PRINT( _L("Object collection unnamed tests.") );

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPObjectCollection* collection = factory->NewObjectCollectionLC();

    // LookupByType should not find anything
    const MXIMPBase* obj = NULL;
    collection->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    // LookupByTypeAndName should not find anything
    collection->LookupByTypeAndName( obj, MXIMPIdentity::KInterfaceId, KTestObjectName );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName found an object" );

    collection->LookupByTypeAndName( obj, MXIMPIdentity::KInterfaceId, KTestObjectNameWC );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName with wildcard found an object" );

    // add some
    MXIMPIdentity* identity2 = factory->NewIdentityLC();
    identity2->SetIdentityL( KIdentity2 );

    MXIMPStatus* status = factory->NewStatusLC();
    status->SetResultCode( KErrArgument );

    MXIMPIdentity* identity1 = factory->NewIdentityLC();
    identity1->SetIdentityL( KIdentity1 );

    collection->AddObjectL( identity1 );
    CleanupStack::Pop(); // identity1

    collection->AddObjectL( status );
    CleanupStack::Pop(); // status

    collection->AddObjectL( identity2 );
    CleanupStack::Pop(); // identity2

    // get and verify the status
    MXIMPBase* getObj = NULL;
    collection->GetByType( getObj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    MXIMPStatus* gotStatus = ( MXIMPStatus* ) getObj->GetInterface( MXIMPStatus::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotStatus );
    EUNIT_ASSERT_DESC( gotStatus->ResultCode() == KErrArgument, "Status got corrupted" );
    CleanupStack::PopAndDestroy(); // gotStatus

    // check that status can't be found anymore
    obj = NULL;
    collection->LookupByType( obj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    collection->LookupByTypeAndName( obj, MXIMPStatus::KInterfaceId, KTestObjectName );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName found an object" );

    collection->LookupByTypeAndName( obj, MXIMPStatus::KInterfaceId, KTestObjectNameWC );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName with wildcard found an object" );

    // get and verify the two identities
    // first identity
    getObj = NULL;
    collection->GetByType( getObj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    MXIMPIdentity* gotIdentity = ( MXIMPIdentity* ) getObj->GetInterface( MXIMPIdentity::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotIdentity );
    EUNIT_ASSERT_DESC( 0 == gotIdentity->Identity().Compare( KIdentity1 ), "Wrong identity object found" );
    CleanupStack::PopAndDestroy(); // gotIdentity

    // second identity
    getObj = NULL;
    collection->GetByType( getObj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    gotIdentity = ( MXIMPIdentity* ) getObj->GetInterface( MXIMPIdentity::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotIdentity );
    EUNIT_ASSERT_DESC( 0 == gotIdentity->Identity().Compare( KIdentity2 ), "Wrong identity object found" );
    CleanupStack::PopAndDestroy(); // gotIdentity

    // now there should not be anything anymore
    obj = NULL;
    collection->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    CleanupStack::PopAndDestroy(); // collection
    CleanupStack::PopAndDestroy( factory );
    }

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_Utils::T_ObjectCollection_Named_L()
    {
    EUNIT_PRINT( _L("Object collection named tests.") );

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPObjectCollection* collection = factory->NewObjectCollectionLC();

    // LookupByType should not find anything
    const MXIMPBase* obj = NULL;
    collection->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    // LookupByTypeAndName should not find anything
    collection->LookupByTypeAndName( obj, MXIMPIdentity::KInterfaceId, KTestObjectName );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName found an object" );

    collection->LookupByTypeAndName( obj, MXIMPIdentity::KInterfaceId, KTestObjectNameWC );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByTypeAndName with wildcard found an object" );

    // add some
    MXIMPIdentity* identity2 = factory->NewIdentityLC();
    identity2->SetIdentityL( KIdentity2 );
    
    MXIMPStatus* status = factory->NewStatusLC();
    status->SetResultCode( KErrArgument );

    MXIMPIdentity* identity1 = factory->NewIdentityLC();
    identity1->SetIdentityL( KIdentity1 );
    
    collection->AddObjectWithNameL( identity1, KTestObjectName );
    CleanupStack::Pop(); // identity1

    TRAPD( err, collection->AddObjectWithNameL( status, KTestObjectNameWC ) );
    EUNIT_ASSERT_DESC( err == KErrBadName, "Did not leave when given wildcards" );
    CleanupStack::PopAndDestroy(); // status

    collection->AddObjectWithNameL( identity2, KTestObjectName2 );
    CleanupStack::Pop(); // identity2

    // should not find the status object
    MXIMPBase* getObj = NULL;
    collection->GetByType( getObj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj == NULL, "LookupByType found wrong object" );

    // get and verify the two identities
    // first identity
    getObj = NULL;
    collection->GetByTypeAndName( getObj, MXIMPIdentity::KInterfaceId, KTestObjectName );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    MXIMPIdentity* gotIdentity = ( MXIMPIdentity* ) getObj->GetInterface( MXIMPIdentity::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotIdentity );
    EUNIT_ASSERT_DESC( 0 == gotIdentity->Identity().Compare( KIdentity1 ), "Wrong identity object found" );
    CleanupStack::PopAndDestroy(); // gotIdentity

    // second identity
    getObj = NULL;
    collection->GetByTypeAndName( getObj, MXIMPIdentity::KInterfaceId, KTestObjectNameWC2 );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    gotIdentity = ( MXIMPIdentity* ) getObj->GetInterface( MXIMPIdentity::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotIdentity );
    EUNIT_ASSERT_DESC( 0 == gotIdentity->Identity().Compare( KIdentity2 ), "Wrong identity object found" );
    CleanupStack::PopAndDestroy(); // gotIdentity

    // now there should not be anything anymore
    obj = NULL;
    collection->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    CleanupStack::PopAndDestroy(); // collection
    CleanupStack::PopAndDestroy( factory );
    }

// ===========================================================================
// TEST CASE
// ===========================================================================
void T_Utils::T_RestrictedObjectCollection_L()
    {
    EUNIT_PRINT( _L("Restricted object collection tests.") );

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPRestrictedObjectCollection* resColl = factory->NewRestrictedObjectCollectionLC();

    // LookupByType should not find anything, empty collection
    const MXIMPBase* obj = NULL;
    resColl->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an object" );

    // add some
    MXIMPIdentity* identity2 = factory->NewIdentityLC();
    identity2->SetIdentityL( KIdentity2 );
    
    MXIMPStatus* status = factory->NewStatusLC();
    status->SetResultCode( KErrArgument );

    MXIMPIdentity* identity1 = factory->NewIdentityLC();
    identity1->SetIdentityL( KIdentity1 );
    
    resColl->AddOrReplaceTypeL( identity1 );
    CleanupStack::Pop(); // identity1

    resColl->AddOrReplaceTypeL( status );
    CleanupStack::Pop(); // identity1

    resColl->AddOrReplaceTypeL( identity2 );
    CleanupStack::Pop(); // identity2

    // there should only be {identity2, status} in the collection
    obj = NULL;
    resColl->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj != NULL, "LookupByType did not find an object" );

    obj = NULL;
    resColl->LookupByType( obj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( obj != NULL, "LookupByType did not find an object" );

    // get the objects and verify their contents
    
    // 1. identity
    MXIMPBase* getObj = NULL;
    resColl->GetByType( getObj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    MXIMPIdentity* gotIdentity = ( MXIMPIdentity* ) getObj->GetInterface( MXIMPIdentity::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotIdentity );
    EUNIT_ASSERT_DESC( 0 == gotIdentity->Identity().Compare( KIdentity2 ), "Second identity object did not replace the first one" );
    CleanupStack::PopAndDestroy(); // gotIdentity

    // 2. status
    getObj = NULL;
    resColl->GetByType( getObj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( getObj != NULL, "LookupByType did not find an object" );

    MXIMPStatus* gotStatus = ( MXIMPStatus* ) getObj->GetInterface( MXIMPStatus::KInterfaceId, MXIMPBase::EPanicIfUnknown );
    CleanupDeletePushL( gotStatus );
    EUNIT_ASSERT_DESC( KErrArgument == gotStatus->ResultCode(), "Status object was corrupted" );
    CleanupStack::PopAndDestroy(); // gotStatus

    // now there should not be anything anymore
    obj = NULL;
    resColl->LookupByType( obj, MXIMPIdentity::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found an identity object although it was removed" );
    
    obj = NULL;
    resColl->LookupByType( obj, MXIMPStatus::KInterfaceId );
    EUNIT_ASSERT_DESC( obj == NULL, "LookupByType found a status object although it was removed" );

    CleanupStack::PopAndDestroy(); // collection
    CleanupStack::PopAndDestroy( factory );
    }

void T_Utils::T_DataSubscriptionState_L()
    {
    EUNIT_PRINT( _L("Data subscription state tests.") );

    CXIMPObjectFactoryImp* factory = CXIMPObjectFactoryImp::NewL();
    CleanupStack::PushL( factory );

    MXIMPDataSubscriptionState* dataState = factory->NewDataSubscriptionStateLC();

    EUNIT_ASSERT_DESC( dataState->DataState() == MXIMPDataSubscriptionState::EDataUnavailable, 
                      "Data was available." );
    EUNIT_ASSERT_DESC( dataState->SubscriptionState() == MXIMPDataSubscriptionState::ESubscriptionInactive, 
                      "Subscription was active." );
    
    dataState->SetDataStateL( MXIMPDataSubscriptionState::EDataAvailable );
    dataState->SetSubscriptionStateL( MXIMPDataSubscriptionState::ESubscriptionActive );

    EUNIT_ASSERT_DESC( dataState->DataState() == MXIMPDataSubscriptionState::EDataAvailable, 
                      "Data was unavailable." );
    EUNIT_ASSERT_DESC( dataState->SubscriptionState() == MXIMPDataSubscriptionState::ESubscriptionActive, 
                      "Subscription was inactive." );

    CleanupStack::PopAndDestroy(); // collection
    CleanupStack::PopAndDestroy( factory );
    }



void T_Utils::T_TXIMPRequestIdBuilder_Test_L()
    {
    EUNIT_PRINT( _L("TXIMPRequestIdbuilder test.") );
    
    TXIMPRequestIdBuilder builder;
    builder.SetProtocolId( 1 );
    builder.SetSessionId( 1 );
    builder.SetRequestId( 1 );    

    TXIMPRequestId id1 = builder.BuildRequestId();
    EUNIT_ASSERT( id1 == id1 );
    EUNIT_ASSERT( id1 != TXIMPRequestId::Null() );


        {
        builder.SetProtocolId( 2 );   
        TXIMPRequestId id2 = builder.BuildRequestId();
        EUNIT_ASSERT( id2 == id2 );
        EUNIT_ASSERT( id1 != id2 );
        EUNIT_ASSERT( id2 != id1 );
        EUNIT_ASSERT( id2 != TXIMPRequestId::Null() );        
        }

        {
        builder.SetSessionId( 2 );   
        TXIMPRequestId id3 = builder.BuildRequestId();
        EUNIT_ASSERT( id3 == id3 );
        EUNIT_ASSERT( id1 != id3 );
        EUNIT_ASSERT( id3 != id1 );
        EUNIT_ASSERT( id3 != TXIMPRequestId::Null() );        
        }


        {
        builder.SetRequestId( 2 );   
        TXIMPRequestId id4 = builder.BuildRequestId();
        EUNIT_ASSERT( id4 == id4 );
        EUNIT_ASSERT( id1 != id4 );
        EUNIT_ASSERT( id4 != id1 );
        EUNIT_ASSERT( id4 != TXIMPRequestId::Null() );                
        }
        
        
        {
        builder.SetProtocolId( 33333333 );
        builder.SetSessionId( 44444444 );
        builder.SetRequestId( 55555555 );    

        TXIMPRequestId id5 = builder.BuildRequestId();
        EUNIT_ASSERT( id5 == id5 );
        EUNIT_ASSERT( id1 != id5 );
        EUNIT_ASSERT( id5 != id1 );
        EUNIT_ASSERT( id5 != TXIMPRequestId::Null() );                
        }
    }


void T_Utils::T_TXIMPRequestId_Test_L()
    {
    EUNIT_PRINT( _L("TXIMPRequestIdbuilder test.") );
    
    TXIMPRequestId id0 = TXIMPRequestId::Null();
    EUNIT_ASSERT( id0 == TXIMPRequestId::Null() );  
    
        {
        TXIMPRequestId id1 = TXIMPRequestId::Init( 1 ); 
        EUNIT_ASSERT( id1 != TXIMPRequestId::Null() ); 
        }


        {
        TXIMPRequestId id2 = TXIMPRequestId::Init( 2);     
        EUNIT_ASSERT( id0 != id2 );             
        }

    
        {
        TXIMPRequestId id3 = TXIMPRequestId::Init( KMaxTInt );     
        EUNIT_ASSERT( id0 != id3 );             
        }


        {
        TXIMPRequestId id4 = TXIMPRequestId::Init( 33333333 );     
        EUNIT_ASSERT( id0 != id4 );             
        }
        
        {
        TXIMPRequestId id5 = TXIMPRequestId::Init( KMaxTInt );     
        TXIMPRequestId id6 = TXIMPRequestId::Init( KMinTInt );     
        EUNIT_ASSERT( id0 != id5 );             
        EUNIT_ASSERT( id0 != id6 );             
        EUNIT_ASSERT( id5 != id6 );             
        }
        
    }




// ===========================================================================
// TEST CASE TABLE
// ===========================================================================
//
EUNIT_BEGIN_TEST_TABLE(
        T_Utils,
        "XIMP test utils tests",
        "MODULE" )

 PRFW_NOT_DECORATED_TEST(
        "Kill servers",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_KillServers_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "Object collection without names",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ObjectCollection_Unnamed_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "Object collection using names",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_ObjectCollection_Named_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "Restricted object collection",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_RestrictedObjectCollection_L,
        Teardown )

PRFW_NOT_DECORATED_TEST(
        "Data subscription state",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_DataSubscriptionState_L,
        Teardown )


PRFW_NOT_DECORATED_TEST(
        "TXIMPRequestIdBuilder test",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_TXIMPRequestIdBuilder_Test_L,
        Teardown )


PRFW_NOT_DECORATED_TEST(
        "TXIMPRequestId test",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_TXIMPRequestId_Test_L,
        Teardown )


/* PRFW_NOT_DECORATED_TEST(
        "FileTool test",
        "",
        "",
        "FUNCTIONALITY",
        Setup_L,
        T_FileToolTest_L,
        Teardown ) */


EUNIT_END_TEST_TABLE


// ===========================================================================
// TEST SUITE CONSTRUCTION
// ===========================================================================
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    return T_Utils::NewL();
    }


// end of file






