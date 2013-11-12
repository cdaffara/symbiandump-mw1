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


//  CLASS HEADER
#include "T_PresenceCache.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>


#include <presenceinfoimp.h>
#include <presenceinfo.h>
#include <personpresenceinfoimp.h>
#include <presenceinfofieldimp.h>
#include <presenceinfofield.h>
#include <presenceinfofieldcollectionimp.h>
#include <presenceinfofieldcollection.h>
#include <ximpbase.h>
#include <presenceinfofieldvaluetext.h>
#include <presenceinfofieldvaluetextimp.h>
#include <presencebuddyinfo.h>
#include <presencebuddyinfolist.h>
#include <presencecacheclient.h>
#include <ximpidentityimp.h>
#include <ximpidentity.h>
#include <presenceinfofieldvaluebinary.h>
#include <presenceinfofieldvaluebinaryimp.h>

#include <presencefeeder.h>


// Implementation files used for testing only
#include "presencebuddyinfoimp.h"
#include "presenceinfoimp.h"
#include "presencebuddyinfolistimp.h"


// CONSTANTS
_LIT8( KIdentityField, "Identity" );

_LIT( KBuddyId1sip, "sip:BuddyId1@xyz.com" );
_LIT( KBuddyId2sip, "sip:BuddyId2@xyz.com" );
_LIT( KBuddyId3sip, "sip:BuddyId3@xyz.com" );
_LIT( KBuddyId1ECE, "ECE:BuddyId1@xyz.com" );
_LIT( KBuddyId2ECE, "ECE:BuddyId2@xyz.com" );
_LIT( KBuddyId3ECE, "ECE:BuddyId3@xyz.com" );
_LIT( KBuddyId1MSN, "MSN:BuddyId1@xyz.com" );
_LIT( KBuddyId2MSN, "MSN:BuddyId2@xyz.com" );
_LIT( KBuddyId3MSN, "MSN:BuddyId3@xyz.com" );
_LIT( KBuddyId1VoIP, "VoIP:BuddyId1@xyz.com" );
_LIT( KBuddyId2VoIP, "VoIP:BuddyId2@xyz.com" );
_LIT( KBuddyId3VoIP, "VoIP:BuddyId3@xyz.com" );
_LIT( KBuddyId1ICQ, "ICQ:BuddyId1@xyz.com" );
_LIT( KBuddyId2ICQ, "ICQ:BuddyId2@xyz.com" );
_LIT( KBuddyId3ICQ, "ICQ:BuddyId3@xyz.com" );
_LIT( KBuddyId1no, "no:BuddyId1@xyz.com" );
_LIT( KBuddyId2no, "no:BuddyId2@xyz.com" );
_LIT( KBuddyId3no, "no:BuddyId3@xyz.com" );

_LIT( KBuddyId1, "BuddyId1@xyz.com" );
_LIT( KBuddyId2, "BuddyId2@xyz.com" );
_LIT( KBuddyId3, "BuddyId3@xyz.com" );

_LIT( KText1, "KText1" );
_LIT( KText2, "KText2" );
_LIT( KText3, "KText3" );
_LIT( KText4, "KText4" );
_LIT( KText5, "KText5" );
_LIT( KText6, "KText6" );

_LIT( KService1, "sip" );
_LIT( KService2, "ECE" );
_LIT( KService3, "MSN" );
_LIT( KService4, "VoIP" );
_LIT( KService5, "ICQ" );
_LIT( KService6, "no" );

//  INTERNAL INCLUDES


