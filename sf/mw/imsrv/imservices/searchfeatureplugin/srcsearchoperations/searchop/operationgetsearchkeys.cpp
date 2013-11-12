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
* Description:  Concrete get search keys  operation
 *
*/



#include <ximpprotocolconnection.h>
#include <protocolsearchfeature.h>
#include <protocolsearch.h>
#include "operationgetsearchkeys.h"
#include "ximphost.h"

#include <ximprequestcompleteevent.h>
#include "protocolsearchdatahostimp.h"
#include "searchlogutils.h"



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationGetSearchKeys::COperationGetSearchKeys()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationGetSearchKeys::COperationGetSearchKeys()
    {
    }
// ---------------------------------------------------------------------------
// COperationGetSearchKeys::ConstructL
// ---------------------------------------------------------------------------
//
void COperationGetSearchKeys::ConstructL( const TDesC8& /*aParamPck*/)
    {
    XSearchLogger::Log(_L("COperationGetSearchKeys::ConstructL "));
    }

// ---------------------------------------------------------------------------
// COperationGetSearchKeys::~COperationGetSearchKeys()
// ---------------------------------------------------------------------------
//
COperationGetSearchKeys::~COperationGetSearchKeys()
    {
    XSearchLogger::Log(_L("COperationGetSearchKeys::~COperationGetSearchKeys Start- End"));
    }


// ---------------------------------------------------------------------------
// COperationGetSearchKeys::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationGetSearchKeys::ProcessL()
    {
	XSearchLogger::Log(_L("COperationGetSearchKeys::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	// Call to Adaptation.
	MProtocolSearchFeature* srhFeature =static_cast< MProtocolSearchFeature*> 
										(iMyHost->GetConnection().GetProtocolInterface(MProtocolSearchFeature::KInterfaceId));
	if( srhFeature )
		{
	    srhFeature->Search().DoGetSearchKeysL( iReqId );		
		}
    else 
	    {
	    User::Leave(KErrNotSupported);	 // if NULL leave : not supported by protocol plugin
	    }
   
    XSearchLogger::Log(_L("COperationGetSearchKeys::ProcessL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationGetSearchKeys::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationGetSearchKeys::RequestCompletedL()
    {
    XSearchLogger::Log(_L("COperationGetSearchKeys::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
    XSearchLogger::Log(_L("COperationGetSearchKeys::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationGetSearchKeys::Type()
// ---------------------------------------------------------------------------
//
TInt COperationGetSearchKeys::Type() const
    {
    return NSearchOps::EGetSearchKeys;
    }


// End of file
