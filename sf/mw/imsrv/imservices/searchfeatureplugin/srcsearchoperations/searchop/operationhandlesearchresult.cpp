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
* Description:  Concrete handle search result operation
 *
*/


#include <badesca.h>
#include <ximpprotocolconnection.h>
#include <protocolsearchfeature.h>

#include <ximppsccontext.h>

#include "searchinfoimp.h"
#include "operationhandlesearchresult.h"
#include "ximpoperationbase.h"
#include "ximpidentityimp.h"
#include "searchsubscriptionitem.h"
#include "searchdatacacheimp.h"

#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "ximphost.h"

#include "protocolsearchdatahostimp.h"
#include "searchlogutils.h"
#include "searchdatacacheimp.h"

#include "ximpsubscriptionitem.h"

#include "searchsubscriptionitem.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationHandleSearchResult::COperationHandleSearchResult()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandleSearchResult::COperationHandleSearchResult()
    {
    XSearchLogger::Log(_L("COperationHandleSearchResult::COperationHandleSearchResult Started"));
    XSearchLogger::Log(_L("COperationHandleSearchResult::COperationHandleSearchResult Completed"));
    }


// ---------------------------------------------------------------------------
// COperationHandleSearchResult::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchResult::ConstructL( const TDesC8& /*aParamPck */)
    {
    XSearchLogger::Log(_L("COperationHandleSearchResult::ConstructL Started")); 
    iSearchResults=new ( ELeave ) RSearchInfoImpArray;  
    XSearchLogger::Log(_L("COperationHandleSearchResult::ConstructL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationHandleSearchResult::~COperationHandleSearchResult()
// ---------------------------------------------------------------------------
//
COperationHandleSearchResult::~COperationHandleSearchResult()
    {
    XSearchLogger::Log(_L("COperationHandleSearchResult::~COperationHandleSearchResult Started"));
   
    XSearchLogger::Log(_L("COperationHandleSearchResult::~COperationHandleSearchResult Completed"));
    }


// ---------------------------------------------------------------------------
// COperationHandleSearchResult::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchResult::ProcessL()
    {
	XSearchLogger::Log(_L("COperationHandleSearchResult::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	MXIMPBase* object = NULL;
	TBool force =  ETrue; 
	CXIMPOperationBase::GetObjCollection()->GetByType( object, MXIMPIdentity::KInterfaceId );
	
	if( object )
		{  
		iSearchId = TXIMPGetImpClassOrPanic< CXIMPIdentityImp  >::From( *object );	
		}
	else
		{
		User::Leave( KErrArgument );
		}
		
	object = NULL;
    CXIMPOperationBase::GetObjCollection()->GetByType( object, MXIMPObjectCollection::KInterfaceId );
    if( object )
        {
        
        MXIMPObjectCollection* objectCollection = static_cast<MXIMPObjectCollection*>(object->GetInterface(MXIMPObjectCollection::KInterfaceId,MXIMPBase::EPanicIfUnknown)) ;
        
       // CXIMPObjectCollectionImp* objectCollection = 
      	//				   TXIMPGetImpClassOrPanic< CXIMPObjectCollectionImp >::From( *object );
        CleanupStack::PushL( objectCollection );
       
        TLinearOrder< CSearchInfoImp > order( ( CSearchInfoImp::InfoLinearOrder ));
        TXIMPObjectMover< MSearchInfo, CSearchInfoImp >::
                                   MoveFromCollectionToArrayL( *objectCollection, *iSearchResults, order );
        CleanupStack::Pop( objectCollection ); 
        delete  objectCollection;
         
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
   	CProtocolSearchDataHostImp* srchProtocolHost = 
					static_cast <CProtocolSearchDataHostImp*> (iMyHost->GetProtocolHost(MProtocolSearchDataHost::KInterfaceId) );
	
	CSearchSubscriptionItem& searchSubsItem = 
			 srchProtocolHost->SearchDataCache().SearchSubscriptionItemLC() ;
    			

	searchSubsItem.SetSearchResults(iSearchResults); // ownership trasferred
	searchSubsItem.SetSearchIdL(*iSearchId);// ownership trasferred
	
	iSearchResults=NULL;
	iSearchId=NULL;
	
	searchSubsItem.SynthesiseSubscriptionEventToL( CXIMPOperationBase::GetContext(), force  );
	CleanupStack::PopAndDestroy(); // searchItem
	iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
	XSearchLogger::Log(_L("COperationHandleSearchResult::ProcessL Completed"));
	}

// ---------------------------------------------------------------------------
// COperationHandleSearchResult::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchResult::RequestCompletedL()
    {
    XSearchLogger::Log(_L("COperationHandleSearchResult::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
	XSearchLogger::Log(_L("COperationHandleSearchResult::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationHandleSearchResult::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandleSearchResult::Type() const
    {
    return NSearchOps::EHandleSearchResults;
    }


// End of file
