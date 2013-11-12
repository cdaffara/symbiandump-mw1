/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Impementation for Presence Cache Server session
*
*/


// INCLUDE FILES
#include <presenceobjectfactory.h>
#include <personpresenceinfo.h>
#include <presencebuddyinfo.h>
#include <presencebuddyinfolist.h>
#include <presenceinfofieldcollection.h>

#include "presencecachesession.h"
#include "presencecacheserver.h"
#include "ximptrace.h"
#include "ximpobjecthelpers.h"
#include "presenceinfoimp.h"
#include "presencebuddyinfolistimp.h"
#include "presencebuddyinfoimp.h"
#include "ximpidentityimp.h"

#include "presencecacheservicestore.h"
#include "presencecachebuddystore.h"

//Include Cache server namespace
using namespace NCacheSrv;


// ---------------------------------------------------------------------------
// CPresenceCacheSession::NewL()
// ---------------------------------------------------------------------------
//
CPresenceCacheSession* CPresenceCacheSession::NewL( CPresenceCacheServer& aServer )
    {
    CPresenceCacheSession* self = CPresenceCacheSession::NewLC( aServer );
    CleanupStack::Pop(self);
    return self;    
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::NewLC()
// ---------------------------------------------------------------------------
//
CPresenceCacheSession* CPresenceCacheSession::NewLC( CPresenceCacheServer& aServer )
    {
    CPresenceCacheSession* self = new ( ELeave ) CPresenceCacheSession( aServer );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;    
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::~CPresenceCacheSession()
// ---------------------------------------------------------------------------
//
CPresenceCacheSession::~CPresenceCacheSession()
    {
    if(iDataPack)
        delete iDataPack;
    if(iDataPackAsync)
        delete iDataPackAsync;
    if(iDataPackNotifier)
        delete iDataPackNotifier;
    if(iBuddypresInfoList)
        delete iBuddypresInfoList;
    if(iActiveHelper)
        delete iActiveHelper;
    iServer.DecrementSessions();
    RemoveMySubscriptions(); // remove all subscriptions subscribed by this session
    iSubscribedStores.Close();
    TRACE_1( _L("CPresenceCacheSession[%d]::~CPresenceCacheSession()"), this );
    }


// ---------------------------------------------------------------------------
// CPresenceCacheSession::ServiceL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::ServiceL( const RMessage2& aMessage )
    {
    switch ( aMessage.Function() )
        {
        case NRequest::EPrepReadPresenceInfoSync:
            HandlePrepReadPresenceInfoSyncL(aMessage);
            break;
            
        case NRequest::EGetLastPreparedPacket:
            HandleGetLastPreparedPacketL(aMessage);
            break;
            
        case NRequest::EGetLastAsyncPreparedPacket:
            HandleGetLastPreparedAsyncPacketL(aMessage);
            break;
            
        case NRequest::EBuddyCountInAllServices:
            HandleBuddyCountInAllServices(aMessage);
            break;
            
        case NRequest::EBuddyCountInService:
            HandleBuddyCountInService(aMessage);
            break;
            
        case NRequest::EGetServiceCount:
            HandleGetServiceCount(aMessage);
            break;
            
        case NRequest::EDeleteService:
            HandleDeleteService(aMessage);
            break;
            
        case NRequest::EDeletePresence:
            HandleDeletePresence(aMessage);
            break;
            
        case NRequest::ECancelAsyncOperation:
            HandleCancel(aMessage);
            break;

        case NRequest::EWritePresenceInfoSync:
            HandleWritePresenceInfoSyncL(aMessage);
            break;

        case NRequest::EWritePresenceInfoAsync:
            HandleWritePresenceInfoAsyncL(aMessage);
            break;
            
        case NRequest::EPrepReadAllBuddiesPresenceInService:
            HandlePrepReadAllBuddiesPresenceInService(aMessage);
            break;
            
        case NRequest::ESubscribeBuddyPresenceChange:// 0:aService.iUid, 1:myIdPack
            HandleSubscribeBuddyPresenceChange(aMessage);
            break;
        
        case NRequest::EUnSubscribeBuddyPresenceChange: // 0:aService.iUid, 1:myIdPack
            HandleUnSubscribeBuddyPresenceChange(aMessage);
            break;
        
        case NRequest::EGetLastNotifiedtPacket: // 0:&ptrBuf
            HandleGetLastNotifiedtPacketL(aMessage);
            break;
        
        case NRequest::EWaitingForNotification: // 0:&sizePckg
            HandleWaitingForNotification(aMessage);
            break;
        
        case NRequest::ECancelWaitingForNotification:
            HandleCancelWaitingForNotification(aMessage);
            break;

        default:
        	TRACE( _L("CPresenceCacheSession::ServiceL - default"));
            aMessage.Complete(KErrArgument);
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandlePrepReadPresenceInfoSyncL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandlePrepReadPresenceInfoSyncL( const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandlePrepReadPresenceInfoSyncL - begin"));

    TInt sizeOfReturnPacket(NULL);
 
    // unpacking identity
    const TInt pckSize = aMessage.GetDesLengthL(1);
    HBufC8* idPack = HBufC8::NewLC( pckSize );
    TPtr8 paramPckPtr = idPack->Des();
    aMessage.ReadL( 1, paramPckPtr );
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL(*identity, *idPack);
    
    TPtrC serviceName = GetServiceName(identity->Identity());
    
    // now processing
    TInt serviceCount = iServer.iPresenceCache.Count();
    MPresenceBuddyInfo* buddyPresenceInfo(NULL);
    CPresenceCacheBuddyStore* buddyStore(NULL);
    
    TRACE_1( _L("_______serviceCount[%d]"), serviceCount);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName == (iServer.iPresenceCache[i])->ServiceName())
            {
            buddyStore = (iServer.iPresenceCache[i])->FindAndGet(*identity);
            if(buddyStore)
                buddyPresenceInfo = buddyStore->PresenceBuddyInfo();
            break;
            }
        }
    
    CleanupStack::PopAndDestroy(identity);
    CleanupStack::PopAndDestroy(idPack);
    
    TInt err = KErrNotFound;
    
    TRACE_1( _L("_______buddyPresenceInfo[%d]"), buddyPresenceInfo);
    if(buddyPresenceInfo) // if found
        {
        const CPresenceInfoImp* presInfo = 
            TXIMPGetImpClassOrPanic< const CPresenceInfoImp >::From(*(buddyPresenceInfo->PresenceInfo()));
        if(iDataPack)
            delete iDataPack; iDataPack=NULL;
        iDataPack = PackPresenceInfoLC(*presInfo);
        CleanupStack::Pop(iDataPack);
        sizeOfReturnPacket = (iDataPack->Des()).Size();
        err = KErrNone;
        }
    
    TPckgBuf<TInt> p(sizeOfReturnPacket);
    aMessage.WriteL(2,p);            
    aMessage.Complete(err);
    TRACE( _L("CPresenceCacheSession::HandlePrepReadPresenceInfoSyncL - end"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleGetLastPreparedPacketL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleGetLastPreparedPacketL( const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleGetLastPreparedPacketL() - begin"));
    if(iDataPack)
        aMessage.WriteL(0, *(iDataPack));
    aMessage.Complete(KErrNone);   
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleGetLastPreparedAsyncPacketL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleGetLastPreparedAsyncPacketL( const RMessage2& aMessage )
    {
    if(iDataPackAsync)
        aMessage.WriteL(0, *(iDataPackAsync));
    aMessage.Complete(KErrNone);   
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleWritePresenceInfoSyncL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleWritePresenceInfoSyncL(const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleWritePresenceInfoSyncL - begin"));
    TInt err(KErrNone);
    const TInt pckSize = aMessage.GetDesLengthL(0);
    if(iDataPack)
        delete iDataPack; iDataPack=NULL;
    iDataPack = HBufC8::NewL( pckSize );
    TPtr8 paramPckPtr = iDataPack->Des();
    aMessage.ReadL( 0, paramPckPtr );
    
    CPresenceBuddyInfoImp* buddypresenceInfo = CPresenceBuddyInfoImp::NewLC();
   

    TXIMPObjectPacker< CPresenceBuddyInfoImp >::UnPackL(*buddypresenceInfo, *iDataPack);
    
    err = TryWriteBuddyToCacheL(buddypresenceInfo);
    
    if(err==KErrNone) //ownership transferred    
        CleanupStack::Pop(buddypresenceInfo);
    else
        CleanupStack::PopAndDestroy(buddypresenceInfo);    
    aMessage.Complete(err);
    TRACE( _L("CPresenceCacheSession::HandleWritePresenceInfoSyncL - end"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandlePrepReadAllBuddiesPresenceInService()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandlePrepReadAllBuddiesPresenceInService
                                                        (const RMessage2& aMessage )
    {
    if(iActiveHelper->IsActive())
        aMessage.Complete(KErrServerBusy);
    else
        {
        iMessage = aMessage;
        
        iAsyncReq = NRequest::EPrepReadAllBuddiesPresenceInService;
        iActiveHelper->Start();
        }
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::ReadAllBuddiesPresenceInServiceL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::ReadAllBuddiesPresenceInServiceL()
    {
    const TInt pckSize = iMessage.GetDesLengthL(0);
    HBufC* serviceName = HBufC16::NewLC( pckSize );
    TPtr16 paramPckPtr = serviceName->Des();
    iMessage.ReadL( 0, paramPckPtr );
    
    TInt serviceCount = iServer.iPresenceCache.Count();
    CPresenceCacheServiceStore* serviceStore(NULL);
    CPresenceBuddyInfoListImp* buddyPresenceInfoList(NULL);
    CPresenceCacheBuddyStore* buddyStore(NULL);
    MPresenceBuddyInfo* presBuddyInfo(NULL);
    TInt presCount(0);
    TInt count(0);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        serviceStore = iServer.iPresenceCache[i];
        if (serviceName->Des() == serviceStore->ServiceName())
            {
            presCount = serviceStore->PresenceCount();
            // if it has presences
            if(presCount)
                {
                buddyPresenceInfoList = CPresenceBuddyInfoListImp::NewLC(*serviceName);
                buddyPresenceInfoList->SetOwnObjects(EFalse);
                count = serviceStore->Count();
                for(TInt j=0; j<count; j++)
                    {
                    buddyStore = serviceStore->GetObjectCollection()[j];
                    presBuddyInfo = buddyStore->PresenceBuddyInfo();
                    if(presBuddyInfo)
                        buddyPresenceInfoList->BlindAddL(presBuddyInfo);    
                    }
                }
            break;
            }
        }
    
    TInt size(KErrNotFound); // this may also indicate error to client if negative
    if(buddyPresenceInfoList) // if found
        {
        if(iDataPackAsync)
            delete iDataPackAsync; iDataPackAsync = NULL;
        iDataPackAsync = TXIMPObjectPacker< CPresenceBuddyInfoListImp>::PackL( *buddyPresenceInfoList);
        size = (iDataPackAsync->Des()).Size();
        CleanupStack::PopAndDestroy(buddyPresenceInfoList);
        }
    
    CleanupStack::PopAndDestroy(serviceName);
    iAsyncReq = NRequest::ENoRequestMade;
    iMessage.Complete(size);
    }
    

// ---------------------------------------------------------------------------
// CPresenceCacheSession::TryWriteBuddyToCache()
// ---------------------------------------------------------------------------
//
TInt CPresenceCacheSession::TryWriteBuddyToCacheL(MPresenceBuddyInfo* aBuddyPresInfo)
    {
    if(!aBuddyPresInfo)
        return KErrArgument;
    
    TPtrC serviceName = GetServiceName(aBuddyPresInfo->BuddyId()->Identity());
    TInt serviceCount = iServer.iPresenceCache.Count();
    CPresenceCacheBuddyStore* buddyStore(NULL);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName == (iServer.iPresenceCache[i])->ServiceName())
            {
            buddyStore = (iServer.iPresenceCache[i])->AddOrReplacePresenceL(aBuddyPresInfo);
            break;
            }
        }
    if(!buddyStore) // if not written
        {
        CPresenceCacheServiceStore* newServiceStore = CPresenceCacheServiceStore::NewLC();
        newServiceStore->SetServiceNameL(serviceName);
        buddyStore = newServiceStore->AddOrReplacePresenceL(aBuddyPresInfo);
        (iServer.iPresenceCache).Append(newServiceStore); //ownership transferred
        CleanupStack::Pop(newServiceStore);
        }
        
    if(buddyStore) //  inform to subscribers
        NotifyAllSubscribersL(buddyStore);
        
    return buddyStore ? KErrNone : KErrGeneral;
    }
    

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleCancel()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleCancel(const RMessage2& aMessage )
    {
    if (iActiveHelper->IsActive())
        iActiveHelper->Cancel(); //see also iActiveHelper->DoCancel()
    aMessage.Complete(KErrNotFound);
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleBuddyCountInAllServices()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleBuddyCountInAllServices(const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleBuddyCountInAllServices - begin"));
    TInt count(NULL);
    TInt serviceCount = iServer.iPresenceCache.Count();
    
    for(TInt i=0; i<serviceCount; i++)
        {
        count = (count) + ((iServer.iPresenceCache[i])->PresenceCount());
        }
    
    TPckgBuf<TInt> p(count);
    aMessage.WriteL(0,p);               
    aMessage.Complete(KErrNone);
    TRACE( _L("CPresenceCacheSession::HandleBuddyCountInAllServices - end"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleBuddyCountInService()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleBuddyCountInService(const RMessage2& aMessage )
    {
    TInt count(NULL);
    
    const TInt pckSize = aMessage.GetDesLengthL(1);
    HBufC* serviceName = HBufC16::NewLC( pckSize );
    TPtr16 paramPckPtr = serviceName->Des();
    aMessage.ReadL( 1, paramPckPtr );
    
    TInt serviceCount = iServer.iPresenceCache.Count();
    TInt err(KErrNotFound);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName->Des() == (iServer.iPresenceCache[i])->ServiceName())
            {
            count = ((iServer.iPresenceCache[i])->PresenceCount());
            err = KErrNone;
            break;
            }
        }
    
    CleanupStack::PopAndDestroy(serviceName);
    TPckgBuf<TInt> p(count);
    aMessage.WriteL(0,p);
    aMessage.Complete(err);    
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleDeleteService()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleDeleteService(const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleDeleteService - begin"));
    TInt err(KErrNotFound);
    
    const TInt pckSize = aMessage.GetDesLengthL(0);
    HBufC* serviceName = HBufC16::NewLC( pckSize );
    TPtr16 paramPckPtr = serviceName->Des();
    aMessage.ReadL( 0, paramPckPtr );
    
    TInt serviceCount = iServer.iPresenceCache.Count();
    TInt buddyCount(NULL);
    
    TRACE_1( _L("_______serviceCount[%d]"), serviceCount);
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName->Des() == (iServer.iPresenceCache[i])->ServiceName())
            {
            (iServer.iPresenceCache[i])->RemoveAllPresences();
            buddyCount = (iServer.iPresenceCache[i])->GetObjectCollection().Count();
            TRACE_1( _L("_______buddyCountAfter[%d]"), buddyCount);
            
            //if there are no buddies left in this service delete this whole service
            if(buddyCount==0)
                {
                delete iServer.iPresenceCache[i];
                iServer.iPresenceCache.Remove(i);
                }
                
            //inform all subscribers
            for(TInt j=0;j<buddyCount;j++)
                {
                NotifyAllSubscribersL((iServer.iPresenceCache[i])->GetObjectCollection()[j]);
                }
            err = KErrNone;
            break;
            }
        }
    CleanupStack::PopAndDestroy(serviceName);
    aMessage.Complete(err);
    TRACE( _L("CPresenceCacheSession::HandleDeleteService - end"));
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleDeletePresence()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleDeletePresence(const RMessage2& aMessage )
    {
    const TInt pckSize = aMessage.GetDesLengthL(0);
    HBufC8* idPack = HBufC8::NewLC( pckSize );
    TPtr8 paramPckPtr = idPack->Des();
    aMessage.ReadL( 0, paramPckPtr );
    TInt err(KErrNotFound);
    TInt buddyCount(NULL);
    
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
    
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL(*identity, *idPack);
    
    TPtrC serviceName = GetServiceName(identity->Identity());
    
    TInt serviceCount = iServer.iPresenceCache.Count();
    
    CPresenceCacheBuddyStore* buddyStore(NULL);
    for(TInt i=0; i<serviceCount; i++)
        {
        if(((iServer.iPresenceCache[i])->ServiceName()) == serviceName)
            {
            buddyStore = (iServer.iPresenceCache[i])->FindAndRemove(*identity,err);
            
            buddyCount = (iServer.iPresenceCache[i])->GetObjectCollection().Count();
            TRACE_1( _L("_______buddyCountAfter[%d]"), buddyCount);
            
            //if there are no buddies left in this service delete this whole service
            if(buddyCount==0)
                {
                delete iServer.iPresenceCache[i];
                iServer.iPresenceCache.Remove(i);
                }
            break;
            }
        }
    
    CleanupStack::PopAndDestroy(identity);
    CleanupStack::PopAndDestroy(idPack);
    aMessage.Complete(err);
    
    if(buddyStore) // if subscribers
        NotifyAllSubscribersL(buddyStore);
    }
    

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleGetServiceCount()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleGetServiceCount(const RMessage2& aMessage )
    {
    // iLastServiceCount is also used for mem aloc by client before calling
    // GetAllServices
    TInt serviceCount = iServer.iPresenceCache.Count();
    iLastServiceCount = 0;
    TRACE_1( _L("CPresenceCacheSession::HandleGetServiceCount realcount[%d]- end"),serviceCount);
    for(TInt i=0;i<serviceCount;i++)
        {
        if((iServer.iPresenceCache[i])->PresenceCount())
            iLastServiceCount++;
        }

    TPckgBuf<TInt> p(iLastServiceCount);
    aMessage.WriteL(0,p);
    aMessage.Complete(KErrNone);
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleGetAllServicesL()
// ---------------------------------------------------------------------------
//
/*
void CPresenceCacheSession::HandleGetAllServicesL(const RMessage2& aMessage )   
    {
    TInt serviceCount = iServer.iPresenceCache.Count();
    TInt size(NULL);
    
    if(serviceCount)
    	{
        CArrayFixFlat<TUid>* services = new (ELeave) CArrayFixFlat<TUid>(1);
        CleanupStack::PushL(services);
    	
    	for(TInt i=0; i<serviceCount; i++)
	        {
	        if((iServer.iPresenceCache[i])->PresenceCount())
	            services->AppendL((iServer.iPresenceCache[i])->Service());
	        }
	    
	    if (iDataPack)
	        {
	        delete iDataPack; iDataPack = NULL;
	        }
	    iDataPack = PackServicesL(services);
	    CleanupStack::PopAndDestroy(services);
	    size = (iDataPack->Des()).Size();
    	}
    aMessage.Complete(size);
    }*/
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleWritePresenceInfoAsyncL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleWritePresenceInfoAsyncL(const RMessage2& aMessage )
    {
    if(iActiveHelper->IsActive())
        {
        aMessage.Complete(KErrServerBusy);
        return;        
        }
    iMessage = aMessage;
    
    const TInt pckSize = aMessage.GetDesLengthL(0);
    if(iDataPackAsync)
        delete iDataPackAsync; iDataPackAsync = NULL;
    iDataPackAsync = HBufC8::NewL( pckSize );
    TPtr8 paramPckPtr = iDataPackAsync->Des();
    aMessage.ReadL( 0, paramPckPtr );
    
    if(iBuddypresInfoList)
        delete iBuddypresInfoList; iBuddypresInfoList = NULL;
    iBuddypresInfoList = CPresenceBuddyInfoListImp::NewLC(KNullDesC);
    CleanupStack::Pop(iBuddypresInfoList);
    
    TXIMPObjectPacker< CPresenceBuddyInfoListImp >::UnPackL(*iBuddypresInfoList, *iDataPackAsync);
    
    iAsyncReq = NRequest::EWritePresenceInfoAsync;
    iActiveHelper->Start();
    return;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::WritePresenceInfoAsyncL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::WritePresenceInfoAsyncL()
    {
    TInt err(KErrNone);
    TInt thisTimeWrites(0);
    if(!iBuddypresInfoList) // return if this function is wrongly called
        return;
    
    // This routine reads data from iBuddypresInfoList and tries to write 
    // data to cache on each runl cycle. It writes either KMaxWriteEachCycle 
    // times, or upto data is finnished. After data is written KMaxWriteEachCycle
    // times and still more data is left, it makes this class active so that this
    // function is again called on next RunL.
    while(iBuddypresInfoList->Count())
        {
        err = TryWriteBuddyToCacheL((iBuddypresInfoList->GetObjectCollection())[0]);
        if(err==KErrNone) // if no erroe then pop the object from list
            (iBuddypresInfoList->GetObjectCollection()).Remove(0);
        else
            break; // break in case of error
        thisTimeWrites++;    
        if(thisTimeWrites == NConstants::KMaxWriteEachCycle) // this times writes are over
            {
            if(iActiveHelper->IsActive())// unusual condition
                {
                err = KErrGeneral;
                break;
                }
            iActiveHelper->Start();
            return; // set for next time writing.
            }
       }
    // if we able to come out of while loop either writing is complete or
    // there was some error while writing, in both cases finnish writing
    iAsyncReq = NRequest::ENoRequestMade;
    iMessage.Complete(err);
    return;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleSubscribeBuddyPresenceChange()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleSubscribeBuddyPresenceChange
                                                (const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleSubscribeBuddyPresenceChange - begin"));

    // Extract info from message
    const TInt pckSize = aMessage.GetDesLengthL(1);
    HBufC8* idPack = HBufC8::NewLC( pckSize );
    TPtr8 paramPckPtr = idPack->Des();
    aMessage.ReadL( 1, paramPckPtr );
    
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
    
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL(*identity, *idPack);
    
    TPtrC serviceName = GetServiceName(identity->Identity());
    
    CPresenceCacheBuddyStore* buddyStore(NULL);
    CPresenceCacheServiceStore* serviceStore(NULL);
    
    TInt err(KErrGeneral); // there must not be error in this handler
                           // but just for debug purpose
    
    
    // now processing
    TInt serviceCount = iServer.iPresenceCache.Count();

    
    TRACE_1( _L("_______serviceCount[%d]"), serviceCount);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName == (iServer.iPresenceCache[i])->ServiceName())
            {
            serviceStore = iServer.iPresenceCache[i];
            buddyStore = (iServer.iPresenceCache[i])->FindAndGet(*identity);
            break;
            }
        }
        
    if(!buddyStore) // if buddy was not found, create and add it
        {
        if(!serviceStore) //if service was also not found, create it
            {
            serviceStore = CPresenceCacheServiceStore::NewLC();
            serviceStore->SetServiceNameL(serviceName);
            (iServer.iPresenceCache).Append(serviceStore); //ownership transferred
            CleanupStack::Pop(serviceStore);  
            }
        buddyStore = CPresenceCacheBuddyStore::NewLC(serviceStore,identity);
        if(buddyStore)
        	{
        	err = serviceStore->AddBlind(buddyStore); //ownership transferred
        	CleanupStack::Pop(buddyStore);
        	}
        CleanupStack::Pop(identity);
        } 
    else
        CleanupStack::PopAndDestroy(identity); // since in this case ownership wasnt tranferred
    
    CleanupStack::PopAndDestroy(idPack);
    
    TRACE_1( _L("buddyStore[%d]"), buddyStore);
    if(buddyStore) // must be there, just for double check
        {
        buddyStore->AddSubscribedSession(this);
        if(iSubscribedStores.Find(buddyStore) < 0) //if session not already there
            iSubscribedStores.Append(buddyStore);
        err = KErrNone;
        }
    
    aMessage.Complete(err); // there shouldnt be any errors
    TRACE( _L("CPresenceCacheSession::HandleSubscribeBuddyPresenceChange - end"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleUnSubscribeBuddyPresenceChange()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleUnSubscribeBuddyPresenceChange
                                                (const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleUnSubscribeBuddyPresenceChange - begin"));

    // Extract info from message
    const TInt pckSize = aMessage.GetDesLengthL(1);
    HBufC8* idPack = HBufC8::NewLC( pckSize );
    TPtr8 paramPckPtr = idPack->Des();
    aMessage.ReadL( 1, paramPckPtr );
    TInt err(KErrNotFound);
    
    CXIMPIdentityImp* identity = CXIMPIdentityImp::NewLC();
    
    TXIMPObjectPacker< CXIMPIdentityImp >::UnPackL(*identity, *idPack);
    
    TPtrC serviceName = GetServiceName(identity->Identity());
    
    
    // now processing
    TInt serviceCount = iServer.iPresenceCache.Count();
    CPresenceCacheBuddyStore* buddyStore(NULL);
    
    TRACE_1( _L("_______serviceCount[%d]"), serviceCount);
    
    for(TInt i=0; i<serviceCount; i++)
        {
        if (serviceName == (iServer.iPresenceCache[i])->ServiceName())
            {
            buddyStore = (iServer.iPresenceCache[i])->FindAndGet(*identity);
            break;
            }
        }
    
    CleanupStack::PopAndDestroy(identity);
    CleanupStack::PopAndDestroy(idPack);
    
    TRACE_1( _L("buddyStore[%d]"), buddyStore);
    if(buddyStore) // if found
        {
        buddyStore->RemoveSubscribedSession(this);
        iSubscribedStores.Remove(iSubscribedStores.Find(buddyStore));
        err = KErrNone;
        }
    
    aMessage.Complete(err);
    TRACE_1( _L("CPresenceCacheSession::HandleUnSubscribeBuddyPresenceChange (%d) - end"),err);
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleGetLastNotifiedtPacketL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleGetLastNotifiedtPacketL(const RMessage2& aMessage )
    {
    TRACE( _L("CPresenceCacheSession::HandleGetLastNotifiedtPacketL() - begin"));
    if(iDataPackNotifier)
        aMessage.WriteL(0, *(iDataPackNotifier));
    aMessage.Complete(KErrNone);
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleWaitingForNotification()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleWaitingForNotification(const RMessage2& aMessage )
    {
    iMessageForNoti = aMessage;
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::HandleCancelWaitingForNotification()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::HandleCancelWaitingForNotification
                                                    (const RMessage2& aMessage )
    {
    aMessage.Complete(KErrNone);
    if(!iMessageForNoti.IsNull())
        iMessageForNoti.Complete(KErrCancel);
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::NotifyPresenceChangeL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::NotifyPresenceChangeL(
                               const CPresenceBuddyInfoImp* aPresenceBuddyInfo)
    {
    TRACE( _L("CPresenceCacheSession::NotifyPresenceChangeL() - begin"));
    TInt sizeOfReturnPacket(NULL);
    if(aPresenceBuddyInfo && (!iMessageForNoti.IsNull())) // if pointers are valid
        {
        if(iDataPackNotifier)
            delete iDataPackNotifier; iDataPackNotifier=NULL;
        iDataPackNotifier = PackBuddyPresenceInfoLC(aPresenceBuddyInfo);
        CleanupStack::Pop(iDataPackNotifier);
        sizeOfReturnPacket = (iDataPackNotifier->Des()).Size();
        TPckgBuf<TInt> p(sizeOfReturnPacket);
        iMessageForNoti.WriteL(0,p);            
        iMessageForNoti.Complete(KErrNone);
        TRACE( _L("_______message was valid"));
        }
    TRACE( _L("CPresenceCacheSession::NotifyPresenceChangeL - end"));
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::NotifyAllSubscribersL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::NotifyAllSubscribersL
                                        (CPresenceCacheBuddyStore* aBuddyStore)
    {
    TRACE( _L("CPresenceCacheSession::NotifyAllSubscribers() - begin"));
    if(!aBuddyStore)
        return;
    TInt subsCount = aBuddyStore->GetSubscribedSessions().Count();
    TRACE_1( _L("_______total subscribed sessions[%d]"), subsCount);
    TBool ownBuddyInfo(EFalse);
    const CPresenceBuddyInfoImp* buddyInfo;
    
    if(subsCount) // start processing if there are subscribers
        {
        //if presence has been deleted we need to identity and service id in the packed
        //which will be sent to clients
        if(!(aBuddyStore->PresenceBuddyInfo()))
            {
            CPresenceBuddyInfoImp* buddyInfoTemp = CPresenceBuddyInfoImp::NewLC();
            MXIMPIdentity* id = CXIMPIdentityImp::NewLC(aBuddyStore->BuddyId()->Identity());
            buddyInfoTemp->SetBuddyId(id);
            CleanupStack::Pop(1); //id
            ownBuddyInfo = ETrue;
            buddyInfo = buddyInfoTemp;
            }
        else
            {
            buddyInfo = 
                TXIMPGetImpClassOrPanic< const CPresenceBuddyInfoImp >::From( *(aBuddyStore->PresenceBuddyInfo()) );    
            }
            
        CPresenceCacheSession* session(NULL);
        for(TInt i=0;i<subsCount;i++)
            {
            session = aBuddyStore->GetSubscribedSessions()[i];
            if(iServer.SessionExists(session))
                {
                TRACE_1( _L("_______valid session notified[%d]"), session);
                session->NotifyPresenceChangeL(buddyInfo);
                }
            else
                {
                aBuddyStore->RemoveSubscribedSession(session);
                subsCount--;//substract this removed session from total count
                i--;
                TRACE_1( _L("_______invalid session removed[%d]"), session);
                }    
            }
        }
        
    if(ownBuddyInfo)
        CleanupStack::PopAndDestroy(1); //buddyInfoTemp
    TRACE( _L("CPresenceCacheSession::NotifyAllSubscribers() - end"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::GetServiceName()
// ---------------------------------------------------------------------------
//
TPtrC CPresenceCacheSession::GetServiceName(const TDesC& aXspId)
    {
    _LIT(KColon, ":");
    TInt pos = aXspId.Find(KColon);
    TPtrC serviceName;
    if(pos>0) // if colon found and there is something before colon, i.e. xsp id
        {
        serviceName.Set(aXspId.Left(pos));
        }
    else
        serviceName.Set(TPtrC());
    return serviceName;
    }
        
// ---------------------------------------------------------------------------
// CPresenceCacheSession::Cancel()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::Cancel()
    {
    if (iAsyncReq != NRequest::ENoRequestMade)
        {
        iMessage.Complete(KErrCancel);
        iAsyncReq = NRequest::ENoRequestMade;
        }
    return;   
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::PackPresenceDocLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceCacheSession::PackPresenceInfoLC(const MPresenceInfo& aPresInfo)
    {
    const CPresenceInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoImp >::From(aPresInfo);
    HBufC8* pack = TXIMPObjectPacker< CPresenceInfoImp>::PackL( *tmp);
    CleanupStack::PushL( pack );

    return pack;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::PackBuddyPresenceInfoLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresenceCacheSession::PackBuddyPresenceInfoLC(
                                    const CPresenceBuddyInfoImp* aBuddyPresInfo)
    {
    HBufC8* pack(NULL);
    if(aBuddyPresInfo)
        {
        //const CPresenceBuddyInfoImp* pifImp = 
        //    TXIMPGetImpClassOrPanic< const CPresenceBuddyInfoImp >::From( *aBuddyPresInfo );    
        
        pack = TXIMPObjectPacker< const CPresenceBuddyInfoImp>::PackL( *aBuddyPresInfo );
        CleanupStack::PushL( pack );
        }

    return pack;
    }
    

// ---------------------------------------------------------------------------
// CPresenceCacheSession::PackServicesL()
// ---------------------------------------------------------------------------
//
/*
HBufC8* CPresenceCacheSession::PackServicesL(CArrayFixFlat<TUid>* aArray )
	{
    CBufFlat* packBuf = CBufFlat::NewL( NConstants::KGranularity );
    CleanupStack::PushL( packBuf );

    RBufWriteStream ws;
    ws.Open( *packBuf ); // CSI: 65 #
    CleanupClosePushL( ws );

    // Get count of objects
    TInt objCount = aArray->Count();
    // write the count
    ws.WriteInt32L( objCount );
    // objects
    for ( TInt count(0); count < objCount; count++ )
        {
        ws<<(aArray->At(count));
        }

    ws.CommitL();
    CleanupStack::PopAndDestroy(); //ws

    HBufC8* packBufDesc = packBuf->Ptr(0).AllocL();
    CleanupStack::PopAndDestroy( packBuf );

    return packBufDesc;
	}*/

// ---------------------------------------------------------------------------
// CPresenceCacheSession::CPresenceCacheSession()
// ---------------------------------------------------------------------------
//
CPresenceCacheSession::CPresenceCacheSession( CPresenceCacheServer& aServer )
                                                        : iServer( aServer ),
                                                        iLastServiceCount(0)
    {
    TRACE( _L("CPresenceCacheSession::CPresenceCacheSession()"));
    }

// ---------------------------------------------------------------------------
// CPresenceCacheSession::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::ConstructL()
    {
    iActiveHelper = CCacheSessionActiveHelper::NewL(this);    
    iServer.IncrementSessions();
    return;
    }
    
// ---------------------------------------------------------------------------
// CPresenceCacheSession::RemoveMySubscriptions()
// ---------------------------------------------------------------------------
//
void CPresenceCacheSession::RemoveMySubscriptions()
    {
    TInt count = iSubscribedStores.Count();
    for(TInt i=0;i<count;i++)
        {
        // it can be assumed that store is alive, since it won't be deleted
        // until there is any session subscribed to it
        (iSubscribedStores[i])->RemoveSubscribedSession(this);
        }
    }
    
    
// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::NewL()
// ---------------------------------------------------------------------------
//
CCacheSessionActiveHelper* CCacheSessionActiveHelper::NewL
                                            (CPresenceCacheSession* aSession )
    {
    CCacheSessionActiveHelper* self = new(ELeave) CCacheSessionActiveHelper(aSession);
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::~CCacheSessionActiveHelper()
// ---------------------------------------------------------------------------
//
CCacheSessionActiveHelper::~CCacheSessionActiveHelper( )
    {
    Cancel();   
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::Start()
// ---------------------------------------------------------------------------
//
void CCacheSessionActiveHelper::Start()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status,KErrNone);
    SetActive();
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::CCacheSessionActiveHelper()
// ---------------------------------------------------------------------------
//
CCacheSessionActiveHelper::CCacheSessionActiveHelper(CPresenceCacheSession* aSession ) 
                                                    : CActive(EPriorityStandard)
    {
    iCacheSession = aSession;
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::RunL()
// ---------------------------------------------------------------------------
//
void CCacheSessionActiveHelper::RunL()
    {
    switch (iCacheSession->iAsyncReq)
        {
        case NRequest::EWritePresenceInfoAsync:
            iCacheSession->WritePresenceInfoAsyncL();
            break;

        case NRequest::EPrepReadAllBuddiesPresenceInService:
            iCacheSession->ReadAllBuddiesPresenceInServiceL();
            break;

        case NRequest::ENoRequestMade:
        default:
            break;
        }    
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::DoCancel()
// ---------------------------------------------------------------------------
//
void CCacheSessionActiveHelper::DoCancel()
    {
    iCacheSession->Cancel();
    }

// ---------------------------------------------------------------------------
// CCacheSessionActiveHelper::ConstructL()
// ---------------------------------------------------------------------------
//
void CCacheSessionActiveHelper::ConstructL()
    {
	CActiveScheduler::Add(this);
    }


// End of File
