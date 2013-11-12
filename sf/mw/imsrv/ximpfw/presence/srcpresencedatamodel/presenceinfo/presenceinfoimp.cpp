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
* Description:  MPresenceDocument API object implementation.
*
*/

#include "presenceinfoimp.h"
#include "personpresenceinfoimp.h"
#include "servicepresenceinfoimp.h"
#include "devicepresenceinfoimp.h"
#include "presenceapidataobjfactory.h"
#include "presencetypehelpers.h"
#include "ximpobjecthelpers.h"
#include "presenceinfofilterimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoImp* CPresenceInfoImp::NewLC()
    {
    CPresenceInfoImp* self = new( ELeave ) CPresenceInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoImp* CPresenceInfoImp::NewL()
    {
    CPresenceInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceInfoImp::~CPresenceInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoImp::~CPresenceInfoImp()
    {
    delete iPersonPresence;
    iServicePresences.ResetAndDestroy();
    iDevicePresences.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CPresenceInfoImp::CPresenceInfoImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoImp::CPresenceInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoImp::ConstructL()
    {
    iPersonPresence = CPersonPresenceInfoImp::NewLC();
    CleanupStack::Pop( iPersonPresence );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoImp )


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoImp, 
                                     MPresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoImp, 
                                           MPresenceInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoImp >::From( aOtherInstance.Base() );

    TBool same = ETrue;
    TBool x = iPersonPresence->EqualsContent( *tmp->iPersonPresence );
    same &= x;
    
    x = TXIMPEqualsContent< CServicePresenceInfoImp >::Array( iServicePresences, tmp->iServicePresences );
    same &= x;

    x = TXIMPEqualsContent< CDevicePresenceInfoImp >::Array( iDevicePresences, tmp->iDevicePresences );
    same &= x;
    
    return same;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iPersonPresence->ExternalizeL( aStream );

    CPresenceApiDataObjFactory::ExternalizeL< CServicePresenceInfoImp >
        ( aStream, iServicePresences );

    CPresenceApiDataObjFactory::ExternalizeL< CDevicePresenceInfoImp >
        ( aStream, iDevicePresences );
    }


// ---------------------------------------------------------------------------
// CPresenceInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoImp::InternalizeL( RReadStream& aStream )
    {
    CPersonPresenceInfoImp* tmp = CPersonPresenceInfoImp::NewLC();
    tmp->InternalizeL( aStream );
    delete iPersonPresence;
    iPersonPresence = tmp;
    CleanupStack::Pop( tmp );

    iServicePresences.ResetAndDestroy();
    CPresenceApiDataObjFactory::InternalizeL< CServicePresenceInfoImp >
        ( aStream, iServicePresences );
    
    iDevicePresences.ResetAndDestroy();
    CPresenceApiDataObjFactory::InternalizeL< CDevicePresenceInfoImp >
        ( aStream, iDevicePresences );
    }
    

// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::PersonPresence()
// ---------------------------------------------------------------------------
//
const MPersonPresenceInfo* CPresenceInfoImp::PersonPresence() const
    {
    return iPersonPresence;
    }
    
// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::PersonPresence()
// ---------------------------------------------------------------------------
//
MPersonPresenceInfo* CPresenceInfoImp::PersonPresence()
    {
    return iPersonPresence;
    }    


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::SetPersonPresenceL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoImp::SetPersonPresenceL( 
    MPersonPresenceInfo* aPersonPresence )
    {
    User::LeaveIfError( aPersonPresence == NULL ? KErrArgument : KErrNone );

    CPersonPresenceInfoImp* personPresenceImp =
    TXIMPGetImpClassOrPanic< CPersonPresenceInfoImp >::From( *aPersonPresence );
    
    delete iPersonPresence;
    iPersonPresence = personPresenceImp;
    }


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::ServicePresenceCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoImp::ServicePresenceCount() const
    {
    return iServicePresences.Count();
    }
    

// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::ServicePresenceAt()
// ---------------------------------------------------------------------------
//
const MServicePresenceInfo& CPresenceInfoImp::ServicePresenceAt(
    TInt aIndex ) const
    {
    return *iServicePresences[ aIndex ];    
    }


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::LookupServicePresenceByServiceType()
// ---------------------------------------------------------------------------
//
void CPresenceInfoImp::LookupServicePresenceByServiceType( 
    MServicePresenceInfo*& aServicePresence,
    const TDesC8& aMatch ) const
    {
    TInt start = KErrNotFound;

    // search from start
    if ( aServicePresence )
        {
        TBool found = EFalse;
        for ( TInt i = 0; ( i < iServicePresences.Count() ) && ( ! found ); i++ )
            {
            CXIMPApiDataObjBase* gotObj = TXIMPGetImpClassOrPanic< CXIMPApiDataObjBase >::From( *aServicePresence );
            CXIMPApiDataObjBase* obj = iServicePresences[ i ];
            if ( &(gotObj->Base()) == &(obj->Base()) )
                {
                found = ETrue;
                start = i;
                }
            }
        }

    start = start == KErrNotFound ? 0 : start;
    aServicePresence = NULL;

    for ( TInt i = start; i < iServicePresences.Count(); i++ )
        {
        CServicePresenceInfoImp* srvInfo = iServicePresences[ i ];

        if ( KErrNotFound != srvInfo->ServiceType().MatchF( aMatch ) )
            {
            // found match, return it
            aServicePresence = srvInfo;
            return;
            }
        }
    }


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::AddServicePresenceL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoImp::AddServicePresenceL( 
    MServicePresenceInfo* aServicePresence )
    {
    User::LeaveIfError( aServicePresence == NULL ? KErrArgument : KErrNone );
    
    CServicePresenceInfoImp* servicePresenceImp =
    TXIMPGetImpClassOrPanic< CServicePresenceInfoImp >::From( *aServicePresence );

    iServicePresences.AppendL( servicePresenceImp );
    }


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::DevicePresenceCount()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoImp::DevicePresenceCount() const
    {
    return iDevicePresences.Count();
    }


// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::DevicePresenceAt()
// ---------------------------------------------------------------------------
//
const MDevicePresenceInfo& CPresenceInfoImp::DevicePresenceAt(
    TInt aIndex ) const
    {
    return *iDevicePresences[ aIndex ];
    }
        

// ---------------------------------------------------------------------------
// From MPresenceInfo class.
// CPresenceInfoImp::AddDevicePresenceL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoImp::AddDevicePresenceL( 
    MDevicePresenceInfo* aDevicePresence )
    {
    User::LeaveIfError( aDevicePresence == NULL ? KErrArgument : KErrNone );

    CDevicePresenceInfoImp* devicePresenceImp =
    TXIMPGetImpClassOrPanic< CDevicePresenceInfoImp >::From( *aDevicePresence );

    iDevicePresences.AppendL( devicePresenceImp );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoImp::FilteredInfoLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoImp* CPresenceInfoImp::FilteredInfoLC(
        const CPresenceInfoFilterImp& aFilter ) const
    {
    CPresenceInfoImp* info = TXIMPObjectCloner< CPresenceInfoImp >::CloneLC( *this );

    // the info imps know how to filter themselves
    info->iPersonPresence->FilterWithL( aFilter );

    for ( TInt i = 0; i < iServicePresences.Count(); i++ )
        {
        info->iServicePresences[ i ]->FilterWithL( aFilter );
        }
    
    for ( TInt i = 0; i < iDevicePresences.Count(); i++ )
        {
        info->iDevicePresences[ i ]->FilterWithL( aFilter );
        }

    return info;
    }


// End of file

