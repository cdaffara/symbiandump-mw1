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
* Description:  MXIMPContextStateEvent API object implementation.
 *
*/

#include <s32strm.h>

#include "ximpcontextstateimp.h"
#include "ximpstatusimp.h"
#include "ximpcontextstateeventimp.h"


namespace
    {
    enum TStatusObjectExistency
        {
        ENoStatusObject = 0,
        EHasStatusObject = 1
        };
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextStateEventImp* CXIMPContextStateEventImp::NewLC()
    {
    CXIMPContextStateEventImp* self = new( ELeave ) CXIMPContextStateEventImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextStateEventImp* CXIMPContextStateEventImp::NewL()
    {
    CXIMPContextStateEventImp* self = CXIMPContextStateEventImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::NewFromStreamLC()
// Instantiation method for event automation.
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CXIMPContextStateEventImp::NewFromStreamLC(
        RReadStream& aStream )
    {
    CXIMPContextStateEventImp* self = new( ELeave ) CXIMPContextStateEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::~CXIMPContextStateEventImp()
// ---------------------------------------------------------------------------
//
CXIMPContextStateEventImp::~CXIMPContextStateEventImp()
    {
    delete iState;
    delete iStateChangeReason;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::CXIMPContextStateEventImp()
// ---------------------------------------------------------------------------
//
CXIMPContextStateEventImp::CXIMPContextStateEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextStateEventImp::ConstructL()
    {
    iState = CXIMPContextStateImp::NewL();
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CXIMPContextStateEventImp::ConstructL( RReadStream& aStream )
    {
    iState = CXIMPContextStateImp::NewL();
    iState->InternalizeL( aStream );


    TStatusObjectExistency exists =
             ( TStatusObjectExistency ) aStream.ReadInt8L();

    if( exists  == EHasStatusObject )
        {
        iStateChangeReason = CXIMPStatusImp::NewL();
        iStateChangeReason->InternalizeL( aStream );
        }
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CXIMPContextStateEventImp,
                                     MXIMPContextStateEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CXIMPContextStateEventImp,
                                           MXIMPContextStateEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this ) 
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CXIMPContextStateEventImp,
                                      MXIMPContextStateEvent )

// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CXIMPContextStateEventImp::EqualsContent( 
    const CXIMPApiEventBase& aOtherInstance ) const 
    {
    const CXIMPContextStateEventImp* tmp =
    TXIMPGetImpClassOrPanic< const CXIMPContextStateEventImp>::From( 
                                                       aOtherInstance.Base() );
    
    TBool same = ETrue;

    TBool x = iState->EqualsContent(*(tmp->iState));
    same &= x;

	
	if( (!iStateChangeReason) && (!tmp->iStateChangeReason) )
		{
		//both are NULL
		same &= ETrue;
		}
	else if ((iStateChangeReason) && (tmp->iStateChangeReason))
		{
		//both not NULL, contents need to be checked
    x = iStateChangeReason->ResultCode() == tmp->iStateChangeReason->ResultCode();
    same &= x;

    x = 0 == iStateChangeReason->ProtocolResultDescription().Compare(
                tmp->iStateChangeReason->ProtocolResultDescription() );
    same &= x;
		}
	else
		{
		// One of them is NULL
		same &= EFalse;		
		}

    return same;
    }

// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CXIMPContextStateEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iState->ExternalizeL( aStream );

    if( iStateChangeReason )
        {
        aStream.WriteInt8L( EHasStatusObject );
        iStateChangeReason->ExternalizeL( aStream );
        }
    else
        {
        aStream.WriteInt8L( ENoStatusObject );
        }
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::StateChangeReason()
// ---------------------------------------------------------------------------
//
const MXIMPStatus* CXIMPContextStateEventImp::StateChangeReason() const
    {
    return iStateChangeReason;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::ContextState()
// ---------------------------------------------------------------------------
//
const MXIMPContextState&
    CXIMPContextStateEventImp::ContextState() const
    {
    return *iState;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::ContextStateImp()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPContextStateImp&
    CXIMPContextStateEventImp::ContextStateImp()
    {
    return *iState;
    }


// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::SetStateChangeReasonImp()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPContextStateEventImp::SetStateChangeReasonImp(
    CXIMPStatusImp* aStateChangeReason )
    {
    delete iStateChangeReason;
    iStateChangeReason = aStateChangeReason; 
    }
    

// ---------------------------------------------------------------------------
// CXIMPContextStateEventImp::StateChangeReasonImp()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPStatusImp*
    CXIMPContextStateEventImp::StateChangeReasonImp()
    {
    return iStateChangeReason;
    }


// End of file

