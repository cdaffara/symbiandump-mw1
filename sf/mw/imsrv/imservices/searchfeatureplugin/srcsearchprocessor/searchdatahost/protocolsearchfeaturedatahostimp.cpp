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
* Description:  Searchdatafeature  datahost.
*
*/


#include <ximpobjectcollection.h>
#include <ximpidentity.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>

#include "ximphost.h"
#include "searchdatacacheimp.h"
#include "searchoperationdefs.h"
#include "searchlogutils.h"
#include "protocolsearchdatahostimp.h"
#include "protocolsearchfeaturedatahostimp.h"



// ============================ MEMBER FUNCTIONS ============================= 


// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::CProtocolSearchFeatureDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolSearchFeatureDataHostImp::CProtocolSearchFeatureDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolSearchFeatureDataHostImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolSearchFeatureDataHostImp* CProtocolSearchFeatureDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolSearchFeatureDataHostImp* self = new( ELeave ) CProtocolSearchFeatureDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolSearchFeatureDataHostImp,
                                     MProtocolSearchFeatureDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolSearchFeatureDataHostImp,
                                           MProtocolSearchFeatureDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::~CProtocolSearchFeatureDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolSearchFeatureDataHostImp::~CProtocolSearchFeatureDataHostImp()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::HandleSearchResultsL
// ---------------------------------------------------------------------------
//     
TXIMPRequestId CProtocolSearchFeatureDataHostImp::HandleSearchResultsL(MXIMPIdentity* aSearchId,
    										    					MXIMPObjectCollection* aSearchResults)
	{
    XSearchLogger::Log(_L("CProtocolSearchFeatureDataHostImp::HandleSearchResultsL Started"));
    return iHost.AddNewOperationL( NSearchOps::EHandleSearchResults,aSearchId,aSearchResults);
	
	}
// ---------------------------------------------------------------------------
// CProtocolSearchFeatureDataHostImp::HandleSearchKeysL
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolSearchFeatureDataHostImp::HandleSearchKeysL( MXIMPObjectCollection* aSearchKeyInfos )
    {
    XSearchLogger::Log(_L("CProtocolSearchFeatureDataHostImp::HandleSearchKeysL  Started"));
    return iHost.AddNewOperationL( NSearchOps::EHandleSearchKeys, aSearchKeyInfos  );
    
    }	
