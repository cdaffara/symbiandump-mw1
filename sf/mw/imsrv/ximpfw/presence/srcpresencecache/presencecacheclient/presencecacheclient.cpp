/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for presence cache client.
*
*/

#include <e32std.h>
#include <s32buf.h>

#include <presenceinfofieldcollection.h>
#include <presencebuddyinfo.h>
#include <ximpidentity.h>
#include <prescachewritehandler.h>
#include <prescachereadhandler.h>
#include <presencebuddyinfolist.h>
#include <personpresenceinfo.h>
#include <presenceinfofield.h>
#include <presenceinfo.h>

#include "presencecacheclient.h"
#include "presencecachedefs.h"
#include "ximpobjecthelpers.h"
#include "presenceinfoimp.h"
#include "personpresenceinfoimp.h"
#include "presenceinfofieldimp.h"
#include "ximpidentityimp.h"
#include "presencebuddyinfoimp.h"
#include "presencebuddyinfolistimp.h"
#include "presenceobjectfactoryimp.h"
#include "presencecacheclientnotification.h"


//Include Cache server namespace
using namespace NCacheSrv;


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceCacheClient::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceCacheClient* CPresenceCacheClient::NewL()
    {
    CPresenceCacheClient* self = new( ELeave ) CPresenceCacheClient();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;    
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::CreateReaderL()
// ---------------------------------------------------------------------------
//
EXPORT_C MPresenceCacheReader* CPresenceCacheClient::CreateReaderL()
    {
    return NewL();
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::CreateWriterL()
// ---------------------------------------------------------------------------
//
EXPORT_C MPresenceCacheWriter* CPresenceCacheClient::CreateWriterL()
    {
    return NewL();
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::~CPresenceCacheClient()
// ---------------------------------------------------------------------------
//
CPresenceCacheClient::~CPresenceCacheClient()
    {
    delete iNotifyHandler;    
    Cancel();
    RSessionBase::Close();
    delete iPresObjFactory;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::CPresenceCacheClient()
// ---------------------------------------------------------------------------
//
CPresenceCacheClient::CPresenceCacheClient(): CActive( EPriorityStandard ), 
                                            iWriteHandler(NULL),
                                            iReadHandler(NULL),
                                            iAsyncReq(NRequest::ENoRequestMade)
    {
        
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheClient::ConstructL()
    {
    Connect();
    CActiveScheduler::Add( this );
    iPresObjFactory = CPresenceObjectFactoryImp::NewL();      
    }
    
// -----------------------------------------------------------------------------
// CPresenceCacheClient::Connect()
// Connects to the server and create a session.
// -----------------------------------------------------------------------------
//
TInt CPresenceCacheClient::Connect()
    {
    TInt error = StartServer();

    if ( KErrNone == error )
        {
        error = CreateSession( NName::KSymbianServer,
                               Version(),
                               NRequest::KMsgSlotCount );
        }
    return error;
    }
    
      
// ----------------------------------------------------
// CPresenceCacheClient::StartServer
//
// ----------------------------------------------------
//
TInt CPresenceCacheClient::StartServer()
    {
    TInt result;
    TRequestStatus status = KRequestPending; 

    TFindServer findCacheServer( NName::KSymbianServer );
    TFullName name;

    result = findCacheServer.Next( name );
    if ( result == KErrNone )
        {
        // Server already running
        return KErrNone;
        }

    RProcess server;
    result = server.Create( NName::KExecutable, KNullDesC );       
    if( result != KErrNone )
        return result;     
    server.Rendezvous( status );    	
    status != KRequestPending ? server.Kill( 0 ) : server.Resume();
    //Wait for start or death 
    User::WaitForRequest( status );	
    result = server.ExitType() == EExitPanic ? KErrGeneral : status.Int();
    server.Close();
    
    return result;	    
    }
    
// -----------------------------------------------------------------------------
// CPresenceCacheClient::Version()
// Gets the version number.
// -----------------------------------------------------------------------------
//
TVersion CPresenceCacheClient::Version() const
    {
    return( TVersion( NVersion::KMajor,
                      NVersion::KMinor,
                      NVersion::KBuild ) );
    }
 

// ---------------------------------------------------------------------------
// CPresenceCacheClient::PresenceInfoLC()
// ---------------------------------------------------------------------------
//    
MPresenceInfo* CPresenceCacheClient::PresenceInfoLC(const MXIMPIdentity& aIdentity)
    {
    CPresenceInfoImp* presInfo(NULL);
    
    if(!ValidateXspId(aIdentity.Identity()))
        return presInfo;
    
    TInt sizeOfPresInfo(0);
    TPckgBuf<TInt> sizePckg;
    
    HBufC8* myIdPack = PackIdentityLC(aIdentity);                
                      
    // Package message arguments before sending to the server
    TIpcArgs args;
    args.Set(1, myIdPack);
    args.Set(2, &sizePckg);    
        
    // We will get either size(+ve) or error code(+ve) after this call
    TInt err = SendReceive( NRequest::EPrepReadPresenceInfoSync, args );
    sizeOfPresInfo = sizePckg();
    
    CleanupStack::PopAndDestroy(myIdPack);
    
    if(sizeOfPresInfo && (err==KErrNone)) // If found
        {
        presInfo = CPresenceInfoImp::NewLC();    
        HBufC8* presInfoDes = HBufC8::NewLC(sizeOfPresInfo);                        
        TPtr8 ptrBuf( presInfoDes->Des() );                        
                               
        SendReceive( NRequest::EGetLastPreparedPacket, TIpcArgs(&ptrBuf) );
        
        TXIMPObjectPacker< CPresenceInfoImp >::UnPackL( *presInfo, *presInfoDes );
        CleanupStack::PopAndDestroy(presInfoDes);
        }
    
    return presInfo;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::BuddyCountInAllServices()
// ---------------------------------------------------------------------------
//    
TInt CPresenceCacheClient::BuddyCountInAllServices()
    {
    TInt buddyCount(0);
    TPckgBuf<TInt> buddyCountPckg;
                      
    // Package message arguments before sending to the server
    TIpcArgs args(&buddyCountPckg);
        
    // This call waits for the server to complete the request before
    // proceeding.
    TInt err = SendReceive( NRequest::EBuddyCountInAllServices, args );
    buddyCount = buddyCountPckg();
    
    return (err<0) ? err  : buddyCount;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::BuddyCountInService()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::BuddyCountInService(const TDesC& aServiceName)
    {
    TInt buddyCount(0);
    TPckgBuf<TInt> buddyCountPckg;
    
    // Package message arguments before sending to the server
    TIpcArgs args(&buddyCountPckg);
    args.Set(1, &aServiceName);
        
    // This call waits for the server to complete the request before
    // proceeding.
    TInt err = SendReceive( NRequest::EBuddyCountInService, args );
    buddyCount = buddyCountPckg();
    
    return (err<0) ? err  : buddyCount;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::ServicesCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::ServicesCount()
    {
    TInt serviceCount(0);
    TPckgBuf<TInt> serviceCountPckg;
    
    // Package message arguments before sending to the server
    TIpcArgs args(&serviceCountPckg);
        
    // This call waits for the server to complete the request before
    // proceeding.
    TInt err = SendReceive( NRequest::EGetServiceCount, args );
    serviceCount = serviceCountPckg();
    
    return (err<0) ? err  : serviceCount;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::ServicesLC()
// ---------------------------------------------------------------------------
//
/* Implementation is not updated because no one is using this functionality
CDesC16Array* CPresenceCacheClient::ServicesLC()
    {
    CDesC16Array* services(NULL); 
    
    // This call waits for the server to complete the request before
    // proceeding.
    TInt size = SendReceive( NRequest::EPrepGetAllServices);
    if(size>0)
    	{
        HBufC8* servicesDes = HBufC8::NewLC(size);                        
        TPtr8 ptrBuf( servicesDes->Des() );                        
                               
        SendReceive( NRequest::EGetAllServices, TIpcArgs(&ptrBuf) );
        
        services =  UnPackServicesLC(*servicesDes);
        CleanupStack::Pop(services);
        CleanupStack::PopAndDestroy(servicesDes);
        CleanupStack::PushL(services);
    	}
    return services;
    }*/                               
                                                
// ---------------------------------------------------------------------------
// CPresenceCacheClient::AllBuddiesPresenceInService()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::AllBuddiesPresenceInService(const TDesC& aServiceName, 
                                            MPresCacheReadHandler* aHandler)
    {
    if (IsActive())
        return KErrNotReady;
    
    if(!aHandler || (aServiceName.Length()==NULL) )
        return KErrArgument;
    else
        iReadHandler = aHandler;
    
    // Package message arguments before sending to the server
    TIpcArgs args(&aServiceName);
        
    // This call is async
    SendReceive( NRequest::EPrepReadAllBuddiesPresenceInService, args, iStatus );
    
    iAsyncReq = NRequest::EPrepReadAllBuddiesPresenceInService;
    
    SetActive(); 
    
    return KErrNone;
        
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::SubscribePresenceBuddyChangeL
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::SubscribePresenceBuddyChangeL(
                                                const MXIMPIdentity& aIdentity)
    {
    if(!ValidateXspId(aIdentity.Identity()))
        return KErrArgument;

    if ( !iNotifyClient )
    	{
    	return KErrNotReady;
    	}
    
    if ( !iNotifyHandler )
    	{
    	iNotifyHandler = CPresenceCacheClientNotification::NewL( *this );  	    	
    	}
    
    TInt ret = iNotifyHandler->SubscribePresenceBuddyChangeL( aIdentity);
    return ret; 
    }  
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::UnSubscribePresenceBuddyChangeL
// ---------------------------------------------------------------------------
//
void CPresenceCacheClient::UnSubscribePresenceBuddyChangeL(
                        const MXIMPIdentity& aIdentity)
    {
    if(!ValidateXspId(aIdentity.Identity()))
        return;
    
    if ( iNotifyHandler )
    	{
    	iNotifyHandler->UnsubscribePresenceBuddyChangeL( aIdentity);  	    	
    	} 
    }      
                                           
// ---------------------------------------------------------------------------
// CPresenceCacheClient::SetObserverForSubscribedNotifications
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::SetObserverForSubscribedNotifications(
    MPresCacheReadHandler* aHandler)
    {
    if ( !aHandler )
    	{
    	return KErrArgument;
    	}
    iNotifyClient = aHandler;
    return KErrNone;               
    }                                          
                                             
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::HandlePreparedAllBuddiesPresenceL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheClient::HandlePreparedAllBuddiesPresenceL(TInt aError)
    {
    TInt status = aError; // size comes in status
    CPresenceBuddyInfoListImp* buddyInfoList(NULL);
    if(status>0) // if anything found
        {
        buddyInfoList = CPresenceBuddyInfoListImp::NewLC(KNullDesC);    
        HBufC8* buddyInfosDes = HBufC8::NewLC(status);                        
        TPtr8 ptrBuf( buddyInfosDes->Des() );                        
                               
        status = SendReceive( NRequest::EGetLastAsyncPreparedPacket, TIpcArgs(&ptrBuf) );
        
        TXIMPObjectPacker< CPresenceBuddyInfoListImp >::UnPackL( *buddyInfoList, *buddyInfosDes );
        CleanupStack::PopAndDestroy(buddyInfosDes);
        CleanupStack::Pop(buddyInfoList); // ownership will transfer to client
        }
    
    if(iReadHandler)
        iReadHandler->HandlePresenceReadL(status, buddyInfoList);
    else if(buddyInfoList) // if failed to transfer ownership and created object
        delete buddyInfoList; 
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::WritePresenceL()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::WritePresenceL(
                                const MPresenceBuddyInfo* aPresenceBuddyInfo)
    {
    if(!aPresenceBuddyInfo)
        return KErrArgument;
    
    if(!ValidateXspId(aPresenceBuddyInfo->BuddyId()->Identity()))
        return KErrArgument;
        

    HBufC8* presInfoPack = PackBuddyPresenceInfoLC(aPresenceBuddyInfo);
    
    // Package message arguments before sending to the server
    TIpcArgs args(presInfoPack);
                                                            
       
    // This call waits for the server to complete the request before
    // proceeding.
    TInt err = SendReceive( NRequest::EWritePresenceInfoSync, args );
    
    CleanupStack::PopAndDestroy(presInfoPack);    
    
    return err;
    
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::DeletePresenceL()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::DeletePresenceL(const MXIMPIdentity& aIdentity)
    {
    HBufC8* idPack = PackIdentityLC(aIdentity);
    
    // Package message arguments before sending to the server
    TIpcArgs args(idPack);
       
    // This call waits for the server to complete the request before
    // proceeding.
    TInt err = SendReceive( NRequest::EDeletePresence, args );
    
    CleanupStack::PopAndDestroy(idPack);    
    
    return err;
    }
    

// ---------------------------------------------------------------------------
// CPresenceCacheClient::CancelWrite()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::CancelWrite()
    {
    TInt ret(KErrNotFound);
    if(IsActive())
        {
        Cancel();
        ret = KErrNone;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::CancelRead()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::CancelRead()
    {
    TInt ret(KErrNotFound);
    if(IsActive())
        {
        Cancel();
        ret = KErrNone;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::DeleteService()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::DeleteService(const TDesC& aServiceName)
    {
    return SendReceive( NRequest::EDeleteService, TIpcArgs(&aServiceName) );
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::PresenceObjectFactory()
// ---------------------------------------------------------------------------
//
MPresenceObjectFactory& CPresenceCacheClient::PresenceObjectFactory() const
    {
    return *iPresObjFactory;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::NewIdentityLC()
// ---------------------------------------------------------------------------
//
MXIMPIdentity* CPresenceCacheClient::NewIdentityLC()
    {
    return CXIMPIdentityImp::NewLC();  
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::NewPresenceInfoLC()
// ---------------------------------------------------------------------------
//    
MPresenceInfo* CPresenceCacheClient::NewPresenceInfoLC()
    {
    return CPresenceInfoImp::NewLC();    
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::NewBuddyPresenceInfoLC()
// ---------------------------------------------------------------------------
//    
MPresenceBuddyInfo* CPresenceCacheClient::NewBuddyPresenceInfoLC()
    {
    return CPresenceBuddyInfoImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::NewPresenceBuddyInfoListLC()
// ---------------------------------------------------------------------------
//    
MPresenceBuddyInfoList* CPresenceCacheClient::NewPresenceBuddyInfoListLC(
                                                        const TDesC& aServiceName)
    {
    return CPresenceBuddyInfoListImp::NewLC(aServiceName);
    }
                                                
// ---------------------------------------------------------------------------
// CPresenceCacheClient::WritePresenceL()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheClient::WritePresenceL(
                            const MPresenceBuddyInfoList* aPresenceBuddyInfos,
                                            MPresCacheWriteHandler* aHandler)
    {
    if (IsActive())
        return KErrNotReady;
    if(!aPresenceBuddyInfos)
        return KErrArgument;
    
    HBufC8* presInfoPack = PackPresenceBuddyListLC(aPresenceBuddyInfos);
    
    // Package message arguments before sending to the server
    TIpcArgs args(presInfoPack);
    args.Set(1,aPresenceBuddyInfos->Count());
                                                            
    // This is async call to server.
    SendReceive( NRequest::EWritePresenceInfoAsync, args, iStatus);
    
    iAsyncReq = NRequest::EWritePresenceInfoAsync;
    
    if(aHandler)
        iWriteHandler = aHandler;    
    
    CleanupStack::PopAndDestroy(presInfoPack);
    
    SetActive(); 
    
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::ValidateXspId()
// ---------------------------------------------------------------------------
//
TBool CPresenceCacheClient::ValidateXspId(const TDesC& aXspId)
    {
    TBool ret(EFalse);
    _LIT(KColon, ":");
    TInt pos = aXspId.Find(KColon);
    if(pos>0) // if colon found and there is something before colon, i.e. xsp id
        {
        ret = ETrue;
        }
    return ret;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::RunL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheClient::RunL()
    {
    
    TInt origStatus = iStatus.Int();
    switch (iAsyncReq)
        {
        case NRequest::EWritePresenceInfoAsync:
            if(iWriteHandler) // if client had requested the call back
                iWriteHandler->HandlePresenceWriteL(origStatus);
            iAsyncReq = NRequest::ENoRequestMade;
            break;

        case NRequest::EPrepReadAllBuddiesPresenceInService:
            HandlePreparedAllBuddiesPresenceL(origStatus);
            iAsyncReq = NRequest::ENoRequestMade;
            break;
        
        case NRequest::ENoRequestMade:
        default:
            break;
        }   
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::DoCancel()
// ---------------------------------------------------------------------------
//
void CPresenceCacheClient::DoCancel()
    {
    iAsyncReq = NRequest::ENoRequestMade;
    iReadHandler = NULL;
    iWriteHandler = NULL;
    SendReceive( NRequest::ECancelAsyncOperation);
    SendReceive( NRequest::ECancelWaitingForNotification);
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheClient::PackBuddyPresenceInfoLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceCacheClient::PackBuddyPresenceInfoLC(
                                    const MPresenceBuddyInfo* aBuddyPresInfo)
    {
    HBufC8* pack(NULL);
    if(aBuddyPresInfo)
        {
        const CPresenceBuddyInfoImp* pifImp = 
            TXIMPGetImpClassOrPanic< const CPresenceBuddyInfoImp >::From( *aBuddyPresInfo );    
        
        pack = TXIMPObjectPacker< const CPresenceBuddyInfoImp>::PackL( *pifImp );
        CleanupStack::PushL( pack );
        }

    return pack;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::PackPresenceBuddyListLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceCacheClient::PackPresenceBuddyListLC(const MPresenceBuddyInfoList* aList)
    {
    HBufC8* pack(NULL);
    if(aList)
        {
        const CPresenceBuddyInfoListImp* pifImp = 
            TXIMPGetImpClassOrPanic< const CPresenceBuddyInfoListImp >::From( *aList );    
        
        pack = TXIMPObjectPacker< CPresenceBuddyInfoListImp>::PackL(*pifImp);
        CleanupStack::PushL( pack );
        }
    return pack;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::PackIdentityLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceCacheClient::PackIdentityLC(const MXIMPIdentity& aIdentity)
    {
    HBufC8* pack(NULL);
    
    const CXIMPIdentityImp* pifImp = 
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From(aIdentity);    
    
    pack = TXIMPObjectPacker< CXIMPIdentityImp>::PackL( *pifImp );
    CleanupStack::PushL( pack );

    return pack;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheClient::UnPackServicesLC()
// ---------------------------------------------------------------------------
//
/*
CDesCArray* CPresenceCacheClient::UnPackServicesLC(TDesC8& aServicesBuffer)
	{
    CArrayFixFlat<TUid>* services = new (ELeave) CArrayFixFlat<TUid>(1);
    CleanupStack::PushL(services);
    
    RDesReadStream rs;
    CleanupClosePushL( rs );
    rs.Open( aServicesBuffer ); // CSI: 65 #

    // read the object count
    TInt objCount( rs.ReadInt32L() );
    TUid service;

	for ( TInt count = 0; count < objCount; count++ )
        {
        rs>>service;
		services->AppendL(service);
		}

    CleanupStack::PopAndDestroy(); // rs
    return services;
	}*/
    
// end of file
