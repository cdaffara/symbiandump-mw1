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
* Description:  Concrete im receive message operation
 *
*/


#include <badesca.h>
#include <ximpprotocolconnection.h>
#include <protocolimfeatures.h>
#include <protocolimconversation.h>
#include "imconversationinfoimp.h"
#include "operationreceivemessage.h"
//#include "ximpoperationbase.h"
//#include "ximpidentityimp.h"
#include "conversationsubscriptionitem.h"
#include "imdatacacheimp.h"

#include "ximpobjecthelpers.h"
#include "ximprestrictedobjectcollection.h"
#include "ximphost.h"
//#include "ximpstatusimp.h"
//#include "ximprequestcompleteeventimp.h"
#include "protocolimdatahostimp.h"
#include "imlogutils.h"

#include "protocolimdatahost.h"

//#include "ximpsubscriptionitem.h"
//#include "ximppsccontextimp.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationReceiveMessage::COperationReceiveMessage()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationReceiveMessage::COperationReceiveMessage()
    {
    XImLogger::Log(_L("COperationReceiveMessage::COperationReceiveMessage Started"));
    XImLogger::Log(_L("COperationReceiveMessage::COperationReceiveMessage Completed"));
    }


// ---------------------------------------------------------------------------
// COperationReceiveMessage::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationReceiveMessage::ConstructL( const TDesC8& /*aParamPck */)
    {
    XImLogger::Log(_L("COperationReceiveMessage::ConstructL Started"));   
    XImLogger::Log(_L("COperationReceiveMessage::ConstructL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationReceiveMessage::~COperationReceiveMessage()
// ---------------------------------------------------------------------------
//
COperationReceiveMessage::~COperationReceiveMessage()
    {
    XImLogger::Log(_L("COperationReceiveMessage::~COperationReceiveMessage Started"));
    delete iConvInfo;
    XImLogger::Log(_L("COperationReceiveMessage::~COperationReceiveMessage Completed"));
    }


// ---------------------------------------------------------------------------
// COperationReceiveMessage::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationReceiveMessage::ProcessL()
    {
	XImLogger::Log(_L("COperationReceiveMessage::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	MXIMPBase* object = NULL;
	TBool force =  ETrue; 
	
	//iObjCollection->GetByType( object, MImConversationInfo::KInterfaceId );
	MXIMPRestrictedObjectCollection *objCollection = CXIMPOperationBase::GetObjCollection() ;
	
	objCollection->GetByType( object, MImConversationInfo::KInterfaceId );
	
	if( object )
		{  
		iConvInfo = TXIMPGetImpClassOrPanic< CImConversationInfoImp >::From( *object );	
		}
	else
		{
		User::Leave( KErrArgument );
		}
		
			
	//CConversationSubscriptionItem& conversationItem = 
	//iMyHost->ImDataAccess().ImDataCache().ConversationSubscriptionItemLC();
	
	//CConversationSubscriptionItem& conversationItem = 
	//	iMyHost->GetProtocolHost(0).ImDataCache().ConversationSubscriptionItemLC() ;
	
	MProtocolImDataHost* imProtocolHost = 
					static_cast <MProtocolImDataHost*> (iMyHost->GetProtocolHost(MProtocolImDataHost::KInterfaceId) );
	
	CConversationSubscriptionItem& conversationItem = 
			imProtocolHost->ImDataCache().ConversationSubscriptionItemLC() ;
	
	conversationItem.SetConversationInfoL( iConvInfo ); 
	
	iConvInfo = NULL; // set to NULL since ownership is transfered

	//conversationItem.SynthesiseSubscriptionEventToL( iContext, force );
	conversationItem.SynthesiseSubscriptionEventToL( CXIMPOperationBase::GetContext(), force );
	
	CleanupStack::PopAndDestroy(); // conversationItem
	iMyHost->HandleAdaptationRequestCompleted( iReqId, KErrNone );
	XImLogger::Log(_L("COperationReceiveMessage::ProcessL Completed"));
	}

// ---------------------------------------------------------------------------
// COperationReceiveMessage::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationReceiveMessage::RequestCompletedL()
    {
    XImLogger::Log(_L("COperationReceiveMessage::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
	XImLogger::Log(_L("COperationReceiveMessage::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationReceiveMessage::Type()
// ---------------------------------------------------------------------------
//
TInt COperationReceiveMessage::Type() const
    {
    return NImOps::EHandleReceiveMessage;
    }


// End of file
