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
* Description:  ?Description
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "imcache_test.h"

#include   "cimcachefactory.h"
#include	"mimcacheupdater.h"
#include	"mimcacheaccessor.h"
#include    "mimcacheeventhandler.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cimcache_test::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cimcache_test::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cimcache_test::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreateSameUpdaterL", Cimcache_test::CreateSameUpdaterL ),
        ENTRY( "CreateDifferentUpdaterL", Cimcache_test::CreateDifferentUpdaterL ),
        ENTRY( "CreateSameAccessorL", Cimcache_test::CreateSameAccessorL ),
        ENTRY( "CreateDifferentAccessorL", Cimcache_test::CreateDifferentAccessorL ),
        ENTRY( "StartNewConversationL", Cimcache_test::StartNewConversationL ),
        ENTRY( "AppendSendMessageL", Cimcache_test::AppendSendMessageL ),
        ENTRY( "AppendRecieveMessageL", Cimcache_test::AppendRecieveMessageL ),
        ENTRY( "FetchExistingMessageL", Cimcache_test::FetchExistingMessageL),
        ENTRY( "DeactivateConversationL", Cimcache_test::DeactivateConversationL),
        ENTRY( "CloseConversationL", Cimcache_test::CloseConversationL ),
        ENTRY( "GetUnreadMsgCountL", Cimcache_test::GetUnreadMsgCountL ),
        ENTRY( "GetNewMsgNotificationtL", Cimcache_test::GetNewMsgNotificationtL ),
        
        ENTRY( "ConversationUnExistTestL", Cimcache_test::ConversationUnExistTestL ),
        ENTRY( "ConversationExistTestL", Cimcache_test::ConversationExistTestL ),
        
        ENTRY( "AppendMessageL", Cimcache_test::AppendMessageL ),
        ENTRY( "GetAllUnreadMsgCountL", Cimcache_test::GetAllUnreadMsgCountL ),
          
        ENTRY( "GetChatListTestL", Cimcache_test::GetChatListTestL ),
        ENTRY( "AccessorCloseConversationTestL", Cimcache_test::AccessorCloseConversationTestL ),
         
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }
  

// -----------------------------------------------------------------------------
// Cimcache_test::CreateSameUpdaterL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::CreateSameUpdaterL( CStifItemParser& /*aItem*/ )
	{

 	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater1 = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
	 
	 MIMCacheUpdater* updater2 = instance->CreateUpdaterL(1, _L("testlm123@gmail.com") ,ETrue);
	 
	 TInt error = KErrGeneral;
	 
	 if(updater1 == updater2 )
		 {
		 error = KErrNone;	
		 }
	 
	 CIMCacheFactory::Release();
	 
	 return error;
	}
  
// -----------------------------------------------------------------------------
// Cimcache_test::CreateDifferentUpdaterL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::CreateDifferentUpdaterL( CStifItemParser& /*aItem*/ )
	{

 	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater1 = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
	 
	 MIMCacheUpdater* updater2 = instance->CreateUpdaterL(2, _L("abc123@gmail.com") ,ETrue);
	 
	 TInt error = KErrGeneral;
	 
	 if(updater1 != updater2 )
		 {
		 error = KErrNone;	
		 }
	 
	 CIMCacheFactory::Release();
	 
	 return error;
	}
   
   // -----------------------------------------------------------------------------
// Cimcache_test::CreateSameAccessorL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::CreateSameAccessorL( CStifItemParser& /*aItem*/ )
	{

 	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheAccessor* accessor1 = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 
	 MIMCacheAccessor* accessor2 = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 
	 TInt error = KErrGeneral;
	 
	 if(accessor1 == accessor2 )
		 {
		 error = KErrNone;	
		 }
	 
	 CIMCacheFactory::Release();
	 
	 return error;
	}
  
// -----------------------------------------------------------------------------
// Cimcache_test::CreateDifferentAccessorL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::CreateDifferentAccessorL( CStifItemParser& /*aItem*/ )
	{

 	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheAccessor* accessor1 = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 
	 MIMCacheAccessor* accessor2 = instance->CreateAccessorL(2, _L("abc123@gmail.com") );
	 
	 TInt error = KErrGeneral;
	 
	 if(accessor1 != accessor2 )
		 {
		 error = KErrNone;	
		 }
	 
	 CIMCacheFactory::Release();
	 
	 return error;
	}  
 // -----------------------------------------------------------------------------
