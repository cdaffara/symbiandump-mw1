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
* Description:  Implementation for managing cached Invite information.
*
*/


#include <ximpobjectcollection.h>
#include <ximpidentity.h>
#include <ximpdatasubscriptionstate.h>
#include <ximpstatus.h>
//#include "ximprestrictedobjectcollectionimp.h"
//#include "ximpoperationbase.h"
//#include "ximpglobals.h"
//#include "ximpoperationfactory.h"
#include "ximphost.h"
#include "imdatacacheimp.h"
#include "imoperationdefs.h"
//#include "ximptrace.h"
#include "imlogutils.h"
#include "protocolimdatahostimp.h"
#include "protocolimconversationdatahostimp.h"
#include <imconversationinfo.h>


// ============================ MEMBER FUNCTIONS ============================= 


// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::CProtocolImConversationDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolImConversationDataHostImp::CProtocolImConversationDataHostImp( MXIMPHost& aHost )
:iHost( aHost )
    {
    }


// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CProtocolImConversationDataHostImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CProtocolImConversationDataHostImp* CProtocolImConversationDataHostImp::NewL( MXIMPHost& aHost )
    {
    CProtocolImConversationDataHostImp* self = new( ELeave ) CProtocolImConversationDataHostImp( aHost );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CProtocolImConversationDataHostImp,
                                     MProtocolImConversationDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CProtocolImConversationDataHostImp,
                                           MProtocolImConversationDataHost )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::~CProtocolImConversationDataHostImp()
// ---------------------------------------------------------------------------
//
CProtocolImConversationDataHostImp::~CProtocolImConversationDataHostImp()
    {
    }

// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleSentMessageL()
// ---------------------------------------------------------------------------
//
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleSentMessageL(MXIMPIdentity* /*aMessageId*/)
//    {
//    }
    
// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleNewTextMessageL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CProtocolImConversationDataHostImp::HandleNewTextMessageL( MImConversationInfo* aImMessage)
    {
    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleNewTextMessageL Started"));
	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleNewTextMessageL Completed"));
    return iHost.AddNewOperationL( NImOps::EHandleReceiveMessage, aImMessage );
    }
    
// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleNewContentMessageL()
// ---------------------------------------------------------------------------
//  
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleNewContentMessageL(  MXIMPIdentity* /*aMessageId*/,
//                                           const TDesC& /*aSender*/,   
//                                           const TDesC& /*aScreenName*/, 
//                                           const MDesCArray& /*aRecipients*/,
//                                           const MDesCArray& /*aScreenNames*/,                  
//                                           const TDesC& /*aContentType*/, 
//                                           const TDesC8& /*aContent*/)
//    {
//    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleNewContentMessageL Started"));
//	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleNewContentMessageL Completed"));		
//    }
    
// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleBlockedListResponseL()
// ---------------------------------------------------------------------------
//  
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleBlockedListResponseL( const MDesCArray* /*aBlockedList*/,
//                                              TBool /*aBlockedListInUse*/,
//                                              const MDesCArray* /*aGrantedList*/,
//                                              TBool /*aGrantedListInUse*/)
//    {
//    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleBlockedListResponseL Started"));
//	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleBlockedListResponseL Completed"));		
//    }
    
// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL()
// ---------------------------------------------------------------------------
//  
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL(MXIMPIdentity* /*aMessageId*/)
//    {
//    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Started"));
//	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Completed"));		
//    }

// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleGetMessageListL()
// ---------------------------------------------------------------------------
//     
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleGetMessageListL( /*MImFtpPluginObjectCollection* aMessageList */)
//{
//    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Started"));
//	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Completed"));		
	
//}
// ---------------------------------------------------------------------------
// CProtocolImConversationDataHostImp::HandleRejectMessageL()
// ---------------------------------------------------------------------------
//  
//TXIMPRequestId CProtocolImConversationDataHostImp::HandleRejectMessageL( MXIMPIdentity* /*aMessageId*/)
//{
//    XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Started"));
//	XImLogger::Log(_L("CProtocolImConversationDataHostImp::HandleForwardMessageCompletionL Completed"));		
	
//}

//End of file