// CONSTRUCTION
T_PresenceCache* T_PresenceCache::NewL()
    {
    T_PresenceCache* self = T_PresenceCache::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_PresenceCache* T_PresenceCache::NewLC()
    {
    T_PresenceCache* self = new( ELeave ) T_PresenceCache();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_PresenceCache::~T_PresenceCache()
    {
    }

// Default constructor
T_PresenceCache::T_PresenceCache()
    {
    }

// Second phase construct
void T_PresenceCache::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    iCoverageTesting=EFalse;
    }

//  METHODS


void T_PresenceCache::SetupL(  )
    {
    __UHEAP_MARK;
    iMPresenceCacheWriter = MPresenceCacheWriter::NewL();
    iMPresenceCacheReader = MPresenceCacheReader::NewL();
    
    iPresenceFeeder = MPresenceFeeder::NewL(KService6);
    }
    

void T_PresenceCache::Teardown(  )
    {
    if(iMPresenceCacheReader)
        delete iMPresenceCacheReader;
    delete iMPresenceCacheWriter;
    delete iPresenceFeeder;
    delete iExpectedNotifyContent;
    
    if(iPresBuddyInfoList)
        delete iPresBuddyInfoList;
    REComSession::FinalClose();
    User::SetJustInTime( ETrue );
    __UHEAP_MARKEND;
    }
    
void T_PresenceCache::T__PresenceInfoL()
    {
    // Try to read while empty
    MXIMPIdentity* identity = iMPresenceCacheReader->NewIdentityLC();
    identity->SetIdentityL(KBuddyId1);
    
    //KErrArgument
    MPresenceInfo* presInfoRead = iMPresenceCacheReader->PresenceInfoLC(*identity);
    EUNIT_ASSERT_EQUALS( (TInt)presInfoRead, NULL );
    
    //empty read
    identity->SetIdentityL(KBuddyId1sip);
    presInfoRead = iMPresenceCacheReader->PresenceInfoLC(*identity);
    EUNIT_ASSERT_EQUALS( (TInt)presInfoRead, NULL );
    
    // write buddy presence info
    MPresenceBuddyInfo* buddyPresInfoWrite = 
                        MakeBuddyPresenceInfoLC(KBuddyId1sip, KText1);
    TInt err = iMPresenceCacheWriter->WritePresenceL(buddyPresInfoWrite);
                                                
    // Now read it
    presInfoRead = iMPresenceCacheReader->PresenceInfoLC(*identity);
    
    
    //Now verify it
    const CPresenceInfoImp* actualInfo = 
        TXIMPGetImpClassOrPanic<const CPresenceInfoImp>::From(*(buddyPresInfoWrite->PresenceInfo()));
        
    CPresenceInfoImp* readInfo = 
        TXIMPGetImpClassOrPanic<CPresenceInfoImp>::From(*presInfoRead);    
    
    TBool same = actualInfo->EqualsContent(*readInfo);

    CleanupStack::PopAndDestroy(3); //identity, buddyPresInfoWrite, presInfoRead;   
                                                
    EUNIT_ASSERT_EQUALS( same, ETrue );
    }
    
   
void T_PresenceCache::T_WriteReadPresenceAsyncL()
    {
  
    //MPresenceBuddyInfoList* buddyInfoList = CPresenceBuddyInfoListImp::NewLC(); 
    MPresenceBuddyInfoList* buddyInfoList =  
                            iMPresenceCacheWriter->NewPresenceBuddyInfoListLC(KNullDesC);
    //buddyInfoList->SetServiceNameL(KService1);
    // write buddies in all services
    MPresenceBuddyInfo* buddyPresInfoWrite(NULL);
    
    TInt err2(KErrNone);
    
    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId1sip, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);
    
    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId2sip, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId3sip, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId1ECE, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);
    
    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId2ECE, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId3ECE, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId1MSN, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);
    
    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId2MSN, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId3MSN, KText1);
    buddyInfoList->AddOrReplace(buddyPresInfoWrite);CleanupStack::Pop(1);

        
    //Only For increasing coverage for lists
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId3no);
    EUNIT_ASSERT_EQUALS( buddyInfoList->FindAndRemove(*identity), KErrNotFound);
    EUNIT_ASSERT_EQUALS( buddyInfoList->Count(), 9);
    buddyPresInfoWrite =MakeBuddyPresenceInfoLC(KBuddyId3no, KText1);
    EUNIT_ASSERT_EQUALS( buddyInfoList->AddOrReplace(buddyPresInfoWrite), KErrNone);
    
    EUNIT_ASSERT_EQUALS( buddyInfoList->Count(), 10);
    EUNIT_ASSERT_EQUALS( buddyInfoList->FindAndRemove(*identity), KErrNone);
    EUNIT_ASSERT_EQUALS( buddyInfoList->Count(), 9);
    EUNIT_ASSERT_EQUALS( buddyInfoList->FindAndRemove(*identity), KErrNotFound);
    CleanupStack::Pop(1); // buddyPresInfoWrite
    EUNIT_ASSERT_EQUALS( (TInt)(buddyInfoList->FindAndGet(*identity)), NULL);
    CleanupStack::PopAndDestroy(1); // identity
    
    //calling async write function
    TInt err = iMPresenceCacheWriter->WritePresenceL(buddyInfoList,this);
    //if (err==KErrNone)
    //    WaitComplete();
   //Try Cancelling
    err = iMPresenceCacheWriter->CancelWrite(); //always cancels receiving callback
    //Write Again
    err = iMPresenceCacheWriter->WritePresenceL(buddyInfoList,this);
    if (err==KErrNone)
        WaitComplete();
    else //Try again
        {
        err = iMPresenceCacheWriter->WritePresenceL(buddyInfoList,this);
        if (err==KErrNone)
            WaitComplete();
        }
    EUNIT_ASSERT_EQUALS( iErrorFromHandler, KErrNone);
    
    
    // read from 3rd service MSN
    err = iMPresenceCacheReader->AllBuddiesPresenceInService(KService3, this);
    //Try Cancelling
    err = iMPresenceCacheReader->CancelRead();
    // Try reading again
    err = iMPresenceCacheReader->AllBuddiesPresenceInService(KService3, this);
    if (err+1)
        WaitComplete();
    EUNIT_ASSERT_EQUALS( iErrorFromHandler, KErrNone);
    
        
    //Now verify it, pick something to verify
    CPresenceBuddyInfoImp* actualInfo(NULL);
    CPresenceBuddyInfoImp* readInfo(NULL);
    RPointerArray<MPresenceBuddyInfo> myArraywrite = buddyInfoList->GetObjectCollection();
    
    RPointerArray<MPresenceBuddyInfo> myArrayread; // wont be owned
    
    TBool same1(EFalse);
    if(iPresBuddyInfoList) // if list is received, compare for 3rd service MSN
    	{
    	myArrayread = iPresBuddyInfoList->GetObjectCollection(); //not owned
        //1
        actualInfo = 
            TXIMPGetImpClassOrPanic<CPresenceBuddyInfoImp>::From(*(myArraywrite[6]));
            
        readInfo = 
            TXIMPGetImpClassOrPanic<CPresenceBuddyInfoImp>::From(*(myArrayread[0]));   
        
        same1 = actualInfo->EqualsContent(*readInfo);
    	}
    
    
    // read for first service sip	
    err = iMPresenceCacheReader->AllBuddiesPresenceInService(KService1, this);
    if (err+1)
        WaitComplete();
    EUNIT_ASSERT_EQUALS( iErrorFromHandler, KErrNone);
    TBool same2(EFalse);
    if(iPresBuddyInfoList) // if list is received, compare for 1st service SIP
        {
        myArrayread = iPresBuddyInfoList->GetObjectCollection(); //not owned 
        
        actualInfo = 
            TXIMPGetImpClassOrPanic<CPresenceBuddyInfoImp>::From(*(myArraywrite[2]));
            
        readInfo = 
            TXIMPGetImpClassOrPanic<CPresenceBuddyInfoImp>::From(*(myArrayread[2]));   
        
        same2 = actualInfo->EqualsContent(*readInfo);  	
     	}
    
    
    // All buddies count
    TInt count = iMPresenceCacheReader->BuddyCountInAllServices();
    
    // buddies count in specific service
    TInt count2 = iMPresenceCacheReader->BuddyCountInService(KService2);
    
    
    // Testing DeletePresenceL    
    MXIMPIdentity* identity2 = CXIMPIdentityImp::NewLC();
    identity2->SetIdentityL(KBuddyId3MSN);
    TInt remove = iMPresenceCacheWriter->DeletePresenceL(*identity2);
    EUNIT_ASSERT_EQUALS( remove, KErrNone );
    remove = iMPresenceCacheWriter->DeletePresenceL(*identity2);
    EUNIT_ASSERT_EQUALS( remove, KErrNotFound );
    identity2->SetIdentityL(KBuddyId3ICQ);
    remove = iMPresenceCacheWriter->DeletePresenceL(*identity2);
    EUNIT_ASSERT_EQUALS( remove, KErrNotFound );
    CleanupStack::PopAndDestroy(1); //identity2;   

   
   
    TInt serviceCount = iMPresenceCacheReader->ServicesCount();
    
    CleanupStack::PopAndDestroy(1); //buddyInfoList;   
                                                
    EUNIT_ASSERT_EQUALS( same1, ETrue );
    EUNIT_ASSERT_EQUALS( same2, ETrue );
    EUNIT_ASSERT_EQUALS( count, 9 );
    EUNIT_ASSERT_EQUALS( count2, 3 );
    EUNIT_ASSERT_EQUALS( serviceCount, 3 );
    
    }
    
    
