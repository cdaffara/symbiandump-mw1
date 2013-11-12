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
* Description:  MImConversationEvent API object implementation.
 *
*/

#include "imconversationeventimp.h"
#include "ximpapiobjbase.h"
#include "ximpapieventbase.h"
#include "imconversationinfoimp.h"
#include <ximpbase.h>
//#include "ximpobjecthelpers.h"
// ---------------------------------------------------------------------------
// CImConversationEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CImConversationEventImp* CImConversationEventImp::NewLC(CImConversationInfoImp* aConvinfo)
    {
    CImConversationEventImp* self = new( ELeave ) CImConversationEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aConvinfo );
    return self;
    }

// ---------------------------------------------------------------------------
// CImConversationEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CImConversationEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CImConversationEventImp* self = new( ELeave ) CImConversationEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }
// ---------------------------------------------------------------------------
// CImConversationEventImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CImConversationEventImp::EqualsContent( 
    const CXIMPApiEventBase& /*aOtherInstance */) const 
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase
// CImConversationEventImp::ExternalizeL
// ---------------------------------------------------------------------------
//
void CImConversationEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iConvInfo->ExternalizeL( aStream );
    }


// ---------------------------------------------------------------------------
// CImConversationEventImp::~CImConversationEventImp()
// ---------------------------------------------------------------------------
//
CImConversationEventImp::~CImConversationEventImp()
    {
   		delete iConvInfo;
    }


// ---------------------------------------------------------------------------
// CImConversationEventImp::CImConversationEventImp()
// ---------------------------------------------------------------------------
//
CImConversationEventImp::CImConversationEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CImConversationEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CImConversationEventImp::ConstructL(CImConversationInfoImp* aConvinfo)
    {
    iConvInfo = aConvinfo; // ownership is transfered here, need to delete iConvInfo
    }


// ---------------------------------------------------------------------------
// CImConversationEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CImConversationEventImp::ConstructL( RReadStream& aStream )
    {
    iConvInfo=CImConversationInfoImp::NewL();
    iConvInfo->InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CImConversationEventImp,
                                     MImConversationEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CImConversationEventImp,
                                           MImConversationEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this ) 
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CImConversationEventImp,
                                      MImConversationEvent )




// ---------------------------------------------------------------------------
// CImConversationEventImp::InternalizeL( )
// ---------------------------------------------------------------------------
//

void CImConversationEventImp::InternalizeL(  RReadStream& aStream ) 
    { 
    iConvInfo->InternalizeL( aStream );     
    }

// ---------------------------------------------------------------------------
// CImConversationEventImp::NewTextMessage()
// ---------------------------------------------------------------------------
//  	 
const MImConversationInfo& CImConversationEventImp:: NewTextMessage( TInt /*aIndex*/ ) const 
	{
	return *iConvInfo;
	}
    
// ---------------------------------------------------------------------------
// CImConversationEventImp::NewTextMessageCount()
// ---------------------------------------------------------------------------
//    
    
TInt CImConversationEventImp::NewTextMessageCount() const
	 {
	 return 1;	// since the count is always 1 
	 }
// End of file