// Cimcache_test::StartNewConversationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::StartNewConversationL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
     updater->RegisterObserverL(*this);
 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
 	
	 if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	 return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Cimcache_test::AppendSendMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::AppendSendMessageL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com") ,ETrue);
     updater->RegisterObserverL(*this);
 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
 	
	 if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
	 updater->AppendSendMessageL( _L("hello this is test message appended") ) ;	
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	 return KErrNone;
    }
 
 // -----------------------------------------------------------------------------
// Cimcache_test::AppendRecieveMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::AppendRecieveMessageL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
     updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is test message appended") ) ;
     	
	 CIMCacheFactory::Release();
	 return KErrNone;
    }
    
  // -----------------------------------------------------------------------------
// Cimcache_test::StartNewConversationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::FetchExistingMessageL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	 TInt error = KErrNotFound;  
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com") ,ETrue);
	 // tow message appended hence counter will be two.
	 updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is test first message appended") ) ;
	 updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is test second message appended") ) ;
	  
     updater->RegisterObserverL(*this);
     
     if( !iWait.IsStarted() )
          {
          iWait.Start(); 
          }
     
 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
	 
	 if( !iWait.IsStarted() )
         {
         iWait.Start(); 
         }
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	if( iMessageFetch == 2) // two message should be fetched
		{
		error = KErrNone;	
		}
	 return error;
    }

// -----------------------------------------------------------------------------
// Cimcache_test::DeactivateConversationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::DeactivateConversationL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
     updater->RegisterObserverL(*this);
 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
 	
	 if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
     // close the active conversation
	 updater->DeactivateConversationL();
	 
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	 return KErrNone;
    }
// -----------------------------------------------------------------------------
// Cimcache_test::CloseConversationL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::CloseConversationL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue );
     updater->RegisterObserverL(*this);

 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
 	
	 if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
     // close the active conversation
	 updater->CloseConversationL( _L("testui123@gmail.com") );
	 
	   if( !iWait.IsStarted() )
         {
         iWait.Start(); 
         }
	 
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	 return KErrNone;
    }


// -----------------------------------------------------------------------------
// Cimcache_test::GetUnreadMsgCountL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::GetUnreadMsgCountL(CStifItemParser& /*aItem*/) 
	{
	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"), ETrue );
     updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is test message appended") ) ;
 	 TInt count = accessor->GetUnreadMessageCountL( _L("testui123@gmail.com") );
	 CIMCacheFactory::Release();
	 if(count == 1)
		 {
		 return KErrNone;	
		 }
	 return KErrArgument;
	}


// -----------------------------------------------------------------------------
// Cimcache_test::GetUnreadMsgCountChangeL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::GetAllUnreadMsgCountL(CStifItemParser& /*aItem*/) 
	{
	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"), ETrue );
     updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is test message appended") ) ;
     updater->AppendReceiveMessageL( _L("meco1111@gmail.com"),_L("i am fine here") ) ;
     
 	 TInt count = accessor->GetAllUnreadMessageCountL( );
	 CIMCacheFactory::Release();
	 if(count == 2)
		 {
		 return KErrNone;	
		 }
	 return KErrArgument;
	}
// -----------------------------------------------------------------------------
// Cimcache_test::ConversationUnExistTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::ConversationUnExistTestL(CStifItemParser& /*aItem*/) 
	{
	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	// not yest conversation open so must be EFalse here
 	 TBool ret = accessor->IsConversationExistL(_L("testui123@gmail.com") );
 	 
	 CIMCacheFactory::Release();
	 
	 if(!ret)
		 {
		 return KErrNone;	
		 }
	 return KErrArgument;
	}

// -----------------------------------------------------------------------------
// Cimcache_test::ConversationExistTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::ConversationExistTestL(CStifItemParser& /*aItem*/) 
	{
    CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
		 
	MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"), ETrue );
	updater->RegisterObserverL(*this);
	updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
	
	if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
	   
	 MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	//  conversation already open so must be ETrue here
 	 TBool ret = accessor->IsConversationExistL( _L("testui123@gmail.com") );
 	 
 	 updater->UnRegisterObserver(*this);
 	 	 
	 CIMCacheFactory::Release();
	 
	 if(ret)
		 {
		 return KErrNone;	
		 }
	 return KErrArgument;
	}

