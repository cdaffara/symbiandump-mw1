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
* Description:  CPresenceBuddyInfoListImp object implementation.
*
*/


#include <s32strm.h>
#include <ximpidentity.h>
#include <presencebuddyinfo.h>

#include "presencebuddyinfolistimp.h"
#include "presencebuddyinfoimp.h"
#include "ximpidentityimp.h"
#include "ximptrace.h"
#include "ximprbufhelpers.h"

_LIT(KColon, ":");

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBuddyInfoListImp* CPresenceBuddyInfoListImp::NewLC(
                                                    const TDesC& aServiceName)
    {
    CPresenceBuddyInfoListImp* self = new( ELeave ) CPresenceBuddyInfoListImp;
    CleanupStack::PushL( self );
    self->ConstructL(aServiceName);
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBuddyInfoListImp* CPresenceBuddyInfoListImp::NewL(
                                                    const TDesC& aServiceName)
    {
    CPresenceBuddyInfoListImp* self = NewLC(aServiceName);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::~CPresenceBuddyInfoListImp()
// ---------------------------------------------------------------------------
//
CPresenceBuddyInfoListImp::~CPresenceBuddyInfoListImp()
    {
    if(iOwnDataObjects)
        {
        TInt count = iBuddyCollection.Count();
        for (TInt i=0;i<count;i++)
            {
            delete iBuddyCollection[i];
            }
        }
    iBuddyCollection.Close();
    iService.Close();    
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::CPresenceBuddyInfoListImp()
// ---------------------------------------------------------------------------
//
CPresenceBuddyInfoListImp::CPresenceBuddyInfoListImp() :iOwnDataObjects(ETrue)
    {
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBuddyInfoListImp::ConstructL(const TDesC& aServiceName)
    {
    iService.Close();
    iService.CreateL( aServiceName );  
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBuddyInfoListImp::ConstructL()
    {
    }    

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::SetOwnObjects()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBuddyInfoListImp::SetOwnObjects(TBool aOwnObjects)
    {
    iOwnDataObjects = aOwnObjects;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::PresenceInfo()
// ---------------------------------------------------------------------------
//
RPointerArray<MPresenceBuddyInfo>& CPresenceBuddyInfoListImp::GetObjectCollection()
    {
    return iBuddyCollection;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::ServiceName()
// ---------------------------------------------------------------------------
//
TPtrC CPresenceBuddyInfoListImp::ServiceName() const
    {
    return iService;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::Count()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoListImp::Count() const
    {
    return iBuddyCollection.Count();
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::AddOrReplace()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoListImp::AddOrReplace(
    MPresenceBuddyInfo* aPresenceBuddyInfo)
    {
    TRACE( _L("CPresenceBuddyInfoListImp::AddOrReplace - begin") );
    if(aPresenceBuddyInfo)
    	TRACE_1( _L("______identity: %S"), &(aPresenceBuddyInfo->BuddyId()->Identity()));        
    else
    	return KErrArgument;
    
    if ( (ServiceName() != KNullDesC) && 
        (ExtractService( aPresenceBuddyInfo->BuddyId()->Identity()).Compare( ServiceName()) ))
        {
        return KErrArgument;
        }        
    TInt count = iBuddyCollection.Count();
    for(TInt i=0;i<count;i++)
        { 
        if( ((iBuddyCollection[i])->EqualsIdentity(aPresenceBuddyInfo)))
            {
            delete iBuddyCollection[i];
            iBuddyCollection.Remove(i);
            TRACE( _L("_________Match found") );
            break;
            }
        }
    iBuddyCollection.Append(aPresenceBuddyInfo);
    TRACE( _L("CPresenceBuddyInfoListImp::AddOrReplace - return"));
    return KErrNone;
    }    

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::FindAndRemove()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoListImp::FindAndRemove(const MXIMPIdentity& aIdentity)
    {
    TRACE( _L("CPresenceBuddyInfoListImp::FindAndRemove - begin") );
    TRACE_1( _L("______identity: %S"), &(aIdentity.Identity()));
    TInt err(KErrNotFound);
    TInt count = iBuddyCollection.Count();
    for(TInt i=0;i<count;i++)
        {
        if( ((iBuddyCollection[i])->BuddyId()->Identity())== aIdentity.Identity() )
            {
            delete iBuddyCollection[i];
            iBuddyCollection.Remove(i);
            err = KErrNone;
            break;
            }
        }
    TRACE_1( _L("CPresenceBuddyInfoListImp::FindAndRemove - err=%d"),err );
    return err;
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::FindAndGet()
// ---------------------------------------------------------------------------
//
MPresenceBuddyInfo* CPresenceBuddyInfoListImp::FindAndGet(
                                     const MXIMPIdentity& aIdentity) const
    {
    TRACE( _L("CPresenceBuddyInfoListImp::FindAndGet - begin") );
    TRACE_1( _L("______identity: %S"), &(aIdentity.Identity()));
    MPresenceBuddyInfo* buddyPresInfo(NULL);
    
        TInt count = iBuddyCollection.Count();
        for(TInt i=0;i<count;i++)
            {
        if( ((iBuddyCollection[i])->BuddyId()->Identity())== aIdentity.Identity() )
                {
                buddyPresInfo = iBuddyCollection[i];
                break;
                }
            }
    TRACE_1( _L("CPresenceBuddyInfoListImp::FindAndGet - buddyPresInfo=%d"),buddyPresInfo );    
    return buddyPresInfo;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceBuddyInfoListImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceBuddyInfoListImp, 
                                     MPresenceBuddyInfoList )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceBuddyInfoListImp, 
                                           MPresenceBuddyInfoList )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBuddyInfoListImp::ExternalizeL( RWriteStream& aStream ) const
    {
    TUint32 count = iBuddyCollection.Count();
    CPresenceBuddyInfoImp* tmp(NULL);
    
    aStream.WriteInt32L(count);
    for (TInt i=0;i<count;i++)
        {
        tmp = TXIMPGetImpClassOrPanic< CPresenceBuddyInfoImp >::From( *(iBuddyCollection[i]));
        tmp->ExternalizeL( aStream );
        }
    
    XIMPRBuf16Helper::ExternalizeL( iService, aStream );    
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBuddyInfoListImp::InternalizeL( RReadStream& aStream )
    {
    TUint32 internalizeCount = aStream.ReadInt32L();
    CPresenceBuddyInfoImp* tmp(NULL);
    for (TInt i=0;i<internalizeCount;i++)
        {
        tmp = CPresenceBuddyInfoImp::NewLC();
        tmp->InternalizeL( aStream );
        iBuddyCollection.Append(tmp);
        CleanupStack::Pop( tmp );
        }
    XIMPRBuf16Helper::InternalizeL( iService, aStream );
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceBuddyInfoListImp::EqualsContent( 
    const CXIMPApiDataObjBase& /*aOtherInstance*/ ) const
    {
    //Not implemented
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::BlindAddL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPresenceBuddyInfoListImp::BlindAddL(
                                    MPresenceBuddyInfo* aPresenceBuddyInfo)
    {
    if(!aPresenceBuddyInfo)
        return KErrArgument;
    iBuddyCollection.Append(aPresenceBuddyInfo);
    return KErrNone;
    }
    
// --------------------------------------------------------------------------
// CPresenceBuddyInfoListImp::ExtractService
// --------------------------------------------------------------------------
//
TPtrC CPresenceBuddyInfoListImp::ExtractService( const TDesC& aXspId )
    {         
    TInt pos = aXspId.Find(KColon);
    if ( pos >= 0)
        {
        // ok input
        return aXspId.Left(pos);
        }
    else
        {
        // return something in illegal input case
        return aXspId;
        }
    }    
    
// eof
