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
* Description:  Implementation of MImConversationImp
*
*/


#include "imconversationimp.h"
#include "ximpobjecthelpers.h"
#include "imoperationdefs.h"
//#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"
#include "imconversationinfoimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CImConversationImp::NewL()
// ---------------------------------------------------------------------------
//

CImConversationImp* CImConversationImp::NewL( MXIMPContextInternal& aContext )
	{
		XImLogger::Log(_L("CImConversationImp::NewL Started"));
		CImConversationImp* self = new( ELeave ) CImConversationImp( aContext );    
		XImLogger::Log(_L("CImConversationImp::NewL Completed"));
		return self;
	}


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CImConversationImp, 
                                     MImConversation )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CImConversationImp, 
                                           MImConversation )
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CImConversationImp::~CImConversationImp()
// ---------------------------------------------------------------------------
//	
CImConversationImp::~CImConversationImp()
    {
		XImLogger::Log(_L("CImConversationImp::~CImConversationImp Started - Completed"));
	}
// ---------------------------------------------------------------------------
// CImConversationImp::CImConversationImp()
// ---------------------------------------------------------------------------
//    
CImConversationImp::CImConversationImp(MXIMPContextInternal& aContext )
    : iContext( aContext )
    {
    }

// ---------------------------------------------------------------------------
// CImConversationImp::PackConvInfoLC()
// ---------------------------------------------------------------------------
//
HBufC8* CImConversationImp::PackConvInfoLC(const MImConversationInfo& aImMessage )
    {
    XImLogger::Log(_L("CImConversationImp::PackConvInfoLC Started"));
    const CImConversationInfoImp* convImp = 
    TXIMPGetImpClassOrPanic< const CImConversationInfoImp >::From( aImMessage );    
    HBufC8* pack = TXIMPObjectPacker< const CImConversationInfoImp >::PackL( *convImp );
    CleanupStack::PushL( pack );
    XImLogger::Log(_L("CImConversationImp::PackConvInfoLC Completed"));
    return pack;
    } 
 
// ---------------------------------------------------------------------------
// CImConversationImp::SendMessageL()
// ---------------------------------------------------------------------------
//    
TXIMPRequestId  CImConversationImp::SendMessageL( const MImConversationInfo& aImMessage)
   {
	XImLogger::Log(_L("CImConversationImp::SendMessageL Started"));
    HBufC8* infoPack = PackConvInfoLC( aImMessage );
    TXIMPRequestId reqId = iContext.QueueOperationL( NImOps::ESendImMessage, *infoPack );
    CleanupStack::PopAndDestroy( infoPack );
    XImLogger::Log(_L("CImConversationImp::SendMessageL Completed"));
    return reqId;
    }
    
// ---------------------------------------------------------------------------
// CImConversationImp::ForwardMessageL()
// ---------------------------------------------------------------------------
//    
    
//TXIMPRequestId  CImConversationImp::ForwardMessageL( const MImConversationInfo& /*aImMessage*/)
//	{
//  }

// ---------------------------------------------------------------------------
// CImConversationImp::SendMessageToGroupL()
// ---------------------------------------------------------------------------
//    
//TXIMPRequestId CImConversationImp::SendMessageToGroupL( const MImConversationInfo& /*aImMessage*/,
//			                                        const TDesC* /*aGroupId*/ )
//	{
//	}

// ---------------------------------------------------------------------------
// CImConversationImp::BlockUsersL()
// ---------------------------------------------------------------------------
//	    
//TXIMPRequestId CImConversationImp::BlockUsersL(const MDesCArray* /*aUserIds*/ )
//    {
//    }

// ---------------------------------------------------------------------------
// CImConversationImp::UnBlockUsersL()
// ---------------------------------------------------------------------------
//    
//TXIMPRequestId CImConversationImp::UnBlockUsersL(const MDesCArray* /*aUserIds*/)
//    {
//	}

// ---------------------------------------------------------------------------
// CImConversationImp::GetBlockedUsersListL()
// ---------------------------------------------------------------------------
//			                                           
//TXIMPRequestId CImConversationImp::GetBlockedUsersListL(void )
//	{
//	}

// ---------------------------------------------------------------------------
// CImConversationImp::GetMessageListL()
// ---------------------------------------------------------------------------
//	
//TXIMPRequestId CImConversationImp::GetMessageListL( const MXIMPIdentity& /*aImMessageId*/,
//			    								   	   const TDesC* /*aGroupId*/,
//			    								   	   const TInt /*aMessageCount*/,
//			                                           TBool /*aDeliveryReportWanted*/)
//	{
//	}

// ---------------------------------------------------------------------------
// CImConversationImp::RejectMessageL()
// ---------------------------------------------------------------------------
//	                                        
//TXIMPRequestId CImConversationImp::RejectMessageL( const MXIMPIdentity& /*aImMessageId*/)
//	{
//	}

// End of file