void T_PresenceCache::T_DeletingAndCancellingL()
    {
    //Empty Reading
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId1);
  
    
    CleanupStack::PopAndDestroy(1); //identity
    
    //Empty counting functions
    
    // All buddies count
    TInt count = iMPresenceCacheReader->BuddyCountInAllServices();
    EUNIT_ASSERT_EQUALS( count, 0 );
    
    // buddies count in specific service
    TInt count2 = iMPresenceCacheReader->BuddyCountInService(KService2);
    EUNIT_ASSERT_EQUALS( count2, KErrNotFound );

   
    //Services count
    TInt serviceCount = iMPresenceCacheReader->ServicesCount();
    EUNIT_ASSERT_EQUALS( serviceCount, 0 );
    
    
    //Empty async reading
    TInt err = iMPresenceCacheReader->AllBuddiesPresenceInService(KService3, this);
    if (err+1)
        WaitComplete();
    EUNIT_ASSERT_EQUALS( iErrorFromHandler, KErrNotFound);
    
    //Writing async and verify results// this checks cancelling also
    T_WriteReadPresenceAsyncL();
    
    // Now try deleting
    err = iMPresenceCacheWriter->DeleteService(KService3);
    EUNIT_ASSERT_EQUALS( err, KErrNone);
    
    //Try deleting again
    err = iMPresenceCacheWriter->DeleteService(KService3);
    EUNIT_ASSERT_EQUALS( err, KErrNotFound);
    
    //Try reading the deleted presence
    err = iMPresenceCacheReader->AllBuddiesPresenceInService(KService3, this);
    if (err==KErrNone)
        WaitComplete();
    EUNIT_ASSERT_EQUALS( iErrorFromHandler, KErrNotFound);
    }
    
