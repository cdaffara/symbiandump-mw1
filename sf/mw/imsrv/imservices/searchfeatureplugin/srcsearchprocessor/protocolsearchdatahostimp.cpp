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
* Description:  searchdata host 
 *
*/



#include <ximpprotocolconnection.h>
#include <protocolimfeatures.h>
#include <searchobjectfactory.h>
#include <protocolsearchfeaturedatahost.h>
#include "ximpapiobjbase.h"
#include "searchapiobjbase.h"
#include "searchobjectfactoryimp.h"
#include "searchdatacacheimp.h"
#include "protocolsearchdatahostimp.h"
#include "protocolsearchfeaturedatahostimp.h"

#include "searchlogutils.h"




// ======== MEMBER FUNCTIONS ========


// This should be variated. Branding variation can be one
// possibility. One is to leave adaptation deside how soon
// cleanup will be done.
// Now 5 sec used for testing purposes. Better value could be
// some minutes.

// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::CProtocolSearchDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolSearchDataHostImp::CProtocolSearchDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::CProtocolSearchDataHostImp Started"));
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::CProtocolSearchDataHostImp Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolSearchDataHostImp::ConstructL()
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::ConstructL Started"));
    
    iSearchObjFactory= CSearchObjectFactoryImp::NewL();
    iSrchFeatureDataHost=CProtocolSearchFeatureDataHostImp::NewL(iHost);
    iSearchDataCache= CSearchDataCache::NewL(iHost);
    
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::ConstructL Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::NewLC()
// ---------------------------------------------------------------------------
//
CProtocolSearchDataHostImp* CProtocolSearchDataHostImp::NewLC( MXIMPHost& aHost )
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::NewLC Started"));
    CProtocolSearchDataHostImp* self = new( ELeave ) CProtocolSearchDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::NewLC Completed"));
    return self;
    
    }

// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
CProtocolSearchDataHostImp* CProtocolSearchDataHostImp::NewL( MXIMPHost& aHost )
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::NewL Started"));
    CProtocolSearchDataHostImp* self = new( ELeave ) CProtocolSearchDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::NewL Completed"));
    return self;
    
    }
// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::~CProtocolSearchDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolSearchDataHostImp::~CProtocolSearchDataHostImp()
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::~CProtocolSearchDataHostImp Started"));
    delete iSearchDataCache;
    delete iSrchFeatureDataHost; 
    delete iSearchObjFactory;
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::~CProtocolSearchDataHostImp Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::ImObjectFactory()
// ---------------------------------------------------------------------------
//
MSearchObjectFactory&  CProtocolSearchDataHostImp::SearchObjectFactory()
    {
    XSearchLogger::Log(_L("CProtocolSearchDataHostImp::SearchObjectFactory Started"));
    return *iSearchObjFactory;
    }




// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::ConversationDataAccess()
// ---------------------------------------------------------------------------
//
MProtocolSearchFeatureDataHost& CProtocolSearchDataHostImp::SearchFeatureDataAccess()
    {
    return *iSrchFeatureDataHost;
    }
// ---------------------------------------------------------------------------
// CProtocolSearchDataHostImp::ConversationDataAccess()
// ---------------------------------------------------------------------------
//
EXPORT_C  CSearchDataCache& CProtocolSearchDataHostImp::SearchDataCache()
	{
	return *iSearchDataCache;	
	}


    
// End of file

