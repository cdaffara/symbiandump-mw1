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
* Description:  Implementation of protocol data host
 *
*/



#include <ximpprotocolconnection.h>
#include <protocolsearchfeature.h>
#include <searchobjectfactory.h>
#include <ximpoperationbase.h>

#include "ximpapiobjbase.h"
#include "searchapiobjbase.h"
#include "searchobjectfactoryimp.h"

#include "protocoldatahostimp.h"
#include "protocolsearchdatahostimp.h"
#include "protocolsearchfeaturedatahostimp.h"
#include "searchlogutils.h"

#include "searchoperationdefs.h"

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
    XSearchLogger::Log(_L("CProtocolDataHostImp::CProtocolDataHostImp Started"));
    XSearchLogger::Log(_L("CProtocolDataHostImp::CProtocolDataHostImp Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolDataHostImp::ConstructL()
    {
    XSearchLogger::Log(_L("CProtocolDataHostImp::ConstructL Started"));
    iSearchDataHost = CProtocolSearchDataHostImp::NewL(iHost) ;
    XSearchLogger::Log(_L("CProtocolDataHostImp::ConstructL Completed"));
    }


// ---------------------------------------------------------------------------
// CProtocolDataHostImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolDataHostImp* CProtocolDataHostImp::NewLC( MXIMPHost& aHost )
    {
    XSearchLogger::Log(_L("CProtocolDataHostImp::NewLC Started"));
    CProtocolDataHostImp* self = new( ELeave ) CProtocolDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    XSearchLogger::Log(_L("CProtocolDataHostImp::NewLC Completed"));
    return self;
    }

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolDataHostImp* CProtocolDataHostImp::NewL( MXIMPHost& aHost )
    {
    XSearchLogger::Log(_L("CProtocolDataHostImp::NewL Started"));
    CProtocolDataHostImp* self = new( ELeave ) CProtocolDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XSearchLogger::Log(_L("CProtocolDataHostImp::NewL Completed"));
    return self;
    }
// ---------------------------------------------------------------------------
// CProtocolDataHostImp::~CProtocolDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolDataHostImp::~CProtocolDataHostImp()
    {
    XSearchLogger::Log(_L("CProtocolDataHostImp::~CProtocolDataHostImp Started"));
    delete iSearchDataHost ;
    XSearchLogger::Log(_L("CProtocolDataHostImp::~CProtocolDataHostImp Completed"));
    }

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::SetHostToProtocol()
// ---------------------------------------------------------------------------
//
void CProtocolDataHostImp::SetHostToProtocol()
	{
	MProtocolSearchFeature* searchProtocolFeature = static_cast< MProtocolSearchFeature*> 
							(iHost.GetConnection().GetProtocolInterface(MProtocolSearchFeature::KInterfaceId) );
	
	if(searchProtocolFeature )
		{
		searchProtocolFeature->SetHost(*iSearchDataHost);	
		}
	
	}

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::GetInterface()
// ---------------------------------------------------------------------------
//
TAny* CProtocolDataHostImp::GetInterface(TInt aInterfaceId)
	{
	if(aInterfaceId == MProtocolSearchDataHost::KInterfaceId )
		{
		return iSearchDataHost ;	
		}
	
	else
		{
		return NULL ;	
		}
		
	}

// ---------------------------------------------------------------------------
// CProtocolDataHostImp::GetCleanupOperation()
// ---------------------------------------------------------------------------
//
TInt CProtocolDataHostImp::GetCleanupOperation() 
	{
	return NSearchOps::ESearchCleaner ; // return cleaner operation : 
	                                    // BUT NO CLEANING IS IMPLEMENTED IN SERACH PLUGIN ( NOT REQUIRED)
	}
// End of file