void T_PresenceCache::T_PerformanceL()
    {
    
    
    
    
    }
    
void T_PresenceCache::T_RunAllL()
    {
    T__PresenceInfoL();    
    T_DeleteAllCacheL();
    
    T_WriteReadPresenceAsyncL();
    T_DeleteAllCacheL();
    
    T_DeletingAndCancellingL();
    // let the destructor delete the cache
    }
    
    
//Utility Functions    
MPresenceInfo* T_PresenceCache::MakePresenceInfoLC()
    {
    MPresenceInfo* presInfo = CPresenceInfoImp::NewLC();
    
    MPresenceInfoFieldValueText* value = CPresenceInfoFieldValueTextImp::NewLC();
    value->SetTextValueL(KBuddyId1);
    
    MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
    infoField->SetFieldTypeL(KIdentityField);
    infoField->SetFieldValue(value);
    
    
    MPersonPresenceInfo* personPres = CPersonPresenceInfoImp::NewLC();
    MPresenceInfoFieldCollection& fieldCol = personPres->Fields();
    fieldCol.AddOrReplaceFieldL(infoField);
    TInt fieldCount= fieldCol.FieldCount();
        
    presInfo->SetPersonPresenceL(personPres);
    
    CleanupStack::Pop(3);//personPres,infoField,value
    
    return presInfo;
    }

// Utility function but also test some code
MPresenceBuddyInfo* T_PresenceCache::MakeBuddyPresenceInfoLC(
                                                const TDesC& id, const TDesC& text)
    {
    // it sets objects differently for different flag values of iCoverageTesting
    // just of code coverage purpose
    if(iCoverageTesting) 
        { // make the new 
        iCoverageTesting = !iCoverageTesting;
        MPresenceBuddyInfo* buddyPresInfo = iMPresenceCacheWriter->NewBuddyPresenceInfoLC();
        MPresenceInfo* presInfo = iMPresenceCacheWriter->NewPresenceInfoLC();
        
        MPresenceInfoFieldValueText* value = CPresenceInfoFieldValueTextImp::NewLC();
        value->SetTextValueL(text);
        
        MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
        infoField->SetFieldTypeL(KIdentityField);
        infoField->SetFieldValue(value);
        
        
        MPersonPresenceInfo* personPres = CPersonPresenceInfoImp::NewLC();
        MPresenceInfoFieldCollection& fieldCol = personPres->Fields();
        fieldCol.AddOrReplaceFieldL(infoField);
        TInt fieldCount= fieldCol.FieldCount();
            
        presInfo->SetPersonPresenceL(personPres);
        
        MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
        identity->SetIdentityL(id);
        
        buddyPresInfo->Set(identity, presInfo);
        
        CleanupStack::Pop(5);//personPres,infoField,value,buddyPresInfo, identity
        
        return buddyPresInfo;
        }
    
    iCoverageTesting = !iCoverageTesting;    
    MPresenceBuddyInfo* buddyPresInfo = iMPresenceCacheWriter->NewBuddyPresenceInfoLC();
    MPresenceInfo* presInfo = iMPresenceCacheWriter->NewPresenceInfoLC();
    
    MPresenceInfoFieldValueText* value = CPresenceInfoFieldValueTextImp::NewLC();
    value->SetTextValueL(text);
    
    MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
    infoField->SetFieldTypeL(KIdentityField);
    infoField->SetFieldValue(value);
    
    
    MPersonPresenceInfo* personPres = CPersonPresenceInfoImp::NewLC();
    MPresenceInfoFieldCollection& fieldCol = personPres->Fields();
    fieldCol.AddOrReplaceFieldL(infoField);
    TInt fieldCount= fieldCol.FieldCount();
        
    presInfo->SetPersonPresenceL(personPres);
    
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(id);
    
   
    //Call again for the sake of code coverage
    EUNIT_ASSERT_EQUALS( buddyPresInfo->SetBuddyId(identity), KErrNone);
    EUNIT_ASSERT_EQUALS( buddyPresInfo->SetPresenceInfo(presInfo), KErrNone);
    
    CleanupStack::Pop(5);//personPres,infoField,value,buddyPresInfo, identity
    
    return buddyPresInfo;
    }
    
