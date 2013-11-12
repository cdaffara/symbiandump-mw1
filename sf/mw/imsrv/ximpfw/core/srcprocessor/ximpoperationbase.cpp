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
* Description:  Base class for operations methods
 *
*/



#include "ximprequestcompleteeventimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpoperationbase.h"
#include "ximpstatusimp.h"
#include "ximppanics.h"
#include "ximppsccontextimp.h"
#include "ximphost.h"

// -----------------------------------------------------------------------------
// CXIMPOperationBase::CXIMPOperationBase()
// C++ default constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationBase::CXIMPOperationBase() : iServerOperation(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::BaseConstructL
// Base class construction
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::BaseConstructL( TXIMPRequestId aRequestId, 
                                            CXIMPPscContext* aContext )
    {
    iStatusObj = CXIMPStatusImp::NewL();    
    iObjCollection = CXIMPRestrictedObjectCollectionImp::NewL();
    iReqId = aRequestId;
    SetOpL( aContext );    

    CreateRequestCompleteEventL();
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::CreateRequestCompleteEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::CreateRequestCompleteEventL()
    {
    // Default value for request complete event to handle low memory situations.
    iReqCompleteEvent = CXIMPRequestCompleteEventImp::NewLC( iReqId, KErrNoMemory );
    CleanupStack::Pop(); // iReqCompleteEvent
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::~CXIMPOperationBase()
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPOperationBase::~CXIMPOperationBase()
    {
    
    GetObjCollection();
    GetContext();
    Type();
    
    delete iStatusObj;
    delete iObjCollection;
    
    if ( iContext )
        {
        iContext->Close();
        }
    delete iReqCompleteEvent;
    if( iMyHost )
        {
        iMyHost->Close();
        }
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetHost()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetHost( MXIMPHost& aMyHost )
    {
    if( iMyHost )
        {
        iMyHost->Close();
        }
    iMyHost = &aMyHost;
    iMyHost->Open(); // CSI: 65 # Does not fail.
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::ProcessL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::ProcessL()
    {
    __ASSERT_ALWAYS( iMyHost, User::Panic( NXIMPPrivPanic::KCategory,
                                           NXIMPPrivPanic::EHostNotFound ) );
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::RequestCompletedL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::RequestCompletedL()
    {
    iReqCompleteEvent->CompletionResultImp().SetResultCode( iStatusObj->ResultCode() );
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::GetReqCompleteEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPRequestCompleteEventImp* CXIMPOperationBase::GetReqCompleteEvent()
    {
    return iReqCompleteEvent;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::Type()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPOperationBase::Type() const
    {
    return NXIMPOps::EPrNone; 
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetOp()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetOpL( CXIMPPscContext* aContext )
    {
    if( aContext )
        {
        User::LeaveIfError( aContext->Open() );
        }
    if( iContext )
        {
        iContext->Close();
        }
    iContext = aContext;
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetReqId()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetReqId( TXIMPRequestId aRequestId )
    {
    iReqId = aRequestId;
    iReqCompleteEvent->SetRequestId( iReqId );
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetResultCode()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetResultCode( TInt aResultCode )
    {
    iStatusObj->SetResultCode( aResultCode );
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetResultCode()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetStatus( MXIMPStatus* aStatus )
    {
    CXIMPStatusImp* status = NULL;
    if( aStatus )
        {
        status =
        TXIMPGetImpClassOrPanic< CXIMPStatusImp >::From( *aStatus );
        delete iStatusObj;
        iStatusObj = status;
        }    
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetObjectCollection()
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetObjectCollection( MXIMPBase* aCollection )
    {
    CXIMPRestrictedObjectCollectionImp* objectCollection = NULL;
    if( aCollection )
        {
        objectCollection =
        TXIMPGetImpClassOrPanic< CXIMPRestrictedObjectCollectionImp >::From( *aCollection );
        delete iObjCollection;
        iObjCollection = objectCollection;
        }
    
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::Context()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPPscContext* CXIMPOperationBase::Context()
    {
    return iContext;
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::RequestId
// -----------------------------------------------------------------------------
//
EXPORT_C TXIMPRequestId CXIMPOperationBase::RequestId() const
    {
    return iReqId;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::BeginOperationL
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::BeginOperation()
    {    
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::EndOperationL
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::EndOperation()
    {
    
    }
    
// -----------------------------------------------------------------------------
// CXIMPOperationBase::Status
// -----------------------------------------------------------------------------
//
EXPORT_C MXIMPStatus& CXIMPOperationBase::Status() const
    {
    return *iStatusObj;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationBase::SetServerOperation
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPOperationBase::SetServerOperation()
	{
	iServerOperation = ETrue;
	}
	
// -----------------------------------------------------------------------------
// CXIMPOperationBase::IsServerOperation
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPOperationBase::IsServerOperation() const
	{
	return iServerOperation;
	}

// -----------------------------------------------------------------------------
// CXIMPOperationBase::GetContext
// -----------------------------------------------------------------------------
//
EXPORT_C MXIMPPscContext* CXIMPOperationBase::GetContext()
{
	return iContext ;
}

// -----------------------------------------------------------------------------
// CXIMPOperationBase::GetObjCollection
// -----------------------------------------------------------------------------
//
EXPORT_C MXIMPRestrictedObjectCollection* CXIMPOperationBase::GetObjCollection()
{
	return iObjCollection ;
}
    
// end of file
