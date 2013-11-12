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
* Description:  XIMP Framework server root session implementation
*
*/

#include "ximpbase.h"
#include "ximprootsession.h"
#include "ximprootserverdefs.h"
#include "ximpglobals.h"
#include "ximppluginfactory.h"
#include "ximpfeaturemanager.h"
#include "ximpsrvmessage.h"
#include "ximptrace.h"

#include <e32base.h>


//Import NRootSrv namespace
using namespace NRootSrv;

// -----------------------------------------------------------------------------
// CXIMPRootSession::NewL()
// -----------------------------------------------------------------------------
//
CXIMPRootSession* CXIMPRootSession::NewL()
    {
    CXIMPRootSession* self = new( ELeave ) CXIMPRootSession();
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::~CXIMPRootSession()
// -----------------------------------------------------------------------------
//
CXIMPRootSession::~CXIMPRootSession()
    {
    delete iBuffer;
    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::CXIMPRootSession()
// -----------------------------------------------------------------------------
//
CXIMPRootSession::CXIMPRootSession()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::TryHandleMessageL()
// -----------------------------------------------------------------------------
//
void CXIMPRootSession::TryHandleMessageL( MXIMPSrvMessage& aMessage )
    {
    TBool msgNeedsToBeCompleted = ETrue;

    switch( aMessage.Function() )
        {
        case NRequest::ERootSsPrepareProtocolList:
            {
            DoPrepareProtocolListL( aMessage );
            aMessage.Complete( iBuffer->Size() );
            msgNeedsToBeCompleted = EFalse;
            break;
            }

        case NRequest::ERootSsPrepareSupportedFeatures:
            {
            DoPrepareSupportedFeaturesL( aMessage );
            aMessage.Complete( iBuffer->Size() );
            msgNeedsToBeCompleted = EFalse;
            break;
            }

        case NRequest::ERootSsFetchServerSideData:
            {
            DoFetchServerDataL( aMessage );
            break;
            }

        default:
            {
            // bogus message
            break;
            }
        }

    if ( msgNeedsToBeCompleted )
        {
        aMessage.Complete( KErrNone );
        }

    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::DoPrepareProtocolListL()
// -----------------------------------------------------------------------------
//
void CXIMPRootSession::DoPrepareProtocolListL(
    const MXIMPSrvMessage& /* aMessage */ )
    {
    HBufC8* tmp = CXIMPGlobals::Instance()->PluginFactory()->GetProtocolsL();
    delete iBuffer;
    iBuffer = tmp;
    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::DoPrepareSupportedFeaturesL()
// -----------------------------------------------------------------------------
//
void CXIMPRootSession::DoPrepareSupportedFeaturesL(
    const MXIMPSrvMessage& /* aMessage */ )
    {
    HBufC8* tmp = CXIMPGlobals::Instance()->FeatureManager()->GetFrameworkFeaturesL();
    delete iBuffer;
    iBuffer = tmp;
    }


// -----------------------------------------------------------------------------
// CXIMPRootSession::DoFetchServerDataL()
// -----------------------------------------------------------------------------
//
void CXIMPRootSession::DoFetchServerDataL(
    const MXIMPSrvMessage& aMessage )
    {
    aMessage.WriteL( MXIMPSrvMessage::Ep0, *iBuffer );
    delete iBuffer;
    iBuffer = NULL;
    }


// End of file