void T_PresenceCache::T_DeleteAllCacheL()
    {
    // all service uids
    TInt count = iMPresenceCacheReader->ServicesCount();
    TInt err(KErrNone);
    
    err = iMPresenceCacheWriter->DeleteService(KService1);
    err = iMPresenceCacheWriter->DeleteService(KService2);
    err = iMPresenceCacheWriter->DeleteService(KService3);
    err = iMPresenceCacheWriter->DeleteService(KService4);
    err = iMPresenceCacheWriter->DeleteService(KService5);
    err = iMPresenceCacheWriter->DeleteService(KService6);
    }
    
void T_PresenceCache::T_MakePerformancePreconditionsL()
    {
    }

// Buddy with status text and avater    
MPresenceBuddyInfo*  T_PresenceCache::T_MakePerformanceBuddyLC(const TDesC& /*id*/ )
    {
    /*
    MPresenceBuddyInfo* buddyPresInfo = iMPresenceCacheWriter->NewBuddyPresenceInfoLC();
    MPresenceInfo* presInfo = iMPresenceCacheWriter->NewPresenceInfoLC();
    
    //Text field
    MPresenceInfoFieldValueText* value = CPresenceInfoFieldValueTextImp::NewLC();
    value->SetTextValueL(text);
    
    MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
    infoField->SetFieldTypeL(KIdentityField);
    infoField->SetFieldValue(value);
    
    //Binary field
    MPresenceInfoFieldValueBinay* value = CPresenceInfoFieldValueBinaryImp::NewLC();
    value->SetBinaryValueL(text);
    
    MPresenceInfoField* infoField = CPresenceInfoFieldImp::NewLC();
    infoField->SetFieldTypeL(KIdentityField);
    infoField->SetFieldValue(value);
    
    
    
    MPersonPresenceInfo* personPres = CPersonPresenceInfoImp::NewLC();
    MPresenceInfoFieldCollection& fieldCol = personPres->Fields();
    fieldCol.AddOrReplaceFieldL(infoField);
    TInt fieldCount= fieldCol.FieldCount();
        
    presInfo->SetPersonPresenceL(personPres);
    
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(id);
    
    TUid serviceid;
    serviceid.iUid = service;
    
    buddyPresInfo->Set(serviceid, identity, presInfo);
    
    CleanupStack::Pop(5);//personPres,infoField,value,buddyPresInfo, identity
    
    return buddyPresInfo;*/
    }


void T_PresenceCache::T_SubscribeBuddy1Response()
    {    
    // Notice: First set the initial state for User1, this is needed at least now. 
    MPresenceBuddyInfo* initInfo = MakeBuddyPresenceInfoLC(KBuddyId1no, KText1);        
    TInt err = iMPresenceCacheWriter->WritePresenceL(initInfo);
    CleanupStack::PopAndDestroy(  ); // initInfo      
    EUNIT_ASSERT_EQUALS( err, 0 );    
        
    err = iMPresenceCacheReader->SetObserverForSubscribedNotifications( this );   
    EUNIT_ASSERT_EQUALS( err, 0 );    
    
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId1no);
    
    err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity);      
    CleanupStack::PopAndDestroy(1); //identity    
    EUNIT_ASSERT_EQUALS( err, 0 );
    
    // Presence feeder 
    iExpectedNotifyStatus = KErrNone;
    iNbrNotifys = 1;
    iPresenceFeeder->SetAvailabilityL( KBuddyId1, MPresenceFeeder::ENotAvailable, KText2);
        
    // TODO: 
    delete iExpectedNotifyContent;
    iExpectedNotifyContent = NULL;
    iExpectedNotifyContent = 
        MakeBuddyPresenceInfoLC(KBuddyId1no, KText2);
    CleanupStack::Pop(  );  // iExpectedNotifyContent
    err = iMPresenceCacheWriter->WritePresenceL(iExpectedNotifyContent);    
    EUNIT_ASSERT_EQUALS( err, 0 );
    
    iPresenceFeeder->RemoveAvailabilityL(KBuddyId2);                
    
    // Now we start to wait callback method HandlePresenceNotificationL  
    WaitComplete();
    }

