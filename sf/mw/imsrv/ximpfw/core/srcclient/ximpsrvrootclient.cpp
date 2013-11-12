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
* Description:  XIMP Framework server root client implementation
*
*/


//  INCLUDE FILES
#include "ximpbase.h"
#include "ximpsrvrootclient.h"
#include "ximpprocessstarter.h"
#include "ximprootserverdefs.h"

#include <e32base.h>
#include <s32mem.h>


//Include Root server namespace
using namespace NRootSrv;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::RXIMPSrvRootClient()
// ---------------------------------------------------------------------------
//
EXPORT_C RXIMPSrvRootClient::RXIMPSrvRootClient()
    {
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::Connect()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RXIMPSrvRootClient::Connect()
    {
    TFileName fullExePath;
    XIMPProcessStarter::FullExePathForClienLocation( NName::KExecutable,
                                                     fullExePath );

    TInt ret = XIMPProcessStarter::ConnectToServer( fullExePath,
                                            KNullDesC,
                                            NULL,
                                            *this,
                                            NName::KSymbianServer,
                                            TVersion( NVersion::KMajor,
                                                      NVersion::KMinor,
                                                      NVersion::KBuild ),
                                            NRequest::KMsgSlotCount );

    if ( ret == KErrNone )
        {
        TIpcArgs msgArgs;
        ret = SendReceive( NRequest::EBecomeRootSession, msgArgs );
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::Close()
// ---------------------------------------------------------------------------
//
EXPORT_C void RXIMPSrvRootClient::Close()
    {
    RSessionBase::Close();
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::GetProtocolListL()
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* RXIMPSrvRootClient::GetKnownProtocolsL()
    {
    return GetDataL( NRequest::ERootSsPrepareProtocolList );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::GetSupportedFrameworkFeaturesL()
// ---------------------------------------------------------------------------
//
EXPORT_C HBufC8* RXIMPSrvRootClient::GetSupportedFrameworkFeaturesL()
    {
    return GetDataL( NRequest::ERootSsPrepareSupportedFeatures );
    }


// ---------------------------------------------------------------------------
// RXIMPSrvRootClient::GetDataL()
// ---------------------------------------------------------------------------
//
HBufC8* RXIMPSrvRootClient::GetDataL( TInt aMsg )
    {
    TInt bufSize = SendReceive( aMsg );
    User::LeaveIfError( bufSize );

    HBufC8* buf = HBufC8::NewLC( bufSize );
    TPtr8 ptrBuf( buf->Des() );

    TIpcArgs msgArgs;
    msgArgs.Set( 0, &ptrBuf );

    User::LeaveIfError( SendReceive( NRequest::ERootSsFetchServerSideData, msgArgs ) );

    CleanupStack::Pop( buf );
    return buf;
    }


// End of file