// -----------------------------------------------------------------------------
// Cimcache_test::AppendMessageL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::AppendMessageL( CStifItemParser& /*aItem*/ )
    {
     CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	   
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com") ,ETrue);
     updater->RegisterObserverL(*this);
 	 updater->StartNewConversationL( _L("testui123@gmail.com") ) ;
 	
	 if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
	 updater->AppendMessageL(_L("testui123@gmail.com"), _L("this is system or information message message") ) ;	
	 updater->UnRegisterObserver(*this);
	 CIMCacheFactory::Release();
	 return KErrNone;
    }
 		
// -----------------------------------------------------------------------------
// Cimcache_test::GetUnreadMsgCountL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::GetNewMsgNotificationtL(CStifItemParser& /*aItem*/) 
	{
	 CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	 MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	 accessor->RegisterObserverL(*this);
	 MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),ETrue);
     updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is new message notification") ) ;
     
     if( !iWait.IsStarted() )
		 {
		 iWait.Start();	
		 }
	 accessor->UnRegisterObserver(*this);	 
	 
 	 CIMCacheFactory::Release();
	
	 return KErrNone;
	}


// -----------------------------------------------------------------------------
// Cimcache_test::GetChatListTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cimcache_test::GetChatListTestL( CStifItemParser& /*aItem*/)
	{
	TInt error = KErrGeneral;
	
	CIMCacheFactory* instance = CIMCacheFactory::InstanceL();
    MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"), EFalse );    
    
    updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is new message notification 1 ") ) ;
    updater->AppendReceiveMessageL( _L("abc123@gmail.com"),_L("hello this is new message notification 2 ") ) ;
    updater->AppendReceiveMessageL( _L("testgizmo123@gmail.com"),_L("hello this is new message notification 3 ") ) ;
   
    
    RArray<SIMCacheChatItem> listArray = updater->GetChatListL( 1 );
    
    if( listArray.Count() == 3 ) // total 3 chat created in server
	    {
	    error = KErrNone;	
	    }

    CIMCacheFactory::Release();  
    
	return error;
	}
// -----------------------------------------------------------------------------
// Cimcache_test::AccessorCloseConversationTestL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
TInt Cimcache_test::AccessorCloseConversationTestL( CStifItemParser& /*aItem*/)
	{
	TInt error = KErrGeneral;

	CIMCacheFactory* instance = CIMCacheFactory::InstanceL(); 
	
	MIMCacheUpdater* updater = instance->CreateUpdaterL(1, _L("testlm123@gmail.com"),EFalse );    
    updater->AppendReceiveMessageL( _L("testui123@gmail.com"),_L("hello this is new message notification 1 ") ) ;
  	
	MIMCacheAccessor* accessor = instance->CreateAccessorL(1, _L("testlm123@gmail.com") );
	accessor->RegisterObserverL(*this);
	
	if( accessor->IsConversationExistL(_L("testui123@gmail.com") ) )
		{
			// close the active conversation
		accessor->CloseConversationL( _L("testui123@gmail.com") );
			
		if( !iWait.IsStarted() )
			 {
			 iWait.Start(); 
			 }
		error = KErrNone;
		}

	accessor->UnRegisterObserver(*this);
	
	CIMCacheFactory::Release();

	return error;
	}

// -----------------------------------------------------------------------------
// Cimcache_test::HandleIMCacheNewMessageEventL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void Cimcache_test::HandleIMCacheEventL( TIMCacheEventType aEventType, TAny* /*aChatMessage*/  )
	{
	switch( aEventType )
		{
		case EIMCacheUnreadMessage :
				{
				if( iWait.IsStarted() )
					 {
					 iWait.AsyncStop();	
					 }	
				break;
				}
			case EIMCacheUnreadChange :
				{
				if( iWait.IsStarted() )
					 {
					 iWait.AsyncStop();	
					 }
				break;
				}
			
			case EIMCacheRequestCompleted :
				{
				if( iWait.IsStarted() )
					 {
					 iWait.AsyncStop();	
					 }
				break;
				}
			case EIMCacheNewMessage :
				{
				iMessageFetch++;
                if( iWait.IsStarted() && iMessageFetch == 2 )
                    {
                    iWait.AsyncStop(); 
                    }
				break;
				}
			case EIMCacheNewChat:
			    {
	             if( iWait.IsStarted() )
                     {
                     iWait.AsyncStop(); 
                     }
                break;
			    }
			case EIMCacheChatClosed:
			    {
	              if( iWait.IsStarted() )
                     {
                     iWait.AsyncStop(); 
                     }
			    break;
			    }
			
		}
	}
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

// end of file