void T_PresenceCache::T_SubscribeBuddyAndUnsubscribe()
    {
    // Test few error cases first
            
    // Test null parameter error case
    TInt err = iMPresenceCacheReader->SetObserverForSubscribedNotifications( NULL );   
    EUNIT_ASSERT_EQUALS( err, KErrArgument );
    
    // Test subscription without callback registeration
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId1ECE);    
 
    err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity); 
    EUNIT_ASSERT_EQUALS( err, KErrNotReady );    
    
    // Now make successful subscription  
    err = iMPresenceCacheReader->SetObserverForSubscribedNotifications( this );   
    EUNIT_ASSERT_EQUALS( err, 0 );    
    err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity);     
    EUNIT_ASSERT_EQUALS( err, 0);
    
    // Make successful unsubscription    
    iMPresenceCacheReader->UnSubscribePresenceBuddyChangeL( *identity);        
    
    CleanupStack::PopAndDestroy(1); //identity        
    // Now we start to wait callback method HandlePresenceNotificationL    
    }

void T_PresenceCache::T_SubscribeBuddyManyResponses()
    {  
    // Notice: First set the initial state for User1, this is needed at least now. 
    MPresenceBuddyInfo* initInfo = MakeBuddyPresenceInfoLC(KBuddyId1no, KText1);        
    TInt err = iMPresenceCacheWriter->WritePresenceL(initInfo);
    CleanupStack::PopAndDestroy(  ); // initInfo      
    EUNIT_ASSERT_EQUALS( err, 0 );  
    
    err = iMPresenceCacheReader->SetObserverForSubscribedNotifications( this );   
    EUNIT_ASSERT_EQUALS( err, 0 );    
    
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId1no);
    
    err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity);      
    CleanupStack::PopAndDestroy(1); //identity    
    EUNIT_ASSERT_EQUALS( err, 0 );
    
    // Presence feeder 
    iExpectedNotifyStatus = KErrNone;
    iNbrNotifys = 4;
    iPresenceFeeder->SetAvailabilityL( KBuddyId1, MPresenceFeeder::ENotAvailable, KText2);
        
    delete iExpectedNotifyContent;
    iExpectedNotifyContent = NULL;
    iExpectedNotifyContent = 
        MakeBuddyPresenceInfoLC(KBuddyId1no, KText1);
    CleanupStack::Pop(  );  // iExpectedNotifyContent 
    err = iMPresenceCacheWriter->WritePresenceL(iExpectedNotifyContent);    
    EUNIT_ASSERT_EQUALS( err, 0 );
                   
    // Now we start to wait callback method HandlePresenceNotificationL  
    WaitComplete();        
    }
    
void T_PresenceCache::T_DeleteBeforeNotify()
    {
    // Notice: First set the initial state for User1, this is needed at least now. 
    TInt err;
    /*
    MPresenceBuddyInfo* initInfo = MakeBuddyPresenceInfoLC( KServiceUid6, KBuddyId1, KText1);        
    TInt err = iMPresenceCacheWriter->WritePresenceL(initInfo);
    CleanupStack::PopAndDestroy(  ); // initInfo      
    EUNIT_ASSERT_EQUALS( err, 0 ); */   
        
    err = iMPresenceCacheReader->SetObserverForSubscribedNotifications( this );   
    EUNIT_ASSERT_EQUALS( err, 0 );    
    
    MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
    identity->SetIdentityL(KBuddyId1no);
    
    err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity);      
    CleanupStack::PopAndDestroy(1); //identity    
    EUNIT_ASSERT_EQUALS( err, 0 );
    
    // Presence feeder 
    iExpectedNotifyStatus = KErrNone;
    iNbrNotifys = 1;
    
    
    delete iMPresenceCacheReader;
    iMPresenceCacheReader = NULL;
        
    // TODO: 
    delete iExpectedNotifyContent;
    iExpectedNotifyContent = NULL;
    iExpectedNotifyContent = 
        MakeBuddyPresenceInfoLC(KBuddyId1no, KText2);
    CleanupStack::Pop(  );  // iExpectedNotifyContent
    err = iMPresenceCacheWriter->WritePresenceL(iExpectedNotifyContent);    
    EUNIT_ASSERT_EQUALS( err, 0 );
    
    //no notification should come, since we have deleted the subscriber
    }


// Callback observers
    
