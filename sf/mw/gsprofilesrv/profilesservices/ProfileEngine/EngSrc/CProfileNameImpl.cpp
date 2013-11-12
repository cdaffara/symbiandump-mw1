/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfileNameImpl.
*
*/



// INCLUDE FILES
#include "CProfileNameImpl.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfileEngUtils.h"
#include <s32strm.h>
#include <MProfilesNamesArray.h>
#include <ProfileEngineConstants.h>
#include <centralrepository.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileNameImpl::CProfileNameImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileNameImpl::CProfileNameImpl(
    TInt aProfileId )
    : iId( aProfileId )
    {
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::ConstructL(
    const TDesC& aProfileName )
    {
    iProfileName = aProfileName.AllocL();
    iShortName = NULL;
    iNaviName = NULL;
    iTitleName = NULL;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::ConstructL(
    const CProfileNameImpl& aProfileNameImpl )
    {
    iId = aProfileNameImpl.iId;
    iProfileName = aProfileNameImpl.iProfileName->AllocL();
    if( aProfileNameImpl.iShortName &&
        aProfileNameImpl.iNaviName &&
        aProfileNameImpl.iTitleName )
        {
        iShortName = aProfileNameImpl.iShortName->AllocL(); 
        iNaviName = aProfileNameImpl.iNaviName->AllocL(); 
        iTitleName = aProfileNameImpl.iTitleName->AllocL(); 
        }
    else
        {
        iShortName = NULL;
        iNaviName = NULL;
        iTitleName = NULL;
        iNameChanged = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::ConstructL(
    const TDesC& aProfileName,
    const TDesC& aShortName,
    const TDesC& aNaviName,
    const TDesC& aTitleName )
    {
    iProfileName = aProfileName.AllocL();
    iShortName = aShortName.AllocL();
    iNaviName = aNaviName.AllocL();
    iTitleName = aTitleName.AllocL();
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileNameImpl::NewL()
    {
    CProfileNameImpl* self = NewLC( 0, KNullDesC );

    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileNameImpl::NewLC(
    TInt aProfileId,
    const TDesC& aProfileName )
    {
    CProfileNameImpl* self = new ( ELeave ) CProfileNameImpl( aProfileId );
    CleanupStack::PushL( self );
    self->ConstructL( aProfileName );
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileNameImpl::NewLC(
    TInt aProfileId,
    const TDesC& aProfileName,
    const TDesC& aShortName,
    const TDesC& aNaviName,
    const TDesC& aTitleName )
    {
    CProfileNameImpl* self = new ( ELeave ) CProfileNameImpl( aProfileId );
    CleanupStack::PushL( self );
    self->ConstructL( aProfileName, aShortName, aNaviName, aTitleName );
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileNameImpl* CProfileNameImpl::NewLC(
    const CProfileNameImpl& aProfileNameImpl )
    {
    CProfileNameImpl* self = 
        new ( ELeave ) CProfileNameImpl( aProfileNameImpl.iId );
    CleanupStack::PushL( self );
    self->ConstructL( aProfileNameImpl );
    return self;
    }

// Destructor
CProfileNameImpl::~CProfileNameImpl()
    {
    delete iTitleName;
    delete iNaviName;
    delete iShortName;
    delete iProfileName;
    }


// -----------------------------------------------------------------------------
// CProfileNameImpl::IsProfileNameChanged
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileNameImpl::IsProfileNameChanged() const
    {
    return iNameChanged;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::SetLocalizedProfileNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::SetLocalizedProfileNameL(
    const MProfilesNamesArray& aNameArray )
    {
    SetNameInternalL( iProfileName,
                      aNameArray.MdcaPoint( aNameArray.FindById( iId ) ) );
    SetNameInternalL( iShortName,
            aNameArray.ProfileName( aNameArray.FindById( iId ) )->ShortName() );
    SetNameInternalL( iNaviName,
            aNameArray.ProfileName( aNameArray.FindById( iId ) )->NaviName() );
    SetNameInternalL( iTitleName,
            aNameArray.ProfileName( aNameArray.FindById( iId ) )->TitleName() );
    iNameChanged = EFalse;
    }

// -----------------------------------------------------------------------------
// CProfileImpl::SetLocalizedProfileNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::SetLocalizedProfileNameL(
    const CProfileNameImpl& aNameImpl, const TDesC& aUniquePart )
    {
    TBuf<KProfileMaxNameLength> nameBuf;

    nameBuf.Copy( aNameImpl.Name() );
    nameBuf.Append( aUniquePart );
    SetNameInternalL( iProfileName, nameBuf );

    nameBuf.Copy( aNameImpl.ShortName() );
    nameBuf.Append( aUniquePart );
    SetNameInternalL( iShortName, nameBuf );

    nameBuf.Copy( aNameImpl.NaviName() );
    nameBuf.Append( aUniquePart );
    SetNameInternalL( iNaviName, nameBuf );

    nameBuf.Copy( aNameImpl.TitleName() );
    nameBuf.Append( aUniquePart );
    SetNameInternalL( iTitleName, nameBuf );

    iNameChanged = EFalse;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::InternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::InternalizeL( CRepository& aCenRep, TInt aProfileId )
    {
    delete iShortName;
    iShortName = NULL;

    delete iNaviName;
    iNaviName = NULL;

    delete iTitleName;
    iTitleName = NULL;

    TInt tmp( 0 );
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey(
            KProEngNameChanged, aProfileId ), tmp ) );

    TBuf< KProfileMaxNameLength > nameBuf;
    User::LeaveIfError(
            aCenRep.Get( ProfileEngUtils::ResolveKey(
            KProEngProfileName, aProfileId ), nameBuf ) );
    HBufC* tmpName = nameBuf.AllocL();
    delete iProfileName;

    iId = aProfileId;
    iProfileName = tmpName;
    iNameChanged = tmp;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::ExternalizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::ExternalizeL( CRepository& aCenRep ) const
    {
    if( iNameChanged )
        {
        User::LeaveIfError(
                aCenRep.Set( ProfileEngUtils::ResolveKey(
                        KProEngNameChanged, iId ),
                        iNameChanged ) );
        User::LeaveIfError(
                aCenRep.Set( ProfileEngUtils::ResolveKey(
                        KProEngProfileName, iId ),
                        *iProfileName ) );
        }
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::SetNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::SetNameL(
    const TDesC& aName )
    {
    SetNameInternalL( iProfileName, aName );
    // User changed the name, iShortName,iNaviName,iTitleName are not needed
    // anymore:
    delete iShortName;
    iShortName = NULL;

    delete iNaviName;
    iNaviName = NULL;

    delete iTitleName;
    iTitleName = NULL;

    iNameChanged = ETrue;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::Name
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileNameImpl::Name() const
    {
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::Id
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileNameImpl::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::ShortName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileNameImpl::ShortName() const
    {
    if( iShortName )
        {
        return *iShortName;
        }
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::NaviName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileNameImpl::NaviName() const
    {
    if( iNaviName )
        {
        return *iNaviName;
        }
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::TitleName
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CProfileNameImpl::TitleName() const
    {
    if( iTitleName )
        {
        return *iTitleName;
        }
    return *iProfileName;
    }

// -----------------------------------------------------------------------------
// CProfileNameImpl::SetNameInternalL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileNameImpl::SetNameInternalL( HBufC*& aNameFieldRef,
    const TDesC& aName )
    {
    HBufC* tmp = aName.Left( KProfileMaxNameLength ).AllocL();
    delete aNameFieldRef;
    aNameFieldRef = tmp;
    }

//  End of File
