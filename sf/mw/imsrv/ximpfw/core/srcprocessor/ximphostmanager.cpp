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
* Description:  implementation of CXIMPHostManager
 *
*/



#include <s32mem.h>
#include <badesca.h>

#include <ximpprotocolconnection.h>
#include <ximpprotocolpluginbase.h>

#include "ximphostmanager.h"
#include "ximpserviceinfoimp.h"
#include "ximphostimp.h"
#include "ximpoperationdefs.h"
#include "ximpobjecthelpers.h"
//#include "ximpcontext.h"
#include "ximppsccontext.h"
#include "ximppanics.h"
#include "ximpglobals.h"
#include "ximppluginfactory.h"
#include "ximpobjectfactoryimp.h"
#include "ximppsccontextimp.h"
#include "ximpcontextclientinfoimp.h"

// ======== LOCAL FUNCTIONS ========

TInt OrderOfHosts( const CXIMPHost& aFirst, const CXIMPHost& aSecond )
    {
    if( &aFirst.GetConnection() < &aSecond.GetConnection() )
        {
        return -1;
        }
    else if( &aFirst.GetConnection() == &aSecond.GetConnection() )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXIMPHostManager::CXIMPHostManager()
// ---------------------------------------------------------------------------
//
CXIMPHostManager::CXIMPHostManager()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPHostManager::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPHostManager::ConstructL()
    {
    iObjFactory = CXIMPObjectFactoryImp::NewL();
    }


// ---------------------------------------------------------------------------
// CXIMPHostManager::NewL()
// ---------------------------------------------------------------------------
//
CXIMPHostManager* CXIMPHostManager::NewL()
    {
    CXIMPHostManager* self = new( ELeave ) CXIMPHostManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPHostManager::~CXIMPHostManager()
// ---------------------------------------------------------------------------
//
CXIMPHostManager::~CXIMPHostManager()
    {
    // host manager CANNOT OWN the hostimp objects.
    // it's a fatal error to have hosts in the array
    // at this point.
    __ASSERT_ALWAYS( iHosts.Count() == 0, User::Panic( NXIMPPrivPanic::KCategory, NXIMPPrivPanic::EHostMgrHostsHaveNotBeenDeleted ) );

    iHosts.Close();
    delete iObjFactory;    
    
    if( iProtocol )
        {
        delete iProtocol;
        }
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPHostManager, MXIMPProtocolPluginHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPHostManager, MXIMPProtocolPluginHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CXIMPHostManager::DisassociateHost()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPHostManager::DisassociateHost( CXIMPHost* aHost )
    {
    // go backwards to avoid modifying index when removing
    TInt count = iHosts.Count();
    for( TInt a = count-1; a >= 0; a-- )
        {
        if( iHosts[ a ] == aHost )
            {
            // remove this host
            MXIMPProtocolConnection& connection = aHost->GetConnection();
            iProtocol->ReleaseConnection( connection );
            iHosts.Remove( a );
            }
        }

    count = iHosts.Count();
    if ( count == 0 )
        {
        // no more hosts
        CXIMPGlobals::Instance()->NoMoreHosts();
        }
    }
    
// ---------------------------------------------------------------------------
// CXIMPHostManager::AcquireHostForContextL()
// ---------------------------------------------------------------------------
//
CXIMPHost* CXIMPHostManager::AcquireHostForContextLC( CXIMPPscContext& aContext )
    {
    CXIMPServiceInfoImp* serviceInfo = aContext.ServiceInfo();
    if( !iProtocol )
        {
        if( !serviceInfo )
            {
            User::Leave( KErrArgument );
            }
        CXIMPPluginFactory* factory = CXIMPGlobals::Instance()->PluginFactory();
        iProtocol = factory->GetProtocolL( serviceInfo->ProtocolImplUid() );
        iProtocol->PrimeHost( *this );
        }
    return GetHostForConnectionLC( 
            iProtocol->AcquireConnectionL( *serviceInfo, *aContext.ClientInfo() ),
            serviceInfo->ProtocolImplUid().iUid );
    }

// ---------------------------------------------------------------------------
// CXIMPHostManager::GetHostForConnectionL()
// ---------------------------------------------------------------------------
//
CXIMPHost* CXIMPHostManager::GetHostForConnectionLC( 
    MXIMPProtocolConnection& aConnection, TInt32 aProtocolUid  )
    {
    CXIMPHost* newHost = CXIMPHost::NewLC( aConnection, aProtocolUid );
    TLinearOrder< CXIMPHost > order( *OrderOfHosts );
    TInt index = iHosts.FindInOrder( newHost, order );
    if( index == KErrNotFound )
        {
        iHosts.InsertInOrderL( newHost, order );
        aConnection.PrimeHost( *newHost );
        // this will allow protocol hosts to set in adaptations
        newHost->SetProtocolHosts() ;
        }
    else
        {
        User::LeaveIfError( index );
        CleanupStack::PopAndDestroy(); //newHost
        newHost = iHosts[ index ];
        newHost->Open(); // CSI: 65 # Does not return a value.
        CleanupClosePushL( *newHost );
        }
    return newHost;
    }

// ---------------------------------------------------------------------------
// CXIMPHostManager::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHostManager::HandleRequestCompleted( TXIMPRequestId aReqId, 
                                              TInt aResultCode )
    {
    XIMP_UNUSED_FORMAL_PARAM( aReqId );
    XIMP_UNUSED_FORMAL_PARAM( aResultCode );
    }

// ---------------------------------------------------------------------------
// CXIMPHostManager::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHostManager::HandleRequestCompleted( TXIMPRequestId aReqId,
                                              MXIMPStatus* aResult )
    {    
    XIMP_UNUSED_FORMAL_PARAM( aReqId );
    XIMP_UNUSED_FORMAL_PARAM( aResult );
    }



// ---------------------------------------------------------------------------
// CXIMPHostManager::HandleRequestCompleted()
// ---------------------------------------------------------------------------
//
void CXIMPHostManager::HandleRequestCompleted( TXIMPRequestId aReqId,
                                              MXIMPStatus* aResult,
                                              MXIMPBase* aParameter )
    {    
    XIMP_UNUSED_FORMAL_PARAM( aReqId );
    XIMP_UNUSED_FORMAL_PARAM( aResult );
    XIMP_UNUSED_FORMAL_PARAM( aParameter );
    }



// ---------------------------------------------------------------------------
// CXIMPHostManager::ObjectFactory()
// ---------------------------------------------------------------------------
//
MXIMPObjectFactory& CXIMPHostManager::ObjectFactory()
    {
    return *iObjFactory;
    }

// End of file
