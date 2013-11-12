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
* Description:  MXIMPRequestCompleteEvent API object implementation.
*
*/


#include <s32strm.h>

#include "ximprequestcompleteeventimp.h"
#include "ximpstatusimp.h"
#include "ximpapidataobjfactory.h"
#include "ximpapidataobjbase.h"


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPRequestCompleteEventImp* CXIMPRequestCompleteEventImp::NewL()
    {
    TXIMPRequestId nullId = TXIMPRequestId::Null();
    
    CXIMPRequestCompleteEventImp* self =
            new( ELeave ) CXIMPRequestCompleteEventImp( nullId );
    CleanupStack::PushL( self );
    self->ConstructL( KErrNone );
    CleanupStack::Pop( self );
    return self;
    }



// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPRequestCompleteEventImp* CXIMPRequestCompleteEventImp::NewLC(
       TXIMPRequestId& aRequestId, TInt aDefaultErrorValue )
    {
    CXIMPRequestCompleteEventImp* self =
            new( ELeave ) CXIMPRequestCompleteEventImp( aRequestId );
    CleanupStack::PushL( self );
    self->ConstructL( aDefaultErrorValue );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::NewLC()
// Instantiation method for event automation.
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase*
    CXIMPRequestCompleteEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CXIMPRequestCompleteEventImp* self = new( ELeave ) CXIMPRequestCompleteEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::~CXIMPRequestCompleteEventImp()
// ---------------------------------------------------------------------------
//
CXIMPRequestCompleteEventImp::~CXIMPRequestCompleteEventImp()
    {
    delete iResult;

    // remove the owned objects
    for ( TInt i = 0; i < iParamArray.Count(); i++ )
        {
        SXIMPReqParam param = iParamArray[i];
        delete param.iParam;
        param.iParam = NULL;
        }
    iParamArray.Close();
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::CXIMPRequestCompleteEventImp()
// ---------------------------------------------------------------------------
//
CXIMPRequestCompleteEventImp::CXIMPRequestCompleteEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::CXIMPRequestCompleteEventImp()
// ---------------------------------------------------------------------------
//
CXIMPRequestCompleteEventImp::CXIMPRequestCompleteEventImp(
    TXIMPRequestId& aRequestId ) :
    iReqId( aRequestId )
    {
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPRequestCompleteEventImp::ConstructL( TInt aDefaultErrorValue )
    {
    iResult = CXIMPStatusImp::NewL();
    iResult->SetResultCode( aDefaultErrorValue );
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPRequestCompleteEventImp::ConstructL( RReadStream& aStream )
    {
    // normal params
    TPckgBuf< TXIMPRequestId > bufReqId;
    aStream.ReadL( bufReqId );
    iReqId = bufReqId();

    iResult = CXIMPStatusImp::NewL();
    iResult->InternalizeL( aStream );

    // the array with variable parameters
    XIMPApiDataObjFactory::InternalizeL( aStream, iParamArray );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPRequestCompleteEventImp,
                                     MXIMPRequestCompleteEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPRequestCompleteEventImp,
                                           MXIMPRequestCompleteEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CXIMPRequestCompleteEventImp,
                                      MXIMPRequestCompleteEvent )

// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::RequestId()
// ---------------------------------------------------------------------------
//
const TXIMPRequestId& CXIMPRequestCompleteEventImp::RequestId() const
    {
    return iReqId;
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::CompletionResult()
// ---------------------------------------------------------------------------
//
const MXIMPStatus& CXIMPRequestCompleteEventImp::CompletionResult() const
    {
    return *iResult;
    }



// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::LookupCompletionDataByType()
// ---------------------------------------------------------------------------
//
const MXIMPBase* CXIMPRequestCompleteEventImp::LookupCompletionDataByType(
    TInt32 aInterfaceId ) const
    {
    const TInt count = iParamArray.Count();
    for( TInt i = 0; i < count; i++ )
        {
        if( iParamArray[ i ].iParamType == aInterfaceId )
            {
            return &iParamArray[ i ].iParam->Base();
            }
        }

    return NULL;
    }




// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::SetRequestId()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPRequestCompleteEventImp::SetRequestId( const TXIMPRequestId& aRequestId )
    {
    iReqId = aRequestId;
    }

// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CXIMPRequestCompleteEventImp::EqualsContent(
    const CXIMPApiEventBase& aOtherInstance ) const
    {
    const CXIMPRequestCompleteEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPRequestCompleteEventImp >::From(
                                                        aOtherInstance.Base() );

    TBool same = ETrue;

    TBool x = ( iResult->ResultCode() == tmp->iResult->ResultCode() );
    same &= x;

    x = ( 0 == iResult->ProtocolResultDescription().Compare(
                tmp->iResult->ProtocolResultDescription() ) );
    same &= x;

    // check parameter arrays for equality
    TInt myCount = iParamArray.Count();
    TInt otherCount = tmp->iParamArray.Count();

    x = myCount == otherCount;
    same &= x;

    if ( x )
        {
        // don't do potentially expensive operation unless arrays are
        // have same number of elements

        for ( TInt i = 0; i < myCount; i++ )
            {
            // assumes the parameters are IN ORDER
            x = iParamArray[ i ].iParam->EqualsContent( *( tmp->iParamArray[ i ].iParam ) );
            same &= x;
            }
        }

    return same;
    }

// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPRequestCompleteEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    // normal params
    TPckgBuf< TXIMPRequestId > bufReqId( iReqId );
    aStream.WriteL( bufReqId );

    iResult->ExternalizeL( aStream );

    // the array with variable parameters
    XIMPApiDataObjFactory::ExternalizeL( aStream, iParamArray );
    }

// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::AppendParamL
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPRequestCompleteEventImp::AppendParamL( CXIMPApiDataObjBase* aObj )
    {
    SXIMPReqParam param;
    param.iParamType = aObj->Base().GetInterfaceId();
    param.iParam = aObj;
    iParamArray.AppendL( param );
    }


// ---------------------------------------------------------------------------
// CXIMPRequestCompleteEventImp::CompletionResultImp()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPStatusImp& CXIMPRequestCompleteEventImp::CompletionResultImp()
    {
    return *iResult;
    }

// End of file

