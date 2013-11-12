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

#ifndef CPROTOCOLIMCONVERSATIONDATAHOSTIMP_H
#define CPROTOCOLIMCONVERSATIONDATAHOSTIMP_H

#include <protocolimconversationdatahost.h>

#include "ximpapiobjbase.h"
#include "imapiobjbase.h"
//#include "ximpitemparent.h"
#include "imtypehelpers.h"
//#include "ximpoperationdefs.h"

class MXIMPHost;
class MXIMPObjectCollection;
/**
 * Implementation for managing cached Invitation information
 *
 * @since S60 
 */
class CProtocolImConversationDataHostImp : public CXIMPApiObjBase,
                                           public MProtocolImConversationDataHost
    {
public: // Definitions

    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CPROTOCOLIMCONVERSATIONDATAHOST };

public: // Construction and destruction

    /**
     * Construction
     */
    IMPORT_C static CProtocolImConversationDataHostImp* NewL( MXIMPHost& aHost );

    /**
     * Destruction
     */
    virtual ~CProtocolImConversationDataHostImp();

private:

    CProtocolImConversationDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: //From CProtocolImConversationDataHost
	
	//TXIMPRequestId HandleSentMessageL(MXIMPIdentity* aMessageId);
    TXIMPRequestId HandleNewTextMessageL( MImConversationInfo* aImMessage);
    //TXIMPRequestId HandleNewContentMessageL(  MXIMPIdentity* aMessageId,
    //                                       const TDesC& aSender,   
    //                                       const TDesC& aScreenName, 
    //                                       const MDesCArray& aRecipients,
    //                                       const MDesCArray& aScreenNames,                  
    //                                       const TDesC& aContentType, 
    //                                       const TDesC8& aContent);
    // TXIMPRequestId HandleBlockedListResponseL( const MDesCArray* aBlockedList,
    //                                          TBool aBlockedListInUse,
    //                                          const MDesCArray* aGrantedList,
    //                                          TBool aGrantedListInUse);
    //TXIMPRequestId HandleForwardMessageCompletionL(MXIMPIdentity* aMessageId);
	//TXIMPRequestId HandleGetMessageListL( /*MImFtpPluginObjectCollection* aMessageList */);
    //TXIMPRequestId HandleRejectMessageL( MXIMPIdentity* aMessageId);
  
 private: // Data

    /**
     * Access to host.
     */
    MXIMPHost& iHost;
    };




#endif // CPROTOCOLIMCONVERSATIONDATAHOSTIMP_H

