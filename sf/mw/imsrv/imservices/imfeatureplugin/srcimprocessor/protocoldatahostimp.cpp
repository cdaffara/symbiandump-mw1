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


//#include <protocolimdatahost.h>
#include <ximpprotocolconnection.h>
#include <cprotocolimfeatures.h>
#include <imobjectfactory.h>
#include <protocolimconversationdatahost.h>
#include "ximpapiobjbase.h"
#include "imapiobjbase.h"
#include "imobjectfactoryimp.h"

#include "protocoldatahostimp.h"
#include "protocolimdatahostimp.h"
#include "protocolimconversationdatahostimp.h"
//#include "ximpprotocolrequestcompleteeventimp.h"
#include "imlogutils.h"
#include <ximpoperationbase.h>
#include "imoperationdefs.h"

// ======== MEMBER FUNCTIONS ========


// This should be variated. Branding variation can be one
// possibility. One is to leave adaptation deside how soon
// cleanup will be done.
// Now 5 sec used for testing purposes. Better value could be
// some minutes.

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::CProtocolDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolDataHostImp::CProtocolDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    XImLogger::Log(_L("CProtocolDataHostImp::CProtocolDataHostImp Started"));
    XImLogger::Log(_L("CProtocolDataHostImp::CProtocolDataHostImp Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolDataHostImp::ConstructL()
    {
    XImLogger::Log(_L("CProtocolDataHostImp::ConstructL Started"));
    
    //iImObjFactory = CImObjectFactoryImp::NewL();
    //iConvDataHost = CProtocolImConversationDataHostImp::NewL( iHost );
    iImDataHost = CProtocolImDataHostImp::NewL(iHost) ;
    XImLogger::Log(_L("CProtocolDataHostImp::ConstructL Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolDataHostImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolDataHostImp* CProtocolDataHostImp::NewLC( MXIMPHost& aHost )
    {
    XImLogger::Log(_L("CProtocolDataHostImp::NewLC Started"));
    CProtocolDataHostImp* self = new( ELeave ) CProtocolDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    XImLogger::Log(_L("CProtocolDataHostImp::NewLC Completed"));
    return self;
    }

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolDataHostImp* CProtocolDataHostImp::NewL( MXIMPHost& aHost )
    {
    XImLogger::Log(_L("CProtocolDataHostImp::NewL Started"));
    CProtocolDataHostImp* self = new( ELeave ) CProtocolDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XImLogger::Log(_L("CProtocolDataHostImp::NewL Completed"));
    return self;
    }
// ---------------------------------------------------------------------------
// CProtocolDataHostImp::~CProtocolDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolDataHostImp::~CProtocolDataHostImp()
    {
    XImLogger::Log(_L("CProtocolDataHostImp::~CProtocolDataHostImp Started"));
    
    //delete iImObjFactory;
    //delete iConvDataHost;
    delete iImDataHost ;
    XImLogger::Log(_L("CProtocolDataHostImp::~CProtocolDataHostImp Completed"));
    }

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::SetHostToProtocol()
// ---------------------------------------------------------------------------
//
void CProtocolDataHostImp::SetHostToProtocol()
	{
	CProtocolImFeatures* imProtocolFeature = static_cast< CProtocolImFeatures*> 
							(iHost.GetConnection().GetProtocolInterface(CProtocolImFeatures::KInterfaceId) );
	
	if(imProtocolFeature)
		{
		imProtocolFeature->SetHost(*iImDataHost);	
		}
	}

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* CProtocolDataHostImp::GetInterface(TInt aInterfaceId)
	{
	if(aInterfaceId == MProtocolImDataHost::KInterfaceId )
		return iImDataHost ;
	else
		return NULL ;
	
	}

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::GetInterface()
// ---------------------------------------------------------------------------
//
TInt CProtocolDataHostImp::GetCleanupOperation() 
	{
	return NImOps::EIMCleaner ;
	}
// End of file

