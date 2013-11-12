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
* Description:  Presence connection host
 *
*/

//#include <ximpprotocolconnection.h>
//#include <ximpprotocolconnectionhostobserver.h>
#include <protocolpresencedatahost.h>
#include <presenceobjectfactory.h>
#include <protocolpresenceauthorizationdatahost.h>
#include <protocolpresencepublishingdatahost.h>
#include <protocolpresencewatchingdatahost.h>
#include <protocolpresentitygroupsdatahost.h>

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
//#include "ximpstatusimp.h"
//#include "ximpoperationbase.h"

#include "protocolpresencedatahostimp.h"
#include "presenceobjectfactoryimp.h"
#include "presencedatacacheimp.h"
//#include "ximpglobals.h"
/*#include "ximppluginfactory.h"
#include "ximpeventmanager.h"
#include "ximphoststates.h"
#include "ximpcontextstateeventimp.h"
#include "ximpobjectfactoryimp.h"
#include "ximppanics.h"
#include "ximppsccontextimp.h"*/
#include "presencedatacacheimp.h"
/*#include "ximpserviceinfoimp.h"
#include "ximpoperationbase.h"
#include "ximpoperationfactory.h"
#include "ximpobjecthelpers.h"
#include "ximpoperationdefs.h"
#include "ximpcontexteventqueue.h"
#include "ximphostmanager.h"
#include "ximpidentityimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximprequestcompleteeventimp.h"
*/
#include "protocolpresenceauthorizationdatahostimp.h"
#include "protocolpresencepublishingdatahostimp.h"
#include "protocolpresencewatchingdatahostimp.h"
#include "protocolpresentitygroupsdatahostimp.h"
#include "ximpprotocolrequestcompleteeventimp.h"

#include "ximpdlluids.hrh"

// ======== LOCAL FUNCTIONS ========



// ======== MEMBER FUNCTIONS ========


// This should be variated. Branding variation can be one
// possibility. One is to leave adaptation deside how soon
// cleanup will be done.
// Now 5 sec used for testing purposes. Better value could be
// some minutes.

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::CProtocolPresenceDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceDataHostImp::CProtocolPresenceDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolPresenceDataHostImp::ConstructL()
    {
    iPresenceDataCache = CPresenceDataCache::NewL( iHost );
    iPresenceObjFactory = CPresenceObjectFactoryImp::NewL();
    iAuthDataHost = CProtocolPresenceAuthorizationDataHostImp::NewL( iHost );
    iPublishingDataHost = CProtocolPresencePublishingDataHostImp::NewL( iHost );
    iWatchingDataHost = CProtocolPresenceWatchingDataHostImp::NewL( iHost );
    iGroupsDataHost = CProtocolPresentityGroupsDataHostImp::NewL( iHost );
    }


// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresenceDataHostImp* CProtocolPresenceDataHostImp::NewLC( MXIMPHost& aHost )
    {
    CProtocolPresenceDataHostImp* self = new( ELeave ) CProtocolPresenceDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolPresenceDataHostImp* CProtocolPresenceDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolPresenceDataHostImp* self = new( ELeave ) CProtocolPresenceDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::~CProtocolPresenceDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolPresenceDataHostImp::~CProtocolPresenceDataHostImp()
    {
    delete iPresenceDataCache;
    delete iPresenceObjFactory;
    delete iAuthDataHost;
    delete iPublishingDataHost;
    delete iWatchingDataHost;
    delete iGroupsDataHost;

    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolPresenceDataHostImp, MProtocolPresenceDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolPresenceDataHostImp, MProtocolPresenceDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::PresenceObjectFactory()
// ---------------------------------------------------------------------------
//
MPresenceObjectFactory& CProtocolPresenceDataHostImp::PresenceObjectFactory()
    {
    return *iPresenceObjFactory;
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::AuthorizationDataHost()
// ---------------------------------------------------------------------------
//
MProtocolPresenceAuthorizationDataHost& CProtocolPresenceDataHostImp::AuthorizationDataHost()
    {
    return *iAuthDataHost;
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::PublishingDataHost()
// ---------------------------------------------------------------------------
//
MProtocolPresencePublishingDataHost& CProtocolPresenceDataHostImp::PublishingDataHost()
    {
    return *iPublishingDataHost;
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::WatchingDataHost()
// ---------------------------------------------------------------------------
//
MProtocolPresenceWatchingDataHost& CProtocolPresenceDataHostImp::WatchingDataHost()
    {
    return *iWatchingDataHost;
    }

// ---------------------------------------------------------------------------
// CProtocolPresenceDataHostImp::GroupsDataHost()
// ---------------------------------------------------------------------------
//
MProtocolPresentityGroupsDataHost& CProtocolPresenceDataHostImp::GroupsDataHost()
    {
    return *iGroupsDataHost;
    }
    
    
EXPORT_C CPresenceDataCache& CProtocolPresenceDataHostImp::PresenceDataCache()
    {
    return *iPresenceDataCache;
    }
EXPORT_C CProtocolPresenceAuthorizationDataHostImp& CProtocolPresenceDataHostImp::AuthorizationDataAccess()
    {
    return *iAuthDataHost;
    }
EXPORT_C CProtocolPresencePublishingDataHostImp& CProtocolPresenceDataHostImp::PublishingDataAccess()
    {
    return *iPublishingDataHost;
    }
EXPORT_C CProtocolPresenceWatchingDataHostImp& CProtocolPresenceDataHostImp::WatchingDataAccess()
    {
    return *iWatchingDataHost;
    }
EXPORT_C CProtocolPresentityGroupsDataHostImp& CProtocolPresenceDataHostImp::GroupsDataAccess()
    {
    return *iGroupsDataHost;
    }

// End of file

