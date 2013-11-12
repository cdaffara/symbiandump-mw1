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
* Description:  Concrete handle search keys from protocol plugin operation
 *
*/


#include <badesca.h>
#include <ximpprotocolconnection.h>
#include <protocolsearchfeature.h>

#include <ximppsccontext.h>

#include "searchinfoimp.h"
#include "operationhandlesearchkeys.h"
#include "ximpoperationbase.h"

#include "searchdatacacheimp.h"

#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "ximphost.h"

#include "protocolsearchdatahostimp.h"
#include "searchlogutils.h"
#include "searchdatacacheimp.h"

#include "ximpsubscriptionitem.h"
#include "searchkeyinfoimp.h"
#include "searchkeyssubscriptionitem.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::COperationHandleSearchKeys()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationHandleSearchKeys::COperationHandleSearchKeys()
    {
    XSearchLogger::Log(_L("COperationHandleSearchKeys::COperationHandleSearchKeys Started"));
    XSearchLogger::Log(_L("COperationHandleSearchKeys::COperationHandleSearchKeys Completed"));
    }


// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchKeys::ConstructL( const TDesC8& /*aParamPck */)
    {
    XSearchLogger::Log(_L("COperationHandleSearchKeys::ConstructL Started")); 
    iKeyInfoArray=new ( ELeave ) RSearchKeyInfoImpArray;  
    XSearchLogger::Log(_L("COperationHandleSearchKeys::ConstructL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::~COperationHandleSearchKeys()
// ---------------------------------------------------------------------------
//
COperationHandleSearchKeys::~COperationHandleSearchKeys()
    {
    XSearchLogger::Log(_L("COperationHandleSearchKeys::~COperationHandleSearchKeys Started"));
   
    XSearchLogger::Log(_L("COperationHandleSearchKeys::~COperationHandleSearchKeys Completed"));
    }


// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchKeys::ProcessL()
    {
	XSearchLogger::Log(_L("COperationHandleSearchKeys::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	MXIMPBase* object = NULL;
	TBool force =  ETrue; 
	CXIMPOperationBase::GetObjCollection()->GetByType( object, MXIMPObjectCollection::KInterfaceId );
	
	if( object )
		{  
		MXIMPObjectCollection* objectCollection = static_cast<MXIMPObjectCollection*>(object->GetInterface(MXIMPObjectCollection::KInterfaceId,MXIMPBase::EPanicIfUnknown)) ;
        
        CleanupStack::PushL( objectCollection );
       
        TLinearOrder< CSearchKeyInfoImp > order( ( CSearchKeyInfoImp::LinearOrder ));
        TXIMPObjectMover< MSearchKeyInfo, CSearchKeyInfoImp >::
                                   MoveFromCollectionToArrayL( *objectCollection, *iKeyInfoArray, order );
        CleanupStack::Pop( objectCollection ); 
        delete  objectCollection;
		}
	else
		{
		User::Leave( KErrArgument );
		}
		
    
 	CProtocolSearchDataHostImp* srchProtocolHost = 
					static_cast <CProtocolSearchDataHostImp*> (iMyHost->GetProtocolHost(MProtocolSearchDataHost::KInterfaceId) );
	
	CSearchKeysSubscriptionItem& searchKeySubsItem = 
			 srchProtocolHost->SearchDataCache().SearchKeysSubscriptionItemLC() ;
    			

	searchKeySubsItem.SetKeyInfoArray( iKeyInfoArray ); // ownership trasferred	
	iKeyInfoArray=NULL;

	searchKeySubsItem.SynthesiseSubscriptionEventToL( CXIMPOperationBase::GetContext(), force  );
	CleanupStack::PopAndDestroy(); // searchKeySubsItem 
	iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
	XSearchLogger::Log(_L("COperationHandleSearchKeys::ProcessL Completed"));
	}

// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationHandleSearchKeys::RequestCompletedL()
    {
    XSearchLogger::Log(_L("COperationHandleSearchKeys::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
	XSearchLogger::Log(_L("COperationHandleSearchKeys::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationHandleSearchKeys::Type()
// ---------------------------------------------------------------------------
//
TInt COperationHandleSearchKeys::Type() const
    {
    return NSearchOps::EHandleSearchKeys;
    }


// End of file
