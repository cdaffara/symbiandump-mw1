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
* Description:  Concrete im send message operation
 *
*/



#include <ximpprotocolconnection.h>
#include <cprotocolimfeatures.h>
#include <protocolimconversation.h>
#include "imconversationinfoimp.h"
#include "operationsendmessage.h"
#include "ximpobjecthelpers.h"
//#include "ximprestrictedobjectcollectionimp.h"
//#include "ximphost.h"
//#include "ximpstatusimp.h"
//#include "ximprequestcompleteeventimp.h"
#include "protocolimdatahostimp.h"
#include "imlogutils.h"
#include <badesca.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// COperationSendMessage::COperationSendMessage()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationSendMessage::COperationSendMessage()
    {
    }
// ---------------------------------------------------------------------------
// COperationSendMessage::ConstructL
// ---------------------------------------------------------------------------
//
void COperationSendMessage::ConstructL( const TDesC8& aParamPck )
    {
    XImLogger::Log(_L("COperationSendMessage::ConstructL Started"));
    CImConversationInfoImp* convInfo = CImConversationInfoImp::NewLC();
    TXIMPObjectPacker< CImConversationInfoImp >::UnPackL( *convInfo,
            aParamPck );
    CleanupStack::Pop( convInfo );
    iConvInfo = convInfo;
    XImLogger::Log(_L("COperationSendMessage::ConstructL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSendMessage::~COperationSendMessage()
// ---------------------------------------------------------------------------
//
COperationSendMessage::~COperationSendMessage()
    {
    XImLogger::Log(_L("COperationSendMessage::~COperationSendMessage Started"));
    delete iConvInfo;
    XImLogger::Log(_L("COperationSendMessage::~COperationSendMessage Completed"));
    }


// ---------------------------------------------------------------------------
// COperationSendMessage::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationSendMessage::ProcessL()
    {
	XImLogger::Log(_L("COperationSendMessage::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
	
	// Call to Adaptation.
	//MProtocolImConversation& imconversation = iMyHost->GetConnection().ProtocolImFeatures()->ImConversation();
	
	
	// Pankaj : added to support dynamic resolution
	CProtocolImFeatures* imProtocolFeature =static_cast< CProtocolImFeatures*> (iMyHost->GetConnection().GetProtocolInterface(CProtocolImFeatures::KInterfaceId));
	
	MProtocolImConversation& imconversation = imProtocolFeature->ImConversation() ; 
	
	
 	imconversation.DoSendMessageL(*iConvInfo,iReqId );
    XImLogger::Log(_L("COperationSendMessage::ProcessL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSendMessage::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationSendMessage::RequestCompletedL()
    {
    XImLogger::Log(_L("COperationSendMessage::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
    XImLogger::Log(_L("COperationSendMessage::RequestCompletedL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationSendMessage::Type()
// ---------------------------------------------------------------------------
//
TInt COperationSendMessage::Type() const
    {
    return NImOps::ESendImMessage;
    }


// End of file
