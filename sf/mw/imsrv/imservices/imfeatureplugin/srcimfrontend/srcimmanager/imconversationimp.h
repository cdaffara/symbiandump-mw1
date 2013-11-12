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

#ifndef CIMCONVERSATIONIMP_H
#define CIMCONVERSATIONIMP_H

#include <imconversation.h>
#include "imlogutils.h"
#include "imapiobjbase.h"
#include "ximpapiobjbase.h"
// FORWARD DECLARATIONS
class MXIMPContextInternal;

/**
 * MImConversation API object implementation.
 *
 * @lib Immanager.dll
 *
 */
NONSHARABLE_CLASS( CImConversationImp ): public CXIMPApiObjBase,public MImConversation
    {
    public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CONVERSATIONIMP };


public:
    static CImConversationImp* NewL( MXIMPContextInternal& aContext );
    ~CImConversationImp();


public:
    CImConversationImp( MXIMPContextInternal& aContext );


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
  XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MImConversation

   		TXIMPRequestId  SendMessageL( const MImConversationInfo& aImMessage);
	    //TXIMPRequestId  ForwardMessageL( const MImConversationInfo& aImMessage);
	    //TXIMPRequestId SendMessageToGroupL( const MImConversationInfo& aImMessage,
		//	                                        const TDesC* aGroupId );
	    
	    //TXIMPRequestId BlockUsersL(const MDesCArray* aUserIds );
	    
	    //TXIMPRequestId UnBlockUsersL(const MDesCArray* aUserIds );
				                                           
		//TXIMPRequestId GetBlockedUsersListL(void );
		
		//TXIMPRequestId GetMessageListL( const MXIMPIdentity& aImMessageId,
		//		    								   	   const TDesC* aGroupId,
		//		    								   	   const TInt aMessageCount,
		//		                                           TBool aDeliveryReportWanted);
		                                        
    	//TXIMPRequestId RejectMessageL( const MXIMPIdentity& aImMessageId);
													
	
private: //Helpers

	HBufC8* PackConvInfoLC(const MImConversationInfo& aImMessage );
private: // data

    /**
     * Context session proxy for accessing context server.
     * Ref.
     */
    MXIMPContextInternal& iContext;


    };


#endif // CIMCONVERSATIONIMP_H
