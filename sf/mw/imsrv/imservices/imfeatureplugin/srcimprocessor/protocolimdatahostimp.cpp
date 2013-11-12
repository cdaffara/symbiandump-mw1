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
* Description:  Im connection host
 *
*/


#include <protocolimdatahost.h>
#include <ximpprotocolconnection.h>
#include <protocolimfeatures.h>
#include <imobjectfactory.h>
#include <protocolimconversationdatahost.h>
#include "ximpapiobjbase.h"
#include "imapiobjbase.h"
#include "imobjectfactoryimp.h"
#include "imdatacacheimp.h"
#include "protocolimdatahostimp.h"
#include "protocolimconversationdatahostimp.h"
//#include "ximpprotocolrequestcompleteeventimp.h"
#include "imlogutils.h"
#include "imdatacacheimp.h"



// ======== MEMBER FUNCTIONS ========


// This should be variated. Branding variation can be one
// possibility. One is to leave adaptation deside how soon
// cleanup will be done.
// Now 5 sec used for testing purposes. Better value could be
// some minutes.

// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::CProtocolImDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolImDataHostImp::CProtocolImDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::CProtocolImDataHostImp Started"));
    XImLogger::Log(_L("CProtocolImDataHostImp::CProtocolImDataHostImp Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolImDataHostImp::ConstructL()
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::ConstructL Started"));
    iConvDataHost = CProtocolImConversationDataHostImp::NewL( iHost );
    iImObjFactory = CImObjectFactoryImp::NewL();
    iImDataCache = CImDataCache::NewL( iHost );
    
    XImLogger::Log(_L("CProtocolImDataHostImp::ConstructL Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::NewLC()
// ---------------------------------------------------------------------------
//
CProtocolImDataHostImp* CProtocolImDataHostImp::NewLC( MXIMPHost& aHost )
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::NewLC Started"));
    CProtocolImDataHostImp* self = new( ELeave ) CProtocolImDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    XImLogger::Log(_L("CProtocolImDataHostImp::NewLC Completed"));
    return self;
    }

// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
CProtocolImDataHostImp* CProtocolImDataHostImp::NewL( MXIMPHost& aHost )
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::NewL Started"));
    CProtocolImDataHostImp* self = new( ELeave ) CProtocolImDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XImLogger::Log(_L("CProtocolImDataHostImp::NewL Completed"));
    return self;
    }
// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::~CProtocolImDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolImDataHostImp::~CProtocolImDataHostImp()
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::~CProtocolImDataHostImp Started"));
    delete iConvDataHost;
    delete iImDataCache;
    delete iImObjFactory;
    XImLogger::Log(_L("CProtocolImDataHostImp::~CProtocolImDataHostImp Completed"));
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
//XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolImDataHostImp, MProtocolImDataHost )
//XIMPIMP_IF_BASE_GET_INTERFACE_END()
//XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolImDataHostImp, MProtocolImDataHost )
//XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::ImObjectFactory()
// ---------------------------------------------------------------------------
//
MImObjectFactory& CProtocolImDataHostImp::ImObjectFactory()
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::ImObjectFactory Started"));
    XImLogger::Log(_L("CProtocolImDataHostImp::ImObjectFactory Completed"));
    return *iImObjFactory;
    }

// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::ConversationDataHost()
// ---------------------------------------------------------------------------
//
MProtocolImConversationDataHost& CProtocolImDataHostImp::ConversationDataHost()
    {
    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Started"));
    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Completed"));
    return *iConvDataHost;
    }

// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::SearchDataHost()
// ---------------------------------------------------------------------------
//
//MProtocolImSearchDataHost& CProtocolImDataHostImp::SearchDataHost()
//    {
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Started"));
    //return *iSearchDataHost;
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Completed"));
//    }

// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::GroupDataHost()
// ---------------------------------------------------------------------------
// 
//MProtocolImGroupDataHost& CProtocolImDataHostImp::GroupDataHost()
//    {
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Started"));
    //return *iGroupDataHost;
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Completed"));
//    }
    
// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::InvitationDataHost()
// ---------------------------------------------------------------------------
//
//MProtocolImInvitationDataHost& CProtocolImDataHostImp::InvitationDataHost()
//    {
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Started"));
    //return *iInvitationDataHost;
//    XImLogger::Log(_L("CProtocolImDataHostImp::ConversationDataHost Completed"));
//    }


// ---------------------------------------------------------------------------
// CProtocolImDataHostImp::ConversationDataAccess()
// ---------------------------------------------------------------------------
//
CProtocolImConversationDataHostImp& CProtocolImDataHostImp::ConversationDataAccess()
    {
    return *iConvDataHost;
    }
    
// ---------------------------------------------------------------------------
// CProtocolDataHostImp::ImDataCache()
// ---------------------------------------------------------------------------
//
CImDataCache& CProtocolImDataHostImp::ImDataCache()
    {
    return *iImDataCache;
    }


    
// End of file