void T_PresenceCache::HandlePresenceWriteL(TInt aErrorCode)
    {
	EUNIT_PRINT(_L("HandlePresenceWriteL"));
	EUNIT_PRINT(_L("    aErrorCode: %d"), aErrorCode);
    iCbReceived = ETrue;
    iErrorFromHandler = aErrorCode;

    if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }

void T_PresenceCache::HandlePresenceReadL(TInt aErrorCode,
                    MPresenceBuddyInfoList* aPresenceBuddyInfoList)
    {
	EUNIT_PRINT(_L("HandlePresenceReadL"));
	EUNIT_PRINT(_L("    aErrorCode: %d"), aErrorCode);
    iCbReceived = ETrue;
    iErrorFromHandler = aErrorCode;
    
    
    if(iPresBuddyInfoList)
    	delete iPresBuddyInfoList; iPresBuddyInfoList=NULL;
    iPresBuddyInfoList = aPresenceBuddyInfoList;
    
    if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }
    }
    
// *****************************************************************************   
                            
void T_PresenceCache::HandlePresenceNotificationL(TInt aErrorCode,
                            MPresenceBuddyInfo* aPresenceBuddyInfo)
    {
	EUNIT_PRINT(_L("HandlePresenceNotiicationL"));
	EUNIT_PRINT(_L("    aErrorCode: %d"), aErrorCode);
    iCbReceived = ETrue;
    iErrorFromHandler = aErrorCode;
    
    // check what is expected
    EUNIT_ASSERT_EQUALS( aErrorCode, iExpectedNotifyStatus );     
    
    if ( aPresenceBuddyInfo )
        {
        // We do not want to get anything when not expecting it.
        TBool isNull = iExpectedNotifyContent ? EFalse : ETrue;
        EUNIT_ASSERT_EQUALS( isNull, EFalse  ); 
        // just check that this does not crash
        const MXIMPIdentity* buddyId = aPresenceBuddyInfo->BuddyId();
        // Then test the whole content 
        TBool eq = iExpectedNotifyContent->EqualsIdentity( aPresenceBuddyInfo );  
        EUNIT_ASSERT_EQUALS( eq, ETrue );          
        }
    else
        {
        EUNIT_PRINT(_L("  received buddyInfo is NULL"));        
        }
               
    delete aPresenceBuddyInfo; 
    
    iNbrNotifys--;
    
    /* The test sequence of complicated case is
     * sub Usr1
     * write Usr1 -> Notify
     * sub Usr2
     * write Usr1 -> Notify
     * write Usr2 -> Notify
     * unsub Usr2
     * write Usr2
     * delete Usr1 -> notify(empty)
     */
    
    switch ( iNbrNotifys )
        {
        case 3: 
            // subscribe U2
            {   
            // Notice: First set the initial state for User1, this is needed at least now. 
            MPresenceBuddyInfo* initInfo = MakeBuddyPresenceInfoLC(KBuddyId2no, KText2);        
            TInt err = iMPresenceCacheWriter->WritePresenceL(initInfo);
            CleanupStack::PopAndDestroy(  ); // initInfo      
            EUNIT_ASSERT_EQUALS( err, 0 ); 
            
            MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
            identity->SetIdentityL(KBuddyId2no);            
            err = iMPresenceCacheReader->SubscribePresenceBuddyChangeL( *identity);      
            CleanupStack::PopAndDestroy(1); //identity    
            EUNIT_ASSERT_EQUALS( err, 0 );
            
            // Presence feeder 
            iExpectedNotifyStatus = KErrNone;                
            delete iExpectedNotifyContent;
            iExpectedNotifyContent = NULL;
            iExpectedNotifyContent = 
                MakeBuddyPresenceInfoLC(KBuddyId1no, KText1);
            CleanupStack::Pop(  );  // iExpectedNotifyContent       
            err = iMPresenceCacheWriter->WritePresenceL(iExpectedNotifyContent);    
            EUNIT_ASSERT_EQUALS( err, 0 );   
            // Now we start to wait callback method HandlePresenceNotificationL               
            }
            break;
        case 2: 
            {
            // Presence feeder 
            iExpectedNotifyStatus = KErrNone;                
            delete iExpectedNotifyContent;
            iExpectedNotifyContent = NULL;
            iExpectedNotifyContent = 
                MakeBuddyPresenceInfoLC(KBuddyId2no, KText3);
            CleanupStack::Pop(  );  // iExpectedNotifyContent       
            TInt err = iMPresenceCacheWriter->WritePresenceL(iExpectedNotifyContent);    
            EUNIT_ASSERT_EQUALS( err, 0 );   
            // Now we start to wait callback method HandlePresenceNotificationL          
            }
            break;
        case 1:
            {
            // Unsubscribe Usr2
            MXIMPIdentity* identity = CXIMPIdentityImp::NewLC();
            identity->SetIdentityL(KBuddyId2no);            
            iMPresenceCacheReader->UnSubscribePresenceBuddyChangeL( *identity );      
            CleanupStack::PopAndDestroy(1); //identity    
            
            // Write Usr2 
            iExpectedNotifyStatus = KErrNone;                
            delete iExpectedNotifyContent;
            iExpectedNotifyContent = NULL;   
            
            // delete Usr1
            iPresenceFeeder->RemovePresenceL(KBuddyId1no); 
            iExpectedNotifyContent = 
               MakeBuddyPresenceInfoLC( KBuddyId1no, KText3);
            TInt err =  iExpectedNotifyContent->SetPresenceInfo( NULL );
            EUNIT_ASSERT_EQUALS( err, KErrArgument ); 
            CleanupStack::Pop(  );  // iExpectedNotifyContent            
            
            // Now we start to wait callback method HandlePresenceNotificationL
            }
            break;
        case 0:
        default:         
            if( iWait.IsStarted() )
                {
                iWait.AsyncStop();
                }
            break;   
        };
    
    }  
    
