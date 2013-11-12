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
* Description:  CPresenceBuddyInfoImp object implementation.
*
*/


#include <s32buf.h>
#include <ximpidentity.h>
#include <presenceinfo.h>
#include <personpresenceinfo.h>

#include "ximpidentityimp.h"
#include "presencebuddyinfoimp.h"
#include "presenceinfoimp.h"
#include "personpresenceinfoimp.h"


// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBuddyInfoImp* CPresenceBuddyInfoImp::NewLC()
    {
    CPresenceBuddyInfoImp* self = new( ELeave ) CPresenceBuddyInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceBuddyInfoImp* CPresenceBuddyInfoImp::NewL()
    {
    CPresenceBuddyInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::~CPresenceBuddyInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceBuddyInfoImp::~CPresenceBuddyInfoImp()
    {
    if (iPresenceInfo)
        delete iPresenceInfo;
    if(iIdentity)
        delete iIdentity;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::CPresenceBuddyInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceBuddyInfoImp::CPresenceBuddyInfoImp()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceBuddyInfoImp::ConstructL()
    {
    iPresenceInfo = CPresenceInfoImp::NewL();
    iIdentity = CXIMPIdentityImp::NewL();
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::Set()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoImp::Set( MXIMPIdentity* aIdentity,
                                 MPresenceInfo* aPresenceInfo)
    {
    TInt err(KErrArgument);
    
    if(aIdentity && aPresenceInfo) // Check whether data is valid
        {
        delete iIdentity;
        iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *aIdentity );
        
        delete iPresenceInfo;
        iPresenceInfo = NULL;
        iPresenceInfo = 
            TXIMPGetImpClassOrPanic< CPresenceInfoImp >::From( *aPresenceInfo );
                
        err = KErrNone;
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::SetPresenceInfo()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoImp::SetPresenceInfo(MPresenceInfo* aPresenceInfo)
    {
    TInt err(KErrArgument);
    if(aPresenceInfo)
        {
        delete iPresenceInfo;
        iPresenceInfo = NULL;
        iPresenceInfo = 
            TXIMPGetImpClassOrPanic< CPresenceInfoImp >::From( *aPresenceInfo );
        err = KErrNone;
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::SetBuddyId()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoImp::SetBuddyId(MXIMPIdentity* aIdentity)
    {
    TInt err(KErrArgument);
    
    if(aIdentity)
        {
    delete iIdentity;
        iIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp >::From( *aIdentity );
    err = KErrNone;
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::SetPersonPresenceL()
// ---------------------------------------------------------------------------
//
TInt CPresenceBuddyInfoImp::SetPersonPresenceL(
                        MPersonPresenceInfo* aPersonPresenceInfo)
    {
    TInt err(KErrArgument);
    if(aPersonPresenceInfo)
        {
        iPresenceInfo->SetPersonPresenceL(aPersonPresenceInfo);
        err = KErrNone;
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::PresenceInfo()
// ---------------------------------------------------------------------------
//
const MPresenceInfo* CPresenceBuddyInfoImp::PresenceInfo() const
    {
    return iPresenceInfo;        
    }

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::BuddyUserId()
// ---------------------------------------------------------------------------
//
const MXIMPIdentity* CPresenceBuddyInfoImp::BuddyId() const
    {
    return iIdentity;        
    }
        
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::EqualsIdentity()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceBuddyInfoImp::EqualsIdentity(
                            const MPresenceBuddyInfo* aOtherInstance ) const
    {
    TBool ret(EFalse);
    if(aOtherInstance)
        {
        const CXIMPIdentityImp* tmp = 
            TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( *(aOtherInstance->BuddyId()));
        ret = iIdentity->EqualsContent( *tmp);
        }
    return ret;
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceBuddyInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceBuddyInfoImp, 
                                     MPresenceBuddyInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceBuddyInfoImp, 
                                           MPresenceBuddyInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceBuddyInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceBuddyInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceBuddyInfoImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x = iPresenceInfo->EqualsContent( *tmp->iPresenceInfo );
    same &= x;
    
    x = iIdentity->EqualsContent( *tmp->iIdentity );
    same &= x;
    
    return same;
    }
    
// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBuddyInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iPresenceInfo->ExternalizeL( aStream );
    iIdentity->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CPresenceBuddyInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceBuddyInfoImp::InternalizeL( RReadStream& aStream )
    {
    CPresenceInfoImp* tmp = CPresenceInfoImp::NewLC();
    tmp->InternalizeL( aStream );
    if (iPresenceInfo)
        delete iPresenceInfo;
    iPresenceInfo = NULL;
    iPresenceInfo = tmp;
    CleanupStack::Pop( tmp );
    
    CXIMPIdentityImp* tmp1 = CXIMPIdentityImp::NewLC();
    tmp1->InternalizeL( aStream );
    if (iIdentity)
        delete iIdentity;
    iIdentity = tmp1;
    CleanupStack::Pop( tmp1 );
    

    }
