/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "tsbacksteppingfilter.h"
#include "tsidlist.h"

const TInt KBlockedApps [] = {0x100058b3/* <-- phoneui */,
                              0x2001843A/* <-- javainstaller */,
                              0x200267D2/* <-- widgetinstallerapp */};

// -----------------------------------------------------------------------------
/** 
 * Symbian two-phase constructor. Allocate and initialize list blocked applications
 * @return backstepping filtering
 */
CTsBacksteppingFilter* CTsBacksteppingFilter::NewL()
    {
    CTsBacksteppingFilter *self = new(ELeave)CTsBacksteppingFilter();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
/** 
 * Destructor
 */
CTsBacksteppingFilter::~CTsBacksteppingFilter()
    {
    delete iBlockedApps;
    }

// -----------------------------------------------------------------------------
/** 
 * Inform if application is blocked for backstepping engine
 * @param aApplicationUid - tested application UID
 * @return EFalse if application is not blocked for backstepping engine, other value in other cases
 */
TBool CTsBacksteppingFilter::isBlocked( TUid aApplicationUid ) const
    {
    return iBlockedApps->IsPresent( aApplicationUid.iUid );
    }

// -----------------------------------------------------------------------------
/** 
 * Constructor
 */
CTsBacksteppingFilter::CTsBacksteppingFilter()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
/** 
 * Symbian second-phase constructor. Initilaize blocked applications list
 */
void CTsBacksteppingFilter::ConstructL()
    {
    const TInt count( sizeof( KBlockedApps ) / sizeof( TInt ) );
    iBlockedApps = CTsIdList::NewL();
    iBlockedApps->AppendL( KBlockedApps, count );
    }
