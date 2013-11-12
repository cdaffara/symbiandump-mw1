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
* Description:  Concrete search operation
 *
*/



#include <ximpprotocolconnection.h>
#include <protocolsearchfeature.h>
#include <protocolsearch.h>
#include "searchelementimp.h"
#include "operationsearch.h"
#include "ximpobjecthelpers.h"
#include <ximprestrictedobjectcollection.h>
#include "ximphost.h"

#include <ximprequestcompleteevent.h>
#include "protocolsearchdatahostimp.h"
#include "searchlogutils.h"
#include <badesca.h>

#include <ximpidentityimp.h> 

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationSearch::COperationSearch()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSearch::COperationSearch()
    {
    }
// ---------------------------------------------------------------------------
// COperationSearch::ConstructL
// ---------------------------------------------------------------------------
//
void COperationSearch::ConstructL( const TDesC8& aParamPck )
    {
    XSearchLogger::Log(_L("COperationSearch::ConstructL Started"));
    
 	RDesReadStream rs;
    CleanupClosePushL( rs );
    rs.Open( aParamPck ); // CSI: 65 #
    
    
    iSearchId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iSearchId );
    iSearchId->InternalizeL( rs );

    // read the object count
    TInt objCount( rs.ReadInt32L() );

	for ( TInt count = 0; count < objCount; count++ )
        {
		// instantiate new object
	    CSearchElementImp* theObject = CSearchElementImp::NewLC();
	    // extract its data
	    theObject->InternalizeL( rs );
	    // append it to the object array
	    iSearchList.AppendL( theObject );
	    CleanupStack::Pop( theObject ); // theObject
		}
		
    iSearchLimit = rs.ReadInt32L();
    CleanupStack::PopAndDestroy(); // rs
    
   	                                                      
    XSearchLogger::Log(_L("COperationSearch::ConstructL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSearch::~COperationSearch()
// ---------------------------------------------------------------------------
//
COperationSearch::~COperationSearch()
    {
    XSearchLogger::Log(_L("COperationSearch::~COperationSearch Started"));
    delete iSearchId;
   	iSearchList.ResetAndDestroy();
    XSearchLogger::Log(_L("COperationSearch::~COperationSearch Completed"));
    }


// ---------------------------------------------------------------------------
// COperationSearch::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSearch::ProcessL()
    {
	XSearchLogger::Log(_L("COperationSearch::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	// Call to Adaptation.
	MProtocolSearchFeature* srhFeature =static_cast< MProtocolSearchFeature*> 
										(iMyHost->GetConnection().GetProtocolInterface(MProtocolSearchFeature::KInterfaceId));
	if( srhFeature )
	    {
	    srhFeature->Search().DoSearchL(*iSearchId, iSearchList,iSearchLimit,iReqId );
	    }
	else
	    {
	    User::Leave( KErrNotSupported ); // if NULL leave : not supported by protocol plugin
	    }
    XSearchLogger::Log(_L("COperationSearch::ProcessL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSearch::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSearch::RequestCompletedL()
    {
    XSearchLogger::Log(_L("COperationSearch::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
    XSearchLogger::Log(_L("COperationSearch::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSearch::Type()
// ---------------------------------------------------------------------------
//
TInt COperationSearch::Type() const
    {
    return NSearchOps::ESearch;
    }


// End of file