// *****************************************************************************                                  
    
    
void T_PresenceCache::T_TestFeeder()
    {
    iPresenceFeeder->SetAvailabilityL(KBuddyId1,MPresenceFeeder::ENotAvailable,KText2);
    iPresenceFeeder->SetAvailabilityL(KBuddyId1,MPresenceFeeder::ENotAvailable,KText2);
    iPresenceFeeder->RemoveAvailabilityL(KBuddyId2);
    iPresenceFeeder->RemoveAvailabilityL(KBuddyId1);
    iPresenceFeeder->SetAvailabilityL(KBuddyId1,MPresenceFeeder::ENotAvailable,KText2);
    iPresenceFeeder->RemovePresenceL(KBuddyId2);
    iPresenceFeeder->RemovePresenceL(KBuddyId1);
    iPresenceFeeder->SetAvailabilityL(KBuddyId1,MPresenceFeeder::ENotAvailable,KText2);
    iPresenceFeeder->RemoveAllPresences();
    }
    
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    T_PresenceCache,
    "Presence Cache module tests.",
    "MODULE" )
    
EUNIT_TEST(
    "T__PresenceInfoL",
    "PresenceCache",
    "T__PresenceInfoL",
    "FUNCTIONALITY",
    SetupL, T__PresenceInfoL, Teardown)
    
EUNIT_TEST(
    "T_WriteReadPresenceAsyncL",
    "PresenceCache",
    "T_WriteReadPresenceAsyncL",
    "FUNCTIONALITY",
    SetupL, T_WriteReadPresenceAsyncL, Teardown)
    
EUNIT_TEST(
	    "T_DeletingAndCancellingL",
	    "PresenceCache",
	    "T_DeletingAndCancellingL",
	    "FUNCTIONALITY",
	    SetupL, T_DeletingAndCancellingL, Teardown)    
    
EUNIT_TEST(
	    "T_RunAllL",
	    "PresenceCache",
	    "T_RunAllL",
	    "FUNCTIONALITY",
	    SetupL, T_RunAllL, Teardown)
	    
EUNIT_TEST(
	    "T_TestFeeder",
	    "PresenceFeeder",
	    "T_TestFeeder",
	    "FUNCTIONALITY",
	    SetupL, T_TestFeeder, Teardown)
    
EUNIT_TEST(
	    "T_SubscribeBuddyAndUnsubscribe",
	    "PresenceCache",
	    "T_SubscribeBuddyAndUnsubscribe",
	    "FUNCTIONALITY",
	    SetupL, T_SubscribeBuddyAndUnsubscribe, Teardown)	    
	    
EUNIT_TEST(
	    "T_SubscribeBuddy1Response",
	    "PresenceCache",
	    "T_SubscribeBuddy1Response",
	    "FUNCTIONALITY",
	    SetupL, T_SubscribeBuddy1Response, Teardown)
        
EUNIT_TEST(
        "T_SubscribeBuddyManyResponses",
        "PresenceCache",
        "T_SubscribeBuddyManyResponses",
        "FUNCTIONALITY",
        SetupL, T_SubscribeBuddyManyResponses, Teardown)
        
EUNIT_TEST(
        "T_DeleteBeforeNotify",
        "PresenceCache",
        "T_DeleteBeforeNotify",
        "FUNCTIONALITY",
        SetupL, T_DeleteBeforeNotify, Teardown)
        
EUNIT_END_TEST_TABLE

//  END OF FILE
