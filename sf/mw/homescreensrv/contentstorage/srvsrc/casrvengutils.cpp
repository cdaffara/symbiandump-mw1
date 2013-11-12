/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 *  Description : The API supports item information that is not supported by
 *                the content arsenal server itself
 *
 */
#include <AknIconUtils.h>
#include <avkon.mbg>
#include <e32property.h>
#include <AknTaskList.h>
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include "casrvengutils.h"
#include "cautils.h"
#include "cadef.h"
#include "castorage.h"
#include "cainnerentry.h"

// ================= LOCAL FUNCTIONS ========================


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMenuSrvEngUtils::~CMenuSrvEngUtils
// ---------------------------------------------------------
//
CCaSrvEngUtils::~CCaSrvEngUtils()
    {
    iApaLsSession.Close();
    }

// ---------------------------------------------------------
// CCaSrvEngUtils::NewL
// ---------------------------------------------------------
//
CCaSrvEngUtils* CCaSrvEngUtils::NewL()
    {
    CCaSrvEngUtils* utils = new ( ELeave ) CCaSrvEngUtils();
    CleanupStack::PushL( utils );
    utils->ConstructL();
    CleanupStack::Pop( utils );
    return utils;
    }

// ---------------------------------------------------------
// GetAppUid
// ---------------------------------------------------------
//
TInt CCaSrvEngUtils::GetAppUid( const CCaInnerEntry& aEntry, TUid& aAppUid )
    {
    TInt err( 0 );
    TInt32 attrVal = aEntry.GetUid();
    if( attrVal )
        {
        aAppUid = TUid::Uid( attrVal );
        }
    else
        {
        err = KErrNotFound;
        }
    return err;
    }

// ---------------------------------------------------------
// GetApaAppInfo
// ---------------------------------------------------------
//
EXPORT_C TInt CCaSrvEngUtils::GetApaAppInfo( TUid aUid, TApaAppInfo& aInfo ) const
    {
    return iApaLsSession.GetAppInfo( aInfo, aUid );
    }

// ---------------------------------------------------------
// CMenuSrvEngUtils::CMenuSrvEngUtils
// ---------------------------------------------------------
//
CCaSrvEngUtils::CCaSrvEngUtils()
    {
    }

// ---------------------------------------------------------
// CMenuSrvEngUtils::ConstructL
// ---------------------------------------------------------
//
void CCaSrvEngUtils::ConstructL()
    {
    User::LeaveIfError( iApaLsSession.Connect() );
    }

// ---------------------------------------------------------
// CMenuSrvEngUtils::GetAppInfo
// ---------------------------------------------------------
//
EXPORT_C TInt CCaSrvEngUtils::GetAppInfo( const CCaInnerEntry& aEntry,
        TApaAppInfo& aAppInfo ) const
    {
    TUid appUid;
    TInt err = GetAppUid( aEntry, appUid );
    if( !err )
        {
        err = iApaLsSession.GetAppInfo( aAppInfo, appUid );
        }
    return err;
    }

// ---------------------------------------------------------
// CMenuSrvEngUtils::IsNativeL
// ---------------------------------------------------------
//
TBool CCaSrvEngUtils::IsNativeL( const CCaInnerEntry& aEntry )
    {
    TBool native( EFalse );
    if( aEntry.GetEntryTypeName().CompareF( KCaTypeApp ) == KErrNone )
        {
        TUid uid;
        if( GetAppUid( aEntry, uid ) == KErrNone )
            {
            IsNativeL( uid, native );
            }
        }
    return native;
    }

// ---------------------------------------------------------
// CMenuSrvEngUtils::IsNativeL
// ---------------------------------------------------------
//

void CCaSrvEngUtils::IsNativeL( TUid aAppUid, TBool& aIsNative )
    {
    TApaAppInfo* appInfo = new( ELeave ) TApaAppInfo();
    aIsNative = EFalse;
    TInt error = iApaLsSession.GetAppInfo( *appInfo, aAppUid );
    if( error == KErrNone )
        {
        error = iApaLsSession.IsProgram( appInfo->iFullName, aIsNative );
        if( error != KErrNone )
            {
            aIsNative = EFalse;
            }
        }
    delete appInfo;
    }
