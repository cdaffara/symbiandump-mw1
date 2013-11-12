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
* Description:  MXIMPClient API object implementation.
*
*/

#include "ximpclientimp.h"
#include "ximpclientifsingleton.h"
#include "ximpobjecthelpers.h"
#include "ximpsrvrootclient.h"
#include "ximpprotocolinfoimp.h"
#include "ximpfeatureinfoimp.h"
#include "ximpcontextimp.h"
#include "ximpobjecthelpers.h"


// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CXIMPClientImp::NewMXIMPClientL()
// ---------------------------------------------------------------------------
//
EXPORT_C MXIMPClient* CXIMPClientImp::NewMXIMPClientL()
    {
    CXIMPClientImp* self = new( ELeave ) CXIMPClientImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPClientImp, 
                                     MXIMPClient )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPClientImp, 
                                           MXIMPClient )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CXIMPClientImp::~CXIMPClientImp()
// ---------------------------------------------------------------------------
//
CXIMPClientImp::~CXIMPClientImp()
    {
    if( iSingleton )
        {
        //Singleton is refercen counted
        //So the Close() is enough to release the reference
        iSingleton->Close();
        }
    }

// ---------------------------------------------------------------------------
// CXIMPClientImp::CXIMPClientImp()
// ---------------------------------------------------------------------------
//
CXIMPClientImp::CXIMPClientImp()
    {
    }


// ---------------------------------------------------------------------------
// void CXIMPClientImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPClientImp::ConstructL()
    {
    iSingleton = CXIMPClientIfSingleton::InstanceL();
    }

// ---------------------------------------------------------------------------
// CXIMPClientImp::GetProtocolsL()
// ---------------------------------------------------------------------------
//
void CXIMPClientImp::GetProtocolsL(
    RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocols )
    {
    aProtocols.ResetAndDestroy();

    RXIMPObjOwningPtrArray< CXIMPProtocolInfoImp > foundProtocols;
    CleanupClosePushL( foundProtocols ); 

    HBufC8* pack = iSingleton->RootClient().GetKnownProtocolsL(); 
    CleanupStack::PushL( pack ); 
    
    TXIMPObjectPacker< CXIMPProtocolInfoImp >::UnPackArrayL( foundProtocols, 
                                                             *pack );
    CleanupStack::PopAndDestroy( pack );

    TXIMPObjectMover< CXIMPProtocolInfoImp, 
                      MXIMPProtocolInfo >::MoveFromPtrArrayToL( foundProtocols, 
                                                                aProtocols ); 
    CleanupStack::PopAndDestroy(); //foundProtocols
    }



// ---------------------------------------------------------------------------
// CXIMPClientImp::GetFrameworkFeaturesLC()
// ---------------------------------------------------------------------------
//
MXIMPFeatureInfo* CXIMPClientImp::GetFrameworkFeaturesLC()
    {
    CXIMPFeatureInfoImp* features = CXIMPFeatureInfoImp::NewLC();
    
    HBufC8* pack = iSingleton->RootClient().GetSupportedFrameworkFeaturesL();
    CleanupStack::PushL( pack );
    TXIMPObjectPacker< CXIMPFeatureInfoImp >::UnPackL( *features, *pack );
    CleanupStack::PopAndDestroy( pack );

    return features;
    }
    
    
// ---------------------------------------------------------------------------
// CXIMPClientImp::NewPresenceContextLC()
// ---------------------------------------------------------------------------
//
MXIMPContext* CXIMPClientImp::NewPresenceContextLC()
    {
    return CXIMPContextImp::NewLC();
    }


